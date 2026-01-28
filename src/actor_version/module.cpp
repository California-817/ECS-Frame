#include "module.h"
#include <dlfcn.h>
#include "log.h"
namespace ecsfrm
{
    static Logger::ptr g_logger = LogSystemUtil::RegisterLogger("system");
    /// @brief 模块卸载的自定义删除器
    class ModuleCloser
    {
    public:
        ModuleCloser(void *handle)
            : _handle(handle)
        {
        }
        void operator()(Module *ptr)
        {
            //1.delete 模块本身
            delete ptr;
            //2.关闭动态库
            int ret = dlclose(_handle);
            if (ret)
            {
                LOG_ERROR(g_logger) << "dlclose " << _handle << " failed, " << dlerror();
            }
            LOG_DEBUG(g_logger) << "delete module " << ptr->GetName();
        }

    private:
        void *_handle = nullptr;
    };
    /// @brief 运行时加载动态库并生成模块
    /// @param lib_name 
    /// @return 
    static Module::ptr Libary_Init(const char *lib_name)
    {
        do
        {
            void *handle = dlopen(lib_name, RTLD_NOW | RTLD_GLOBAL);
            if (!handle)
            {
                LOG_ERROR(g_logger) << "dlopen " << lib_name << " failed, " << dlerror();
                break;
            }
            create_module interface = (create_module)dlsym(handle, "CreateModule");
            if (!interface)
            {
                LOG_ERROR(g_logger) << "dlsym " << lib_name << " failed, " << dlerror();
                dlclose(handle);
                break;
            }
            Module *module = interface();
            return std::shared_ptr<Module>(module, ModuleCloser(handle));
        } while (false);
        return nullptr;
    }
    Module::Module(const std::string &name)
        : _name(name)
    {
    }
    bool Module::OnAppStart()
    {
        LOG_DEBUG(g_logger) << "Module::OnAppStart " << _name;
        return true;
    }
    bool Module::OnDistory()
    {
        LOG_DEBUG(g_logger) << "Module::OnDistory " << _name;
        return true;
    }
    bool ModuleMgr::Init(const std::string &path)
    {
        // AddModule(path, Libary_Init(path.c_str()));
        // todo
    }
    bool ModuleMgr::OnAppStart()
    {
        LOCK_GUARD(lock, _mutex);
        for (auto &i : _modules)
        {
            i.second->OnAppStart();
        }
    }
    void ModuleMgr::DestroyModule(const std::string &module_name)
    {
        LOCK_GUARD(lock, _mutex);
        auto iter = _modules.find(module_name);
        if (iter != _modules.end())
        {
            iter->second->OnDistory();
            _modules.erase(iter);
        }
    }
    void ModuleMgr::DestroyAll()
    {
        LOCK_GUARD(lock, _mutex);
        for (auto &i : _modules)
        {
            i.second->OnDistory();
        }
        _modules.clear();
    }
    void ModuleMgr::AddModule(const std::string &lib_name, Module::ptr module)
    {
        LOCK_GUARD(lock, _mutex);
        _modules[lib_name] = module;
    }
} // namespace ecsfrm
