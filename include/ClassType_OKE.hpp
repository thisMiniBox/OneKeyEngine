#pragma once
#ifndef __OKE_CLASS_TYPE_H
#define __OKE_CLASS_TYPE_H
#include <memory>

namespace OneKeyEngine{

template<typename T>
using WeaPtr = std::weak_ptr<T>;

template<typename T>
using ShaPtr = std::shared_ptr<T>;

class ClassType_OKE
{
private:

public:
    enum class DeviceSystem{
        Unknown,
        Windows,
        Unix,
        Apple,
    };
    virtual DeviceSystem get_device_system();
    template<typename DerivedClass>
    static DerivedClass* is_derived_class(ClassType_OKE* baseClass)
    {
        return dynamic_cast<DerivedClass*>(baseClass);
    }
    template<typename DerivedClass>
    DerivedClass* is_derived_class()
    {
        return dynamic_cast<DerivedClass*>(this);
    }
};
ClassType_OKE::DeviceSystem ClassType_OKE::get_device_system()
{
    #if defined(_WIN32)
        return ClassType_OKE::DeviceSystem::Windows;
    #elif defined(__unix__) || defined(__unix) // All UNIX-like systems, including Linux
        return ClassType_OKE::DeviceSystem::Unix;
    #elif defined(__APPLE__)
        return ClassType_OKE::DeviceSystem::Apple;
    #else
        return ClassType_OKE::DeviceSystem::Unknown;
    #endif
}


}
#endif