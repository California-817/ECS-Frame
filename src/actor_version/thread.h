#ifndef __ECS_FRAME_THREAD_H__
#define __ECS_FRAME_THREAD_H__
#include <thread>
#include <atomic>
#include <string>
#include<list>
#include "Idisposable.h"
#include"actor.h"
namespace ecsfrm
{
    /// @brief 线程类
    class Thread : public IDisposable
    {
    public:
        Thread(const std::string &name);
        /// @brief 线程启动函数
        void start();
        virtual void Dispose() override;
        ~Thread();
        /// @brief 线程是否运行
        /// @return
        bool IsRunning() const { return _is_running; }
        /// @brief 暂停线程
        void Stop() { _is_running = false; }
        /// @brief 向该线程添加一个actor对象
        /// @param atr 
        void PushActor(Actor* atr);
    private:
        /// @brief 线程执行函数
        void update();

    private:
        std::thread *_thread = nullptr;
        std::atomic<bool> _is_running{false};
        std::string _name;
        std::list<Actor*> _actors;
        std::list<Actor*> _copy_list;
        std::mutex _actors_mtx;
    };
} // namespace ecsfrm

#endif