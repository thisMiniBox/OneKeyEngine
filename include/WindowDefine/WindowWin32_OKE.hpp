#ifndef __OKE_WINDOW_WIN32_H
#define __OKE_WINDOW_WIN32_H

//**********************************************
//要记得链接库gdi32,User32
//**********************************************

#include"Window_OKE.hpp"
#include"OpenGL_OKE.hpp"

#ifdef _WIN32

#include<windows.h>

#include<iostream>

#pragma comment(lib, "User32")
#pragma comment(lib, "gdi32")
#pragma comment(lib, "opengl32")
#pragma comment(lib, "Advapi32")

namespace OneKeyEngine
{
namespace WindowDefine
{
class Window2DWin32:public Window2DBase
{
protected:
#define __WINDOW_UPDATE_TIMER 1
    const wchar_t* c_windowClassName=(L"__OKE__WINDOW__");

    HWND m_windowHandle=nullptr;
    HDC m_paintHDC=nullptr;
    PAINTSTRUCT m_paintStruct={};
    MSG m_message={};

    HBITMAP m_canvas=nullptr;

    float m_updateTime=-1;
    float m_shouldUpdateTime=0.0f;


    mutable Enhancement::FontCacheWin32 font_cache;

    /// @brief 获取窗口消息
    /// @return 
    int get_event();
    /// @brief 分配窗口消息
    /// @return 
    int execution_event();


    /// @brief 窗口回调
    /// @param hWnd 
    /// @param message 
    /// @param wParam 
    /// @param lParam 
    /// @return 
    static LRESULT CALLBACK WindowCAllback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    virtual int on_paint();
    int on_keyDown(WPARAM wParam, LPARAM lParam);
    int on_keyUp(WPARAM wParam, LPARAM lParam);
    int on_input(WPARAM wParam,LPARAM lParam);
    virtual int on_size(WPARAM wParam,LPARAM lParam);
    int on_mouse_callback(UINT message,WPARAM wParam,LPARAM lParam);

    WindowDrawResult draw_line_HDC(HDC hdc,int x1,int y1,int x2,int y2,LineStyle*style=nullptr)const;
    WindowDrawResult draw_pixel_HDC(HDC hdc,int x,int y,float colorR, float colorG, float colorB)const;
    WindowDrawResult draw_rectangle_HDC(HDC hdc,int x, int y, int width, int height, LineStyle *style=nullptr)const;
    WindowDrawResult clear_background_HDC(HDC hdc,COLORREF color)const;
    WindowDrawResult draw_text_HDC(HDC hdc,int x,int y,const char*text,const TextStyle* style)const;
    WindowDrawResult draw_polygonal_HDC(HDC hdc,float x,float y,float* points,uint32_t pointsSize,LineStyle* style)const;
    WindowDrawResult draw_fill_polygonal_HDC(HDC hdc,float x,float y,float* points,uint32_t pointsSize,float r = 1.f, float g = 1.f, float b = 1.f, float a = 1.f)const;

    HDC begin_paint();
    void end_paint();

    HBITMAP create_canvas(int w,int h);
    void delete_canvas();
public:
    Window2DWin32(Window2DWin32* parent=nullptr);
    ~Window2DWin32();
    /// @brief 创建窗口
    /// @param parent 父窗口
    /// @param x 窗口位置
    /// @param y 
    /// @param width 窗口大小
    /// @param height 
    /// @return 
    virtual WindowCreateResult create_window(const std::string& titleName="窗口",int x=200,int y=100,int width=800,int height=600,WindowBase* parent=nullptr)override;
    /// @brief 关闭窗口
    /// @return 
    int close()override;
    /// @brief 设置窗口大小
    /// @param width 
    /// @param height 
    /// @return 返回0表示失败
    int set_size_(int width,int height)const override;
    int set_size_win32(int width, int height)const;
    /// @brief 获取窗口大小
    /// @param width 
    /// @param height 
    /// @return 返回0表示失败
    int get_size_(int* width,int* height)const override;
    /// @brief 设置窗口位置
    /// @param x 
    /// @param y 
    /// @return 
    int set_position(int x,int y)const override;
    /// @brief 获取窗口位置
    /// @param x 
    /// @param y 
    /// @return 
    int get_position(int* x,int *y)const override;
    void set_window_title(const std::string& title)const override;
    virtual std::string get_window_title()const override;

    virtual WindowDrawResult draw_line_(float x1,float y1,float x2,float y2,LineStyle*style=nullptr)const override;
    virtual WindowDrawResult draw_pixel_(float x, float y, float colorR, float colorG, float colorB,float colorA=1.f)const override;
    virtual WindowDrawResult draw_rectangle_(float x, float y, float width, float height,LineStyle* style=nullptr)const override;
    virtual WindowDrawResult draw_polygonal_(float x,float y,float* points,uint32_t pointsSize,LineStyle* style=nullptr)const override;
    virtual WindowDrawResult draw_fill_polygonal_(float x, float y, float *points, uint32_t pointsArraySize, float r = 1.f, float g = 1.f, float b = 1.f, float a = 1.f) const override;
    virtual WindowDrawResult draw_text_(float x,float y,const char* text,TextStyle* style=nullptr)const override;
    virtual WindowDrawResult draw_image(WindowControl::Picture* image)override;
    virtual WindowDrawResult clear_background_(float r = 0, float g = 0, float b = 0,float a = 1.0f) const override;
    WindowDrawResult get_pixel(int x,int y,float* r,float* g,float *b)const override;

    virtual void set_transform(const VEC::Mat4&)override;
    virtual VEC::Mat4 get_transform()const override;

    virtual void set_mouse_position(const VEC::Vec2&pos)override;
    void set_mode(ModeType mode,ModeValue value)const override;

    int processing_event()override;
    /// @brief 处理窗口消息直到触发重绘
    /// @return 
    virtual int should_draw()override;
    /// @brief 设置窗口的刷新频率
    /// @param frameRate 每秒刷新多少次
    /// @return 
    virtual float set_frame_rate(float frameRate)override;
    virtual void refresh_screen()const override;

    virtual bool set_clipboard_text(const std::string& str)const override;
    virtual std::string get_clipboard_text()const override;

    KeyCode virtual_key_to_key_code(WPARAM wParam);
    void quaternion_to_matrix(const VEC::Quaternion& q, XFORM& xForm);

    /// @brief 获取画布句柄，不是客户区的句柄，除非画布创建失败
    /// @return 
    virtual HDC get_current_hdc()const;
    /// @brief 获取窗口句柄
    /// @return 
    HWND get_handle()const;
    std::string get_font_path(const std::string& fontName) const;

    void operator=(const Window2DWin32& other)=delete;
};

#ifdef __glad_h_
#pragma comment(lib, "freetype")
/// @brief 在win32环境下运行的openGL窗口
class WindowOpenGLWin32 : public Window2DWin32
{
protected:
    mutable GL::ShaderGL* s_shaderBase=nullptr;
    mutable GL::LineStrip* s_lineBase=nullptr;
    mutable GL::ShaderGL* s_textShader=nullptr;

    GL::ShaderGL s_imageShader;
    GL::Element s_imageVectex;
    mutable GL::Buffer s_textVectex;

#ifdef FT2BUILD_H_
    mutable FT_Library s_ft=nullptr;
    mutable std::unordered_map<Font,FT_Face> s_face;
    mutable std::unordered_map<FT_Face,std::unordered_map<GLuint, Character>> s_characters;
#endif

    

    HGLRC m_hglrc = nullptr;
    HDC m_hdc=nullptr;
    static bool s_initedOpenGL;

    GLuint m_fontBase=0;

    GL::LineStrip* get_line_base()const;


    int on_size(WPARAM wParam,LPARAM lParam)override;

    VEC::Mat4 m_mat4=VEC::Mat4::unit_matrix();

public:
    /// @brief 多边形绘制支持的最大顶点数，在所有opengl窗口第一次绘制多边形前修改有效
    static uint32_t max_polygon_vertices;

    /// @brief 激活绘图回调和交换缓冲区
    /// @return 
    int on_paint()override;

