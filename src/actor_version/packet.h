#ifndef __ECS_FRAME_PACKET_H__
#define __ECS_FRAME_PACKET_H__
#include "Idisposable.h"
#include "macro.h"
namespace ecsfrm
{
    class Packet : public IDisposable
    {
    public:
        Packet(int msgid, SOCKET socket);
        ~Packet();
        virtual void Dispose() override;
        /// @brief 获取消息id
        /// @return id
        int GetMsgId() const;
        SOCKET GetSocket() const;

    private:
        const int _msgid;
        const SOCKET _socket;
    };

} // namespace ecsfrm

#endif