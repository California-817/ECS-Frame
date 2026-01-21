#ifndef __ECS_FRAME_NETWORK_H__
#define __ECS_FRAME_NETWORK_H__
#include "disposable.h"
#include "connect_obj.h"
#include "macro.h"
#include <unordered_map>
namespace ecsfrm
{
    class Network : public IDisposable
    {
    public:
        Network();
        virtual ~Network();

        void Dispose() override;

    protected:
        /// @brief 每次进行非阻塞epoll看哪些事件就绪并执行io操作
        /// @return
        void epoll();
        /// @brief 创建epoll模型并将listen套接字或者连接本身放入epoll模型中
        void init_epoll();
    private:
        
    protected:
        int _epfd = -1;
        struct epoll_event _events[MAX_EVENTS];
        bool _b_accpet_event = false;
        SOCKET _master_socket = INVAILD_SOCKET;
        std::unordered_map<SOCKET, ConnectObj *> _connects_map;
    };
} // namespace ecsfrm

#endif