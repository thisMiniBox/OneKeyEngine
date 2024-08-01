#pragma once

#include"../Vector_OKE.hpp"
#include"Scenes.hpp"
#include"ScenesObject.hpp"
#include<vector>

namespace OneKeyEngine
{
namespace Scenes
{
namespace Module
{


class Module
{
public:
    bool enable=true;
    ScenesObject* object=nullptr;

    Module(ScenesObject* obj=nullptr);

    /// @brief 创建对象基本参数赋值好后调用,整个生命周期只调用一次
    virtual void init(){};
    /// @brief 每帧调用，已经自动设置变换矩阵，所以可以直接绘制到窗口，也可通过object获取transformMat4，这是投影矩阵与变换矩阵的乘积
    virtual void draw(){};
    /// @brief 当窗口大小改变时调用
    /// @param w 
    /// @param h 
    virtual void size(int w,int h){};
    /// @brief 进入场景时调用
    virtual void start(){};
    /// @brief 每帧绘制前调用
    virtual void update(){};
    /// @brief 离开场景时调用
    virtual void exit(){};

    WindowInput* input()const;
    float get_delta_time()const;

    WindowDefine::WindowBase* get_window()const;
    WindowDefine::WindowOpenGLWin32* get_opengl_window()const;
    bool is_opengl_window()const;
    /// @brief 获取自己类型的专属着色器
    /// @return 
    GL::ShaderGL* get_shader();
    template<typename type>
    GL::ShaderGL* get_shader_of();

    GL::Buffer* get_buffer();
    GL::Element* get_buffer_element();


};

inline Module::Module(ScenesObject *obj):object(obj)
{
}


inline WindowInput *Module::input() const
{
    if(object)
    {
        return object->scenes->input();
    }
    return nullptr;
}

inline float Module::get_delta_time() const
{
    return object->scenes->get_delta_time();
}

inline WindowDefine::WindowBase *Module::get_window() const
{
    return object->scenes->get_window();
}

inline WindowDefine::WindowOpenGLWin32 *Module::get_opengl_window() const
{
    return dynamic_cast<WindowDefine::WindowOpenGLWin32 *>(object->scenes->get_window());
}

inline bool Module::is_opengl_window() const
{
    return object->scenes->manage->isOpenGLWindow;
}

inline GL::ShaderGL *Module::get_shader()
{
    auto hash = typeid(*this).hash_code();
    //std::cout<<typeid(*this).name()<<hash<<std::endl;
    auto it = object->scenes->manage->s_shader.find(hash);
    if (it != object->scenes->manage->s_shader.end())
        return &it->second;
    return &object->scenes->manage->s_shader[hash];
}

inline GL::Buffer *Module::get_buffer()
{
    auto hash = typeid(*this).hash_code();
    auto it = object->scenes->manage->s_buffer.find(hash);
    if (it != object->scenes->manage->s_buffer.end())
        return &it->second;
    return &object->scenes->manage->s_buffer[hash];
}

inline GL::Element *Module::get_buffer_element()
{
    auto hash = typeid(*this).hash_code();
    auto& elementList=object->scenes->manage->s_element;
    auto it = elementList.find(hash);
    if (it != elementList.end())
        return &it->second;
    return &elementList[hash];
}

template <typename type>
inline GL::ShaderGL *Module::get_shader_of()
{
    auto hash = typeid(type).hash_code();
    //std::cout<<typeid(this).name()<<hash<<std::endl;
    auto it = object->scenes->manage->s_shader.find(hash);
    if (it != object->scenes->manage->s_shader.end())
        return &it->second;
    return &object->scenes->manage->s_shader[hash];
}

} // namespace Module

inline ScenesObject::ScenesObject(const std::string &name_):name(name_)
{
    transform.userData=this;
}
inline ScenesObject::~ScenesObject()
{
}
inline void ScenesObject::start()
{
    for (auto& mod : objectModules)
    {
        if (mod && mod->enable)
        {
            mod->start();
        }
    }
}

inline void ScenesObject::update()
{
    for (auto &mod : objectModules)
    {
        if (mod && mod->enable)
        {
            mod->update();
        }
    }
}

inline void ScenesObject::exit()
{
    for (auto &mod : objectModules)
    {
        if (mod && mod->enable)
        {
            mod->exit();
        }
    }
}
inline void ScenesObject::draw(int l)
{
    auto window=scenes->get_window();
    auto parent=transform.get_parent();
    if(parent)
    {
        transformMat4=((ScenesObject*)(parent->userData))->transformMat4*transform.to_local_matrix();
    }
    else
    {
        transformMat4=transformMat4*transform.to_local_matrix();
    }

    window->set_transform(transformMat4);
    for (auto &mod : objectModules)
    {
        if (mod && mod->enable)
        {
            mod->draw();
        }
    }
    for (int i = 0; i < transform.get_childs_count(); ++i)
    {
        auto child = transform.get_child(i);
        auto obj = static_cast<ScenesObject *>(child->userData);
        if (obj && obj->enable)
            obj->draw(l);
    }
}
inline void ScenesObject::size(int w,int h)
{
    for (auto &mod : objectModules)
    {
        if (mod && mod->enable)
        {
            mod->size(w, h);
        }
    }
}
inline ScenesObject *ScenesObject::get_parent() const
{
    return (ScenesObject*)(transform.get_parent());
}
inline ScenesObject *ScenesObject::get_child(int i) const
{
    return (ScenesObject*)(transform.get_child(i)->userData);
}
inline int ScenesObject::get_childs_count() const
{
    return transform.get_childs_count();
}
inline VEC::Vec2 ScenesObject::get_relative_mouse_position_NDC() const
{
    auto window=scenes->get_window();
    auto zeroPos = transformMat4 * VEC::Vec4(0, 0, 0, 1);
    return window->pixel_to_NDC_LT(scenes->input()->get_mouse_position())- VEC::Vec2(zeroPos.x, zeroPos.y);
}
} // namespace Scenes

} // namespace OneKey



