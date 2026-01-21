#ifndef __ECS_FRAME_NETWORK_LISTEN_H__
#define __ECS_FRAME_NETWORK_LISTEN_H__
#include"network.h"
#include<string>
namespace ecsfrm
{
    class NetworkListen : public Network
    {
    public:
        NetworkListen();
        ~NetworkListen();
        /// @brief 开启监听状态
        /// @param ip 
        /// @param port 
        /// @return is success
        bool Listen(const std::string &ip, int port);
        /// @brief 每帧调用的更新函数
        void Update();
    private:
        /// @brief 接受客户端连接
        void accept();
   };
} // namespace ecsfrm

#endif