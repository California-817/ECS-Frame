#include "network_listen.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "util.h"
namespace ecsfrm
{
    static Logger::ptr logger = LogSystemUtil::RegisterLogger("system");
    bool NetworkListen::Init()
    {
        return true;
    }
    bool NetworkListen::Listen(const std::string &ip, int port)
    {
        _master_socket = socket(AF_INET, SOCK_STREAM, 0);
        ASSERT_INFO(_master_socket != INVAILD_SOCKET);
        // set socket opt
        init_socket(_master_socket);
        // bind
        sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = inet_addr(ip.c_str());
        address.sin_port = htons(port);
        ASSERT_INFO(bind(_master_socket, (struct sockaddr *)&address, sizeof(address)) == 0);
        int ret = ::listen(_master_socket, 4096);
        if (ret == 0)
        {
            init_epoll();
            return true;
        }
        LOG_ERROR(logger) << "listen failed, errstr=" << strerror(errno);
        close(_master_socket);
        return false;
    }
    void NetworkListen::Update()
    {
        // 每帧调用
        epoll();
        if (_b_accept_event)
            accept();
        Network::Update();
    }
    void NetworkListen::RegisterMsgFunc()
    {
        Network::RegisterMsgFunc();
    }
    void NetworkListen::accept()
    {
        while (true)
        {
            int newfd = ::accept(_master_socket, NULL, NULL); // no block
            if (newfd == INVAILD_SOCKET && errno != EWOULDBLOCK)
            {
                // error
                LOG_ERROR(logger) << "accept failed, errstr=" << strerror(errno);
                break;
            }
            else if (newfd == INVAILD_SOCKET)
            {
                // no more connection
                break;
            }
            else
            {
                // new connection
                LOG_DEBUG(logger) << "accept new connection, fd=" << newfd;
                // 1. set socket opt
                init_socket(newfd);
                // 2. add to epoll
                add_event(_epfd, newfd, EPOLLIN | EPOLLET | EPOLLRDHUP);
                _connects_map[newfd] = new ConnectObj(this, newfd);
            }
        }
    }
} // namespace ecsfrm
