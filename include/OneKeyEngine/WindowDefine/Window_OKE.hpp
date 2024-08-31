#pragma once
#ifndef __OKE_WINDOW_H
#define __OKE_WINDOW_H

#include<chrono>
#include"../Vector_OKE.hpp"
#include"../Style_OKE.hpp"
#include"../ClassType_OKE.hpp"

namespace OneKeyEngine
{
namespace WindowDefine
{
class WindowBase;
namespace WindowCallback
{
    using DrawCallback=void(*)(WindowBase* window);
    using WindowSizeCallback = void (*)(WindowBase *fromWho, int weight, int height);
    using KeyCallback = void (*)(WindowBase *fromWho, KeyCode keyCode);
    using InputCallback=void (*)(WindowBase*fromWho,uint32_t input);
    using MouseWheelCallback=void(*)(WindowBase*fromWho,float xoffset,float yoffset);
    using MouseMoveCallback=void(*)(WindowBase*fromWho,float xpos,float ypos);
}

class WindowBase : public ClassType_OKE
{
protected:
    float m_realUpdateTime = -1.0f;
    float m_lastRefreshTime;

    VEC::Size m_windowSize;

    bool m_inFocus=true;
public:
    void* userData=nullptr;
    VEC::Vec2 mousePosition;
    /// @brief 指定非虚函数绘制基础图元使用的坐标系
    CoordinateStyle coordinateStyle = CoordinateStyle::NormalizedDeviceCoordinates;
    /// @brief 绘图回调
    WindowCallback::DrawCallback m_drawCallback = nullptr;
    /// @brief 字符输入回调
    WindowCallback::InputCallback m_inputCallback = nullptr;
    /// @brief 按键按下与抬起回调
    WindowCallback::KeyCallback m_keyDownCallback = nullptr, m_keyUpCallback = nullptr;
    /// @brief 窗口大小调整回调
    WindowCallback::WindowSizeCallback m_windowSizeCallback = nullptr;
    /// @brief 鼠标移动回调
    WindowCallback::MouseMoveCallback m_mouseMoveCallback=nullptr;
    /// @brief 鼠标滚轮回调
    WindowCallback::MouseWheelCallback m_mouseWheelCallbck = nullptr;
    /// @brief 鼠标按键回调
    WindowCallback::KeyCallback m_mouseButtonDown=nullptr,m_mouseButtonUp=nullptr;

    WindowBase(WindowBase* parent=nullptr)
    {}
    virtual ~WindowBase(){}
    /// @brief 设置按键按下回调
    /// @param callback 
    void set_key_down_callback(WindowCallback::KeyCallback callback);
    /// @brief 设置按键弹起回调
    /// @param callback 
    void set_key_up_callback(WindowCallback::KeyCallback callback);
    /// @brief 设置字符输入回调
    /// @param callback 
    void set_input_callback(WindowCallback::InputCallback callback);
    /// @brief 设置绘图回调
    /// @param callback 
    void set_draw_callback(WindowCallback::DrawCallback callback);
    /// @brief 设置窗口大小调整回调
    /// @param callback 
    void set_size_callback(WindowCallback::WindowSizeCallback callback);
    /// @brief 设置鼠标移动回调
    /// @param callback 
    void set_mouse_move_callback(WindowCallback::MouseMoveCallback callback);
    /// @brief 设置鼠标滚轮回调
    /// @param callback 
    void set_mouse_wheel_callback(WindowCallback::MouseWheelCallback callback);
    /// @brief 设置鼠标按键按下回调
    /// @param callback 
    void set_mouse_botton_down_callback(WindowCallback::KeyCallback callback);
    /// @brief 设置鼠标按键抬起回调
    /// @param callback 
    void set_mouse_botton_up_callback(WindowCallback::KeyCallback callback);
    /// @brief 场景窗口
    /// @param parent 父窗口
    /// @param x 位置X（像素）
    /// @param y 位置Y（像素）
    /// @param width 窗口宽度（像素）包含边框
    /// @param height 窗口高度（像素）包含边框与标题栏
    /// @return 
    virtual WindowCreateResult create_window(const std::string& titleName="new window",int x=200,int y=100,int width=800,int height=600,WindowBase* parent=nullptr)=0;
    /// @brief 关闭窗口
    /// @return 
    virtual int close()=0;
    /// @brief 调整窗口大小
    /// @param width 宽度（像素）
    /// @param height 高度（像素）
    /// @return 
    virtual int set_size_(int width,int height)const=0;
    /// @brief 调整窗口大小
    /// @param width 宽度（像素）
    /// @param height 高度（像素）
    /// @return 
    int set_size(int width,int height)const;
    /// @brief 获取窗口大小
    /// @param width 接受窗口宽度的整数指针
    /// @param height 接受窗口高度的整数指针
    /// @return 
    virtual int get_size_(int* width,int* height)const=0;
    /// @brief 调整窗口大小
    /// @param size 
    /// @return 
    int set_size(const VEC::Vector2<int>& size)const;
    /// @brief 获取窗口大小
    /// @return 
    VEC::Vector2<int> get_size()const;