    WindowOpenGLWin32();
    ~WindowOpenGLWin32();

    /// @brief 创建窗口
    /// @param parent 父窗口
    /// @param x 窗口位置X
    /// @param y 窗口位置y
    /// @param width 窗口宽度、包含边框
    /// @param height 窗口高度、包含边框及标题栏
    /// @return 
    WindowCreateResult create_window(const std::string& titleName="窗口",int x=200,int y=100,int width=800,int height=600,WindowBase* parent=nullptr)override;
    HDC get_current_hdc()const;
    bool switch_openGL()const;
    void swap_buffer()const;

    void clear_buffer()const;

    virtual WindowDrawResult draw_line_(float x1,float y1,float x2,float y2,LineStyle*style=nullptr)const override;
    virtual WindowDrawResult draw_pixel_(float x, float y, float colorR, float colorG, float colorB,float colorA=1.f)const override;
    virtual WindowDrawResult draw_rectangle_(float x, float y, float width, float height,LineStyle* style=nullptr)const override;
    virtual WindowDrawResult draw_polygonal_(float x,float y,float* points,uint32_t pointsSize,LineStyle* style=nullptr)const override;
    virtual WindowDrawResult draw_fill_polygonal_(float x, float y, float *points, uint32_t pointsArraySize, float r = 1.f, float g = 1.f, float b = 1.f, float a = 1.f) const override;
    virtual WindowDrawResult draw_text_(float x,float y,const char* text,TextStyle* style=nullptr)const override;
    virtual WindowDrawResult draw_image(WindowControl::Picture* image)override;
    virtual WindowDrawResult clear_background_(float r = 0, float g = 0, float b = 0,float a=1.0f) const override;

    virtual void set_transform(const VEC::Mat4&)override;
    virtual VEC::Mat4 get_transform()const override;
    

    GL::ShaderGL* get_line_shader()const;
    GL::ShaderGL* get_text_shader()const;
    
