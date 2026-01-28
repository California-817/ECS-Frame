#ifndef __ECS_FRAME_BUFFER_H__
#define __ECS_FRAME_BUFFER_H__
#include "Idisposable.h"
#include <stdint.h>
namespace ecsfrm
{
    /// @brief 缓冲区
    class Buffer : public IDisposable
    {
    public:
        virtual ~Buffer();

    protected:
        char *_memory;
        uint32_t _begin_index;
        uint32_t _end_index;
    };
} // namespace ecsfrm

#endif