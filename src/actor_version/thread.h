#ifndef __ECS_FRAME_THREAD_H__
#define __ECS_FRAME_THREAD_H__
#include <thread>
#include <atomic>
#include <string>
#include <list>
#include"Isn_obj.h"
#include "Idisposable.h"
#include "packet.h"
#include "actor.h"
namespace ecsfrm
{
    class ThreadActorList : public IDisposable
    {
    public:
        /// @brief 向该线程添加一个actor对象
        /// @param atr
        void PushActor(Actor *atr);
        /// @brief 向该线程中所有对该packet的消息id对应msg感兴趣的actor中添加packet对象
        /// @param packet
        void PushPacket(Packet *packet);
        virtual ~ThreadActorList() = default;
        /// @brief 销毁剩余的actor
        virtual void Dispose() override;

    protected:
        std::list<Actor *> _actors;
        std::list<Actor *> _copy_list;
        std::mutex _actors_mtx;
    };
    /// @brief 线程类
    class Thread : public ThreadActorList,public ISnObject
    {
    public:
        Thread(const std::string &name);
        /// @brief 线程启动函数
        void Start();
        /// @brief 回收线程并销毁剩余的actor
        virtual void Dispose() override;
        ~Thread();
        /// @brief 线程是否运行
        /// @return
        bool IsRunning() const { return _is_running; }
        /// @brief 暂停线程
        void Stop() { _is_running = false; }

    private:
        /// @brief 线程执行函数
        void update();

    private:
        std::thread *_thread = nullptr;
        std::atomic<bool> _is_running{false};
        std::string _name;
    };
} // namespace ecsfrm

#endif