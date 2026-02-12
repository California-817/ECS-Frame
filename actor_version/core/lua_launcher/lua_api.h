#ifndef __ECS_FRAME_LUA_API_H__
#define __ECS_FRAME_LUA_API_H__
#include <string>
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
/// @brief launcher
namespace ecsfrm
{
    namespace LuaAPI
    {
        void RegisterLuaAPI(lua_State *L);
    }
} // namespace ecsfrm

#endif