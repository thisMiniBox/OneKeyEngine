#pragma once
#include<GLFW/glfw3.h>
#include"Window_OKE.hpp"
#include<unordered_map>
#include <ft2build.h>
#include FT_FREETYPE_H 
#include FT_OUTLINE_H

#include<OneKeyEngine/WindowDefine/OpenGL_OKE.hpp>

namespace OneKeyEngine
{
namespace WindowDefine
{

class WindowGL_GLFW:public WindowBase
{
    static int windowCount;
    GLFWwindow *window=nullptr;


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

    GL::LineStrip* get_line_base()const;
    static void on_size(GLFWwindow* window, int width, int height);
public:
    ~WindowGL_GLFW();
    VEC::Mat4 transform;

    /// @brief 创建窗口
    /// @param parent 父窗口
    /// @param x 窗口位置
    /// @param y 
    /// @param width 窗口大小
    /// @param height 
    /// @return 
    virtual WindowCreateResult create_window(const std::string& titleName="GLFW窗口",int x=100 ,int y=100 ,int width=800 ,int height=600 ,WindowBase* parent=nullptr)override;
    /// @brief 关闭窗口
    /// @return 
    int close()override;
    /// @brief 设置窗口大小
    /// @param width 
    /// @param height 
    /// @return 返回0表示失败
    int set_size_(int width,int height)const override;
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

    void set_view(const VEC::Rectangle& rect)const;

    static KeyCode glfw_key_code_to_oke_key_code(int code);

    void switch_openGL()const;

    GL::ShaderGL *get_line_shader() const;
    GL::ShaderGL *get_text_shader() const;

    FT_Face get_free_type_font(const TextStyle *style) const;
    Character *get_char_character(char32_t ch, FT_Face ft_font) const;
    void clear_char_texture(uint32_t id) const;
    void clear_buffer() const;
};

}
}