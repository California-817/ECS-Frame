#pragma once
#include "../actor.h"
#include "../module.h"
#include "../log.h"
#include "../util.h"
#include "../thread_mgr.h"
static ecsfrm::Logger::ptr g_logger = ecsfrm::LogSystemUtil::RegisterLogger("test");
class TestActor : public ecsfrm::Actor
{
public:
    virtual void Dispose() override
    {
        LOG_INFO(g_logger) << "TestActor Dispose";
    }
    virtual bool Init() override
    {
        LOG_INFO(g_logger) << "TestActor Init";
        return true;
    }
    virtual void Update() override
    {
        LOG_INFO(g_logger) << "TestActor Update";
    }
    virtual void RegisterMsgFunc() override
    {
    }
    virtual std::string GetTypeName() override
    {
        return ecsfrm::Util::GetTypeString<TestActor>();
    }
};

#define ADD_ACTOR(actor)                                           \
    ecsfrm::Actor *atr = new actor();                              \
    ecsfrm::ThreadMgr::GetInstance()->PushActorToChildThread(atr); \
    _actors.push_back(atr);

class TestModule : public ecsfrm::Module
{
public:
    TestModule(const std::string &name)
        : Module(name)
    {
    }
    virtual bool OnAppStart() override
    {
        ecsfrm::Module::OnAppStart();
        ADD_ACTOR(TestActor);
        return true;
    }
    virtual bool OnDistory() override
    {
        ecsfrm::Module::OnDistory();
        for (auto &actor : _actors)
        {
            actor->SetNoActive();
        }
        _actors.clear();
        return true;
    }

private:
    std::list<ecsfrm::Actor *> _actors;
};
extern "C"
{
    /// @brief 创建模块函数
    /// @return
    ecsfrm::Module *CreateModule()
    {
        return new TestModule("test");
    }
}