#include "lua_api.h"
#include "log.h"
#include "LuaBridge/LuaBridge.h"
#include "thread_mgr.h"
#include "lua_service.h"
using namespace luabridge;
namespace ecsfrm
{
    static Logger::ptr g_logger = LogSystemUtil::RegisterLogger("root");

    void l_LogAPI(lua_State *L)
    {
        // 使用std::function包装lambda表达式
        auto debugFunc = std::function<void(const std::string &)>([](const std::string &msg)
                                                                  { LOG_DEBUG(g_logger) << msg; });
        auto infoFunc = std::function<void(const std::string &)>([](const std::string &msg)
                                                                 { LOG_INFO(g_logger) << msg; });
        auto warnFunc = std::function<void(const std::string &)>([](const std::string &msg)
                                                                 { LOG_WARN(g_logger) << msg; });
        auto errorFunc = std::function<void(const std::string &)>([](const std::string &msg)
                                                                  { LOG_ERROR(g_logger) << msg; });
        auto criticalFunc = std::function<void(const std::string &)>([](const std::string &msg)
                                                                     { LOG_CRITICAL(g_logger) << msg; });
        getGlobalNamespace(L)
            .beginNamespace("Log")
            .addFunction("debug", debugFunc)
            .addFunction("info", infoFunc)
            .addFunction("warn", warnFunc)
            .addFunction("error", errorFunc)
            .addFunction("critical", criticalFunc)
            .endNamespace();
    }
    void l_ThreadMgrAPI(lua_State *L)
    {
        std::function<void(const std::string &)> newLuaService = [](const std::string &name)
        { 
                LuaService* lua_service = new LuaService(name);
                ThreadMgr::GetInstance()->PushActorToChildThread(lua_service); };

        getGlobalNamespace(L)
            .beginNamespace("ThreadMgr")
            .addFunction("newLuaService", newLuaService)
            .endNamespace();
    }

    namespace LuaAPI
    {
        void RegisterLuaAPI(lua_State *L)
        {
            // 导出cpp接口给lua使用
            l_LogAPI(L);
            l_ThreadMgrAPI(L);
        }

    } // namespace LuaAPI

} // namespace ecsfrm
