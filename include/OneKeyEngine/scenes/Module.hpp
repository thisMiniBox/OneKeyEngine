#pragma once

#include"../Vector_OKE.hpp"
#include"Scenes.hpp"
#include"ScenesObject.hpp"
#include<vector>
#include <tuple>
#include <functional>
#include<unordered_map>


namespace OneKeyEngine
{
namespace Scenes
{
namespace Module
{


class Module
{
public:
    bool enable=true;
    ScenesObject* object=nullptr;

    Module(ScenesObject* obj=nullptr);

    /// @brief 创建对象基本参数赋值好后调用,整个生命周期只调用一次
    virtual void init(){};
    /// @brief 每帧调用，已经自动设置变换矩阵，所以可以直接绘制到窗口，也可通过object获取transformMat4，这是投影矩阵与变换矩阵的乘积
    virtual void draw(){};
    /// @brief 当窗口大小改变时调用
    /// @param w 
    /// @param h 
    virtual void size(int w,int h){};
    /// @brief 进入场景时调用
    virtual void start(){};
    /// @brief 每帧绘制前调用
    virtual void update(){};
    /// @brief 离开场景时调用
    virtual void exit(){};

    WindowInput* input()const;
    float get_delta_time()const;

    WindowDefine::WindowBase* get_window()const;
    WindowDefine::WindowOpenGLWin32* get_opengl_window()const;
    bool is_opengl_window()const;
    /// @brief 获取自己类型的专属着色器
    /// @return 
    GL::ShaderGL* get_shader();
    template<typename type>
    GL::ShaderGL* get_shader_of();

    GL::Buffer* get_buffer();
    GL::Element* get_buffer_element();


};







}
} // namespace Scenes

} // namespace OneKey