    FT_Face get_free_type_font(const TextStyle* style)const;
    Character* get_char_character(char32_t ch,FT_Face ft_font)const;
    void clear_char_texture(uint32_t id)const;

};
#endif





inline HBITMAP Window2DWin32::create_canvas(int w, int h)
{
    delete_canvas();
    m_paintHDC = CreateCompatibleDC(m_paintStruct.hdc);
    m_canvas=CreateCompatibleBitmap(m_paintStruct.hdc,w,h);
    SelectObject(m_paintHDC,m_canvas);

    SetGraphicsMode(m_paintHDC, GM_ADVANCED);
    SetBkMode(m_paintHDC, TRANSPARENT);
    return m_canvas;
}

inline void Window2DWin32::delete_canvas()
{
    if (m_canvas)
        DeleteObject(m_canvas);
    if (m_paintHDC)
        DeleteDC(m_paintHDC);
    m_paintHDC=nullptr;
    m_canvas=nullptr;
}

Window2DWin32::Window2DWin32(Window2DWin32 *parent)
{
    m_lastRefreshTime = get_time();
    m_shouldUpdateTime = m_lastRefreshTime + m_updateTime;
}

Window2DWin32::~Window2DWin32()
{
    delete_canvas();
    close();
}
inline int Window2DWin32::close()
{
    if(m_windowHandle)
        return DestroyWindow(m_windowHandle);
    m_windowHandle=nullptr;
    return 0;
}
inline int Window2DWin32::set_size_(int width, int height) const
{
    return set_size_win32(width,height);
}
inline int Window2DWin32::set_size_win32(int width, int height) const
{
    if(m_windowHandle==nullptr)
        return 0;
    return SetWindowPos(m_windowHandle, NULL, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE);
}
inline int Window2DWin32::get_size_(int *width, int *height) const
{
    RECT rect;
    if(GetClientRect(m_windowHandle,&rect))
    {
        if (width)
            *width = rect.right;
        if (height)
            *height = rect.bottom;
        return 1;
    }
    return 0;
}
inline int Window2DWin32::set_position(int x, int y) const
{
    if(m_windowHandle==nullptr)
        return 0;
    return SetWindowPos(m_windowHandle, NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}
inline int Window2DWin32::get_position(int *x, int *y) const
{
    if (x == nullptr || y == nullptr)
        return 0;
    RECT rect;
    if (GetWindowRect(m_windowHandle, &rect))
    {
        *x = rect.left;
        *y = rect.top;
        return 1;
    }
    return 0;
}
inline void Window2DWin32::set_window_title(const std::string &title) const
{
    auto titleW=utf8_to_utf16(title);
    SetWindowTextW(m_windowHandle,titleW.c_str());
}
inline std::string Window2DWin32::get_window_title() const
{
    std::string title;
    int length= GetWindowTextLengthA(m_windowHandle)+1;
    title.resize(length);
    GetWindowTextA(m_windowHandle,title.data(),length);
    title.pop_back();
    return title;
}
inline WindowDrawResult Window2DWin32::draw_line_(float x1, float y1, float x2, float y2, LineStyle *style) const
{
    if (m_windowHandle == nullptr)
        return WindowDrawResult::WDR_WINDOW_NOT_CREATED;
    HDC hdc = get_current_hdc();
    WindowDrawResult result = draw_line_HDC(hdc, x1, y1, x2, y2, style);
    // ReleaseDC(m_windowHandle, hdc);
    return result;
}
inline WindowDrawResult Window2DWin32::draw_pixel_(float x, float y, float colorR, float colorG, float colorB,float colorA)const
{
    if (m_windowHandle == nullptr)
        return WindowDrawResult::WDR_WINDOW_NOT_CREATED;
    HDC hdc = get_current_hdc();
    WindowDrawResult result = draw_pixel_HDC(hdc,x,y,colorR,colorG,colorB);
    //ReleaseDC(m_windowHandle, hdc);
    return result;
}
inline WindowDrawResult Window2DWin32::draw_rectangle_(float x, float y, float width, float height,LineStyle* style)const
{
    if (m_windowHandle == nullptr)
        return WindowDrawResult::WDR_WINDOW_NOT_CREATED;
    HDC hdc = get_current_hdc();
    WindowDrawResult result = draw_rectangle_HDC(hdc,x, y, width, height);
    return result;
}

inline WindowDrawResult Window2DWin32::draw_polygonal_(float x, float y, float *points, uint32_t pointsSize, LineStyle *style) const
{
    if (m_windowHandle == nullptr)
        return WindowDrawResult::WDR_WINDOW_NOT_CREATED;
    HDC hdc = get_current_hdc();
    WindowDrawResult result = draw_polygonal_HDC(hdc,x, y, points, pointsSize,style);
    return result;
}

inline WindowDrawResult Window2DWin32::draw_fill_polygonal_(float x, float y, float *points, uint32_t pointsArraySize, float r, float g, float b, float a) const
{
    if (m_windowHandle == nullptr)
        return WindowDrawResult::WDR_WINDOW_NOT_CREATED;
    HDC hdc = get_current_hdc();
    WindowDrawResult result = draw_fill_polygonal_HDC(hdc,x, y, points, pointsArraySize,r,g,b,a);
    return result;
}

inline WindowDrawResult Window2DWin32::draw_text_(float x, float y, const char *text,TextStyle *style) const
{
    if (m_windowHandle == nullptr)
        return WindowDrawResult::WDR_WINDOW_NOT_CREATED;
    HDC hdc = get_current_hdc();
    WindowDrawResult result = draw_text_HDC(hdc,x, y, text,style);
    //ReleaseDC(m_windowHandle, hdc);
    return result;
}

inline WindowDrawResult Window2DWin32::clear_background_(float r, float g, float b,float a) const
{
    if (m_windowHandle == nullptr)
        return WindowDrawResult::WDR_WINDOW_NOT_CREATED;
    HDC hdc = get_current_hdc();
    WindowDrawResult result = clear_background_HDC(hdc,RGB(r*255,g*255,b*255));
    return result;
}

inline WindowDrawResult Window2DWin32::get_pixel(int x, int y, float *r, float *g, float *b) const
{
    HDC hdc = get_current_hdc();
    if (!hdc)
        return WindowDrawResult::WDR_WIN32_FAILED_TO_OBTAIN_DEVICE_HANDLE;
    if (x > m_windowSize.width || y > m_windowSize.height || x < 0 || y < 0)
        return WindowDrawResult::WDR_PARAMETER_ERROR;
    COLORREF color = GetPixel(hdc, x, y);
    *r = (color & 0xFF)/255.0f;
    *g = ((color >> 8) & 0xFF)/255.0f;
    *b = ((color >> 16) & 0xFF)/255.0f;
    return WindowDrawResult::WDR_SUCCESS;
}
inline void Window2DWin32::set_transform(const VEC::Mat4 & mat4)
{

    XFORM xform;
    xform.eM11 = mat4[0][0];
    xform.eM12 = mat4[0][1];
    xform.eM21 = mat4[1][0];
    xform.eM22 = mat4[1][1];
    xform.eDx = mat4[0][3];
    xform.eDy = mat4[1][3];
    SetWorldTransform(get_current_hdc(),&xform);
}
inline VEC::Mat4 Window2DWin32::get_transform() const
{
    XFORM xform;
    HDC hdc = get_current_hdc();
    if (!GetWorldTransform(hdc, &xform))
    {
        return VEC::Mat4(1.0);
    }
    VEC::Mat4 result(1.0);
    result[0][0] = xform.eM11;
    result[0][1] = xform.eM12;
    result[1][0] = xform.eM21;
    result[1][1] = xform.eM22;
    result[3][0] = xform.eDx;
    result[3][1] = xform.eDy;
    return result;
}
inline void Window2DWin32::set_mouse_position(const VEC::Vec2 &pos)
{
    SetCursorPos(pos.x, pos.y);
    POINT point;
    point.x=pos.x;
    point.y=pos.y;
    ScreenToClient(m_windowHandle,&point);
    mousePosition=VEC::Vec2(point.x,point.y);
}
inline void Window2DWin32::set_mode(ModeType mode, ModeValue value) const
{
    switch (mode)
    {
    case ModeType::ShowCursor:
        switch (value)
        {
        case ModeValue::True:
            ShowCursor(true);
            break;
        case ModeValue::False:
            ShowCursor(false);
            break;
        default:
            break;
        }
        break;
    case ModeType::CaptureCursor:
        switch (value)
        {
        case ModeValue::True:
            SetCapture(m_windowHandle);
            break;
        case ModeValue::False:
            ReleaseCapture();
            break;
        default:
            break;
        }
        break;  
    default:
        break;
    }
}
inline int Window2DWin32::processing_event()
{
    if (get_event())
    {
        execution_event();
        return 1;
    }
    return 0;
}
inline int Window2DWin32::should_draw()
{
    while (1)
    {
        if(get_event()<2)
        {
            execution_event();
            if (m_message.hwnd == m_windowHandle && m_message.message == WM_PAINT)
            {
                return 1;
            }
            if(m_message.message==WM_QUIT)
            {
                break;
            }
        }
        else
        {
            Sleep(1);
        }
    }
    return 0;
}
inline float Window2DWin32::set_frame_rate(float frameRate)
{
    if (frameRate == 0)
    {
        m_updateTime = -1;
        KillTimer(m_windowHandle,__WINDOW_UPDATE_TIMER);
        return 0;
    }
    if(m_updateTime==1.0 / frameRate)
        return m_updateTime;
    KillTimer(m_windowHandle,__WINDOW_UPDATE_TIMER);
    m_updateTime = 1.0 / frameRate;
    SetTimer(m_windowHandle,__WINDOW_UPDATE_TIMER,m_updateTime,NULL);
    return m_updateTime;
}
inline void Window2DWin32::refresh_screen() const
{
    InvalidateRect(m_windowHandle,nullptr,false);
    //UpdateWindow(m_windowHandle);
}
inline bool Window2DWin32::set_clipboard_text(const std::string &text) const
{
    // 打开剪贴板
    if (!OpenClipboard(m_windowHandle))
        return false;

    // 清空剪贴板内容
    EmptyClipboard();

    // 将UTF-8字符串转换为宽字符（Unicode）
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    std::wstring wide_text = converter.from_bytes(text);

    // 分配内存并设置剪贴板数据
    HGLOBAL hClipboardData;
    size_t size = (wide_text.length() + 1) * sizeof(wchar_t);
    hClipboardData = GlobalAlloc(GMEM_MOVEABLE, size);
    if (hClipboardData == nullptr)
    {
        CloseClipboard();
        return false;
    }

    wchar_t *clipboardText = static_cast<wchar_t *>(GlobalLock(hClipboardData));
    if (clipboardText == nullptr)
    {
        GlobalFree(hClipboardData);
        CloseClipboard();
        return false;
    }

    // 复制数据到剪贴板内存
    memcpy(clipboardText, wide_text.c_str(), size);

    GlobalUnlock(hClipboardData);
    SetClipboardData(CF_UNICODETEXT, hClipboardData);
    CloseClipboard();

    return true;
}
inline std::string Window2DWin32::get_clipboard_text() const
{
    // 打开剪贴板
    if (!OpenClipboard(m_windowHandle))
        return "";

    // 获取剪贴板中数据句柄
    HANDLE hClipboardData = GetClipboardData(CF_UNICODETEXT);
    if (hClipboardData == nullptr)
    {
        CloseClipboard();
        return "";
    }

    // 锁定数据句柄并获取数据指针
    wchar_t* clipboardText = static_cast<wchar_t*>(GlobalLock(hClipboardData));
    if (clipboardText == nullptr)
    {
        CloseClipboard();
        return "";
    }

    // 将宽字符转换为UTF-8字符串
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    std::string utf8_text = converter.to_bytes(clipboardText);

    // 释放句柄和关闭剪贴板
    GlobalUnlock(hClipboardData);
    CloseClipboard();

    return utf8_text;
}
inline KeyCode Window2DWin32::virtual_key_to_key_code(WPARAM wParam)
{
    return (KeyCode)wParam;
}
inline void Window2DWin32::quaternion_to_matrix(const VEC::Quaternion &q, XFORM &xForm)
{
    float ww = q.w * q.w;
    float xx = q.x * q.x;
    float yy = q.y * q.y;
    float zz = q.z * q.z;
    float wx = q.w * q.x;
    float wy = q.w * q.y;
    float wz = q.w * q.z;
    float xy = q.x * q.y;
    float xz = q.x * q.z;
    float yz = q.y * q.z;

    xForm.eM11 = ww + xx - yy - zz;
    xForm.eM12 = 2 * (xy + wz);
    xForm.eM21 = 2 * (xy - wz);
    xForm.eM22 = ww - xx + yy - zz;
}
// inline double Window2DWin32::get_time() const
// {
//     LARGE_INTEGER time;
//     if (!QueryPerformanceCounter(&time))
//     {
//         return -1;
//     }

//     return (double)time.QuadPart / frequency.QuadPart;
// }
inline int Window2DWin32::get_event()
{
    // if(m_updateTime>0)
    // {
    //     auto nowTime = get_time();
    //     if (nowTime > m_shouldUpdateTime)
    //     {
    //         m_shouldUpdateTime = nowTime + m_updateTime;
    //         refresh_screen();
    //     }
    // }
    if(PeekMessage(&m_message, NULL, 0, 0, PM_REMOVE))
    {
        if (m_message.message == WM_QUIT)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    return 2;
}

inline int Window2DWin32::execution_event()
{
    TranslateMessage(&m_message);
    DispatchMessageW(&m_message);
    return 1;
}
LRESULT CALLBACK Window2DWin32::WindowCAllback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Window2DWin32 *mainWind = reinterpret_cast<Window2DWin32 *>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
    switch (message)
    {
    case WM_KEYDOWN:
    {
        mainWind->on_keyDown(wParam, lParam);
        break;
    }

    case WM_KEYUP:
    {
        mainWind->on_keyUp(wParam, lParam);
        break;
    }
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MOUSEWHEEL:
    case WM_MOUSEHWHEEL:
    case WM_MOUSEMOVE:
    {
        mainWind->on_mouse_callback(message, wParam, lParam);
        break;
    }
    case WM_CHAR:
    {
        mainWind->on_input(wParam,lParam);
        break;
    }

    // case WM_IME_CHAR:
    // {
    //     mainWind->on_input(wParam, lParam);
    //     break;
    // }
    case WM_TIMER:
    {
        if(wParam==__WINDOW_UPDATE_TIMER)
        {
            mainWind->refresh_screen();
        }
        break;
    }
    case WM_PAINT:
    {
        mainWind->on_paint();
        break;
    }
    case WM_SIZE:
    {
        mainWind->on_size(wParam,lParam);
        break;
    }
    case WM_SETFOCUS:
    {
        mainWind->m_inFocus=true;
        break;
    }
    case WM_KILLFOCUS:
    {
        mainWind->m_inFocus=false;
        break;
    }
    case WM_CREATE:
    {
        LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
        mainWind = (Window2DWin32 *)pcs->lpCreateParams;
        ::SetWindowLongPtrW(
            hWnd,
            GWLP_USERDATA,
            reinterpret_cast<LONG_PTR>(mainWind));
        POINT mousePos;
        GetCursorPos(&mousePos);
        mainWind->mousePosition.x = mousePos.x;
        mainWind->mousePosition.y = mousePos.y;
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }
    return 0;
}
inline int Window2DWin32::on_paint()
{
    auto nowTime = get_time();
    m_realUpdateTime = nowTime - m_lastRefreshTime;
    m_lastRefreshTime = nowTime;
    begin_paint();
    if (m_drawCallback)
        m_drawCallback(this);
    end_paint();
    return 0;
}
inline int Window2DWin32::on_keyDown(WPARAM wParam, LPARAM lParam)
{
    if (m_keyDownCallback)
        m_keyDownCallback(this, virtual_key_to_key_code(wParam));
    return 0;
}

inline int Window2DWin32::on_keyUp(WPARAM wParam, LPARAM lParam)
{
    if (m_keyUpCallback)
        m_keyUpCallback(this, virtual_key_to_key_code(wParam));
    return 0;
}
static std::string convert_to_UTF8(const std::wstring& utf16Char) {
    std::vector<char> utf8Str(4*utf16Char.size()); // 一个UTF-16字符最多可以转换为4个UTF-8字节
    int length = WideCharToMultiByte(CP_UTF8, 0, utf16Char.c_str(), utf16Char.size(), utf8Str.data(), utf8Str.size(), NULL, NULL);
    return std::string(utf8Str.data(), length);
}
inline int Window2DWin32::on_input(WPARAM wParam, LPARAM lParam)
{
    // static wchar_t charBuffer;
    // if(wParam>128)
    // {
    //     if(charBuffer!=0)
    //     {
    //         std::cout << "第二个字符：" << std::hex << wParam << std::endl;
    //         charBuffer|=(wParam<<8);
    //         if(m_inputCallback)
    //             m_inputCallback(this,charBuffer);
    //         charBuffer=0;
    //         return 0;
    //     }   
    //     else
    //     {
    //         std::cout << "第一个字符：" << std::hex << wParam << std::endl;
    //         charBuffer=wParam;
    //         return 1;
    //     }
    // }

    if(m_inputCallback)
        m_inputCallback(this,wParam);
    return 0;
}

inline int Window2DWin32::on_size(WPARAM wParam, LPARAM lParam)
{
    //std::cout<<"onSize"<<std::endl;
    m_windowSize=VEC::Size(LOWORD(lParam),HIWORD(lParam));
    if(m_canvas)
        create_canvas(LOWORD(lParam),HIWORD(lParam));
    if(m_windowSizeCallback)
        m_windowSizeCallback(this,m_windowSize.width,m_windowSize.height);
    else
        return DefWindowProc(m_windowHandle,WM_SIZE,wParam,lParam);
    return 0;
}

inline int Window2DWin32::on_mouse_callback(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_LBUTTONDOWN:
    {
        if (m_keyDownCallback)
            m_keyDownCallback(this, KeyCode::MouseLeftButton);
        if (m_mouseButtonDown)
            m_mouseButtonDown(this, KeyCode::MouseLeftButton);
        break;
    }
    case WM_RBUTTONDOWN:
    {
        if (m_keyDownCallback)
            m_keyDownCallback(this, KeyCode::MouseRightButton);
        if (m_mouseButtonDown)
            m_mouseButtonDown(this, KeyCode::MouseRightButton);
        break;
    }
    case WM_MBUTTONDOWN:
    {
        if (m_keyDownCallback)
            m_keyDownCallback(this, KeyCode::MouseMiddleButton);
        if (m_mouseButtonDown)
            m_mouseButtonDown(this, KeyCode::MouseMiddleButton);
        break;
    }
    case WM_MBUTTONUP:
    {
        if (m_keyUpCallback)
            m_keyUpCallback(this, KeyCode::MouseMiddleButton);
        if (m_mouseButtonUp)
            m_mouseButtonUp(this, KeyCode::MouseMiddleButton);
        break;
    }
    case WM_LBUTTONUP:
    {
        if (m_keyUpCallback)
            m_keyUpCallback(this, KeyCode::MouseLeftButton);
        if (m_mouseButtonUp)
            m_mouseButtonUp(this, KeyCode::MouseLeftButton);
        break;
    }
    case WM_RBUTTONUP:
    {
        if (m_keyUpCallback)
            m_keyUpCallback(this, KeyCode::MouseRightButton);
        if (m_mouseButtonUp)
            m_mouseButtonUp(this, KeyCode::MouseRightButton);
        break;
    }
    case WM_MOUSEMOVE:
    {
        mousePosition.x = LOWORD(lParam);
        mousePosition.y = HIWORD(lParam);
        if (m_mouseMoveCallback)
        {
            m_mouseMoveCallback(this, mousePosition.x, mousePosition.y);
        }
        break;
    }
    case WM_MOUSEWHEEL:
    {
        short w=HIWORD(wParam);
        if (m_mouseWheelCallbck)
            m_mouseWheelCallbck(this, 0, (float)w / (WHEEL_DELTA));
        break;
    }
    case WM_MOUSEHWHEEL:
    {
        short w=HIWORD(wParam);
        if (m_mouseWheelCallbck)
            m_mouseWheelCallbck(this, (float)w / (WHEEL_DELTA), 0);
        break;
    }
    default:
        break;
    }
    return 0;
}

inline WindowDrawResult Window2DWin32::draw_line_HDC(HDC hdc, int x1, int y1, int x2, int y2, LineStyle *style)const
{
    if (hdc == nullptr)
        return WindowDrawResult::WDR_WIN32_FAILED_TO_OBTAIN_DEVICE_HANDLE;
    if (style)
    {
        HPEN hpen;
        if (style->spacing == nullptr)
        {
            hpen = CreatePen(PS_SOLID, 0, RGB(style->color.r*255, style->color.g*255, style->color.b*255));
        }
        else
        {
            hpen = CreatePen(PS_DASH, 0, RGB(style->color.r*255, style->color.g*255, style->color.b*255));
        }
        if(hpen==nullptr)
        {
            return WindowDrawResult::WDR_WIN32_PEN_CREATED_FAILED;
        }
        HPEN oldhpen = (HPEN)SelectObject(hdc, hpen);
        auto result= MoveToEx(hdc, x1, y1, NULL);
        result =LineTo(hdc, x2, y2);
        SelectObject(hdc, oldhpen);
        DeleteObject(hpen);
        if(!result)return WindowDrawResult::WDR_FALSE;
    }
    else
    {
        auto result= MoveToEx(hdc, x1, y1, NULL);
        if(!result)return WindowDrawResult::WDR_FALSE;
        result =LineTo(hdc, x2, y2);
    }
    return WindowDrawResult::WDR_SUCCESS;
}
inline WindowDrawResult Window2DWin32::draw_pixel_HDC(HDC hdc, int x, int y, float colorR, float colorG, float colorB)const
{
    if (hdc == nullptr)
        return WindowDrawResult::WDR_WIN32_FAILED_TO_OBTAIN_DEVICE_HANDLE;
    COLORREF color;
    if(colorR>1||colorG>1||colorB>1)
    {
        color=RGB(colorR,colorG,colorB);
    }
    else
    {
        color=RGB(colorR*255,colorG*255,colorB*255);
    }
    if(SetPixel(hdc,x,y,color)==color)
    {
        return WindowDrawResult::WDR_SUCCESS;
    }
    return WindowDrawResult::WDR_FALSE;
}
inline WindowDrawResult Window2DWin32::draw_rectangle_HDC(HDC hdc, int x, int y, int width, int height, LineStyle *style)const
{
    if (hdc == nullptr)
        return WindowDrawResult::WDR_WIN32_FAILED_TO_OBTAIN_DEVICE_HANDLE;
    RECT rectangle={x,y,x+width,y+height};
    HBRUSH rectLine;
    if(style)
    {
        auto& color=style->color;
        rectLine=CreateSolidBrush(RGB(color.r,color.g,color.b));
    }
    else
    {
        rectLine=CreateSolidBrush(RGB(0,0,0));
    }
    if(rectLine==nullptr)
        return WindowDrawResult::WDR_WIN32_BRUSH_CREATE_FAILED;
    auto result=FrameRect(hdc,&rectangle,rectLine);
    DeleteObject(rectLine);
    if(result)
    {
        return WindowDrawResult::WDR_SUCCESS;
    }
    return WindowDrawResult::WDR_FALSE;
}
inline WindowDrawResult Window2DWin32::clear_background_HDC(HDC hdc, COLORREF color)const
{
    if(!hdc)return WindowDrawResult::WDR_WIN32_FAILED_TO_OBTAIN_DEVICE_HANDLE;
    RECT windowSize;
    if(!GetClientRect(m_windowHandle,&windowSize))return WindowDrawResult::WDR_FALSE;
    HBRUSH brush=CreateSolidBrush(color);
    if(!brush)return WindowDrawResult::WDR_WIN32_BRUSH_CREATE_FAILED;
    FillRect(hdc,&windowSize,brush);
    DeleteObject(brush);
    return WindowDrawResult::WDR_SUCCESS;
}
inline WindowDrawResult Window2DWin32::draw_text_HDC(HDC hdc, int x, int y, const char *text, const TextStyle *style) const
{
    if (hdc == nullptr)
        return WindowDrawResult::WDR_WIN32_FAILED_TO_OBTAIN_DEVICE_HANDLE;
    if (text == nullptr)
        return WindowDrawResult::WDR_PARAMETER_ERROR;
    if (style == nullptr)
    {
        TextStyle textStyle;
        return draw_text_HDC(hdc, x, y, text, &textStyle);
    }

    HFONT hFont = nullptr;
    if (style)
    {
        hFont = font_cache.get_font(style->font);
        if (hFont == nullptr)
            return WindowDrawResult::WDR_WIN32_FONT_CREATE_FAILED;
    }
    HFONT oldFont = nullptr;
    if (hFont)
    {
        oldFont = (HFONT)SelectObject(hdc, hFont);
    }

    if (style)
    {
        SetTextColor(hdc, style->color.to_win32_gdi_color());
    }
    else
    {
        SetTextColor(hdc, RGB(0, 0, 0));
    }
    UINT format = 0;
    if (style)
    {
        switch (style->horizontalAlign)
        {
        case TextAlign::Left:
            format |= DT_LEFT;
            break;
        case TextAlign::Center:
            format |= DT_CENTER;
            break;
        case TextAlign::Right:
            format |= DT_RIGHT;
            break;
        }

        switch (style->verticalAlign)
        {
        case TextAlign::Top:
            format |= DT_TOP;
            break;
        case TextAlign::Center:
            format |= DT_VCENTER;
            break;
        case TextAlign::Bottom:
            format |= DT_BOTTOM;
            break;
        }

        if (style->wordWrap)
        {
            format |= DT_WORDBREAK;
        }

        if (style->singleLine)
        {
            format |= DT_SINGLELINE;
        }
    }
    else
    {
        format = DT_LEFT | DT_TOP;
    }

    // Convert UTF-8 text to wide string
    int wlen = MultiByteToWideChar(CP_UTF8, 0, text, -1, nullptr, 0);
    if (wlen == 0)
    {
        return WindowDrawResult::WDR_FALSE;
    }
    std::wstring wtext(wlen, 0);
    MultiByteToWideChar(CP_UTF8, 0, text, -1, &wtext[0], wlen);

    RECT rect = {x, y, x, y};
    if (style->bounds.x <= 0)
    {
        DrawTextW(hdc, wtext.c_str(), -1, &rect, format | DT_CALCRECT);
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;
        rect = {x, y, x + width, y + height};
    }
    else
    {
        rect = {x, y, x + (long)style->bounds.width, y + (long)style->bounds.height};
    }
    int result = DrawTextW(hdc, wtext.c_str(), -1, &rect, format);

    if (hFont)
    {
        SelectObject(hdc, oldFont);
    }

    return (result > 0) ? WindowDrawResult::WDR_SUCCESS : WindowDrawResult::WDR_FALSE;
}
inline WindowDrawResult Window2DWin32::draw_polygonal_HDC(HDC hdc, float x, float y, float *points, uint32_t pointsSize, LineStyle *style) const
{
    std::vector<POINT> point(pointsSize+1);
    for (uint32_t i = 0; i < pointsSize; ++i)
    {
        POINT p = { static_cast<LONG>(*points + x), static_cast<LONG>(*(points + 1) + y) };
        point[i] = p;
        points += 2;
    }
    point[pointsSize]=point[0];
    HPEN hPen=nullptr;
    HPEN hOldPen=nullptr;
    if (style)
    {
        if (style->spacing)
            hPen = CreatePen(PS_DOT, style->width, style->color.to_win32_gdi_color()); // 蓝色边框
        else
            hPen = CreatePen(PS_SOLID, style->width, style->color.to_win32_gdi_color()); // 蓝色边框
        hOldPen = (HPEN)SelectObject(hdc, hPen);
    }
    Polyline(hdc,point.data(),pointsSize+1);
    if(style)
    {
        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
    }

    return WindowDrawResult::WDR_SUCCESS;
}
inline WindowDrawResult Window2DWin32::draw_fill_polygonal_HDC(HDC hdc, float x, float y, float *points, uint32_t pointsSize, float r, float g, float b, float a) const
{
    std::vector<POINT> point(pointsSize);
    for (uint32_t i = 0; i < pointsSize; ++i)
    {
        POINT p = { static_cast<LONG>(*(points) + x), static_cast<LONG>(*(points + 1) + y) };
        point[i] = p;
        points += 2;
    }
    
    // 创建颜色并设置透明度
    COLORREF fillColor = RGB(static_cast<BYTE>(r * 255), static_cast<BYTE>(g * 255), static_cast<BYTE>(b * 255));
    HBRUSH hBrush = CreateSolidBrush(fillColor);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    // 使用透明的画笔
    HPEN hPen = (HPEN)GetStockObject(NULL_PEN);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    // 设置混合模式以支持透明度
    BLENDFUNCTION blendFunction = { AC_SRC_OVER, 0, static_cast<BYTE>(a * 255), 0 };
    AlphaBlend(hdc, 0, 0, 0, 0, hdc, 0, 0, 0, 0, blendFunction);

    // 绘制填充多边形
    Polygon(hdc, point.data(), pointsSize);

    // 恢复旧的画刷和画笔
    SelectObject(hdc, hOldBrush);
    SelectObject(hdc, hOldPen);
    DeleteObject(hBrush);

    return WindowDrawResult::WDR_SUCCESS;
}
inline HDC Window2DWin32::get_current_hdc() const
{
    if(m_paintHDC)return m_paintHDC;
    return m_paintStruct.hdc;
}
inline HWND Window2DWin32::get_handle() const
{
    return m_windowHandle;
}
inline std::string Window2DWin32::get_font_path(const std::string &fontName) const
{
    HKEY hKey;
    // 打开注册表键
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        std::cerr << "ERROR:: Could not open registry key" << std::endl;
        return "";
    }

    char value[256];          // 用于存储注册表中的键名（即字体名称）
    DWORD value_length = 256; // 键名长度
    DWORD type = 0;           // 注册表值的类型（通常为REG_SZ）
    DWORD index = 0;          // 用于枚举注册表值的索引
    char data[256];           // 用于存储注册表中的值（即字体文件名）
    DWORD data_length = 256;  // 值的长度

    // 枚举注册表中的所有值
    while (RegEnumValueA(hKey, index, value, &value_length, nullptr, &type, reinterpret_cast<LPBYTE>(data), &data_length) == ERROR_SUCCESS) {
        // 如果找到与指定字体名称匹配的注册表值
        if (std::string(value).find(fontName) != std::string::npos) {
            // 构建完整的字体文件路径
            std::string fontPath = "C:\\Windows\\Fonts\\" + std::string(data);
            // 关闭注册表键
            RegCloseKey(hKey);
            // 返回找到的字体文件路径
            return fontPath;
        }
        // 重置长度变量以用于下一次枚举
        value_length = 256;
        data_length = 256;
        // 增加索引以枚举下一个注册表值
        index++;
    }

    // 如果没有找到匹配的字体名称，关闭注册表键并返回错误信息
    RegCloseKey(hKey);
    std::cerr << "ERROR:: Could not find font file path for " << fontName << std::endl;
    return "";
}
inline HDC Window2DWin32::begin_paint()
{
    HDC hdc= BeginPaint(m_windowHandle,&m_paintStruct);
    if(!m_paintHDC)
    {
        create_canvas(m_windowSize.width,m_windowSize.height);
    }
    return hdc;
}
inline void Window2DWin32::end_paint()
{
    BitBlt(m_paintStruct.hdc, 0, 0, m_windowSize.width,m_windowSize.height, m_paintHDC, 0, 0, SRCCOPY);
    EndPaint(m_windowHandle,&m_paintStruct);
}
WindowCreateResult Window2DWin32::create_window(const std::string& titleName,int x,int y,int width,int height,WindowBase* parentWindow)
{
    if (m_windowHandle) {
        return WindowCreateResult::WCR_WINDOW_CREATED;
    }
    
    std::wstring titleNameW=utf8_to_utf16(titleName);
    HWND parent;
    auto detection = parentWindow->is_derived_class<Window2DWin32>(parentWindow);
    if (detection == nullptr)
    {
        parent = nullptr;
    }
    else
    {
        parent = detection->m_windowHandle;
    }
    HINSTANCE hInstance=GetModuleHandleA(nullptr);
    WNDCLASSEXW windowClass = {};
    if (GetClassInfoExW(hInstance, c_windowClassName, &windowClass) == 0) {
        WNDCLASSEXW wc = { 0 };
        wc.cbSize = sizeof(WNDCLASSEXW);
        wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WindowCAllback;
        wc.hInstance = hInstance;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.lpszClassName = c_windowClassName;
        wc.hbrBackground = NULL;

        // 注册窗口类
        if (!RegisterClassExW(&wc)) {
            return WindowCreateResult::WCR_WIN32_WINODW_CLASS_REGISTRATION_FAILED;
        }
    }



    if (parent) {
        m_windowHandle = CreateWindowW(
            c_windowClassName,
            titleNameW.c_str(),
            WS_CHILD | WS_VISIBLE,
            x, y, width, height,
            parent, NULL,
            hInstance, this);
    } else {
        m_windowHandle = CreateWindowW(
            c_windowClassName,
            titleNameW.c_str(),
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            x, y, width, height,
            NULL, NULL,
            hInstance, this);
    }
    if (!m_windowHandle) {
        return WindowCreateResult::WCR_WIN32_WINDOW_CREATE_FAILED;
    }
    return WindowCreateResult::WCR_SUCCESS;
}



//win32OpenGL窗口定义******************************************************

#ifdef __glad_h_

inline GL::ShaderGL *WindowOpenGLWin32::get_line_shader() const
{
    if(s_shaderBase)return s_shaderBase;
    const char *vertexShaderBaseSource = "#version 330 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "\n"
                                     "uniform mat4 transform;\n"
                                     "void main()\n"
                                     "{\n"
                                     "    gl_Position = transform*vec4(aPos, 1.0f);\n"
                                     "}\n";
    const char *fragmentShaderSource = "#version 330 core\n"
                                    "out vec4 FragColor;\n"
                                    "\n"
                                    "uniform vec4 ourColor;\n"
                                    "\n"
                                    "void main()\n"
                                    "{\n"
                                    "    FragColor = vec4(ourColor);\n"
                                    "}\n";
    s_shaderBase=new GL::ShaderGL(vertexShaderBaseSource,fragmentShaderSource,true);
    return s_shaderBase;
}

inline GL::ShaderGL *WindowOpenGLWin32::get_text_shader() const
{
    if (s_textShader)
        return s_textShader;
    // 着色器源代码
    const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection*vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}
)";

