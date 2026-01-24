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

    Network::Network() {}
    Network::~Network() {}

    void Network::Dispose() {}
    void Network::Update()
    {
        //todo
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
        int evNums = epoll_wait(_epfd, _events, MAX_EVENTS, 0); // no block wait
        for (int i = 0; i < evNums; i++)
        {
            if (_events[i].data.fd == _master_socket)
            {
                // new connection
                _b_accpet_event = true;
            }
        }
    }

} // namespace ecsfrm
