#include "actor.h"
namespace ecsfrm
{
    bool Actor::IsActive() const { return _is_active; }
    void Actor::SetNoActive() { _is_active = false; }
} // namespace ecsfrm
