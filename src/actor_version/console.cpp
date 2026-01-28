#include "console.h"
#include <sstream>
#include "log.h"
namespace ecsfrm
{
    static Logger::ptr g_logger = LogSystemUtil::RegisterLogger("system");
#define REGISTER_CMD(cmd, func, help)        \
    _cmd2func[#cmd] = BindFunP1(func, this); \
    _cmd2help[#cmd] = std::string(help);
    void Cmd2Func::Init()
    {
        // 初始化一些基本框架命令
        LOCK_GUARD(lock, _mtx);
        REGISTER_CMD(--help, &Cmd2Func::CmdHelp, "show all commands");
    }
    void Cmd2Func::RegisterCmd(const std::string &cmd, CmdFunc func, const std::string &help)
    {
        LOCK_GUARD(lock, _mtx);
        _cmd2func[cmd] = func;
        _cmd2help[cmd] = help;
    }
    void Cmd2Func::ProcessCmd(const std::string &cmd)
    {
        CmdFunc func;
        {
            LOCK_GUARD(lock, _mtx);
            auto iter = _cmd2func.find(cmd);
            if (iter != _cmd2func.end())
                func = iter->second;
        }
        if (func)
            func(cmd);
        else
        {
            std::cout << "cmd is invalid:" << cmd << std::endl;
            CmdHelp("--help");
        }
    }
    void Console::Update()
    {
        std::string cmd;
        {
            LOCK_GUARD(lock, _mtx);
            if (!_cmd_list.empty())
            {
                cmd = _cmd_list.front();
                _cmd_list.pop_front();
            }
        }
        if (!cmd.empty())
        {
            Cmd2Func::GetInstance()->ProcessCmd(cmd);
        }
    }
    bool Console::Init()
    {
        _b_running = true;
        _thread = new std::thread(BindFunP0(&Console::thread_func, this));
        return true;
    }
    void Console::RegisterMsgFunc()
    {
        // nothing
    }
    void Console::Dispose()
    {
        _b_running = false;
        // 给console线程发一个命令行消息 防止其一直等待输入
        std::cout << "please input any command to exit" << std::endl;
        if (_thread && _thread->joinable())
        {
            _thread->join();
            delete _thread;
            _thread = nullptr;
        }
        LOCK_GUARD(lock, _mtx);
        _cmd_list.clear();
    }
    void Console::thread_func()
    {
        pthread_setname_np(pthread_self(), "thread_console");
        std::string cmd;
        while (_b_running)
        {
            cmd.clear();
            std::getline(std::cin, cmd);
            if (!cmd.empty())
            {
                LOCK_GUARD(lock, _mtx);
                _cmd_list.push_back(cmd);
            }
        }
        LOG_DEBUG(g_logger) << "thread console stop";
    }
    void Cmd2Func::CmdHelp(const std::string &cmd)
    {
        std::stringstream ss;
        ss << "Help:" << std::endl;
        for (auto &item : _cmd2help)
        {
            ss << "\t" << item.first << "                         "
               << item.second << std::endl;
        }
        std::cout << ss.str();
    }
} // namespace ecsfrm
