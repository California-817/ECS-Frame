#ifndef __ECS_FRAME_SERVER_APP_H__
#define __ECS_FRAME_SERVER_APP_H__
#include <signal.h>
#include "Idisposable.h"
#include "global.h"
#include "thread_mgr.h"
#include "util.h"
#include "network_listen.h"
#include "module.h"
#include "console.h"
#include "log.h"
namespace ecsfrm
{
    static Logger::ptr g_logger = LogSystemUtil::RegisterLogger("app");
    /// @brief 每个app进程的启动主函数
    /// @tparam T app的具体类型
    /// @param argc
    /// @param argv
    /// @return
    template <class T>
    int MainTemplate(int argc, char **argv)
    {
        T *app = nullptr;
        do
        {
            app = new T(101);
            if (!app->InitApp())
                break;
            if (!app->StartApp())
                break;
            app->RunApp();
            app->Dispose();
        } while (false);
        delete app;
        return -1;
    }
    /// @brief 捕获到退出信号时的处理函数
    /// @param
    void stop_handler(int signal);
    class ServerApp : public IDisposable
    {
    public:
        friend void stop_handler(int signal);
        ServerApp(uint32_t server_id)
            : _server_id(server_id), _app_type(APP_TYPE::APP_COMMON)
        {
            // 捕获退出信号
            struct sigaction sa;
            sa.sa_handler = stop_handler;
            sigemptyset(&sa.sa_mask);
            sa.sa_flags = SA_RESTART;
            sigaction(SIGINT, &sa, NULL);

            // init all instance
            ThreadMgr::CreateInstance();
            Global::CreateInstance(server_id);
            ModuleMgr::CreateInstance();
            Cmd2Func::CreateInstance();

            _thread_mgr = ThreadMgr::GetInstance();
        }
        /// @brief 销毁接口
        virtual void Dispose() override
        {
            ModuleMgr::GetInstance()->DestroyAll();
            if (_thread_mgr)
                _thread_mgr->Dispose();
        }
        /// @brief 初始化app接口
        /// @return success
        virtual bool InitApp()
        {
            do
            {
                for (int i = 0; i < 3; ++i)
                    _thread_mgr->NewThread();
                if (!ModuleMgr::GetInstance()->Init("./modules"))
                {
                    LOG_ERROR(g_logger) << "ModuleMgr::Init failed";
                    break;
                }
                Cmd2Func::GetInstance()->Init();
                Global::GetInstance()->YearDay = Util::GetYearDay();
                Global::GetInstance()->TimeStamp = Util::GetTimeStamp();
                return true;
            } while (false);
            return false;
        }
        /// @brief 启动app
        bool StartApp()
        {
            if (_is_running)
                return true;
            ASSERT_INFO(_thread_mgr);
            bool ret = true;
            // 1. 启动线程管理器
            _thread_mgr->Start();
            // 2. 添加服务端必要的网络NetWork和控制台actor
            NetworkListen *network_listen = new NetworkListen();
            ret &= network_listen->Listen("0.0.0.0", 8080);
            _thread_mgr->PushActorToChildThread(network_listen);
            _thread_mgr->PushActor(new Console());
            // 3. 调用模块的OnAppStart
            ret &= ModuleMgr::GetInstance()->OnAppStart();
            _is_running = true;
            return ret;
        }
        /// @brief 运行app
        void RunApp()
        {
            LOG_INFO(g_logger) << "server begin run...";
            ASSERT_INFO(_thread_mgr);
            while (_is_running)
            {
                if (!_thread_mgr->InGameLoop())
                {
                    break;
                }
                _thread_mgr->Update();
                Global::GetInstance()->TimeStamp = Util::GetTimeStamp();
                Global::GetInstance()->YearDay = Util::GetYearDay();
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            LOG_INFO(g_logger) << "server stop";
        }

    private:
        ThreadMgr *_thread_mgr = nullptr;
        uint64_t _server_id;
        APP_TYPE _app_type;
        static std::atomic_bool _is_running;
    };
    std::atomic_bool ServerApp::_is_running{false};
    void stop_handler(int signal)
    {
        if (signal == SIGINT)
        {
            LOG_INFO(g_logger) << "Catch signal: SIGINT";
            ServerApp::_is_running = false;
        }
    }
} // namespace ecsfrm

#endif