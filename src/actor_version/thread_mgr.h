#ifndef __ESC_FRAME_THREAD_MGR_H__
#define __ESC_FRAME_THREAD_MGR_H__
#include "thread.h"
#include "singleton.h"
#include "Idisposable.h"
namespace ecsfrm
{
    class ThreadMgr : public Singleton<ThreadMgr>, public IDisposable
    {
    public:
        ThreadMgr() = default;
        ~ThreadMgr() = default;
        /// @brief 创建一个线程
        void NewThread();
        /// @brief 启动所有线程
        void Start();
        /// @brief 检查所有线程是否都在运行
        bool InGameLoop();
        /// @brief 添加Actor到子线程中
        /// @param actor
        void PushActorToThread(Actor *actor);
        virtual void Dispose() override;

    private:
        std::vector<Thread *> _threads;
        int _index;
        int _size;
    };
} // namespace ecsfrm

#endif