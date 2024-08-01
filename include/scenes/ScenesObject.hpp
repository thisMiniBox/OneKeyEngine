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
    ShaPtr<mod> create_module();
    template <typename mod, typename value, typename = std::enable_if_t<IsModuleOrDerived<mod>::value>>
    ShaPtr<mod> create_module(const value &val);

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
    ShaPtr<mod>add_module();
    template <typename mod, typename value, typename = std::enable_if_t<IsModuleOrDerived<mod>::value>>
    ShaPtr<mod>add_module(const value &val);
    template <typename mod, typename = std::enable_if_t<IsModuleOrDerived<mod>::value>>
    ShaPtr<mod>get_module() const;

    template <typename mod, typename = std::enable_if_t<IsModuleOrDerived<mod>::value>>
    std::vector<ShaPtr<mod>> get_modules() const;
    template <typename mod, typename = std::enable_if_t<IsModuleOrDerived<mod>::value>>
    void delete_module();

    template <typename mod, typename = std::enable_if_t<IsModuleOrDerived<mod>::value>>
    std::vector<ShaPtr<mod>> find_all_modules(ScenesObject* obj);

    ScenesObject* get_parent()const;
    ScenesObject* get_child(int i)const;
    int get_childs_count()const; 
    VEC::Vec2 get_relative_mouse_position_NDC()const;
};

template<typename mod, typename >
inline ShaPtr<mod>ScenesObject::create_module()
{
    return std::make_shared<mod>();
}

template <typename mod, typename value, typename>
inline ShaPtr<mod>ScenesObject::create_module(const value &val)
{
    return std::make_shared<mod>(val);
}

template <typename mod, typename >
inline ShaPtr<mod> ScenesObject::add_module()
{
    auto m = create_module<mod>();
    if (m)
    {
        m->object=this;
        objectModules.push_back(m);
        m->init();
        if(scenes==scenes->manage->get_current_scenes().get())
        {
            m->start();
        }
    }
    return m;
}
template <typename mod, typename value, typename>
inline ShaPtr<mod>ScenesObject::add_module(const value &val)
{
    auto m = create_module<mod,value>(val);
    if (m)
    {
        m->object = this;

        objectModules.push_back(m);
        m->init();
        if (scenes == scenes->manage->get_current_scenes())
        {
            m->start();
        }
    }
    return m;
}
template <typename mod, typename>
inline ShaPtr<mod>ScenesObject::get_module() const
{
    for (auto& m : objectModules)
    {
        if (auto casted = std::dynamic_pointer_cast<mod>(m))
        {
            return casted;
        }
    }
    return nullptr;
}

template <typename mod, typename>
inline std::vector<ShaPtr<mod>> ScenesObject::get_modules() const
{
    std::vector<ShaPtr<mod>> result;
    for (auto& m : objectModules)
    {
        if (auto casted = std::dynamic_pointer_cast<mod>(m))
        {
            result.push_back(casted);
        }
    }
    return result;
}

template <typename mod, typename>
inline void ScenesObject::delete_module()
{
    for (auto it = objectModules.begin(); it != objectModules.end(); ++it)
    {
        auto& m = *it;
        if (typeid(*m) == typeid(mod))
        {
            objectModules.erase(it);  // 从容器中移除智能指针
            return;
        }
    }
}

