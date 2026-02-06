#ifndef __ECS_FRAME_SN_OBJ_H__
#define __ECS_FRAME_SN_OBJ_H__
#include <stdint.h>
#include "global.h"
namespace ecsfrm
{
    /// @brief 带序列号的对象
    class ISnObject
    {
    public:
        ISnObject()
        {
            _sn = Global::GetInstance()->GenerateSN();
        }
        virtual ~ISnObject() = default;
        /// @brief 获取当前对象的序列号
        /// @return sn 序列号
        uint64_t GetSN() const
        {
            return _sn;
        }

    private:
        uint64_t _sn;
    };
} // namespace ecsfrm

#endif