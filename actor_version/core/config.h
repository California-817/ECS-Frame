#ifndef __ECS_FRAME_CONFIG_H__
#define __ECS_FRAME_CONFIG_H__
#include "singleton.h"
#include <yaml-cpp/yaml.h>
namespace ecsfrm
{
    class ConfigMgr : public Singleton<ConfigMgr>
    {
    public:
        ConfigMgr(const char *path);
        bool Init();
        std::string GetConfigPath() const { return _path; }
        std::string GetConfig(const std::string &key) ;

    private:
        const std::string _path;
        std::map<std::string, std::string> _config;
    };
    class ConfigUtil
    {
    public:
        static int GetConfigInt(const std::string &key)
        {
            return std::atoi(ConfigMgr::GetInstance()->GetConfig(key).c_str());
        }
    };
} // namespace ecsfrm

#endif