#ifndef __ECS_FRAME_NETWORK_H__
#define __ECS_FRAME_NETWORK_H__
#include "Idisposable.h"
#include "connect_obj.h"
#include "Isocket_obj.h"
#include "macro.h"
#include "actor.h"
#include <sys/epoll.h>
#include <unordered_map>
namespace ecsfrm
{
    /// @brief 网络类
    class Network : public Actor, public ISocketObj
    {
    public:
        Network();
        virtual ~Network();

        virtual void Dispose() override;
        virtual SOCKET GetSocket() const override;

    protected:
        /// @brief 每次进行非阻塞epoll看哪些事件就绪并执行io操作
        /// @return
        void epoll();
        /// @brief 创建epoll模型并将listen套接字或者连接本身放入epoll模型中
        void init_epoll();
        /// @brief 初始化socket 设置reuseaddr和非阻塞，禁用negal算法
        void init_socket(SOCKET socket);

    protected:
        int _epfd = -1;
        struct epoll_event _events[MAX_EVENTS];
        bool _b_accpet_event = false;
        SOCKET _master_socket = INVAILD_SOCKET;
        std::unordered_map<SOCKET, ConnectObj *> _connects_map;
    };
    /// @brief 添加事件到epoll
    /// @param epfd
    /// @param fd
    /// @param events
    void add_event(int epfd, int fd, uint32_t events);
    /// @brief 修改epoll中事件
    /// @param epfd
    /// @param fd
    /// @param events
    void modify_event(int epfd, int fd, uint32_t events);

} // namespace ecsfrm

#endif