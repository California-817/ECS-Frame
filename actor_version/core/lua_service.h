#ifndef __ECS_FRAME_LUA_SERVICE_H__
#define __ECS_FRAME_LUA_SERVICE_H__
#include "actor.h"
#include "util.h"
#include "singleton.h"
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

namespace ecsfrm
{
    class LuaService : public Actor
    {
    public:
        LuaService(const std::string &lua_file);
        /// @brief 更新函数
        virtual void Update() override;
        /// @brief 初始化函数
        virtual bool Init() override;
        /// @brief 注册这个actor感兴趣的所有消息类型对应的msgid
        virtual void RegisterMsgFunc() override;
        /// @brief 获取actor的类型名
        virtual std::string GetTypeName() override
        {
            return Util::GetTypeString<LuaService>()+_script;
        }
        virtual void Dispose() override;

    private:
        lua_State *_lua_state = nullptr; // lua vm
        std::string _script;
        std::unordered_map<int, std::string> _msg2names;
    };
} // namespace ecsfrm

#endif