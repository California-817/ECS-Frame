#ifndef __ECS_FRAME_MODULE_H__
#define __ECS_FRAME_MODULE_H__
#include "Isn_obj.h"
#include "singleton.h"
#include <string>
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
    class ModuleMgr : public Singleton<ModuleMgr>
    {
    public:
        /// @brief 初始化模块管理器
        /// @param path
        /// @return
        bool Init(const std::string &path);
        /// @brief 模块启动函数-->在程序启动时调用
        /// @return
        bool OnAppStart();
        /// @brief 卸载指定name的module
        /// @param module_name 
        void DestroyModule(const std::string &module_name);
        /// @brief 卸载所有模块
        void DestroyAll();
        /// @brief 获取模块信息
        /// @return 
        std::string Info();
    private:
        /// @brief 添加模块
        /// @param name 模块名称
        /// @param create_func 创建模块函数
        void AddModule(const std::string &module_name, Module::ptr module);

    private:
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