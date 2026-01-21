#ifndef __ECS_FRAME_MACRO_H__
#define __ECS_FRAME_MACRO_H__
#include <mutex>
namespace ecsfrm
{
#define LOCK_GUARD(name, mtx) \
    std::lock_guard<std::mutex> name(mtx)

#define INVAILD_SOCKET -1
#define SOCKET int
#define MAX_EVENTS 4096
} // namespace ecsframe

#endif