    virtual int set_position(int x,int y)const=0;
    virtual int get_position(int* x,int *y)const=0;
    int set_position_(const VEC::Vector2<int>& position)const;
    VEC::Vector2<int> get_position_()const;
    virtual void set_window_title(const std::string& title)const=0;
    virtual std::string get_window_title()const=0;

    /// @brief 处理窗口一个事件
    /// @return 
    virtual int processing_event()=0;
    /// @brief 一直运行直到触发重绘
    /// @return 
    virtual int should_draw()=0;
    /// @brief 刷新屏幕
    virtual void refresh_screen()const=0;
    /// @brief 设置刷新频率，默认是惰性刷新（只在窗口大小改变与窗口遮挡后刷新）
    /// @param frameRate 每秒刷新次数,当为非正数时变为惰性刷新
    /// @return 
    virtual float set_frame_rate(float frameRate)=0;
    /// @brief 清空背景对应颜色，接受的范围为0-1的浮点数代表的颜色
    /// @param r 红色通道0-1
    /// @param g 绿色通道0-1
    /// @param b 蓝色通道0-1
    /// @param a 不透明度通道0-1
    /// @return 
    virtual WindowDrawResult clear_background_(float r = 0, float g = 0, float b = 0,float a=1) const = 0;
    /// @brief 清空背景对应颜色，接受的范围为0-1的浮点数代表的颜色
    /// @param r 红色通道0-1
    /// @param g 绿色通道0-1
    /// @param b 蓝色通道0-1
    /// @param a 不透明度通道0-1
    /// @return 
    WindowDrawResult clear_background(float r = 0, float g = 0, float b = 0,float a=1.0f)const;
    /// @brief 清空背景，接受的范围为0-1的浮点数代表的颜色
    /// @param color 对应颜色
    /// @return 
    WindowDrawResult clear_background(const VEC::Color& color)const;
    /// @brief 绘制直线,固定使用窗口坐标系，以像素为单位绘制
    /// @param x1 点1x坐标
    /// @param y1 点1y坐标
    /// @param x2 点2x坐标
    /// @param y2 点2y坐标
    /// @param style 线条样式，默认为黑色细实线
    /// @return 
    virtual WindowDrawResult draw_line_(float x1, float y1, float x2, float y2, LineStyle *style = nullptr)const = 0;
    /// @brief 绘制直线,以参数coordinateStyle指定的坐标系绘制
    /// @param x1 点1x坐标
    /// @param y1 点1y坐标
    /// @param x2 点2x坐标
    /// @param y2 点2y坐标
    /// @param style 线条样式，默认为黑色细实线
    /// @return 
    WindowDrawResult draw_line(float x1, float y1, float x2, float y2, LineStyle *style = nullptr)const;
    /// @brief 绘制直线,以参数coordinateStyle指定的坐标系绘制
    /// @param pos1 点一的坐标
    /// @param pos2 点二的坐标
    /// @param style 线条样式，默认为黑色细实线
    /// @return 
    WindowDrawResult draw_line(VEC::Vector3<float> pos1, VEC::Vector3<float> pos2, LineStyle *style = nullptr)const;
    /// @brief 绘制像素点,固定使用窗口坐标系，以像素为单位
    /// @param x 
    /// @param y 
    /// @param colorR 红色通道0-1
    /// @param colorG 绿色通道0-1
    /// @param colorB 蓝色通道0-1
    /// @param colorA 不透明通道0-1，默认为1
    /// @return 
    virtual WindowDrawResult draw_pixel_(float x, float y, float colorR, float colorG, float colorB,float colorA=1.f)const = 0;
    /// @brief 绘制像素点,以参数coordinateStyle指定的坐标系绘制
    /// @param x
    /// @param y
    /// @param colorR 红色通道0-1
    /// @param colorG 绿色通道0-1
    /// @param colorB 蓝色通道0-1
    /// @param colorA 不透明通道0-1
    /// @return 
    WindowDrawResult draw_pixel(float x, float y, float colorR, float colorG, float colorB,float colorA=1.f)const;
    /// @brief 绘制像素点,以参数coordinateStyle指定的坐标系绘制
    /// @param x
    /// @param y
    /// @param color 对应颜色
    /// @return 
    WindowDrawResult draw_pixel(float x, float y, const VEC::Color& color)const;
    /// @brief 绘制像素点，对应像素坐标
    /// @param pos 像素坐标
    /// @param color 颜色
    /// @return 
    WindowDrawResult draw_pixel(const VEC::Vector2<int>& pos,const VEC::Color& color)const;
    /// @brief 绘制像素点,以参数coordinateStyle指定的坐标系绘制
    /// @param pos 
    /// @param color 颜色
    /// @return 
    WindowDrawResult draw_pixel(const VEC::Vec2& pos,const VEC::Color& color)const;
    /// @brief 绘制矩形,固定使用窗口坐标系，以像素为单位绘制
    /// @param x 
    /// @param y 
    /// @param width 
    /// @param height 
    /// @return
    virtual WindowDrawResult draw_rectangle_(float x, float y, float width, float height,LineStyle* style=nullptr) const = 0;
    WindowDrawResult draw_rectangle(float x, float y, float width, float height,LineStyle* style=nullptr) const;
    WindowDrawResult draw_rectangle(const VEC::Rectangle &rect,LineStyle* style=nullptr) const;
    //virtual WindowDrawResult draw_fill_rectangle_(float x, float y, float width, float height)const =0;
    virtual WindowDrawResult draw_polygonal_(float x,float y,float* points,uint32_t pointsArraySize,LineStyle* style=nullptr)const=0;
    virtual WindowDrawResult draw_fill_polygonal_(float x, float y, float *points, uint32_t pointsArraySize, float r = 1.f, float g = 1.f, float b = 1.f, float a = 1.f) const = 0;
    virtual WindowDrawResult draw_text_(float x,float y,const char* text,TextStyle* style=nullptr)const=0;
    WindowDrawResult draw_text(float x,float y,const char* text,TextStyle* style=nullptr)const;

