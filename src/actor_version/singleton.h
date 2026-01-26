#ifndef __ECS_FRAME_SINGLETON_H__
#define __ECS_FRAME_SINGLETON_H__
#include "macro.h"
namespace ecsfrm
{
    template <class T>
    class Singleton
    {
    public:
        /// @brief 创建单例实例
        /// @tparam ...Args
        /// @param ...args
        /// @return
        template <class... Args>
        static T *CreateInstance(Args &&...args)
        {
            if (_instance == nullptr)
                _instance = new T(std::forward<Args>(args)...);
            return _instance;
        }
        /// @brief 获取单例实例
        /// @return
        static T *GetInstance()
        {
            ASSERT_INFO(_instance);
            return _instance;
        }
        /// @brief 销毁单例实例
        static void DestoryInstance()
        {
            ASSERT_INFO(_instance);
            delete _instance;
            _instance = nullptr;
        }

    private:
        static T *_instance;
    };
    template <class T>
    T *Singleton<T>::_instance = nullptr;
} // namespace ecsfrm

#endif