#ifndef __ECS_FRAME_PACKET_H__
#define __ECS_FRAME_PACKET_H__
#include "Idisposable.h"
#include "macro.h"
namespace ecsfrm
{
    class Packet : public IDisposable
    {
    public:
        Packet(/* args */);
        ~Packet();
        virtual void Dispose() override;
        /// @brief 获取消息id
        /// @return id
        int GetMsgId() const;
        SOCKET GetSocket() const;

    private:
    };

} // namespace ecsfrm

#endif