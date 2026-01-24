#ifndef __ECS_FRAME_MACRO_H__
#define __ECS_FRAME_MACRO_H__
#include <mutex>
#include "log.h"
namespace ecsfrm
{
#define LOCK_GUARD(name, mtx) \
    std::lock_guard<std::mutex> name(mtx)

#define ASSERT_INFO(rule)                                             \
    if (!(rule))                                                      \
    {                                                                 \
        Logger::ptr logger = LogSystemUtil::RegisterLogger("system"); \
        LOG_ERROR(logger) << "assert failed: " << #rule;              \
        assert(rule);                                                 \
    }

#define INVAILD_SOCKET -1
#define SOCKET int
#define MAX_EVENTS 4096
#define DEFAULT_INTERVAL_TIME_MS 1
} // namespace ecsframe

#endif