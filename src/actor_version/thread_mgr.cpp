#include "thread_mgr.h"
#include "log.h"
namespace ecsfrm
{
    static Logger::ptr g_logger = LogSystemUtil::RegisterLogger("system");
    void ThreadMgr::NewThread()
    {
        LOCK_GUARD(lock, _th_mtx);
        Thread *th = new Thread(std::string("thread_" + std::to_string(_size++)));
        _threads.insert(std::make_pair(th->GetSN(), th));
        _thread_ids.push_back(th->GetSN());
    }
    void ThreadMgr::Start()
    {
        LOCK_GUARD(lock, _th_mtx);
        for (auto &thread : _threads)
        {
            thread.second->Start();
        }
    }
    bool ThreadMgr::InGameLoop()
    {
        LOCK_GUARD(lock, _th_mtx);
        bool is_running = false;
        for (auto iter = _threads.begin(); iter != _threads.end(); iter++)
        {
            auto th = iter->second;
            is_running |= th->IsRunning();
            if (!th->IsRunning())
            {
                _threads.erase(th->GetSN());
                th->Dispose();
                delete th;
            }
        }
        return is_running;
    }
    bool ThreadMgr::PushActorToChildThread(Actor *actor)
    {
        LOCK_GUARD(lock, _th_mtx);
        int init_index = _index;
        while (++_index % _size != init_index)
        {
            int cur_index = _index % _size;
            if (_threads[_thread_ids[cur_index]]->IsRunning())
            {
                _threads[_thread_ids[cur_index]]->PushActor(actor);
                return true;
            }
        }
        LOG_ERROR(g_logger) << "ThreadMgr::PushActorToChildThread() no thread is running now";
        return false;
    }
    void ThreadMgr::Dispose()
    {
        {
            LOCK_GUARD(lock, _th_mtx);
            for (auto &thread : _threads)
            {
                thread.second->Stop();
                thread.second->Dispose();
                delete thread.second;
            }
            _threads.clear();
        }
        // 主线程的actorlist中actor销毁
        ThreadActorList::Dispose();
        LOG_INFO(g_logger) << "thread mgr dispose";
    }
    void ThreadMgr::Update()
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
    }
    void ThreadMgr::DispatchPacket(Packet *packet)
    {
        // self thread
        ThreadActorList::PushPacket(packet);
        // child threads
        LOCK_GUARD(_lock, _th_mtx);
        for (auto &th : _threads)
        {
            if (th.second->IsRunning())
                th.second->PushPacket(packet);
        }
    }
    void ThreadMgr::SendNetPacket(Packet *packet)
    {
        Network *network = GetNetwork(APP_TYPE::APP_LISTEN);
        network->SendNetPacket(packet);
    }
    void ThreadMgr::PushNetwork(APP_TYPE id, Network *network)
    {
        if (!PushActorToChildThread(network))
            return;
        LOCK_GUARD(lock, _net_mtx);
        _networks[id] = network;
    }
    Network *ThreadMgr::GetNetwork(APP_TYPE id)
    {
        LOCK_GUARD(lock, _net_mtx);
        auto iter = _networks.find(id);
        return iter != _networks.end() ? iter->second : nullptr;
    }
} // namespace ecsfrm
