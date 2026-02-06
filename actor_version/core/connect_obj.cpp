#include "connect_obj.h"
#include"log.h"
namespace ecsfrm
{
    static Logger::ptr g_logger = LogSystemUtil::RegisterLogger("system");
    ConnectObj::ConnectObj(Network *network, SOCKET sockfd)
        : _network(network), _sockfd(sockfd)
    {
    }
    void ConnectObj::Dispose()
    {
    }
    void ConnectObj::SendNetPacket(Packet *packet)
    {
    }
    bool ConnectObj::HasSendData()
    {
    }
    int ConnectObj::SendData()
    {
    }
    int ConnectObj::RecvData()
    {
        char _buf[1024];
        while (true)
        {
            ssize_t n = ::read(_sockfd, _buf, sizeof(_buf));
            if (n > 0)
            {
                _buf[n]=0;
                LOG_DEBUG(g_logger) << "recv data:" << _buf;
                return n;
            }
            else if (n == 0)
            {
                LOG_DEBUG(g_logger) << "client close";
                break;
            }
            else
            { 
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                    return 0;
                else
                    break;
            }
        }
        return -1;
    }
} // namespace ecsfrm
