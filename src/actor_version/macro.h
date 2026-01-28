#ifndef __ECS_FRAME_MACRO_H__
#define __ECS_FRAME_MACRO_H__
#include <mutex>
#include <assert.h>
#include "log.h"
namespace ecsfrm
{
    /// @brief network app type
    enum APP_TYPE
    {
        APP_LISTEN = 0,
        APP_COMMON = 1,
    };

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

#define BindFunP4(_func, _this) \
    std::bind(_func, _this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)
#define BindFunP3(_func, _this) \
    std::bind(_func, _this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
#define BindFunP2(_func, _this) \
    std::bind(_func, _this, std::placeholders::_1, std::placeholders::_2)
#define BindFunP1(_func, _this) \
    std::bind(_func, _this, std::placeholders::_1)
#define BindFunP0(_func, _this) \
    std::bind(_func, _this)
} // namespace ecsframe

#endif