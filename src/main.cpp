#include <iostream>
#include "actor_version/util.h"
#include "actor_version/thread_mgr.h"
#include "actor_version/network_listen.h"
#include"actor_version/module.h"
static ecsfrm::Logger::ptr g_logger = ecsfrm::LogSystemUtil::RegisterLogger("App");
int main()
{
    ecsfrm::ModuleMgr::CreateInstance();
    ecsfrm::Global::CreateInstance(101);
    ecsfrm::ThreadMgr::CreateInstance();

    for (int i = 0; i < 3; i++)
        ecsfrm::ThreadMgr::GetInstance()->NewThread();
    ecsfrm::ThreadMgr::GetInstance()->Start();
    ecsfrm::NetworkListen* listen=new ecsfrm::NetworkListen();
    listen->Listen("0.0.0.0", 8080);
    ecsfrm::ThreadMgr::GetInstance()->PushNetwork(ecsfrm::APP_TYPE::APP_LISTEN,listen);

    ecsfrm::ModuleMgr::GetInstance()->Init("./modules");
    ecsfrm::ModuleMgr::GetInstance()->OnAppStart();
    while (true)
    {
        if (!ecsfrm::ThreadMgr::GetInstance()->InGameLoop())
        {
            break;
        }
        ecsfrm::ThreadMgr::GetInstance()->Update();
        ecsfrm::Global::GetInstance()->TimeStamp = ecsfrm::Util::GetTimeStamp();
        ecsfrm::Global::GetInstance()->YearDay = ecsfrm::Util::GetYearDay();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    ecsfrm::ModuleMgr::GetInstance()->DestroyAll();
    ecsfrm::ThreadMgr::GetInstance()->Dispose();
    LOG_INFO(g_logger) << "my ecs server ending";
    return 0;
}