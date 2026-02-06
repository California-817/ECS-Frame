#include "config.h"
#include "util.h"
#include "log.h"
namespace ecsfrm
{
    static Logger::ptr g_logger = LogSystemUtil::RegisterLogger("system");
    ConfigMgr::ConfigMgr(const char *path)
        : _path(path)
    {
    }
    bool ConfigMgr::Init()
    {
        auto files = Util::GetFilesBySuffix(_path, ".yaml");
        for (auto &file : files)
        {
            try
            {
                auto node = YAML::LoadFile(file);
                int server_id = node["id"].as<uint32_t>();
                _config.insert(std::make_pair("server_id", std::to_string(server_id)));
            }
            catch (...)
            {
                LOG_ERROR(g_logger) << "load config file " << file << " error";
            }
        }
    }
    std::string ConfigMgr::GetConfig(const std::string &key) 
    {
        return _config[key];
    }

} // namespace ecsfrm
