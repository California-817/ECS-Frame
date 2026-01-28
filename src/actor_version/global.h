#ifndef __ECS_FRAME_GLOBAL_H__
#define __ECS_FRAME_GLOBAL_H__
#include <stdint.h>
#include <atomic>
#include "singleton.h"
namespace ecsfrm
{
    /// @brief 单例全局变量--保存时间信息并生成序列号
    class Global : public Singleton<Global>
    {
    public:
        uint64_t TimeStamp; // 当前时间戳 ms
        int YearDay;        //
        /// @brief 设置服务器ID
        /// @param server_id
        Global(uint32_t server_id)
            : _server_id(server_id)
        {
        }
        /// @brief 产生进程中唯一的SN
        /// @return sn
        uint64_t GenerateSN()
        {
            return (TimeStamp << 32) | (_server_id << 16) | _sn_id.fetch_add(1);
        }

    private:
        const uint32_t _server_id;
        std::atomic_uint16_t _sn_id;
    };
} // namespace ecsfrm

#endif