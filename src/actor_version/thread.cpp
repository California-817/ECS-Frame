#include "thread.h"
#include "log.h"
#include <chrono>
#include <pthread.h>
namespace ecsfrm
{
    static Logger::ptr logger = LogSystemUtil::RegisterLogger("system");
    Thread::Thread(const std::string &name)
        : _name(name)
    {
    }
    void Thread::Start()
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
        // 1. 停止线程
        if (_thread)
        {
            if (_thread->joinable())
                _thread->join();
            delete _thread;
            LOG_DEBUG(logger) << "Thread " << _name << " stop";
        }
        // 2. 销毁所有还没有被及时处理的actor
        ThreadActorList::Dispose();
        _thread = nullptr;
    }
    Thread::~Thread()
    {
        ASSERT_INFO(!_is_running && !_thread);
    }
    void Thread::update()
    {
        pthread_setname_np(pthread_self(), _name.c_str());
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
    void ThreadActorList::PushActor(Actor *atr)
    {
        if (!atr)
        {
            LOG_ERROR(logger) << "ThreadActorList::PushActor() error: " << "actor is null";
            return;
        }
        LOCK_GUARD(lock, _actors_mtx);
        atr->Init();
        atr->RegisterMsgFunc();
        _actors.push_back(atr);
    }

    void ThreadActorList::PushPacket(Packet *packet)
    {
        if (!packet)
        {
            LOG_ERROR(logger) << "ThreadActorList::PushPacket() error: " << "packet is null";
            return;
        }
        LOCK_GUARD(lock, _actors_mtx);
        for (auto &actor : _actors)
        {
            if (actor->IsFollowMsgId(packet->GetMsgId()))
            {
                actor->AddPacket(packet);
            }
        }
    }
    void ThreadActorList::Dispose()
    {
        LOCK_GUARD(lock, _actors_mtx);
        for (auto &atr : _actors)
        {
            atr->SetNoActive();
            atr->Dispose();
            delete atr;
        }
        _actors.clear();
    }
    std::string Thread::Info()
    {
        std::stringstream ss;
        LOCK_GUARD(lock, _actors_mtx);
        ss << "\tThread:" << _name << " [";
        for (auto &atr : _actors)
        {
            ss << atr->GetTypeName() << " ";
        }
        ss << "]";
        return ss.str();
    }
} // namespace ecsfrm
