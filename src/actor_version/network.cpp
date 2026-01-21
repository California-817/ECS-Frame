#include "network.h"
#include "log.h"
#include <sys/epoll.h>
#include <string.h>
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
            if(_events[i].data.fd == _master_socket)
            {
                // new connection 
                _b_accpet_event=true;
            }
        }
    }

} // namespace ecsfrm
