#pragma once

#include"../WindowDefine/Window_OKE.hpp"
#include"Camera.hpp"
#include"../Style_OKE.hpp"


#include<unordered_map>
#include<utility>


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
    enum class Function
    {
        CreateObjectForName,
        DeleteObjectForName,
        DeleteObjectForObj,
        DeleteObjectForPos,
        MoveObjct,
    };
    std::vector<std::pair<Function,void*>>commands;
    std::vector<ShaPtr<ScenesObject>>objects;
    void on_start();
    void on_update();
    void on_exit();
    void on_windowSize(int w,int h);
    /// @brief 将包含负数的索引转换为正数，当索引非法时返回-1
    /// @param inc 对应元素，-1表示最后及-n~n-1
    /// @return 
    int include_to_pos(int inc)const;

    void run_delay_instruction();
public:
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
    ShaPtr<ScenesObject> find_object(const std::string& name)const;
    std::vector<ShaPtr<ScenesObject>> find_objects(const std::string& name)const;
    int get_objects_size()const;
    ShaPtr<ScenesObject> get_object(int pos)const;
    int get_object_pos(const WeaPtr<ScenesObject>& obj)const;
    void immediate_delete_object(const std::string& name);
    void immediate_delete_object(const WeaPtr<ScenesObject>& obj);
    void immediate_delete_object(int pos);
    void delete_object(const std::string& name);
    void delete_object(const WeaPtr<ScenesObject>& obj);
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
    ScenesManage(WindowDefine::WindowBase& window);
    
    void set_window(WindowDefine::WindowBase*);
    WindowDefine::WindowBase* get_window()const;
    /// @brief 添加场景到管理器
    /// @param scenes 目标场景
    /// @param id 对应id，需大于0，负数表示自动分配,如果id重合会自动分配
    /// @return 返回实际id,错误返回-1
    template <typename scenesType>
    int add_scenes(const std::string& name, ShaPtr<scenesType>* scenes=nullptr);

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
};


}
}