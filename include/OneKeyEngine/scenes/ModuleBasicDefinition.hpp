#pragma once

#include"../WindowElementOKE/ImageLoadOKE.hpp"

#include"ScenesObject.hpp"
#include"Module.hpp"
#include"../WindowDefine/OpenGL_OKE.hpp"
#include"EventManager.hpp"

#include<memory>
#include<unordered_map>
#include<algorithm>


namespace OneKeyEngine
{
namespace Scenes
{
namespace Module
{

class RectTransform:public Module
{
public:
    VEC::Vec2 size;
    VEC::Transform* transform=nullptr;
    RectTransform(float w=100,float h=100);

    void init()override;

    void draw()override;

    const VEC::Vec2& get_size()const;

    static bool is_point_in_triangle(const VEC::Vec2& p, const VEC::Vec2& a, const VEC::Vec2& b, const VEC::Vec2& c);
};


enum class PlotMode
{
    screenScaling,
    fixedPixelSize,
};
enum class ProjectorMode
{
    screen,
    camera,
    world,
};
class Button;
class CameraModule;
class Canvas : public Module
{
public:
    ShaPtr<RectTransform> rectTransform;
    WeaPtr<Camera> outPutCamera;
    ProjectorMode projectorMode=ProjectorMode::screen;
    PlotMode plotMode=PlotMode::screenScaling;
    /// @brief 缩放匹配0：宽度，1：高度
    float matchWidthOrHeight=0;

    Canvas();

    void init()override;
    void start()override;
    void draw()override;

    /// @brief 获取画布缩放
    /// @return 
    float get_scale()const;

    VEC::Vec2 screen_to_canvas(const VEC::Vec2& position)const;
    VEC::Vec2 NDC_to_pixel_CC(const VEC::Vec2& position)const;
};

class Image : public Module
{
public:
    GL::ShaderGL* shader=nullptr;
    GL::Element* vertexBuffer=nullptr;
    std::shared_ptr<WindowControl::Picture> currentImage;
    WeaPtr<RectTransform> rectTransform;

    VEC::Color color=VEC::Color(1,1,1);
    Image(const std::string &filePath = "");
    ~Image();
    std::shared_ptr<WindowControl::Picture> &load_image_file(const std::string &filePath);
    std::shared_ptr<WindowControl::Picture> create_image(int width,int height,const VEC::Color& fillColor=VEC::Color(1.0f,1.0f,1.0f));
    void init()override;

    virtual void draw() override;

};

class Text:public Module
{
    VEC::Vec2 textDrawSize;
    GL::ShaderGL* shader=nullptr;
    GL::Element* vertexBuffer=nullptr;
public:
    WeaPtr<RectTransform> rectTransform;
    TextStyle textStyle;
    std::string showText;
    Text();
    
    void init()override;
    void draw()override;
    float get_str_line_size(char* str)const;
    /// @brief 获取字符在绘图情况下的像素位置
    /// @param pos utf-8情况下的第几个字符
    /// @return 在绘图情况下的位置
    VEC::Vec2 get_character_position(size_t char_index)const;
    size_t get_character_index_from_position(const VEC::Vec2& position) const;
    size_t get_character_count_utf_8()const;
    size_t get_character_pos_in_string(size_t char_index)const;
    void delete_prev_utf8_char(size_t char_index);
    static void delete_prev_utf8_char(std::string& str, size_t char_index);
    // 判断一个字节是否是 UTF-8 字符的起始字节
    static bool is_utf8_start_byte(unsigned char byte);
    // 获取 UTF-8 编码字符的位置
    static size_t utf8_char_position(const std::string& str, size_t char_index);
};

/// @brief 摄像机模块，默认朝向为z的负半轴
class CameraModule : public Module
{
public:
    ShaPtr<Camera> camera=std::make_shared<Camera>();
    CameraModule();

    void transform_to_camera_orientation();

    void init()override;
    void draw()override;
    void size(int w,int h)override;
    Camera* operator->();
};

class UIEventsSystem;
class Button : public Module
{
    void on_press();
    void on_popUp();
    void on_mouse_over();
    void on_mouse_off();
    void on_lose_focus();
    bool focus=false;
public:
    using ButtonCallback=void(*)(Button* button);
    void* userData=nullptr;
    /// @brief 只会调用空返回函数
    EventManager pressCallbackManager;
    EventManager popUpCallbackManager;
    EventManager mouseOverCallbackManager;
    EventManager mouseOffCallbackManager;
    EventManager loseFocusCallbackManager;


    WeaPtr<RectTransform> rectTransform;
    WeaPtr<Image> drawAim;
    WeaPtr<UIEventsSystem> eventsSystem;

    VEC::Color defaultColor = VEC::Color(0.7,0.7,0.7);
    VEC::Color highlightColor = VEC::Color(1, 1, 1);
    VEC::Color pressColor = VEC::Color(0.5, 0.5, 0.5);
    VEC::Color selectColor = VEC::Color(0.7,0.7,0.7);


    Button();
    ~Button();

    void init()override;
    void start()override;
    //void draw()override;
    

    virtual void press(){};
    virtual void popUp(){};
    virtual void mouse_over(){};
    virtual void mouse_off(){};
    virtual void lose_focus(){};

    friend class UIEventsSystem;
};

class UIEventsSystem : public Module
{
    static void key_down_callback(WindowDefine::WindowBase* window,KeyCode key);
    static void key_up_callback(WindowDefine::WindowBase* window,KeyCode key);
    static void mouse_move_callback(WindowDefine::WindowBase* window,float x,float y);

    WeaPtr<Button> lastFocus;
    WeaPtr<Button> lastMouseOver;
public:
    WeaPtr<Canvas> canvas;
    std::vector<WeaPtr<Button>> buttonSet;
    bool needUpdateButtonSet = false;

    UIEventsSystem();

    void init()override;
    void start()override;
    void update()override;
    void exit()override;

    void update_button_set();
    void click(int x,int y);
    void mouse_move(int x,int y);
    WeaPtr<Button> get_button(int x,int y);
};

class EditText:public Button
{
    size_t charSize=0;
    float timer=0.0f;
    VEC::Color oldPressColor;
    VEC::Vec2 cursorDrawPos;
    //是否在拖动鼠标选词
    bool selectedWords=false;
    //是否处于框选
    bool boxSelect=false;
    //static void mouse_move_callback(WindowDefine::WindowBase* window,float x,float y);
public:
    //光标显示隐藏的间隔时间，单位秒，小于0永不闪烁
    float cursorBlinking=0.5f;
    //是否绘制光标
    bool drawCursor=true;
    //光标线条样式
    LineStyle cursorStyle;
    //当前光标的位置,utf-8字符
    size_t cursorPosition=0;
    //绘制文本
    WeaPtr<Text> text;
    //是否捕获输入
    bool getInput=false;
    //允许编辑
    bool editAble=true;
    //框选位置
    size_t boxSelectNext=0;
    //框选颜色
    VEC::Color boxSelectColor=VEC::Color(0.2f,0.7f,0.9f);

    ShaPtr<Canvas> canvas;
    EditText();
    void start()override;

    void update()override;

    void draw()override;
    
    void press()override;
    void lose_focus()override;

    void delete_last_char();

    void update_cursor_draw();

    void insort_text_in_cursor(const std::string& str);

    void erase_utf_8_character(size_t start,size_t size);

    std::string get_box_select_string()const;
};






} // namespace Module
}
}