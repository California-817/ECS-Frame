#ifndef __ECS_FRAME_SOCKET_OBJ_H__
#define __ECS_FRAME_SOCKET_OBJ_H__
#include "macro.h"
namespace ecsfrm
{
    /// @brief socket对象接口
    class ISocketObj
    {
    public:
        /// @brief 获取socketfd
        /// @return
        virtual SOCKET GetSocket() const = 0;
        virtual ~ISocketObj() = default;
    };
} // namespace ecsfrm

#endif