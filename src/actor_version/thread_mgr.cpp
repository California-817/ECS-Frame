#include "thread_mgr.h"
#include"log.h"
namespace ecsfrm
{
    static Logger::ptr g_logger=LogSystemUtil::RegisterLogger("system");
    void ThreadMgr::NewThread()
    {
        _threads.emplace_back(new Thread(std::string("thread_" + std::to_string(_size++))));
    }
    void ThreadMgr::Start()
    {
        for (auto &thread : _threads)
        {
            thread->Start();
        }
    }
    bool ThreadMgr::InGameLoop()
    {
        bool is_running = false;
        for (auto &thread : _threads)
        {
            is_running |= thread->IsRunning();
        }
        return is_running;
    }
    void ThreadMgr::Dispose()
    {
        for (auto &thread : _threads)
        {
            thread->Stop();
            thread->Dispose();
            delete thread;
        }
        _threads.clear();
        LOG_INFO(g_logger) << "thread mgr dispose";
    }
} // namespace ecsfrm
