#include "lua_service.h"
#include "log.h"
#include"LuaBridge/LuaBridge.h"
#include"lua_launcher/engine.h"
namespace ecsfrm
{
    static Logger::ptr g_logger = LogSystemUtil::RegisterLogger("system");
    LuaService::LuaService(const std::string &lua_file)
        : _script(lua_file)
    {
    }
    bool LuaService::Init()
    {
        _lua_state = luaL_newstate();
        luaL_openlibs(_lua_state);
        //导出cpp接口给lua使用
        using namespace luabridge;
            getGlobalNamespace(_lua_state)
            .beginClass<EcsEngine>("EcsEngine")
                .addConstructor<void(*)(int)>()
                .addFunction("LogInfo",&EcsEngine::LogInfo)
            .endClass();      
          
        if (luaL_dofile(_lua_state, _script.c_str()) != 0)
        {
            LOG_ERROR(g_logger) << "Lua Service Init Failed , Script : " << _script<<" Error : "<< lua_tostring(_lua_state, -1);
            return false;
        }
        /// 调用lua实现的Init函数
        LOG_INFO(g_logger) << "Lua Service Init , _Script : " << _script;
        return true;
    }
    void LuaService::Update() 
    {
        //调用lua的Update函数
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
