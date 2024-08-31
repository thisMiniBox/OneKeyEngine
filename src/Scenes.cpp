#include <OneKeyEngine/scenes/Scenes.hpp>
#include<OneKeyEngine/scenes/ScenesObject.hpp>
#include<OneKeyEngine/WindowDefine/WindowGLFW.hpp>
#include<string>




namespace OneKeyEngine
{
namespace Scenes
{
    
Scenes::Scenes(const std::string &name_)
{
}

Scenes::~Scenes()
{
}

void Scenes::on_start()
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

void Scenes::on_update()
{
    auto window=get_window();
    
    // static int tt=0;
    // static float start,end,sum=0;
    // start=window->get_time();

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
                        obj->transformMat4 = mainCamera->matrix4();

                        obj->draw(1 << i);
                    }
            }
        }
    }
    try
    {
        run_delay_instruction();
    }
    catch (const std::exception &e)
    {
        // 记录异常信息
        std::cerr << "Caught an exception: " << e.what() << std::endl;
        // 可以在这里记录到日志文件或通知用户
    }
    catch (...)
    {
        // 捕获所有其他类型的异常
        std::cerr << "Caught an unknown exception." << std::endl;
    }

    window->set_transform(VEC::Mat4::unit_matrix());


    // end=window->get_time();
    // sum+=end-start;
    // tt++;
    // if(tt>=60)
    // {
    //     tt-=60;
    //     sum=sum/60.0f;
    //     std::cout<<"一帧平均用时:"<<sum<<"秒,最大帧数:"<<1.0f/sum<<std::endl;
    //     sum=0;
    // }
}

void Scenes::on_exit()
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

void Scenes::on_windowSize(int w, int h)
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


int Scenes::include_to_pos(int inc) const
{
    if (inc < 0)
    {
        inc += objects.size();
    }
    if (inc < 0 || inc >= objects.size())
        return -1;
    return inc;
}

void Scenes::run_delay_instruction()
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
    for(auto shaObj:needToDeleteObjSet)
    {
        immediate_delete_object(shaObj);
    }
    commands.clear();
    needToDeleteObjSet.clear();
}

ShaPtr<ScenesObject> Scenes::immediate_create_object(const std::string &name)
{
    auto obj=std::make_shared<ScenesObject>();
    if(obj)
    {
        objects.push_back(obj);
        obj->name=name;
        obj->scenes=this;
    }
    return obj;
}

ShaPtr<ScenesObject> Scenes::create_object(const std::string &name)
{
    return immediate_create_object(name);

    //commands.push_back(std::make_pair(Function::CreateObjectForName,new std::string(name)));
    //return nullptr;
}

ShaPtr<ScenesObject> Scenes::create_object(const std::string &name, ShaPtr<ScenesObject> &parent)
{
    return create_object(name,parent.get());
}
ShaPtr<ScenesObject> Scenes::create_object(const std::string &name, ScenesObject *parent)
{
    auto obj=immediate_create_object(name);
    if(parent)
    {
        parent->transform.set_child(obj->transform);
    }
    return obj;
}
ShaPtr<ScenesObject> Scenes::find_object(const std::string &name) const
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

std::vector<ShaPtr<ScenesObject>> Scenes::find_objects(const std::string &name) const
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

int Scenes::get_objects_size() const
{
    return objects.size();
}

ShaPtr<ScenesObject> Scenes::get_object(int pos) const
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

ShaPtr<ScenesObject> Scenes::get_object(ScenesObject *ptr) const
{
    if (!ptr)
    {
        return ShaPtr<ScenesObject>();
    }
    for (auto &sPtr : objects)
    {
        if (sPtr.get() == ptr)
        {
            return sPtr;
        }
    }
    return ShaPtr<ScenesObject>();
}
int Scenes::get_object_pos(WeaPtr<ScenesObject>obj) const
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

void Scenes::immediate_delete_object(const std::string &name)
{
    immediate_delete_object(find_object(name));
}

void Scenes::immediate_delete_object(const WeaPtr<ScenesObject>&object)
{
    if (object.expired())
        return;
    auto obj=object.lock();
    if(obj->scenes!=this)
    {
        std::cout<<"不能删除其他场景的对象"<<std::endl;
        return;
    }
    for(auto it=objects.begin();it!=objects.end();)
    {
        if(*it==obj)
        {
            for(int c=0;c<obj->get_childs_count();c++)
            {
                auto child=obj->get_child(c);
                auto childSha=get_object(child);
                immediate_delete_object(childSha);
            }
            objects.erase(it);
            obj->transform.set_parent(nullptr);
            break;
        }
        ++it;
    }
}

void Scenes::immediate_delete_object(int pos)
{
    pos= include_to_pos(pos);
    if(pos<0)
        return;
    immediate_delete_object(objects[pos]);
}

