#include "OneKeyEngine/Vector_OKE.hpp"
namespace OneKeyEngine
{
namespace VEC
{

void Transform::set_world_position(const VEC::Vec3 &worldPosition)
{
    auto nowPos = get_world_position();
    position += worldPosition - nowPos;
}

void Transform::set_parent(Transform &Parent)
{
    return set_parent(&Parent);
}

void Transform::set_parent(Transform *Parent)
{
    if (Parent)
        Parent->set_child(this);
    else if (parent)
    {
        parent->delete_child(this);
    }
}

int Transform::get_childs_count() const
{
    return childs.size();
}

void Transform::set_child(Transform &child, int pos)
{
    set_child(&child, pos);
}

void Transform::set_child(Transform *child, int pos)
{
    if (!child)
        return;
    if (pos < 0)
    {
        pos += childs.size() + 1;
    }
    if (pos > childs.size())
    {
        set_child(child, -1);
    }
    auto p = childs.begin() + pos;
    childs.insert(p, child);
    if (child->parent)
    {
        child->parent->delete_child(child);
    }
    child->parent = this;
}

Transform *Transform::get_child(int id) const
{
    if (id < childs.size())
    {
        return childs[id];
    }
    return nullptr;
}

Transform *Transform::get_parent() const
{
    return parent;
}

bool Transform::is_child(Transform *aim) const
{
    for (auto T : childs)
    {
        if (T == aim)
            return true;
    }
    return false;
}

int Transform::get_child_id(Transform *child) const
{
    for (int i = 0; i < childs.size(); ++i)
    {
        if (childs[i] == child)
            return i;
    }
    return -1;
}

void Transform::delete_child(Transform *child)
{
    int id = get_child_id(child);
    if(id<0)
        return;
    childs.erase(childs.begin() + id);
}
void Transform::delete_child(Transform &child)
{
    delete_child(&child);
}
VEC::Vec3 Transform::calculate_absolute_position(const VEC::Vec3 &relativePosition) const
{
    auto mat = to_world_matrix();
    auto result = mat * Vec4(relativePosition, 1.0f);
    return Vec3(result.x, result.y, result.z);

    // VEC::Vec3 scaledRelativePosition(relativePosition.x * scale.x, relativePosition.y * scale.y, relativePosition.z * scale.z);
    // VEC::Quaternion rotatedPoint = rotation * VEC::Quaternion(0, scaledRelativePosition.x, scaledRelativePosition.y, scaledRelativePosition.z) * rotation.conjugate();
    // VEC::Vec3 absolutePosition(position.x + rotatedPoint.x, position.y + rotatedPoint.y, position.z + rotatedPoint.z);

    // if(parent)
    // {
    //     return parent->calculate_absolute_position(absolutePosition);
    // }
    // return absolutePosition;
}

VEC::Vec3 Transform::calculate_parent_relative_position(const VEC::Vec3 &relativePosition) const
{
    VEC::Vec3 scaledRelativePosition(relativePosition.x * scale.x, relativePosition.y * scale.y, relativePosition.z * scale.z);
    VEC::Quaternion rotatedPoint = rotation * VEC::Quaternion(0, scaledRelativePosition.x, scaledRelativePosition.y, scaledRelativePosition.z) * rotation.conjugate();
    VEC::Vec3 absolutePosition(position.x + rotatedPoint.x, position.y + rotatedPoint.y, position.z + rotatedPoint.z);
    return absolutePosition;
}
VEC::Vec3 Transform::calculate_relative_position(const VEC::Vec3 &absolutePosition) const
{
    VEC::Vec3 relativePosition = absolutePosition;

    if (parent)
    {
        relativePosition = parent->calculate_relative_position(absolutePosition);
    }

    relativePosition.x -= position.x;
    relativePosition.y -= position.y;
    relativePosition.z -= position.z;

    VEC::Quaternion rotatedPoint(0, relativePosition.x, relativePosition.y, relativePosition.z);
    VEC::Quaternion inverseRotation = rotation.conjugate();
    rotatedPoint = inverseRotation * rotatedPoint * rotation;

    relativePosition.x = rotatedPoint.x / scale.x;
    relativePosition.y = rotatedPoint.y / scale.y;
    relativePosition.z = rotatedPoint.z / scale.z;

    return relativePosition;
}

Mat4 Transform::to_local_matrix() const
{
    // glm::mat4 res(1.0f);
    // res = glm::scale(res, to_glm(scale));
    // res*=glm::mat4_cast(glm::quat(rotation.w,rotation.x,rotation.y,rotation.z));
    // //res = glm::rotate(res, rotation.w, glm::vec3(rotation.x, rotation.y, rotation.z));
    // res = glm::translate(res, to_glm(position));
    // return res;

    auto result = Mat4::unit_matrix();

    result = Mat4::translate(result, position);
    result = Mat4::rotate(result, rotation);
    result = Mat4::scale(result, scale);
    // auto sc= glm::scale(to_glm(result),to_glm(scale));
    // return Mat4(sc);
    return result;
}

Mat4 Transform::to_world_matrix() const
{
    if (parent)
    {
        return parent->to_world_matrix() * to_local_matrix();
    }
    return to_local_matrix();
}


Transform::Transform() : position(0, 0, 0), rotation(1, 0, 0, 0), scale(1, 1, 1) {}
Transform::Transform(const VEC::Vec3 &pos, const VEC::Vec3 &scale, const VEC::Quaternion &rot, Transform *parent_) : position(pos), scale(scale), rotation(rot), parent(parent_)
{
}
void Transform::translate(const VEC::Vec3 &translation)
{
    position.x += translation.x;
    position.y += translation.y;
    position.z += translation.z;
}
void Transform::rotate(const VEC::Quaternion &deltaRotation)
{
    rotation = rotation * deltaRotation;
}
void Transform::rotate(const VEC::Vec3 &axis, float radian)
{
    rotate(VEC::Quaternion(axis,radian));
}
void Transform::scaleBy(const VEC::Vec3 &scaleFactor)
{
    scale.x *= scaleFactor.x;
    scale.y *= scaleFactor.y;
    scale.z *= scaleFactor.z;
}
VEC::Vec3 Transform::get_world_position() const
{
    if (parent == nullptr)
        return position;
    return position + parent->get_world_position();
}

} // namespace VEC
    
} // namespace OneKeyEngine


