#pragma once

#include"../WindowInput_OKE.hpp"
#include"Scenes.hpp"
#include"../Vector_OKE.hpp"
#include<vector>
#include<unordered_set>
#include <algorithm>
#include<functional>
#include<string>



namespace OneKeyEngine
{
namespace Scenes
{
namespace Module
{
    class Module;
}

// 辅助类模板，用于SFINAE
template<typename mod, typename = void>
struct IsModuleOrDerived : std::false_type {};

// 特化，仅当T是Module的子类时为true
template<typename mod>
struct IsModuleOrDerived<mod, std::enable_if_t<std::is_base_of<Module::Module, mod>::value>> : std::true_type {};

class ScenesObject
{
private:
    template <typename mod, typename = std::enable_if_t<IsModuleOrDerived<mod>::value>>
    ShaPtr<mod> create_module()
    {
        return std::make_shared<mod>();
    }
    template <typename mod, typename value, typename = std::enable_if_t<IsModuleOrDerived<mod>::value>>
    ShaPtr<mod> create_module(const value &val)
    {
        return std::make_shared<mod>(val);
    }

public:
    bool enable=true;
    std::string name;
    uint32_t layer=1;
    VEC::Transform transform;
    /// @brief 在绘制过程中存储父对象及摄像机的变换
    VEC::Mat4 transformMat4 = VEC::Mat4(1.0f);
    Scenes *scenes = nullptr;
    std::vector<ShaPtr<Module::Module>> objectModules;

    ScenesObject(const std::string &name = "新建对象");
    ~ScenesObject();
    /// @brief 在进入场景时调用，会依次调用模块的start
    void start();
    /// @brief 每帧调用,顺序与创建顺序有关
    void update();
    /// @brief 在退出场景时调用
    void exit();
    /// @brief 每帧调用,顺序与父子关系有关
    void draw(int lay);
    /// @brief 在窗口大小调整时调用
    void size(int w,int h);

    template <typename mod, typename = std::enable_if_t<IsModuleOrDerived<mod>::value>>
    ShaPtr<mod> add_module()
    {
        auto m = create_module<mod>();
        if (m)
        {
            m->object = this;
            objectModules.push_back(m);
            m->init();
            if (scenes == scenes->manage->get_current_scenes().get())
            {
                m->start();
            }
        }
        return m;
    }
    template <typename mod, typename value, typename = std::enable_if_t<IsModuleOrDerived<mod>::value>>
    ShaPtr<mod> add_module(const value &val)
    {
        auto m = create_module<mod>(val);
        if (m)
        {
            m->object = this;
            objectModules.push_back(m);
            m->init();
            if (scenes == scenes->manage->get_current_scenes().get())
            {
                m->start();
            }
        }
        return m;
    }
    template <typename mod, typename = std::enable_if_t<IsModuleOrDerived<mod>::value>>
    ShaPtr<mod> get_module() const
    {
        for (auto &m : objectModules)
        {
            if (auto casted = std::dynamic_pointer_cast<mod>(m))
            {
                return casted;
            }
        }
        return nullptr;
    }

    template <typename mod, typename = std::enable_if_t<IsModuleOrDerived<mod>::value>>
    std::vector<ShaPtr<mod>> get_modules() const
    {
        std::vector<ShaPtr<mod>> result;
        for (auto &m : objectModules)
        {
            if (auto casted = std::dynamic_pointer_cast<mod>(m))
            {
                result.push_back(casted);
            }
        }
        return result;
    }
    template <typename mod, typename = std::enable_if_t<IsModuleOrDerived<mod>::value>>
    void delete_module()
    {
        for (auto it = objectModules.begin(); it != objectModules.end(); ++it)
        {
            auto &m = *it;
            if (typeid(*m) == typeid(mod))
            {
                objectModules.erase(it); // 从容器中移除智能指针
                return;
            }
        }
    }

    template <typename mod, typename = std::enable_if_t<IsModuleOrDerived<mod>::value>>
    std::vector<ShaPtr<mod>> find_all_modules(ScenesObject *obj)
    {
        std::vector<ShaPtr<mod>> buttons;

        if (obj == nullptr)
        {
            // 如果 obj 为空指针，则递归查找所有场景对象中的模块
            for (int i = 0; i < scenes->get_objects_size(); ++i)
            {
                auto object=scenes->get_object(i);
                if(object->transform.get_parent())
                    continue;
                auto childButtons = find_all_modules<mod>(object.get());
                buttons.insert(buttons.end(), std::make_move_iterator(childButtons.begin()), std::make_move_iterator(childButtons.end()));
            }
            return buttons;
        }

        auto button = obj->get_module<mod>();
        if (button)
        {
            buttons.push_back(button);
        }

        // 递归处理子对象的模块
        for (int i = 0; i < obj->transform.get_childs_count(); ++i)
        {
            auto childObj = static_cast<ScenesObject *>(obj->transform.get_child(i)->userData);
            auto childButtons = find_all_modules<mod>(childObj);
            buttons.insert(buttons.end(), std::make_move_iterator(childButtons.begin()), std::make_move_iterator(childButtons.end()));
        }
        return buttons;
    }

    ScenesObject* get_parent()const;
    ScenesObject* get_child(int i)const;
    int get_childs_count()const; 
    /// @brief 获取鼠标相对此对象为0点的相对窗口的标准化坐标系
    /// @return 
    VEC::Vec2 get_relative_mouse_position_NDC()const;
};




}
}