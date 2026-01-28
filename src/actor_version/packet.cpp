#include"packet.h"
namespace ecsfrm
{
           Packet::Packet(int msgid, SOCKET socket)
           :_msgid(msgid), _socket(socket)
           {}
        Packet::~Packet(){}
        void Packet::Dispose() 
        {}
        int Packet::GetMsgId() const 
        {
            return _msgid;
        }

        SOCKET Packet::GetSocket() const
        {
            return _socket;
        }
} // namespace ecsfrm
