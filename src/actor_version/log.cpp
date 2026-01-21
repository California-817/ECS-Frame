#include "log.h"
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
namespace ecsfrm
{
    Logger::ptr LogSystemUtil::RegisterLogger(const std::string &name, bool is_replace)
    {
        std::lock_guard<std::mutex> lock(GetMutex());
        LOCK_GUARD(lock,GetMutex());
        if (GetLoggerMap().find(name) == GetLoggerMap().end() || is_replace)
        {
            Logger::ptr logger = std::make_shared<Logger>(name);
            GetLoggerMap()[name] = logger;
            return logger;
        }
        return GetLoggerMap()[name];
    }
    bool LogSystemUtil::ConfigLoggers()
    {
        // todo
    }
    LogSystemUtil::LoggerMap &LogSystemUtil::GetLoggerMap()
    {
        static LoggerMap s_map;
        return s_map;
    }
    std::mutex &LogSystemUtil::GetMutex()
    {
        static std::mutex s_mtx;
        return s_mtx;
    }

} // namespace ecsfrm