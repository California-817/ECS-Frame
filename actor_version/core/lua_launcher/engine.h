#ifndef __ECS_FRAME_ENGINE_H__
#define __ECS_FRAME_ENGINE_H__
#include<string>
/// @brief launcher
class EcsEngine
{
public:
    EcsEngine(int server_id);
    virtual ~EcsEngine();
    void LogInfo(const std::string& msg);
};
#endif