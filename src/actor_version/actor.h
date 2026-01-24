#ifndef __ECS_FRAME_ACTOR_H__
#define __ECS_FRAME_ACTOR_H__
#include "Idisposable.h"
#include "mailbox.h"
namespace ecsfrm
{
    /// @brief 每个线程需要执行的的actor对象
    class Actor : public IDisposable, public MailBox
    {
    public:
        virtual ~Actor() = default;
        /// @brief 更新函数
        virtual void Update() = 0;
        /// @brief 初始化函数
        virtual bool Init() = 0;
    };
} // namespace ecsfrm

#endif