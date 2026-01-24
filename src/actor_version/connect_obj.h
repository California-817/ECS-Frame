#ifndef __ECS_FRAME_CONNECT_OBJ_H__
#define __ECS_FRAME_CONNECT_OBJ_H__
#include "Idisposable.h"
#include "Isocket_obj.h"
#include "macro.h"
namespace ecsfrm
{
    class Network;
    /// @brief 一个tcp连接对应一个connectobj
    class ConnectObj : public IDisposable, public ISocketObj
    {
    public:
        ConnectObj(Network* network,SOCKET sockfd);
        ~ConnectObj();
        virtual void Dispose() override;
        virtual SOCKET GetSocket() const override;

    private:
        // read buffer
        // write buffer
        SOCKET _sockfd;
    };
} // namespace ecsframe

#endif