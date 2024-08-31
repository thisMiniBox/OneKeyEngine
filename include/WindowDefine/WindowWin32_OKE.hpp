#pragma once

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

#include <ft2build.h>
#include FT_FREETYPE_H 
#include FT_OUTLINE_H


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
    virtual WindowCreateResult create_window(const std::string& titleName="Win32窗口",int x=CW_USEDEFAULT ,int y=CW_USEDEFAULT ,int width=CW_USEDEFAULT ,int height=CW_USEDEFAULT ,WindowBase* parent=nullptr)override;
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
    void set_mode(ModeType mode,ModeValue value=ModeValue::True)const override;

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

    GLuint m_fontBase=0;

    GL::LineStrip* get_line_base()const;


    int on_size(WPARAM wParam,LPARAM lParam)override;

    VEC::Mat4 m_mat4=VEC::Mat4::unit_matrix();

public:
    /// @brief 多边形绘制支持的最大顶点数
    uint32_t max_polygon_vertices=8;

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
    WindowCreateResult create_window(const std::string& titleName="窗口",int x=CW_USEDEFAULT ,int y=CW_USEDEFAULT ,int width=CW_USEDEFAULT ,int height=CW_USEDEFAULT ,WindowBase* parent=nullptr)override;
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

    bool inited_glad()const;
};
#endif



}// namespace WindowDefine
} // namespace OneKeyEngine
#endif

#endif