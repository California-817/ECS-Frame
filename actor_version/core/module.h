#ifndef __ECS_FRAME_MODULE_H__
#define __ECS_FRAME_MODULE_H__
#include "Isn_obj.h"
#include "singleton.h"
#include "actor.h"
#include <string>
#include"util.h"
#include <memory>
namespace ecsfrm
{
    /// @brief 模块基类
    class Module : public ISnObject
    {
    public:
        typedef std::shared_ptr<Module> ptr;
        Module(const std::string &name);
        virtual ~Module() = default;
        /// @brief 模块启动函数-->在程序启动时调用
        /// @return
        virtual bool OnAppStart();
        /// @brief 模块销毁函数-->在模块销毁时调用
        /// @return
        virtual bool OnDistory();
        /// @brief 获取模块名字
        /// @return
        std::string GetName() const { return _name; }

    private:
        std::string _name;
    };
    /// @brief 模块管理器
    class ModuleMgr : public Actor
    {
    public:
        ModuleMgr(const std::string &path);
        /// @brief 更新模块,如果有新的模块放到目录里面或者被移除，则也加载这个模块或者卸载这个模块---热更新
        virtual void Update() override;
        /// @brief 初始化函数
        virtual bool Init() override;
        /// @brief 注册这个actor感兴趣的所有消息类型对应的msgid
        virtual void RegisterMsgFunc() override;
        /// @brief 卸载所有模块
        virtual void Dispose() override;
        virtual std::string GetTypeName() override
        {
            return Util::GetTypeString<ModuleMgr>();
        }
        /// @brief 获取模块信息
        /// @return
        std::string Info();

    private:
        /// @brief 模块启动函数-->在程序启动时调用
        /// @return
        bool OnAppStart();
        /// @brief 添加模块
        /// @param name 模块名称
        /// @param create_func 创建模块函数
        void addModule(const std::string &module_name, Module::ptr module);
        /// @brief 初始化模块管理器
        /// @param path
        /// @return
        bool init(const std::string &path);
        /// @brief 卸载指定name的module
        /// @param module_name
        void destroyModule(const std::string &module_name);
        /// @brief 卸载所有模块
        void destroyAll();
        /// @brief 加载模块的msg处理
        /// @param packet 
        /// @return 
        int msgLoadModule(Packet* packet);
        /// @brief 删除模块的msg处理
        /// @param packet 
        /// @return 
        int msgDeleteModule(Packet* packet);
        /// @brief 模块信息查询
        /// @param packet 
        /// @return 
        int msgInfoModule(Packet* packet);
    private:
        std::string _path;
        std::unordered_map<std::string, Module::ptr> _modules;
        std::mutex _mutex;
    };
} // namespace ecsfrm
typedef ecsfrm::Module *(*create_module)(void);

// extern "C"
// {
//     /// @brief 创建模块函数
//     /// @return
//     ecsfrm::Module *CreateModule()
//     {
//         return new ecsfrm::Module("test");
//     }
// }

#endif