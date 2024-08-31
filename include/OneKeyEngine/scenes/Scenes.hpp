#pragma once

#include"../WindowDefine/Window_OKE.hpp"
#include"Camera.hpp"
#include"../Style_OKE.hpp"
#include"../WindowInput_OKE.hpp"

#include<unordered_map>
#include<utility>
#include<unordered_set>


namespace OneKeyEngine
{
namespace Scenes
{
class ScenesObject;
class ScenesManage;

namespace Module
{
    class Module;
} // namespace Module


/// @brief 基础场景类，可通过继承场景自定义的场景，需要加入场景管理器并跳转到对应场景才能绘制
class Scenes
{
public:
    enum class Function
    {
        CreateObjectForName,
        DeleteObjectForName,
        DeleteObjectForPos,
        MoveObjct,
    };

    ShaPtr<Camera> mainCamera;
    std::string name;
    ScenesManage* manage=nullptr;
    VEC::Color backgroundColor=VEC::Color(0.4,0.4,0.4);

    Scenes(const std::string& name_="新建场景");
    ~Scenes();

    virtual void init(){};

    virtual void start(){};
    virtual void update(){};
    virtual void exit(){};

    virtual void size(int w,int h){};

    ShaPtr<ScenesObject> immediate_create_object(const std::string& name="新建对象");
    ShaPtr<ScenesObject> create_object(const std::string& name="新建对象");
    ShaPtr<ScenesObject> create_object(const std::string& name,ShaPtr<ScenesObject>& parent);
    ShaPtr<ScenesObject> create_object(const std::string& name,ScenesObject* parent);
    ShaPtr<ScenesObject> find_object(const std::string& name)const;
    std::vector<ShaPtr<ScenesObject>> find_objects(const std::string& name)const;
    int get_objects_size()const;
    ShaPtr<ScenesObject> get_object(int pos)const;
    ShaPtr<ScenesObject> get_object(ScenesObject* ptr)const;
    int get_object_pos(WeaPtr<ScenesObject>obj) const;
    void immediate_delete_object(const std::string& name);
    void immediate_delete_object(const WeaPtr<ScenesObject>& obj);
    void immediate_delete_object(int pos);
    void delete_object(const std::string& name);
    void delete_object(WeaPtr<ScenesObject> obj);
    void delete_object(int pos);
    void clear_null_object();
    /// @brief 将指定位置对象移动到指定位置
    /// @param form 第几个元素
    /// @param to 对应元素，-1表示最后
    /// @return 插入后的位置,-1表示失败
    int immediate_move_object(int form,int to);
    int move_object(int form,int to);

    WindowInput* input()const;
    WindowDefine::WindowBase* get_window()const;
    float get_delta_time()const;

    friend class ScenesManage;

private:
    std::vector<std::pair<Function, void *>> commands;
    std::unordered_set<ShaPtr<ScenesObject>> needToDeleteObjSet;
    std::vector<ShaPtr<ScenesObject>> objects;
    void on_start();
    void on_update();
    void on_exit();
    void on_windowSize(int w, int h);
    /// @brief 将包含负数的索引转换为正数，当索引非法时返回-1
    /// @param inc 对应元素，-1表示最后及-n~n-1
    /// @return
    int include_to_pos(int inc) const;

    void run_delay_instruction();


};




/// @brief 管理场景，并且会占用窗口的按键回调和用户数据指针,如果为2d窗口，无法正确绘制图片的透视状态
class ScenesManage
{
    std::unordered_map<int,ShaPtr<Scenes>>m_dictionary;
    ShaPtr<Scenes> m_currentScenes=nullptr;
    WindowDefine::WindowBase* m_window=nullptr;

    static void draw_callback(WindowDefine::WindowBase*window);
    static void size_callback(WindowDefine::WindowBase*window,int w,int h);
public:
    void* userData=nullptr;
    void* userDataMore=nullptr;
    WindowInput input;
    bool isOpenGLWindow=false;

    std::unordered_map<size_t ,GL::ShaderGL>s_shader;
    std::unordered_map<size_t ,GL::Buffer>s_buffer;
    std::unordered_map<size_t ,GL::Element>s_element;

    ScenesManage(WindowDefine::WindowBase* window);
    
    void set_window(WindowDefine::WindowBase*);
    WindowDefine::WindowBase* get_window()const;
    /// @brief 创建场景管理器
    /// @tparam scenesType 
    /// @param name 场景名称
    /// @param scenes 返回创建的场景
    /// @return 场景id
    template <typename scenesType=Scenes>
    int add_scenes(const std::string& name, ShaPtr<scenesType>* getScenes=nullptr)
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

    /// @brief 依据id获取场景
    /// @param id 
    /// @return 
    ShaPtr<Scenes> get_scenes(int id)const;
    /// @brief 获取当前播放的场景
    /// @return 
    ShaPtr<Scenes> get_current_scenes()const;
    /// @brief 删除场景
    /// @param id 对应id
    /// @return 是否成功删除
    bool erase_scenes(int id);
    /// @brief 跳转到对应场景
    /// @param id
    /// @return 是否成功跳转
    bool to_scenes(int id);

    bool to_scenes(const std::string& name);
    /// @brief 按照场景名称获取场景id
    /// @param name 
    /// @return -1没找到，-2名字为空
    int get_id(const std::string& name)const;

    FT_Face get_free_type_font(const TextStyle *style) const;
    Character *get_char_character(char32_t ch, FT_Face ft_font) const;
    void clear_char_texture(uint32_t id) const;
};


}
}