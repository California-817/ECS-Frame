#ifndef __ECS_FRAME_DISPOSABLE_H__
#define __ECS_FRAME_DISPOSABLE_H__
namespace ecsfrm
{
    /// @brief 提供销毁接口 dispose
    class IDisposable
    {
    public:
        virtual ~IDisposable() = default;
        /// @brief 子类实现销毁接口
        virtual void Dispose() = 0;
    };
} // namespace ecsframe

#endif