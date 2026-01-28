#include "connect_obj.h"
namespace ecsfrm
{
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
    }
} // namespace ecsfrm
