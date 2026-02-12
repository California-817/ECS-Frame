#include "lua_service.h"
#include "log.h"
#include "lua_launcher/lua_api.h"
namespace ecsfrm
{
    static Logger::ptr g_logger = LogSystemUtil::RegisterLogger("system");
    LuaService::LuaService(const std::string &lua_file)
        : _script(lua_file)
    {
    }
    bool LuaService::Init()
    {
        do
        {
            _lua_state = luaL_newstate();
            luaL_openlibs(_lua_state);

            LuaAPI::RegisterLuaAPI(_lua_state);

            if (luaL_dofile(_lua_state, _script.c_str()) != 0)
                break;
            /// 调用lua实现的Init函数
            // 1.将lua的OnInit函数地址压入栈中
            lua_getglobal(_lua_state, "Init");
            // 2.参数入栈
            // 3.调用
            if (lua_pcall(_lua_state, 0, 1, 0) != 0)
                break;
            bool ret=lua_toboolean(_lua_state, -1);
            lua_pop(_lua_state, 1);
            LOG_INFO(g_logger) << "Lua Service Init , _Script : " << _script;
            return ret;
        } while (false);

        LOG_ERROR(g_logger) << "Lua Service Init Failed , Script : " << _script << " Error : " << lua_tostring(_lua_state, -1);
        lua_pop(_lua_state, 1);
        return false;
    }
    void LuaService::Update()
    {
        // 调用lua的Update函数
    }
    void LuaService::RegisterMsgFunc()
    {
        // 由Lua脚本实现函数本身并进行绑定

        // 1.调用Lua实现的RegisterMsgFunc函数---->进行了id与name的映射

        // 2.拿映射关系，根据函数namestring，绑定对应的lua函数-->间接调用lua实现的函数
    }
    void LuaService::Dispose()
    {
        ASSERT_INFO(_lua_state);
        // 调用lua实现的Dispose函数
        lua_close(_lua_state);
        _lua_state = nullptr;
    }

} // namespace ecsfrm
