#include "network.h"
#include "log.h"
#include "util.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
namespace ecsfrm
{
    static Logger::ptr g_logger = LOG_REGISTER("system");

#define RemoveConObj(obj)                      \
    if (obj)                                   \
    {                                          \
        obj->Dispose();                        \
        delete_event(_epfd, obj->GetSocket()); \
        _connects_map.erase(obj->GetSocket()); \
        delete obj;                            \
    }

    void add_event(int epfd, int fd, uint32_t events)
    {
        struct epoll_event ev;
        ev.events = events;
        ev.data.fd = fd;
        if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1)
        {
            LOG_ERROR(g_logger) << "epoll_ctl failed errstr=" << strerror(errno);
        }
    }
    void modify_event(int epfd, int fd, uint32_t events)
    {
        struct epoll_event ev;
        ev.events = events;
        ev.data.fd = fd;
        if (epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) == -1)
        {
            LOG_ERROR(g_logger) << "epoll_ctl failed errstr=" << strerror(errno);
        }
    }
    void delete_event(int epfd, int fd)
    {
        if (epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL) == -1)
        {
            LOG_ERROR(g_logger) << "epoll_ctl failed errstr=" << strerror(errno);
        }
    }

    Network::Network() {}
    Network::~Network() {}

    void Network::Dispose() {}
    void Network::RegisterMsgFunc()
    {
        //todo
        MailBox::RegisterMsgFunc(0, BindFunP1(&Network::OnRecvCloseMsgFunc, this));
    }
    void Network::Update()
    {
        std::list<Packet *> tmpList;
        {
            LOCK_GUARD(lock, _netpackets_list_mutex);
            _netpackets_list.swap(tmpList);
        }
        for (auto &packet : tmpList)
        {
            auto iter = _connects_map.find(packet->GetSocket());
            if (iter == _connects_map.end())
            {
                LOG_ERROR(g_logger) << "packet->GetSocket() not find,fd=" << packet->GetSocket();
                continue;
            }
            iter->second->SendNetPacket(packet);
        }
        tmpList.clear();
    }
    void Network::SendNetPacket(Packet *packet)
    {
        if (!packet)
        {
            LOCK_GUARD(_lock, _netpackets_list_mutex);
            _netpackets_list.push_back(packet);
        }
    }
    SOCKET Network::GetSocket() const { return _master_socket; }
    void Network::init_socket(SOCKET socket)
    {
        // set socket option
        int opt = 1;
        setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, (char *)&opt, sizeof(opt));
        // non-block
        Util::SetNonBlock(socket);
    }
    void Network::init_epoll()
    {
        _epfd = epoll_create(1);
        if (_epfd == INVAILD_SOCKET)
        {
            LOG_CRITICAL(g_logger) << "epoll_create failed";
            return;
        }
        add_event(_epfd, _master_socket, EPOLLIN | EPOLLOUT | EPOLLRDHUP);
    }
    void Network::epoll()
    {
        for (auto &pair : _connects_map)
        {
            ConnectObj *connect = pair.second;
            if (connect->IsClose())
            {
                // 连接已经关闭
                LOG_DEBUG(g_logger) << "connect in logical layer closed,fd=" << pair.first;
                RemoveConObj(connect);
                continue;
            }
            if (connect->HasSendData())
            {
                // 有数据可发送
                modify_event(_epfd, pair.first, EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET);
                continue;
            }
        }
        int evNums = epoll_wait(_epfd, _events, MAX_EVENTS, 50);
        for (int i = 0; i < evNums; i++)
        {
            SOCKET fd = _events[i].data.fd;
            int events = _events[i].events;
            if (fd == _master_socket)
            {
                // new connection
                _b_accpet_event = true;
            }
            auto iter = _connects_map.find(fd);
            if (iter == _connects_map.end())
            {
                // listen socket accpet event ready
                continue;
            }
            ConnectObj *connect = iter->second;
            if (events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP))
            {
                // error
                LOG_WARN(g_logger) << "epoll error,fd=" << fd << " events=" << events;
                RemoveConObj(connect);
                continue;
            }
            if (events & EPOLLIN)
            {
                // 读事件就绪
                if (connect->RecvData() < 0)
                {
                    // 接收数据失败
                    RemoveConObj(connect);
                    continue;
                }
            }
            if (events & EPOLLOUT)
            {
                // 写事件就绪
                if (connect->SendData() < 0)
                {
                    // 发送数据失败
                    RemoveConObj(connect);
                    continue;
                }
                // 取消写事件
                modify_event(_epfd, fd, EPOLLIN | EPOLLET | EPOLLRDHUP);
            }
        }
    }
    int Network::OnRecvCloseMsgFunc(Packet *packet)
    {
        auto iter = _connects_map.find(packet->GetSocket());
        if (iter == _connects_map.end())
        {
            LOG_ERROR(g_logger) << "OnRecvCloseMsgFunc: not found connect";
            return -1;
        }
        iter->second->Close();
        return 0;
    }
} // namespace ecsfrm