template <typename mod, typename>
inline std::vector<ShaPtr<mod>> ScenesObject::find_all_modules(ScenesObject *obj)
{
    std::vector<ShaPtr<mod>> buttons;

    if (obj == nullptr)
    {
        // 如果 obj 为空指针，则递归查找所有场景对象中的模块
        for (int i = 0; i < scenes->get_objects_size(); ++i)
        {
            auto childButtons = find_all_modules<mod>(scenes->get_object(i).get());
            buttons.insert(buttons.end(), std::make_move_iterator(childButtons.begin()), std::make_move_iterator(childButtons.end()));
        }
        return buttons;
    }

    // 获取当前对象的模块
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


inline Scenes::Scenes(const std::string &name_)
{
}

Scenes::~Scenes()
{
}

inline void Scenes::on_start()
{
    auto window=get_window();
    if (!window)
    {
        throw std::runtime_error("窗口获取失败");
        return;
    }
    start();
    for (auto &obj : objects)
    {
        //std::cout << obj->name << std::endl;
        if (obj && obj->enable)
        {

            obj->start();
        }
    }
}

inline void Scenes::on_update()
{
    auto window=get_window();
    static int tt=0;
    static float start,end,sum=0;
    start=window->get_time();


    window->clear_background(backgroundColor);
    update();
    for (int i = 0; i < 32; i++)
    {
        for (auto &obj : objects)
        {
            if (obj && obj->enable&&obj->layer==1<<i)
            {
                obj->update();
                if (mainCamera)
                    if (obj->transform.get_parent() == nullptr)
                    {
                        obj->transformMat4 =mainCamera->matrix4();

                        obj->draw(1 << i);
                    }
            }
        }
    }
    run_delay_instruction();
    window->set_transform(VEC::Mat4::unit_matrix());


    end=window->get_time();
    sum+=end-start;
    tt++;
    if(tt>=60)
    {
        tt-=60;
        sum=sum/60.0f;
        std::cout<<"一帧平均用时:"<<sum<<"秒,最大帧数:"<<1.0f/sum<<std::endl;
        sum=0;
    }
}

inline void Scenes::on_exit()
{
    exit();
    for (auto &obj : objects)
    {
        if (obj && obj->enable)
        {
            obj->exit();
        }
    }
}

inline void Scenes::on_windowSize(int w, int h)
{
    size(w, h);
    for (auto &obj : objects)
    {
        if (obj && obj->enable)
        {
            obj->size(w, h);
        }
    }
}


inline int Scenes::include_to_pos(int inc) const
{
    if (inc < 0)
    {
        inc += objects.size();
    }
    if (inc < 0 || inc >= objects.size())
        return -1;
    return inc;
}

inline void Scenes::run_delay_instruction()
{
    for(auto& c:commands)
    {
        switch (c.first)
        {
        case Function::CreateObjectForName:
        {
            auto objName = static_cast<std::string *>(c.second);
            immediate_create_object(*objName);
            delete objName;
            break;
        }
        case Function::DeleteObjectForName:
        {
            auto objName = static_cast<std::string *>(c.second);
            immediate_delete_object(*objName);
            delete objName;
            break;
        }
        case Function::DeleteObjectForObj:
        {
            auto objName = static_cast<ScenesObject *>(c.second);
            immediate_delete_object(ShaPtr<ScenesObject>(objName));
            delete objName;
            break;
        }
        case Function::DeleteObjectForPos:
        {
            auto objName = static_cast<int *>(c.second);
            immediate_delete_object(*objName);
            delete objName;
            break;
        }
        case Function::MoveObjct:
        {
            auto objName = static_cast<int *>(c.second);
            immediate_move_object(objName[0], objName[1]);
            delete objName;
            break;
        }
        default:
            break;
        }
    }
}

inline ShaPtr<ScenesObject> Scenes::immediate_create_object(const std::string &name)
{
    auto obj=std::make_shared<ScenesObject>();
    if(obj)
    {
        objects.push_back(obj);
        obj->scenes=this;
    }
    return obj;
}

inline ShaPtr<ScenesObject> Scenes::create_object(const std::string &name)
{
    return immediate_create_object(name);

    //commands.push_back(std::make_pair(Function::CreateObjectForName,new std::string(name)));
    //return nullptr;
}

inline ShaPtr<ScenesObject> Scenes::find_object(const std::string &name) const
{
    for(auto& obj:objects)
    {
        if(obj->name==name)
        {
            return obj;
        }
    }
    return nullptr;
}

inline std::vector<ShaPtr<ScenesObject>> Scenes::find_objects(const std::string &name) const
{
    std::vector<ShaPtr<ScenesObject>> result;
    for(auto& obj:objects)
    {
        if(obj->name==name)
        {
            result.push_back(obj);
        }
    }
    return result;
}

inline int Scenes::get_objects_size() const
{
    return objects.size();
}

inline ShaPtr<ScenesObject> Scenes::get_object(int pos) const
{
    if (pos < 0)
    {
        pos += objects.size();
    }
    if (pos < 0 || pos >= objects.size())
    {
        return nullptr;
    }
    return objects[pos];
}

inline int Scenes::get_object_pos(const WeaPtr<ScenesObject>&obj) const
{
    for (int i = 0; i < objects.size(); i++)
    {
        if (objects[i] == obj.lock())
        {
            return i;
        }
    }
    return -1;
}

inline void Scenes::immediate_delete_object(const std::string &name)
{
    immediate_delete_object(find_object(name));
}

inline void Scenes::immediate_delete_object(const WeaPtr<ScenesObject>&object)
{
    if (object.expired())
        return;

    std::unordered_set<ScenesObject *> toDelete;
    std::function<void(ScenesObject *)> markForDeletion = [&](ScenesObject *obj)
    {
        if (!obj || toDelete.count(obj))
            return;
        toDelete.insert(obj);

        for (int i = 0; i < obj->transform.get_childs_count(); i++)
        {
            auto *child = static_cast<ScenesObject *>(obj->transform.get_child(i)->userData);
            markForDeletion(child);
        }
    };

    markForDeletion(object.lock().get());

    for (auto it = objects.begin(); it != objects.end();)
    {
        if (toDelete.count(it->get()))
        {
            it = objects.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

inline void Scenes::immediate_delete_object(int pos)
{
    pos= include_to_pos(pos);
    if(pos<0)
        return;
    objects.erase(objects.begin()+pos);
}

inline void Scenes::delete_object(const std::string &name)
{
    commands.push_back(std::make_pair(Function::DeleteObjectForName,new std::string(name)));
}

inline void Scenes::delete_object(const WeaPtr<ScenesObject>& obj)
{
    commands.push_back(std::make_pair(Function::DeleteObjectForObj,obj.lock().get()));
}

inline void Scenes::delete_object(int pos)
{
    commands.push_back(std::make_pair(Function::DeleteObjectForPos,new int(pos)));
}

inline void Scenes::clear_null_object()
{
    auto newEnd = std::remove_if(
        objects.begin(), objects.end(),
        [](const WeaPtr<ScenesObject> &ptr)
        { return ptr.expired(); });
    objects.erase(newEnd, objects.end());
}

inline int Scenes::immediate_move_object(int form, int to)
{
    int o = include_to_pos(form);
    int i = include_to_pos(to);
    if (o < 0 || i < 0 || i == o)
        return -1;
    auto tem=objects[o];
    objects[o]=objects[i];
    objects[i]=tem;
    return i;
}

inline int Scenes::move_object(int form, int to)
{
    int* f= new int[2];
    f[0]=form;
    f[1]=to;
    commands.push_back(std::make_pair(Function::MoveObjct,f));
}

inline WindowInput *Scenes::input() const
{
    if(manage)
        return &manage->input;
    return nullptr;
}

inline WindowDefine::WindowBase *Scenes::get_window() const
{
    if(manage)
        return manage->get_window();
    return nullptr;
}

inline float Scenes::get_delta_time() const
{
    return get_window()->get_delta_time();
}


inline void ScenesManage::draw_callback(WindowDefine::WindowBase *window)
{
    auto manage = static_cast<ScenesManage *>(window->userData);
    if (!manage||!manage->m_currentScenes)
        return;
    auto openGLWindow=dynamic_cast<WindowDefine::WindowOpenGLWin32*>(window);
    if(openGLWindow)
        openGLWindow->switch_openGL();
    manage->m_currentScenes->on_update();
}

inline void ScenesManage::size_callback(WindowDefine::WindowBase *window, int w, int h)
{
    auto manage = static_cast<ScenesManage *>(window->userData);
    if (!manage)
        return;
    auto openGLWindow = dynamic_cast<WindowDefine::WindowOpenGLWin32 *>(window);
    if (openGLWindow)
        openGLWindow->switch_openGL();
    manage->m_currentScenes->on_windowSize(w, h);
}

ScenesManage::ScenesManage(WindowDefine::WindowBase *window)
{
    set_window(window);
}

inline ScenesManage::ScenesManage(WindowDefine::WindowBase &window)
{
    set_window(&window);
}

inline void ScenesManage::set_window(WindowDefine::WindowBase *window)
{
    if (!window)
    {
        return;
    }
    if (dynamic_cast<WindowDefine::WindowOpenGLWin32 *>(window))
    {
        isOpenGLWindow = true;
    }
    else
    {
        isOpenGLWindow = false;
        std::cout << "\033[33m" << "2d窗口无法正确绘制透视视图的图片" << "\033[0m" << std::endl;
    }
    
    input.set_window(window);

    window->coordinateStyle=CoordinateStyle::NormalizedDeviceCoordinates;

    m_window = window;
    window->userData = this;
    window->set_draw_callback(draw_callback);
    window->set_size_callback(size_callback);
}

inline WindowDefine::WindowBase *ScenesManage::get_window() const
{
    return m_window;
}

inline ShaPtr<Scenes>ScenesManage::get_scenes(int id)const
{
    auto pt = m_dictionary.find(id);
    if (pt == m_dictionary.end())
    {
        return nullptr;
    }
    return pt->second;
}
inline ShaPtr<Scenes>ScenesManage::get_current_scenes() const
{
    return m_currentScenes;
}
inline bool ScenesManage::erase_scenes(int id)
{
    auto pt = m_dictionary.find(id);
    if (pt == m_dictionary.end())
    {
        return false;
    }
    pt->second->manage=nullptr;
    m_dictionary.erase(pt);
    return true;
}
inline bool ScenesManage::to_scenes(int id)
{
    auto next=get_scenes(id);
    if(next==nullptr)return false;
    if(m_currentScenes)
        m_currentScenes->on_exit();
    m_currentScenes=next;
    m_currentScenes->on_start();
    return true;
}
template <typename scenesType>
inline int ScenesManage::add_scenes(const std::string &name, ShaPtr<scenesType> *getScenes)
{
    for(int i=m_dictionary.size();i<INT_MAX;++i)
    {
        auto pt= m_dictionary.find(i);
        if(pt==m_dictionary.end())
        {
            auto openglWindow=dynamic_cast<WindowDefine::WindowOpenGLWin32*>(m_window);
            if(openglWindow)
                openglWindow->switch_openGL();
            auto scenes = std::make_shared<scenesType>();
            scenes->name = name;
            if (getScenes)
                *getScenes = scenes;
            scenes->manage = this;
            m_dictionary.insert(std::make_pair(i, scenes));
            scenes->init();
            auto windowSize = m_window->get_size();
            scenes->on_windowSize(windowSize.width,windowSize.height);
            return i;
        }
    }
    return -1;
}
}
}