void Scenes::delete_object(const std::string &name)
{
    delete_object(find_object(name));
}

void Scenes::delete_object(WeaPtr<ScenesObject> obj)
{
    if(obj.expired())
        return;
    needToDeleteObjSet.insert(obj.lock());
}

void Scenes::delete_object(int pos)
{
    delete_object(get_object(pos));
}

void Scenes::clear_null_object()
{
    auto newEnd = std::remove_if(
        objects.begin(), objects.end(),
        [](const WeaPtr<ScenesObject> &ptr)
        { return ptr.expired(); });
    objects.erase(newEnd, objects.end());
}

int Scenes::immediate_move_object(int form, int to)
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

int Scenes::move_object(int form, int to)
{
    int* f= new int[2];
    f[0]=form;
    f[1]=to;
    commands.push_back(std::make_pair(Function::MoveObjct,f));
    return to;
}

WindowInput *Scenes::input() const
{
    if(manage)
        return &manage->input;
    return nullptr;
}

WindowDefine::WindowBase *Scenes::get_window() const
{
    if(manage)
        return manage->get_window();
    return nullptr;
}

float Scenes::get_delta_time() const
{
    return get_window()->get_delta_time();
}


void ScenesManage::draw_callback(WindowDefine::WindowBase *window)
{
    auto manage = static_cast<ScenesManage *>(window->userData);
    if (!manage||!manage->m_currentScenes)
        return;
    auto openGLWindow=dynamic_cast<WindowDefine::WindowOpenGLWin32*>(window);
    if(openGLWindow)
        openGLWindow->switch_openGL();
    manage->m_currentScenes->on_update();
}

void ScenesManage::size_callback(WindowDefine::WindowBase *window, int w, int h)
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

void ScenesManage::set_window(WindowDefine::WindowBase *window)
{
    if (!window)
    {
        return;
    }
    if (dynamic_cast<WindowDefine::WindowOpenGLWin32 *>(window)||dynamic_cast<WindowDefine::WindowGL_GLFW *>(window))
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

WindowDefine::WindowBase *ScenesManage::get_window() const
{
    return m_window;
}

ShaPtr<Scenes>ScenesManage::get_scenes(int id)const
{
    auto pt = m_dictionary.find(id);
    if (pt == m_dictionary.end())
    {
        return nullptr;
    }
    return pt->second;
}
ShaPtr<Scenes>ScenesManage::get_current_scenes() const
{
    return m_currentScenes;
}
bool ScenesManage::erase_scenes(int id)
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
bool ScenesManage::to_scenes(int id)
{
    auto next=get_scenes(id);
    if(next==nullptr)return false;
    if(m_currentScenes)
        m_currentScenes->on_exit();
    m_currentScenes=next;
    m_currentScenes->on_start();
    return true;
}

bool ScenesManage::to_scenes(const std::string &name)
{
    return to_scenes(get_id(name));
}

int ScenesManage::get_id(const std::string &name) const
{
    if(name.empty())
        return -2;
    for(auto& dic:m_dictionary)
    {
        if(dic.second->name==name)
        {
            return dic.first;
        }
    }
    return -1;
}
FT_Face ScenesManage::get_free_type_font(const TextStyle *style) const
{
    if(!style||!m_window)
    {
        return nullptr;
    }
    auto glfwGL=m_window->is_derived_class<WindowDefine::WindowGL_GLFW>();
    if(glfwGL)
    {
        return glfwGL->get_free_type_font(style);
    }
    auto win32GL = m_window->is_derived_class<WindowDefine::WindowOpenGLWin32>();
    if (win32GL)
    {
        return win32GL->get_free_type_font(style);
    }
    return nullptr;
}
Character *ScenesManage::get_char_character(char32_t ch, FT_Face ft_font) const
{
    if(!ft_font||!m_window)
    {
        return nullptr;
    }
    auto glfwGL=m_window->is_derived_class<WindowDefine::WindowGL_GLFW>();
    if(glfwGL)
    {
        return glfwGL->get_char_character(ch,ft_font);
    }
    auto win32GL = m_window->is_derived_class<WindowDefine::WindowOpenGLWin32>();
    if (win32GL)
    {
        return win32GL->get_char_character(ch,ft_font);
    }
    return nullptr;
}
void ScenesManage::clear_char_texture(uint32_t id) const
{
    if(!m_window)
    {
        return;
    }
    auto glfwGL=m_window->is_derived_class<WindowDefine::WindowGL_GLFW>();
    if(glfwGL)
    {
        return glfwGL->clear_char_texture(id);
    }
    auto win32GL = m_window->is_derived_class<WindowDefine::WindowOpenGLWin32>();
    if (win32GL)
    {
        return win32GL->clear_char_texture(id);
    }
    return;
}
}
} // namespace OneKeyEngine
