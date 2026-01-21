#ifndef __ECS_FRAME_CONNECT_OBJ_H__
#define __ECS_FRAME_CONNECT_OBJ_H__
#include "disposable.h"
namespace ecsfrm
{
    /// @brief 一个tcp连接对应一个connectobj
    class ConnectObj : public IDisposable
    {
    public:
        ConnectObj();
        ~ConnectObj();
        void IDispose();
    private:
        //read buffer
        //write buffer
    };
} // namespace ecsframe

#endif