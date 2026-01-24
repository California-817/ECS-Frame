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
        _thread = new std::thread(std::bind(&Thread::update, this));
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
            // todo
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
} // namespace ecsfrm
