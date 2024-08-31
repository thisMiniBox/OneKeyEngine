#include <OneKeyEngine/scenes/Camera.hpp>

namespace OneKeyEngine
{

Camera::Camera(int width, int height, CameraMode mode_) : mode(mode_)
{
    screen = VEC::Size(width, height);
}

Camera::~Camera()
{
}

const VEC::Size& Camera::get_camera_size() const
{
    return screen;
}
bool Camera::set_camera_size(const VEC::Size &size)
{
    screen=size;
    return true;
}
VEC::Mat4 Camera::look_at_matrix4() const
{
    VEC::Vec3 const f(VEC::normalize(forward));
    VEC::Vec3 const s(VEC::normalize(right));
    VEC::Vec3 const u(cross(s, f));

    VEC::Mat4 Result(1);

    Result[0][0] = s.x;
    Result[0][1] = s.y;
    Result[0][2] = s.z;
    Result[1][0] = u.x;
    Result[1][1] = u.y;
    Result[1][2] = u.z;
    Result[2][0] = -f.x;
    Result[2][1] = -f.y;
    Result[2][2] = -f.z;
    Result[0][3] = -VEC::dot(s, position);
    Result[1][3] = -VEC::dot(u, position);
    Result[2][3] = VEC::dot(f, position);
    return Result;
}

VEC::Mat4 Camera::perspective_matrix4() const
{
    return VEC::perspective(fov,(float)screen.width/screen.height,near_,far_);
}

VEC::Mat4 Camera::ortho_matrix4() const
{
    auto w=screen.width*0.5f;
    auto h=screen.height*0.5f;
    return VEC::ortho(-w,w,-h,h,near_,far_);
}

VEC::Mat4 Camera::matrix4() const
{
    if(mode==CameraMode::Perspective)
    {
        return perspective_matrix4()*look_at_matrix4();

    }
    else if(mode==CameraMode::Orthographic)
    {
        return ortho_matrix4()*look_at_matrix4();
    }
    return perspective_matrix4()*look_at_matrix4();
}

void Camera::update_data()
{
    if (target == position)
    {
        target=VEC::normalize(position+forward);
    }
    forward = VEC::normalize(target - position);
    right = VEC::normalize(VEC::cross(forward, up));
}

void Camera::set_target(const VEC::Vec3 &tar)
{
    target=tar;
    update_data();

}

void Camera::set_position(const VEC::Vec3 &pos)
{
    position=pos;
    update_data();
}
void Camera::set_position_k(const VEC::Vec3 &pos)
{
    target = pos + forward;
    position = pos;
    update_data();
}
void Camera::move(const VEC::Vec3 &m)
{
    position+=m;
    target+=m;
    update_data();
}
void Camera::move_position(const VEC::Vec3 &m)
{
    position += m;
    update_data();
}
const VEC::Vec3 &Camera::get_forward() const
{
    return forward;
}
void Camera::set_forward(const VEC::Vec3 &zforward)
{
    if (zforward.x == 0 && zforward.y == 0 && zforward.z == 0)
        return;
    set_target(position + zforward);
}
const VEC::Vec3 &Camera::get_right() const
{
    return right;
}
const VEC::Vec3 &Camera::get_position() const
{
    return position;
}
VEC::Vec3 &Camera::get_up()
{
    return up;
}
const VEC::Vec3 &Camera::get_up() const
{
    return up;
}
float Camera::get_aspect() const
{
    return (float)screen.width/screen.height;
}
void Camera::rotate(const VEC::Vec3 &axis, float radian)
{
    VEC::Quaternion rotation(axis,radian);
    VEC::Quaternion rotatedPoint = rotation * VEC::Quaternion(0, forward.x, forward.y, forward.z) * rotation.conjugate();
    target = position + VEC::Vec3(rotatedPoint);
    update_data();
}
VEC::Vec2 Camera::project_to_screen_space(const VEC::Vec4 &pos) const
{
    return project_to_screen_space(VEC::Vec3(pos));
}
VEC::Vec2 Camera::project_to_screen_space(const VEC::Vec3 &pos) const
{
    VEC::Vec2 result(pos.x, pos.y);
    result /= pos.z;
    result.width = (result.width + 1) * screen.width * 0.5f;
    result.height = (1-result.height) * screen.height * 0.5f;
    return result;
}
} // namespace OneKeyEngine
