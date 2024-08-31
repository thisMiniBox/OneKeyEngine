#pragma once
#ifndef __OKE_CAMERA_H
#define __OKE_CAMERA_H

#include"../WindowElementOKE/WindowElementOKE.hpp"
#include"../WindowElementOKE/ImageLoadOKE.hpp"

namespace OneKeyEngine
{

enum class CameraMode
{
    Perspective,
    Orthographic,
};

class Camera
{
protected:
    VEC::Vec3 position=VEC::Vec3(0,0,1);
    VEC::Vec3 forward=VEC::Vec3(0,0,-1);
    VEC::Vec3 right=VEC::Vec3(1,0,0);
    VEC::Vec3 target=VEC::Vec3(0);
public:
    VEC::Vec3 up=VEC::Vec3(0,1,0);
    VEC::Size screen=VEC::Size(800,600);
    float fov=VEC::angle_to_radian(70);
    float near_=0.1;
    float far_=300;
    //WindowControl::Picture outCanvas; 
    CameraMode mode=CameraMode::Perspective;
    Camera(int width=800,int height=600,CameraMode mode=CameraMode::Perspective);
    ~Camera();

    const VEC::Size& get_camera_size()const;
    bool set_camera_size(const VEC::Size&size);

    VEC::Mat4 look_at_matrix4()const;
    VEC::Mat4 perspective_matrix4()const;
    VEC::Mat4 ortho_matrix4()const;
    VEC::Mat4 matrix4()const;


    void update_data();
    /// @brief 设定目标
    /// @param tar 
    void set_target(const VEC::Vec3& tar);
    /// @brief 设置位置，但不改变朝向的目标点
    /// @param pos 
    void set_position(const VEC::Vec3& pos);
    /// @brief 设置位置，保持方向不变
    /// @param pos 
    void set_position_k(const VEC::Vec3& pos);
    /// @brief 保持方向不变整体移动
    /// @param m 
    void move(const VEC::Vec3& m);
    /// @brief 只移动位置坐标
    /// @param m 
    void move_position(const VEC::Vec3& m);
    const VEC::Vec3& get_forward()const;
    void set_forward(const VEC::Vec3& zforward);
    const VEC::Vec3& get_right()const;
    const VEC::Vec3& get_position()const;
    VEC::Vec3& get_up();
    const VEC::Vec3& get_up()const;
    float get_aspect()const;
    void rotate(const VEC::Vec3&axis,float radian);
    VEC::Vec2 project_to_screen_space(const VEC::Vec4& pos)const;
    VEC::Vec2 project_to_screen_space(const VEC::Vec3& pos)const;
};

}

#endif