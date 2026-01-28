#ifndef __ECS_FRAME_CONNECT_OBJ_H__
#define __ECS_FRAME_CONNECT_OBJ_H__
#include "Idisposable.h"
#include "Isocket_obj.h"
#include"packet.h"
#include "macro.h"
namespace ecsfrm
{
    class Network;
    /// @brief 一个tcp连接对应一个connectobj
    class ConnectObj : public IDisposable, public ISocketObj
    {
    public:
        friend class Network;
        ConnectObj(Network *network, SOCKET sockfd);
        ~ConnectObj()=default;
        virtual void Dispose() override;
        /// @brief 发送网络数据包到Connect内部缓冲区
        /// @param packet 
        void SendNetPacket(Packet *packet);
        /// @brief 获取socket的文件描述符
        /// @return fd
        virtual SOCKET GetSocket() const override { return _sockfd; }
        /// @brief 连接是否关闭
        /// @return
        bool IsClose() const { return _b_close; }
        /// @brief 关闭连接
        void Close() {_b_close=true;}
    private:
        /// @brief 是否有需要发送到网络层的数据
        /// @return
        bool HasSendData();
        /// @brief 从内部缓冲区发送数据到网络层
        /// @return
        int SendData();
        /// @brief 从网络层接收数据到内部缓冲区并且将完整的Packet包dispatch到所有感兴趣的actor的mailbox中
        /// @return 
        int RecvData();
    private:
        // read buffer
        // write buffer
        const SOCKET _sockfd;
        Network *_network=nullptr;
        bool _b_close = false;
    };
} // namespace ecsframe

#endif