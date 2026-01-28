#ifndef __ECS_FRAME_UTIL_H__
#define __ECS_FRAME_UTIL_H__
#include "macro.h"
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
        /// @brief 获取指定目录下的指定后缀的文件列表
        /// @param path
        /// @param suffix
        /// @return files
        static std::vector<std::string> GetFilesBySuffix(const std::string &path, const std::string &suffix);
    };
} // namespace ecsfrm

#endif