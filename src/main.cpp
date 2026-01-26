#include<iostream>
#include"actor_version/thread_mgr.h"
int main()
{
    auto instance=ecsfrm::ThreadMgr::CreateInstance();
    instance->NewThread();
    instance->Start();
    while (true)
    {
        if(!instance->InGameLoop())
        {
            break;
        }
    }
    std::cout<<"my ecs server ending"<<std::endl;
    return 0;
}