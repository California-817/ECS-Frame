#ifndef __ECS_FRAME_CONSOLE_H__
#define __ECS_FRAME_CONSOLE_H__
#include "actor.h"
#include <thread>
#include "singleton.h"
#include"util.h"
namespace ecsfrm
{
    class Cmd2Func : public Singleton<Cmd2Func>
    {
    public:
        typedef std::function<void(const std::string &)> CmdFunc;
        /// @brief 初始化框架基本命令
        void Init();
        /// @brief 处理命令
        /// @param cmd
        void ProcessCmd(const std::string &cmd);
        /// @brief 注册命令处理函数
        /// @param cmd
        /// @param func
        void RegisterCmd(const std::string &cmd, CmdFunc func, const std::string &help);

    private:
        /// 一些基本框架命令的处理函数

        /// @brief 帮助
        /// @param cmd --help
        void CmdHelp(const std::string &cmd);
        /// @brief 显示版本信息
        /// @param cmd --version
        void CmdVersion(const std::string &cmd);
        /// @brief 模块管理
        /// @param cmd --module
        void CmdModule(const std::string &cmd);
        /// @brief 线程管理
        /// @param cmd
        void CmdThread(const std::string &cmd);
        /// @brief 网络管理
        /// @param cmd 
        void CmdNet(const std::string &cmd);
    private:
        std::unordered_map<std::string, std::string> _cmd2help;
        std::unordered_map<std::string, CmdFunc> _cmd2func;
        std::mutex _mtx;
    };
    /// @brief 控制台actor: 提供程序命令行交互功能
    class Console : public Actor
    {
    public:
        /// @brief 更新函数
        virtual void Update() override;
        /// @brief 初始化函数
        virtual bool Init() override;
        /// @brief 注册这个actor感兴趣的所有消息类型对应的msgid
        virtual void RegisterMsgFunc() override;
        /// @brief 销毁线程
        virtual void Dispose() override;
        virtual std::string GetTypeName() override
        {
            return Util::GetTypeString<Console>();
        }

    private:
        void thread_func();

    private:
        std::thread *_thread = nullptr;
        std::mutex _mtx;
        std::list<std::string> _cmd_list;
        std::atomic_bool _b_running{false};
    };
} // namespace ecsfrm

#endif