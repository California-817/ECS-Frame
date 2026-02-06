#include "module.h"
#include <dlfcn.h>
#include "protobuf/msg_id.pb.h"
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
            // 1.delete 模块本身
            delete ptr;
            // 2.关闭动态库
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

    ModuleMgr::ModuleMgr(const std::string &path)
        : _path(path)
    {
    }

    bool ModuleMgr::Init()
    {
        return ModuleMgr::init(_path) &
               ModuleMgr::OnAppStart();
    }
    void ModuleMgr::RegisterMsgFunc()
    {
        MailBox::RegisterMsgFunc(Proto::MI_ModuleLoad, BindFunP1(&ModuleMgr::msgLoadModule, this));
        MailBox::RegisterMsgFunc(Proto::MI_ModuleDelete, BindFunP1(&ModuleMgr::msgDeleteModule, this));
        MailBox::RegisterMsgFunc(Proto::MI_ModuleInfo, BindFunP1(&ModuleMgr::msgInfoModule, this));
    }
    void ModuleMgr::Dispose()
    {
        ModuleMgr::destroyAll();
    }
    bool ModuleMgr::init(const std::string &path)
    {
        std::vector<std::string> libs = Util::GetFilesBySuffix(path, ".so");
        if (!libs.empty() && libs[0] == std::string("Failed to open directory: ") + path)
            return false;
        for (auto &lib_name : libs)
        {
            Module::ptr module = Libary_Init(lib_name.c_str());
            if (module)
            {
                addModule(module->GetName(), module);
            }
        }
        return true;
    }
    void ModuleMgr::Update()
    {
        // todo
    }
    bool ModuleMgr::OnAppStart()
    {
        LOCK_GUARD(lock, _mutex);
        for (auto &i : _modules)
        {
            i.second->OnAppStart();
        }
        return true;
    }
    void ModuleMgr::destroyModule(const std::string &module_name)
    {
        LOCK_GUARD(lock, _mutex);
        auto iter = _modules.find(module_name);
        if (iter != _modules.end())
        {
            iter->second->OnDistory();
            _modules.erase(iter);
        }
    }
    void ModuleMgr::destroyAll()
    {
        LOCK_GUARD(lock, _mutex);
        for (auto &i : _modules)
        {
            i.second->OnDistory();
        }
        _modules.clear();
    }
    void ModuleMgr::addModule(const std::string &module_name, Module::ptr module)
    {
        LOCK_GUARD(lock, _mutex);
        _modules[module_name] = module;
    }
    std::string ModuleMgr::Info()
    {
        std::stringstream ss;
        LOCK_GUARD(lock, _mutex);
        ss << "\tModules:[";
        for (auto &i : _modules)
        {
            ss << i.first << " ";
        }
        ss << "]";
        return ss.str();
    }

    int ModuleMgr::msgLoadModule(Packet *packet)
    {
        std::string lib_name; // todo
        Module::ptr module = Libary_Init(lib_name.c_str());
        if (module)
        {
            addModule(module->GetName(), module);
            return 0;
        }
        return -1;
    }
    int ModuleMgr::msgDeleteModule(Packet *packet)
    {
        std::string module_name; // todo
        destroyModule(module_name);
        return 0;
    }
    int ModuleMgr::msgInfoModule(Packet *packet)
    {
        std::cout<<Info()<<std::endl;
        return 0;
    }

} // namespace ecsfrm
