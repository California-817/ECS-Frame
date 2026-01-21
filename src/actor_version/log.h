#ifndef __ECS_FRAME_LOG_H__
#define __ECS_FRAME_LOG_H__
#include <spdlog/spdlog.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <unordered_set>
#include <spdlog/fmt/bin_to_hex.h>
#include <mutex>
#include"macro.h"
/*log macro*/
#define LOG_REGISTER(name) \
    ecsfrm::LogSystemUtil::RegisterLogger(name)

#define LOG_DEBUG(logger) LOG_LEVEL(spdlog::level::level_enum::debug, logger)
#define LOG_INFO(logger) LOG_LEVEL(spdlog::level::level_enum::info, logger)
#define LOG_WARN(logger) LOG_LEVEL(spdlog::level::level_enum::warn, logger)
#define LOG_ERROR(logger) LOG_LEVEL(spdlog::level::level_enum::err, logger)
#define LOG_CRITICAL(logger) LOG_LEVEL(spdlog::level::level_enum::critical, logger)

#define LOG_HEX_DATA(logger, container) \
    if (logger)                         \
    logger->log(spdlog::level::debug, container)

#define LOG_LEVEL(lev, logger)                        \
    if (logger && lev >= logger->get_level())         \
    ecsfrm::LogEvent(lev, logger, __FILE__, __LINE__) \
        .GetStream()

namespace ecsfrm
{
    /// @brief 封装spdlog的logger
    class Logger
    {
    public:
        typedef std::shared_ptr<Logger> ptr;
        Logger(const std::string &name)
            : _name(name)
        {
            _logger = std::make_shared<spdlog::logger>(name,
                                                       std::make_shared<spdlog::sinks::ansicolor_stdout_sink_st>());
            _logger->set_level(spdlog::level::debug);
            _logger->set_pattern("[%Y-%m-%d %T.%e] "
                                 "%^[%l]%$ [%n] [thread:%t] %v");
        }
        void log(spdlog::level::level_enum lev, const std::string &msg)
        {
            LOCK_GUARD(lock, _mtx);
            return _logger->log(lev, msg);
        }
        template <typename Container>
        void log(spdlog::level::level_enum lev, const Container &cnta)
        {
            LOCK_GUARD(lock, _mtx);
            return _logger->log(lev, "{}", spdlog::to_hex(cnta));
        }
        void set_level(spdlog::level::level_enum lev)
        {
            LOCK_GUARD(lock, _mtx);
            return _logger->set_level(lev);
        }
        void set_pattern(const std::string &pattern)
        {
            LOCK_GUARD(lock, _mtx);
            return _logger->set_pattern(pattern);
        }
        void reset_logger(std::shared_ptr<spdlog::logger> logger)
        {
            LOCK_GUARD(lock, _mtx);
            _logger.reset();
            _logger = logger;
        }
        void set_sinks(std::vector<spdlog::sink_ptr> sinks)
        {
            LOCK_GUARD(lock, _mtx);
            _logger->sinks().swap(sinks);
        }
        const std::string &get_name() const
        {
            return _name;
        }
        spdlog::level::level_enum get_level()
        {
            LOCK_GUARD(lock, _mtx);
            return _logger->level();
        }

    private:
        std::shared_ptr<spdlog::logger> _logger;
        std::mutex _mtx;
        std::string _name;
    };
    /// @brief 日志管理单元
    class LogSystemUtil
    {
    public:
        typedef std::unordered_map<std::string, Logger::ptr> LoggerMap;
        /// @brief 注册日志器
        /// @param name
        /// @param is_replace 是否覆盖已经存在的同名logger
        /// @return is success
        static Logger::ptr RegisterLogger(const std::string &name, bool is_replace = false);
        /// @brief 通过配置文件修改所有日志配置
        /// @return is success
        static bool ConfigLoggers();
        static LoggerMap &GetLoggerMap();
        static std::mutex &GetMutex();
    };

    /// @brief 日志事件封装
    struct LogEvent
    {
        LogEvent(spdlog::level::level_enum level, std::shared_ptr<Logger> logger, const char *mfile, int mline)
            : lev(level), logr(logger), file(mfile), line(mline)
        {
        }
        std::stringstream &GetStream()
        {
            ss << "[" << file << ":" << line << "]: ";
            return ss;
        }
        /// @brief 自动输出日志
        ~LogEvent()
        {
            logr->log(lev, ss.str());
            if (logr->get_level() == spdlog::level::critical)
                _exit(-1);
        }
        std::stringstream ss;
        std::shared_ptr<Logger> logr;
        const char *file;
        int line;
        spdlog::level::level_enum lev;
    };
} // namespace ecsfrm

#endif