    const char *fragmentShaderSource = R"(
#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec4 textColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = textColor * sampled;
}
)";
    s_textShader=new GL::ShaderGL;
    s_textShader->load_shader(vertexShaderSource,fragmentShaderSource);
    return s_textShader;
}

inline FT_Face WindowOpenGLWin32::get_free_type_font(const TextStyle *style) const
{
    switch_openGL();
    if(style==nullptr)
    {
        TextStyle sty;
        return get_free_type_font(&sty);
    }
    auto p = s_face.find(style->font);
    if (p != s_face.end())
        return p->second;

    if (s_ft == nullptr) {
        if (FT_Init_FreeType(&s_ft)) {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
            return nullptr;
        }
    }

    std::string fontPath = style->font.fontPath;
    if (fontPath.empty()) {
        std::cout << "ERROR::FREETYPE: Font not found in system " << style->font.fontPath << std::endl;
        return nullptr;
    }

    FT_Face face;
    if (FT_New_Face(s_ft, fontPath.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font " << fontPath << std::endl;
        return nullptr;
    }

    FT_Set_Pixel_Sizes(face, 0, style->font.size);

    // 实现加粗效果
    if (style->font.style == FontStyle::Bold) {
        FT_Outline_Embolden(&face->glyph->outline, 1 << 6);  // 单位是1/64像素
    }
    // 实现斜体效果
    if (style->font.style == FontStyle::Italic) {
        FT_Matrix matrix;
        matrix.xx = 0x10000L;
        matrix.xy = 0x6000L;  // 斜体变换矩阵
        matrix.yx = 0;
        matrix.yy = 0x10000L;
        FT_Set_Transform(face, &matrix, nullptr);
    }
    else
    {
        FT_Matrix identity_matrix = {0x10000L, 0, 0, 0x10000L};
        FT_Vector zero_vector = {0, 0};
        FT_Set_Transform(face, &identity_matrix, &zero_vector);
    }

    s_face.insert(std::make_pair(style->font, face));
    return face;
}

inline Character* WindowOpenGLWin32::get_char_character(char32_t ch, FT_Face ft_font) const
{
    switch_openGL();
    // 查找字体
    auto ft_char = s_characters.find(ft_font);
    if (ft_char == s_characters.end())
    {
        // 如果未找到字体，插入新的字体映射
        auto result = s_characters.insert({ft_font, {}});
        if (!result.second)
        {
            return nullptr; // 插入失败
        }
        ft_char = result.first;
    }

    // 查找字符纹理
    auto charTexture = ft_char->second.find(ch);
    if (charTexture != ft_char->second.end())
    {
        return &(charTexture->second); // 找到
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // 加载字符到FT_Face
    if (FT_Load_Char(ft_font, ch, FT_LOAD_DEFAULT | FT_LOAD_RENDER))
    {
        std::cerr << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
        return nullptr;
    }

    // 生成纹理
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        ft_font->glyph->bitmap.width,
        ft_font->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        ft_font->glyph->bitmap.buffer
    );

    // 设置纹理选项
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 储存字符供之后使用
    Character character = {
        texture,
        VEC::iVec2(ft_font->glyph->bitmap.width, ft_font->glyph->bitmap.rows),
        VEC::iVec2(ft_font->glyph->bitmap_left, ft_font->glyph->bitmap_top),
        (GLuint)ft_font->glyph->advance.x
    };
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    // 插入字符纹理到字体映射
    auto inserted = ft_char->second.insert({ch, character});

    return &inserted.first->second;
}

inline void WindowOpenGLWin32::clear_char_texture(uint32_t id) const
{
    switch_openGL();
    for(auto font=s_characters.begin();font!=s_characters.end();font++)
    {
        for (auto ch = font->second.begin(); ch != font->second.end(); ch++)
        {
            if(ch->second.textureID==id)
            {
                glDeleteTextures(1,&id);
                font->second.erase(ch);
                if(font->second.size()==0)
                {
                    s_characters.erase(font);
                }
            }
            return;
        }
    }
}


inline GL::LineStrip *WindowOpenGLWin32::get_line_base() const
{
    if(s_lineBase)return s_lineBase;
    s_lineBase=new GL::LineStrip({{0,0,0},{1,0,0}});
    return s_lineBase;
}

inline int WindowOpenGLWin32::on_paint()
{
    switch_openGL();
    if(m_drawCallback)
        m_drawCallback(this);
    auto nowTime=get_time();
    m_realUpdateTime = nowTime - m_lastRefreshTime;
    m_lastRefreshTime = nowTime;
    swap_buffer();
    ValidateRect(m_windowHandle, NULL);
    return 0;
}

inline int WindowOpenGLWin32::on_size(WPARAM wParam, LPARAM lParam)
{
    switch_openGL();
    m_windowSize = VEC::Size(LOWORD(lParam), HIWORD(lParam));
    if (s_initedOpenGL)
        glViewport(0, 0, m_windowSize.width, m_windowSize.height);
    if (m_windowSizeCallback)
        m_windowSizeCallback(this, m_windowSize.width, m_windowSize.height);

    else
        return DefWindowProc(m_windowHandle, WM_SIZE, wParam, lParam);
    return 0;
}

bool WindowOpenGLWin32::s_initedOpenGL=false;
#ifdef FT2BUILD_H_
//FT_Library WindowOpenGLWin32::s_ft=nullptr;
// std::unordered_map<Font,FT_Face> WindowOpenGLWin32::s_face;
// std::unordered_map<FT_Face,std::unordered_map<GLuint, Character>> WindowOpenGLWin32::s_characters;
#endif
uint32_t WindowOpenGLWin32::max_polygon_vertices=16;

WindowOpenGLWin32::WindowOpenGLWin32()
{
}

inline WindowOpenGLWin32::~WindowOpenGLWin32() 
{
    clear_buffer();
    wglMakeCurrent(NULL, NULL);
    if(m_hglrc)
        wglDeleteContext(m_hglrc);
    if(m_hdc)
        ReleaseDC(m_windowHandle,m_hdc);
    if(s_lineBase)
    {
        delete s_lineBase;
    }
    if(s_shaderBase)
    {
        delete s_shaderBase;
    }
    if(s_textShader)
    {
        delete s_textShader;
    }
}

inline WindowCreateResult WindowOpenGLWin32::create_window(const std::string& titleName,int x,int y,int width,int height,WindowBase* parentWindow)
{
    auto result = Window2DWin32::create_window(titleName, x, y, width, height,parentWindow);
    if (result != WindowCreateResult::WCR_SUCCESS)
    {
        std::cerr << "Failed to create base window." << std::endl;
        return result;
    }

    HDC hdc = GetDC(m_windowHandle);
    if (!hdc)
    {
        std::cerr << "Failed to get device context." << std::endl;
        return WindowCreateResult::WCR_FALSE;
    }
    m_hdc = hdc;

    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    if (pixelFormat == 0)
    {
        std::cerr << "Failed to choose pixel format." << std::endl;
        return WindowCreateResult::WCR_FALSE;
    }

    if (!SetPixelFormat(hdc, pixelFormat, &pfd))
    {
        std::cerr << "Failed to set pixel format." << std::endl;
        return WindowCreateResult::WCR_FALSE;
    }

    m_hglrc = wglCreateContext(hdc);
    if (!m_hglrc)
    {
        std::cerr << "Failed to create OpenGL context." << std::endl;
        return WindowCreateResult::WCR_FALSE;
    }

    if (!wglMakeCurrent(hdc, m_hglrc))
    {
        std::cerr << "Failed to make OpenGL context current." << std::endl;
        return WindowCreateResult::WCR_FALSE;
    }
    if(!switch_openGL())
    {
        std::cout << "切换OpenGL失败" << std::endl;
    }
    if (!gladLoadGLLoader((GLADloadproc)wglGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return WindowCreateResult::WCR_FALSE;
    }
    s_initedOpenGL=true;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    return WindowCreateResult::WCR_SUCCESS;
}

inline HDC WindowOpenGLWin32::get_current_hdc() const
{
    return m_hdc;
}

inline bool WindowOpenGLWin32::switch_openGL() const
{
    if (m_hglrc == nullptr)
        return false;
    static HGLRC last=nullptr;
    if(m_hglrc==last)
        return false;
    last=m_hglrc;
    return wglMakeCurrent(m_hdc, m_hglrc);
}

inline void WindowOpenGLWin32::swap_buffer() const
{
    SwapBuffers(m_hdc);
}

inline void WindowOpenGLWin32::clear_buffer() const
{
    switch_openGL();
    if(s_lineBase)
    {
        delete s_lineBase;
        s_lineBase=nullptr;
    }
    if(s_shaderBase)
    {
        delete s_shaderBase;
        s_shaderBase=nullptr;
    }
    if(s_textShader)
    {
        delete s_textShader;
        s_textShader=nullptr;
    }
    for(auto p:s_face)
    {
        if(p.second)
        {
            FT_Done_Face(p.second);
        }
    }
    s_face.clear();
    if (s_ft)
        FT_Done_FreeType(s_ft);
    s_ft = nullptr;
    for(auto f:s_characters)
    {
        for(auto c:f.second)
        {
            glDeleteTextures(1,&c.second.textureID);
        }
    }
    s_characters.clear();
}

inline WindowDrawResult WindowOpenGLWin32::draw_line_(float x1, float y1, float x2, float y2, LineStyle *style) const
{
    if (style && style->width > 1)
    {
        auto start=VEC::Vec2(x1,y1);
        auto end=VEC::Vec2(x2,y2);
        auto dir=end-start;
        dir=VEC::Vec2(-dir.y,dir.x).normalized()*style->width*0.5f;
        std::vector<float>points={
            x1+dir.x,y1+dir.y,
            x1-dir.x,y1-dir.y,
            x2-dir.x,y2-dir.y,
            x2+dir.x,y2+dir.y,
        };

        return draw_fill_polygonal_(0,0,points.data(),4,style->color.r,style->color.g,style->color.b,style->color.a);
    }

    auto shader = get_line_shader();
    shader->use();
    if (style)
        shader->set_color("ourColor", style->color);
    else
        shader->set_color("ourColor", VEC::Color(0, 0, 0, 1));
    VEC::Vec3 start = pixel_to_NDC_LT(x1, y1);
    VEC::Vec3 end = pixel_to_NDC_LT(x2, y2);
    VEC::Vec3 f = end - start;
    float rad = VEC::angle_between(f, VEC::Vec3(1, 0, 0));
    if (f.y < 0)
    {
        rad = -rad;
    }
    float len = (f).length();
    VEC::Mat4 tran(1.0f);
    tran.translate(start);
    tran.scale(VEC::Vec3(len));
    tran.rotate(VEC::Vec3(0, 0, 1), rad);
    shader->set_mat4("transform", m_mat4*tran);

    get_line_base()->draw(*shader);

    return WindowDrawResult::WDR_SUCCESS;
}

inline WindowDrawResult WindowOpenGLWin32::draw_pixel_(float x, float y, float r, float g, float b, float a)const
{
    auto shader=get_line_shader();
    shader->use();
    shader->set_color("ourColor", VEC::Color(r,g,b,a));
    VEC::Mat4 tran(1.0f);
    tran.translate(pixel_to_NDC_LT(x,y));
    tran.scale(VEC::Vec3(1.0f/m_windowSize.width*2));
    //tran.rotate(VEC::Vec3(0,1,0),VEC::angle_to_radian(90.0f));
    shader->set_mat4("transform",tran);
    get_line_base()->draw(*shader);
    return WindowDrawResult::WDR_SUCCESS;
}

inline WindowDrawResult WindowOpenGLWin32::draw_rectangle_(float x, float y, float width, float height,LineStyle* style)const
{
    draw_line_(x,y,x+width,y,style);
    draw_line_(x+width,y,x+width,y+height);
    draw_line_(x+width,y+height,x,y+height);
    draw_line_(x,y,x,y+height);
    return WindowDrawResult::WDR_SUCCESS;
}

inline WindowDrawResult WindowOpenGLWin32::draw_polygonal_(float x, float y, float *points, uint32_t pointsSize, LineStyle *style) const
{
    VEC::Vec2 start(x + *points, y + *(points + 1));
    VEC::Vec2 end = start;
    for (auto i = 1u; i < pointsSize; ++i)
    {
        end.x = x + *(points + 2 * i);
        end.y = y + *(points + 2 * i + 1);
        draw_line_(start.x, start.y, end.x, end.y, style);
        start = end;
    }
    draw_line_(start.x, start.y, x + *points, y + *(points + 1), style);
    return WindowDrawResult::WDR_SUCCESS;
}

inline WindowDrawResult WindowOpenGLWin32::draw_fill_polygonal_(float x, float y, float *points, uint32_t pointsArraySize, float r, float g, float b, float a) const
{
    static GL::Buffer buffer;
    static bool needInit = true;

    if (needInit)
    {
        // 初始顶点缓冲区
        buffer.init_buffer(nullptr, sizeof(float)*3*max_polygon_vertices, BufferType::DynamicDraw);
        buffer.set_causality(0, 3, 3 * sizeof(float), (void*)0, GL_FLOAT);
        needInit = false;
    }

    // 更新顶点数据
    std::vector<float> vertices(pointsArraySize * 3);
    for (uint32_t i = 0; i < pointsArraySize; ++i)
    {
        auto nPos = pixel_to_NDC_LT(points[i * 2 + 0] + x, points[i * 2 + 1] + y);
        vertices[i * 3 + 0] = nPos.x; // X 坐标
        vertices[i * 3 + 1] = nPos.y; // Y 坐标
        vertices[i * 3 + 2] = 0.0f;   // Z 坐标，假设在二维空间中Z为0
    }
    buffer.update_buffer(vertices.data(), vertices.size() * sizeof(float));

    // 使用着色器程序
    auto shader = get_line_shader();
    shader->use();
    //auto tran = VEC::Mat4::unit_matrix();
    shader->set_mat4("transform",m_mat4);
    shader->set_color("ourColor", r, g, b, a);

    // 绘制多边形
    glBindVertexArray(buffer.VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, pointsArraySize);
    glBindVertexArray(0);

    return WindowDrawResult::WDR_SUCCESS;
}

inline WindowDrawResult WindowOpenGLWin32::draw_text_(float x, float y, const char *text,TextStyle *style) const
{
    if(!text)
        return WindowDrawResult::WDR_PARAMETER_ERROR;
    if (!style)
    {
        TextStyle textStyle;
        return draw_text_(x,y,text,&textStyle);
    }
    switch_openGL();
#ifdef FT2BUILD_H_
    auto &hglrc = m_hglrc;
    if (style)
        y = m_windowSize.height - y - style->font.size;
    else
        y = m_windowSize.height - y-24;

    // 检查OpenGL上下文
    if (!hglrc)
    {
        std::cerr << "OpenGL context is not available" << std::endl;
        return WindowDrawResult::WDR_OPENGL_CONTEXT_UNAVAILABLE;
    }

    // 设置默认文本颜色和比例
    float scale = 1.0f;                 // 默认比例
    VEC::Color color(1.0f, 1.0f, 1.0f); // 默认颜色为白色

    if (style)
    {
        color = style->color;
    }
    // 获取字体
    auto font = get_free_type_font(style);

    if (s_textVectex.VAO == 0)
    {
        s_textVectex.init_buffer(nullptr,sizeof(GLfloat) * 6 * 4,BufferType::DynamicDraw);
        s_textVectex.set_causality(0,4,4 * sizeof(GLfloat),0,GL_FLOAT);
        // glGenVertexArrays(1, &VAO);
        // glGenBuffers(1, &VBO);
        // glBindVertexArray(VAO);
        // glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        // glEnableVertexAttribArray(0);
        // glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        // glBindBuffer(GL_ARRAY_BUFFER, 0);
        // glBindVertexArray(0);
    }

    // 激活纹理单元和绑定VAO
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(s_textVectex.VAO);
    // 使用文本着色器
    auto textShader = get_text_shader();
    textShader->use();

    // 设置投影矩阵
    auto projection = VEC::ortho(0, m_windowSize.width, 0, m_windowSize.height);
    textShader->set_mat4("projection", projection);
    textShader->set_vec4("textColor",color);

    std::string strText=text;
    char* strData=const_cast<char*>( strText.data());
    // 循环绘制每个字符
    while(*strData!='\0')
    {
        char32_t str=utf8_decode(strData,&strData);
        if(str=='\0')
            break;
        Character *ch = get_char_character(str, font);
        if (!ch)
            continue;

        GLfloat xpos = x + ch->bearing.x * scale;
        GLfloat ypos = y - (ch->size.y - ch->bearing.y) * scale;

        GLfloat w = ch->size.x * scale;
        GLfloat h = ch->size.y * scale;

        // 对每个字符更新VBO
        GLfloat vertices[6][4] = {
            {xpos, ypos + h, 0.0f, 0.0f},
            {xpos, ypos, 0.0f, 1.0f},
            {xpos + w, ypos, 1.0f, 1.0f},

            {xpos, ypos + h, 0.0f, 0.0f},
            {xpos + w, ypos, 1.0f, 1.0f},
            {xpos + w, ypos + h, 1.0f, 0.0f}};

        // 绑定字符纹理
        glBindTexture(GL_TEXTURE_2D, ch->textureID);
        // 更新VBO内存的内容
        glBindBuffer(GL_ARRAY_BUFFER, s_textVectex.VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // 绘制四边形
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // 移动到下一个字符位置
        x += (ch->advance >> 6) * scale; // 位移6位来获得单位为像素的值 (2^6 = 64)
    }

    // 解绑VAO
    glBindVertexArray(0);
    // 解绑纹理
    glBindTexture(GL_TEXTURE_2D, 0);

    return WindowDrawResult::WDR_SUCCESS;
#else
    std::cout<<R"(
无法绘制gl文本，需要链接freetype库：https://github.com/ubawurinna/freetype-windows-binaries
#include <ft2build.h>
#include FT_FREETYPE_H 
#include FT_OUTLINE_H
    )"<<std::endl;
    return WindowDrawResult::WDR_FALSE;
#endif
}

inline WindowDrawResult WindowOpenGLWin32::clear_background_(float r, float g, float b,float a) const
{
    glClearColor(r,g,b,a);
    glClear(GL_COLOR_BUFFER_BIT);
    return WindowDrawResult::WDR_SUCCESS;
}

inline void WindowOpenGLWin32::set_transform(const VEC::Mat4 &m)
{
    m_mat4=m;
}
inline VEC::Mat4 WindowOpenGLWin32::get_transform() const
{
    return m_mat4;
}
#endif
}// namespace WindowDefine
} // namespace OneKeyEngine
#endif

#endif