    virtual WindowDrawResult draw_image(WindowControl::Picture* image)=0;

    WindowDrawResult draw(WindowControl::WindowElement* control);
    WindowDrawResult draw(WindowControl::Picture* image);
    WindowDrawResult draw(WindowControl::WindowElement& control);
    WindowDrawResult draw(WindowControl::Picture& image);

    virtual WindowDrawResult get_pixel(int x,int y,float* r,float* g,float *b)const=0;
    VEC::Color get_pixel(const VEC::Vec2&pos)const;
    VEC::Color get_pixel(int x,int y)const;

    virtual void set_transform(const VEC::Mat4 &);
    virtual VEC::Mat4 get_transform() const;

    virtual bool in_focus() const;

    virtual void set_mouse_position(const VEC::Vec2&pos)=0;
    virtual void set_mode(ModeType mode,ModeValue value=ModeValue::True)const=0;

    virtual bool set_clipboard_text(const std::string& str)const =0;
    virtual std::string get_clipboard_text()const =0;

    double get_time() const;
    float get_delta_time() const;

    VEC::Point NDC_to_pixel_LT(const VEC::Vec2 &ndcPoint) const;
    VEC::Point NDC_to_pixel_LT(const VEC::Vec3 &ndcPoint) const;
    VEC::Point NDC_to_pixel_LT(float x,float y) const;
    VEC::Vec2 pixel_to_NDC_LT(const VEC::Vec2 &point) const;
    VEC::Vec2 pixel_to_NDC_LT(const VEC::Size &point) const;
    VEC::Vec2 pixel_to_NDC_LT(int x,int y) const;
    VEC::Point NDC_to_pixel_CC(const VEC::Vec2 &ndcPoint) const;
    VEC::Point NDC_to_pixel_CC(const VEC::Vec3 &ndcPoint) const;
    VEC::Point NDC_to_pixel_CC(float x,float y) const;
    VEC::Vec2 pixel_to_NDC_CC(const VEC::Vec2 &point) const;
    VEC::Vec2 pixel_to_NDC_CC(const VEC::Size &point) const;
    VEC::Vec2 pixel_to_NDC_CC(int x,int y) const;
    VEC::Point current_coordinate_to_window_coordinate(float x,float y)const;
    VEC::Point current_coordinate_to_window_coordinate(const VEC::Vec2& point)const;
    VEC::Point current_coordinate_to_window_coordinate(const VEC::Vec3& point)const;

};

class Window2DBase:public WindowBase
{
public:
    Window2DBase() {}
};






inline WindowDrawResult WindowBase::draw_line(VEC::Vector3<float> pos1, VEC::Vector3<float> pos2, LineStyle *style)const
{
    auto p1= current_coordinate_to_window_coordinate(pos1);
    auto p2=current_coordinate_to_window_coordinate(pos2);
    return draw_line_(p1.x,p1.y,p2.x,p2.y,style);
}
inline WindowDrawResult WindowBase::draw_pixel(float x, float y, float colorR, float colorG, float colorB, float colorA) const
{
    auto p=current_coordinate_to_window_coordinate(x,y);
    return draw_pixel_(p.x,p.y,colorR,colorG,colorB,colorA);
}
inline WindowDrawResult WindowBase::draw_pixel(float x, float y, const VEC::Color &color) const
{
    auto p = current_coordinate_to_window_coordinate(x, y);
    return draw_pixel_(p.x, p.y, color.r, color.g, color.b, color.a);
}
inline WindowDrawResult WindowBase::draw_pixel(const VEC::Vector2<int>& pos, const VEC::Color& color)const
{
    auto np=pixel_to_NDC_LT(pos);
    return draw_pixel_(np.x,np.y,color.r,color.g,color.b,color.a);
}
inline WindowDrawResult WindowBase::draw_pixel(const VEC::Vec2 &pos, const VEC::Color &color) const
{
    auto p = current_coordinate_to_window_coordinate(pos.x, pos.y);
    return draw_pixel_(p.x, p.y, color.r, color.g, color.b, color.a);
}
inline WindowDrawResult WindowBase::draw_rectangle(float x, float y, float width, float height,LineStyle* style) const
{
    auto p = current_coordinate_to_window_coordinate(x, y);
    return draw_rectangle_(p.x, p.y, width, height,style);
}
inline WindowDrawResult WindowBase::draw_rectangle(const VEC::Rectangle &rect,LineStyle* style) const
{
    auto lt = current_coordinate_to_window_coordinate(rect.left, rect.top);
    auto rb = current_coordinate_to_window_coordinate(rect.right, rect.bottom);
    return draw_rectangle_(lt.x, lt.y, rb.x - lt.x, rb.y - lt.y,style);
}
inline WindowDrawResult WindowBase::draw_text(float x, float y, const char *text,TextStyle *style) const
{
    auto p = current_coordinate_to_window_coordinate(x, y);
    return draw_text_(p.x, p.y,text,style);
}
inline WindowDrawResult WindowDefine::WindowBase::draw(WindowControl::Picture *image)
{
    return draw_image(image);
}
inline WindowDrawResult WindowBase::draw(WindowControl::WindowElement &control)
{
    return draw(&control);
}
inline WindowDrawResult WindowBase::draw(WindowControl::Picture &image)
{
    return draw_image(&image);
}
inline VEC::Color WindowBase::get_pixel(const VEC::Vec2 & pos) const
{
    VEC::Color color;
    get_pixel(pos.x,pos.y,&color.r,&color.g,&color.b);
    return color;
}
inline VEC::Color WindowBase::get_pixel(int x, int y) const
{
    VEC::Color color;
    get_pixel(x,y,&color.r,&color.g,&color.b);
    return color;
}
inline void WindowBase::set_transform(const VEC::Mat4 &)
{}
inline VEC::Mat4 WindowBase::get_transform() const
{
    return VEC::Mat4(1.0f);
}
inline bool WindowBase::in_focus() const
{
    return m_inFocus;
}
inline void WindowBase::set_key_down_callback(WindowCallback::KeyCallback callback)
{
    m_keyDownCallback=callback;
}
inline void WindowBase::set_key_up_callback(WindowCallback::KeyCallback callback)
{
    m_keyUpCallback=callback;
}
inline void WindowBase::set_input_callback(WindowCallback::InputCallback callback)
{
    m_inputCallback=callback;
}
inline void WindowBase::set_draw_callback(WindowCallback::DrawCallback callback)
{
    m_drawCallback=callback;
}
inline void WindowBase::set_size_callback(WindowCallback::WindowSizeCallback callback)
{
    m_windowSizeCallback = callback;
}
inline void WindowBase::set_mouse_move_callback(WindowCallback::MouseMoveCallback callback)
{
    m_mouseMoveCallback = callback;
}
inline void WindowBase::set_mouse_wheel_callback(WindowCallback::MouseWheelCallback callback)
{
    m_mouseWheelCallbck = callback;
}
inline void WindowBase::set_mouse_botton_down_callback(WindowCallback::KeyCallback callback)
{
    m_mouseButtonDown = callback;
}
inline void WindowBase::set_mouse_botton_up_callback(WindowCallback::KeyCallback callback)
{
    m_mouseButtonUp = callback;
}
inline int WindowBase::set_size(int width, int height) const
{
    return set_size_(width,height);
}
inline int WindowBase::set_size(const VEC::Vector2<int> &size) const
{
    return set_size_(size.x,size.y);
}
inline VEC::Vector2<int> WindowBase::get_size() const
{
    VEC::Vector2<int> size;
    get_size_(&size.x,&size.y);
    return size;
}
inline int WindowBase::set_position_(const VEC::Vector2<int>& position) const
{
    return set_position(position.x,position.y);
}
inline VEC::Vector2<int> WindowBase::get_position_() const
{
    VEC::Vector2<int> position;
    get_position(&position.x,&position.y);
    return position;
}
inline WindowDrawResult WindowBase::clear_background(float r, float g, float b, float a) const
{
    return clear_background_(r, g, b, a);
}
inline WindowDrawResult WindowBase::clear_background(const VEC::Color &color) const
{
    if(color.r>1||color.g>1||color.b>1||color.a>1)
    {
        return clear_background(color/255);
    }
    return clear_background_(color.r,color.g,color.b);
}
inline WindowDrawResult WindowBase::draw_line(float x1, float y1, float x2, float y2, LineStyle *style) const
{
    auto p1= current_coordinate_to_window_coordinate(x1,y1);
    auto p2=current_coordinate_to_window_coordinate(x2,y2);
    
    return draw_line_(p1.x,p1.y,p2.x,p2.y,style);
}
inline double WindowBase::get_time() const
{
    auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(now.time_since_epoch()).count();
}
inline float WindowBase::get_delta_time() const
{
    return m_realUpdateTime;
}

inline VEC::Point WindowBase::NDC_to_pixel_LT(const VEC::Vec2 &ndcPoint) const
{
    return NDC_to_pixel_LT(ndcPoint.x, ndcPoint.y);
}

inline VEC::Point WindowBase::NDC_to_pixel_LT(const VEC::Vec3 &ndcPoint) const
{
    return NDC_to_pixel_LT(ndcPoint.x, ndcPoint.y);
}

inline VEC::Point WindowBase::NDC_to_pixel_LT(float x, float y) const
{
    int screenWidth = m_windowSize.width;
    int screenHeight = m_windowSize.height;

    // 将 NDC 坐标转换为标准化设备坐标
    float normalizedX = (x + 1.0f) / 2.0f;
    float normalizedY = (1.0f - y) / 2.0f;

    // 将标准化设备坐标转换为像素坐标，并四舍五入
    int pixelX = static_cast<int>(std::round(normalizedX * screenWidth));
    int pixelY = static_cast<int>(std::round(normalizedY * screenHeight));

    return VEC::Point(pixelX, pixelY);
}

inline VEC::Vec2 WindowBase::pixel_to_NDC_LT(const VEC::Vec2 &point) const
{
    return pixel_to_NDC_LT(point.width,point.height);
}

inline VEC::Vec2 WindowBase::pixel_to_NDC_LT(const VEC::Size &point) const
{
    return pixel_to_NDC_LT(point.width,point.height);
}

inline VEC::Vec2 WindowBase::pixel_to_NDC_LT(int x, int y) const
{
    int screenWidth = m_windowSize.width;
    int screenHeight = m_windowSize.height;

    // 计算屏幕宽度和高度的倒数
    float invScreenWidth = 1.0f / static_cast<float>(screenWidth);
    float invScreenHeight = 1.0f / static_cast<float>(screenHeight);

    // 计算标准化设备坐标 (NDC)
    float ndcX = (static_cast<float>(x) * invScreenWidth) * 2.0f - 1.0f;
    float ndcY = 1.0f - (static_cast<float>(y) * invScreenHeight) * 2.0f;

    return VEC::Vec2(ndcX, ndcY);
}

inline VEC::Point WindowBase::NDC_to_pixel_CC(const VEC::Vec2 &ndcPoint) const
{
    int screenWidth;
    int screenHeight;
    get_size_(&screenWidth, &screenHeight);
    float x = (ndcPoint.x + 1.0f) * 0.5f * screenWidth;
    float y = (1.0f - ndcPoint.y) * 0.5f * screenHeight;

    x -= screenWidth * 0.5f;
    y -= screenHeight * 0.5f;

    return VEC::Point(x, -y);
}

inline VEC::Point WindowBase::NDC_to_pixel_CC(const VEC::Vec3 &ndcPoint) const
{
    VEC::Vec2 ndcPoint2D(ndcPoint.x, ndcPoint.y);
    return NDC_to_pixel_CC(ndcPoint2D);
}

inline VEC::Point WindowBase::NDC_to_pixel_CC(float x, float y) const
{
    return NDC_to_pixel_CC(VEC::Vec2(x, y));
}

inline VEC::Vec2 WindowBase::pixel_to_NDC_CC(const VEC::Vec2 &point) const
{
    int screenWidth;
    int screenHeight;
    get_size_(&screenWidth, &screenHeight);

    float ndcX = (2.0f * point.x / screenWidth) - 1.0f;
    float ndcY = 1.0f - (2.0f * point.y / screenHeight);

    return VEC::Vec2(ndcX, ndcY);
}

inline VEC::Vec2 WindowBase::pixel_to_NDC_CC(const VEC::Size &point) const
{
    return pixel_to_NDC_CC(VEC::Vec2(point.width, point.height));
}

inline VEC::Vec2 WindowBase::pixel_to_NDC_CC(int x, int y) const
{
    return pixel_to_NDC_CC(VEC::Vec2(x, y));
}

inline VEC::Point WindowBase::current_coordinate_to_window_coordinate(float x, float y) const
{
    switch (coordinateStyle)
    {
    case CoordinateStyle::NormalizedDeviceCoordinates:
        return NDC_to_pixel_LT(x, y);
    case CoordinateStyle::WindowCoordinates:
        return VEC::Point(x, y);
    default:
        return VEC::Point(x, y);
        break;
    }
}

inline VEC::Point WindowBase::current_coordinate_to_window_coordinate(const VEC::Vec2 &point) const
{
    return current_coordinate_to_window_coordinate(point.x,point.y);
}

inline VEC::Point WindowBase::current_coordinate_to_window_coordinate(const VEC::Vec3 &point) const
{
    return current_coordinate_to_window_coordinate(point.x,point.y);
}

}// namespace WindowDefine

} // namespace OneKeyEngine
#endif