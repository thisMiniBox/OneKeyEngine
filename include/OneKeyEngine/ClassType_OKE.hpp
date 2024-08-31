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
    virtual int define_virtual()
    { 
        return 0;
    }
public:


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

}
#endif