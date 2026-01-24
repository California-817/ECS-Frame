#include "thread.h"
#include "log.h"
#include <chrono>
namespace ecsfrm
{
    static Logger::ptr logger = LogSystemUtil::RegisterLogger("system");
    Thread::Thread(const std::string &name)
        : _name(name)
    {
    }
    /// @brief 线程启动函数
    void Thread::start()
    {
        if (_is_running)
        {
            return;
        }
        _is_running = true;
        _thread = new std::thread(BindFunP0(&Thread::update, this));
        LOG_DEBUG(logger) << "Thread " << _name << " start";
    }
    void Thread::Dispose()
    {
        ASSERT_INFO(!_is_running);
        if (_thread)
        {
            _thread->join();
            delete _thread;
            LOG_DEBUG(logger) << "Thread " << _name << " stop";
        }
        _thread = nullptr;
    }
    Thread::~Thread()
    {
        ASSERT_INFO(!_is_running && !_thread);
    }
    void Thread::update()
    {
        while (_is_running)
        {
            {
                LOCK_GUARD(lock, _actors_mtx);
                std::copy(_actors.begin(), _actors.end(), std::back_inserter(_copy_list)); // optimize
            }
            for (auto &actor : _copy_list)
            {
                if (actor->IsActive())
                {
                    actor->ProcessPacket(); // 1.处理消息
                    actor->Update();        // 2.执行更新函数
                }
                else
                {
                    // actor no more active
                    {
                        LOCK_GUARD(_lock, _actors_mtx);
                        _actors.remove(actor); // 1.移除管理
                    }
                    actor->Dispose(); // 2.销毁接口
                    delete actor;     // 3.释放对象内存资源
                }
            }
            _copy_list.clear();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    void Thread::PushActor(Actor *atr)
    {
        if (!_is_running || !atr)
            return;
        LOCK_GUARD(lock, _actors_mtx);
        _actors.push_back(atr);
    }

} // namespace ecsfrm
