#ifndef __ECS_FRAME_UTIL_H__
#define __ECS_FRAME_UTIL_H__
#include"macro.h"
namespace ecsfrm
{
    /// @brief 工具类
    class Util
    {
    public:
        /// @brief 设置非阻塞
        /// @param sock 
        static void SetNonBlock(SOCKET sock);
        /// @brief 获取当前时间戳 ms
        /// @return ms
        static uint64_t GetTimeStamp();
        /// @brief 获取在当年的第几天[0-365]
        /// @return 
        static int GetYearDay();
};
} // namespace ecsfrm

#endif