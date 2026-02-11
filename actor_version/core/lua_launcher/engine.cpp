#include "engine.h"
#include "log.h"
static ecsfrm::Logger::ptr g_logger = ecsfrm::LogSystemUtil::RegisterLogger("root");
EcsEngine::EcsEngine(int server_id)
{
}
EcsEngine::~EcsEngine()
{
}
void EcsEngine::LogInfo(const std::string& msg)
{
    LOG_INFO(g_logger) << msg;
}