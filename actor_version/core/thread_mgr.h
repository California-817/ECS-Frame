#ifndef __ESC_FRAME_THREAD_MGR_H__
#define __ESC_FRAME_THREAD_MGR_H__
#include "thread.h"
#include "singleton.h"
#include "Idisposable.h"
#include "network.h"
namespace ecsfrm
{
    /// @brief 线程管理类,本身有一个actorlist
    class ThreadMgr : public Singleton<ThreadMgr>, public ThreadActorList
    {
    public:
        ThreadMgr() = default;
        ~ThreadMgr() = default;
        /// @brief 创建一个线程
        void NewThread();
        /// @brief push一个网络actor
        /// @param network
        void PushNetwork(APP_TYPE id, Network *network);
        /// @brief 根据apptype获取一个网络actor
        /// @param id
        /// @return
        Network *GetNetwork(APP_TYPE id);
        /// @brief 启动所有线程
        void Start();
        /// @brief 检查所有线程是否都在运行
        bool InGameLoop();
        /// @brief 添加Actor到子线程中
        /// @param actor
        bool PushActorToChildThread(Actor *actor);
        virtual void Dispose() override;
        /// @brief 帧更新函数
        void Update();
        /// @brief 分发数据包到所有线程的所有Actor中
        /// @param packet
        void DispatchPacket(Packet *packet);
        /// @brief 发送网络数据包到Network这个actor中的指定ConnectObj中
        /// @param packet
        void SendNetPacket(Packet *packet);
        /// @brief 返回所有线程信息
        /// @return 
        std::string Info();
        /// @brief 返回所有网络actor信息
        /// @return 
        std::string NetInfo();
    private:
        std::unordered_map<uint64_t,Thread *> _threads;
        std::vector<uint64_t> _thread_ids;
        std::mutex _th_mtx;
        int _index = 0;
        int _size = 0;
        std::unordered_map<APP_TYPE, Network *> _networks;
        std::mutex _net_mtx;
    };
} // namespace ecsfrm

#endif