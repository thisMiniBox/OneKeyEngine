#include <OneKeyEngine/scenes/Module.hpp>
namespace OneKeyEngine
{
namespace Scenes
{
namespace Module
{
    
Module::Module(ScenesObject *obj):object(obj)
{
}


WindowInput *Module::input() const
{
    if(object)
    {
        return object->scenes->input();
    }
    return nullptr;
}

float Module::get_delta_time() const
{
    return object->scenes->get_delta_time();
}

WindowDefine::WindowBase *Module::get_window() const
{
    return object->scenes->get_window();
}

WindowDefine::WindowOpenGLWin32 *Module::get_opengl_window() const
{
    return dynamic_cast<WindowDefine::WindowOpenGLWin32 *>(object->scenes->get_window());
}

bool Module::is_opengl_window() const
{
    return object->scenes->manage->isOpenGLWindow;
}

GL::ShaderGL *Module::get_shader()
{
    auto hash = typeid(*this).hash_code();
    //std::cout<<typeid(*this).name()<<hash<<std::endl;
    auto it = object->scenes->manage->s_shader.find(hash);
    if (it != object->scenes->manage->s_shader.end())
        return &it->second;
    return &object->scenes->manage->s_shader[hash];
}

GL::Buffer *Module::get_buffer()
{
    auto hash = typeid(*this).hash_code();
    auto it = object->scenes->manage->s_buffer.find(hash);
    if (it != object->scenes->manage->s_buffer.end())
        return &it->second;
    return &object->scenes->manage->s_buffer[hash];
}

GL::Element *Module::get_buffer_element()
{
    auto hash = typeid(*this).hash_code();
    auto& elementList=object->scenes->manage->s_element;
    auto it = elementList.find(hash);
    if (it != elementList.end())
        return &it->second;
    return &elementList[hash];
}

template <typename type>
GL::ShaderGL *Module::get_shader_of()
{
    auto hash = typeid(type).hash_code();
    //std::cout<<typeid(this).name()<<hash<<std::endl;
    auto it = object->scenes->manage->s_shader.find(hash);
    if (it != object->scenes->manage->s_shader.end())
        return &it->second;
    return &object->scenes->manage->s_shader[hash];
}

} // namespace Module

ScenesObject::ScenesObject(const std::string &name_):name(name_)
{
    transform.userData=this;
}
ScenesObject::~ScenesObject()
{
    //父子对象由场景管理
}
void ScenesObject::start()
{
    for (auto& mod : objectModules)
    {
        if (mod && mod->enable)
        {
            mod->start();
        }
    }
}

void ScenesObject::update()
{
    for (auto &mod : objectModules)
    {
        if (mod && mod->enable)
        {
            mod->update();
        }
    }
}

void ScenesObject::exit()
{
    for (auto &mod : objectModules)
    {
        if (mod && mod->enable)
        {
            mod->exit();
        }
    }
}
void ScenesObject::draw(int l)
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
void ScenesObject::size(int w,int h)
{
    for (auto &mod : objectModules)
    {
        if (mod && mod->enable)
        {
            mod->size(w, h);
        }
    }
}
ScenesObject *ScenesObject::get_parent() const
{
    return (ScenesObject*)(transform.get_parent());
}
ScenesObject *ScenesObject::get_child(int i) const
{
    return (ScenesObject*)(transform.get_child(i)->userData);
}
int ScenesObject::get_childs_count() const
{
    return transform.get_childs_count();
}
VEC::Vec2 ScenesObject::get_relative_mouse_position_NDC() const
{
    auto window=scenes->get_window();
    auto zeroPos = transformMat4 * VEC::Vec4(0, 0, 0, 1);
    return window->pixel_to_NDC_LT(scenes->input()->get_mouse_position())- VEC::Vec2(zeroPos.x, zeroPos.y);
}


} // namespace Scenes

} // namespace OneKeyEngine
