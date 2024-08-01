#pragma once

#ifndef __OKE_VECTOR_H
#define __OKE_VECTOR_H

#ifdef USE_GLM
#include<glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#endif

#include<cmath>
#include<vector>
#include <stdexcept>

#ifdef _WIN32
#include<windows.h>
#endif

namespace OneKeyEngine
{
namespace VEC
{
template<typename DATATYPE>
class Vector2;
template <typename DATATYPE>
class Vector3;
template<typename DATATYPE>
class Vector4;
template <class dataType>
static Vector3<dataType> cross(const Vector3<dataType> &vec1, const Vector3<dataType> &vec2);
template <class dataType>
static dataType dot(const Vector3<dataType> &vec1, const Vector3<dataType> &vec2);





/// @brief 2维向量
/// @tparam DATATYPE 
template<typename DATATYPE>
class Vector2
{
public:
    union
    {
        DATATYPE x, width;
    };
    union
    {
        DATATYPE y, height;
    };
    Vector2(DATATYPE v=0):x(v),y(v){};
    Vector2(DATATYPE X,DATATYPE Y):x(X),y(Y){}
    ~Vector2(){};
    double length()const
    {
        return sqrt(x*x+y*y);
    }
    Vector2 normalized()const
    {
        return *this/length();
    }
	Vector2 operator+(const Vector2&other)const
	{
		return Vector2(x + other.x, y + other.y);
	}
	Vector2 operator-(const Vector2& other)const
	{
		return Vector2(x - other.x, y - other.y);
	}
	Vector2 operator*(const DATATYPE& mul)const
	{
		return Vector2(x * mul, y * mul);
	}
    Vector2 operator*(const Vector2 &other) const
    {
        return Vector2(x * other.x, y * other.y);
    }
	Vector2 operator/(const DATATYPE& mul)const
	{
		return Vector2(x / mul, y / mul);
	}
    Vector2 operator/(const Vector2 &other) const
    {
        return Vector2(x / other.x, y / other.y);
    }
    Vector2 operator+=(const Vector2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}
	Vector2 operator-=(const Vector2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}
	Vector2 operator*=(const DATATYPE& mul)
	{
		x *= mul;
		y *= mul;
		return *this;
	}
    Vector2 operator*=(const Vector2 &other)
    {
        x *= other.x;
        y *= other.y;
        return *this;
    }
	Vector2 operator/=(const DATATYPE& mul)
	{
		x /= mul;
		y /= mul;
		return *this;
	}
    Vector2 operator/=(const Vector2 &other)
    {
        x /= other.x;
        y /= other.y;
        return *this;
    }
    bool operator==(const Vector2& other)const
	{
		return other.x == x && other.y == y;
	}
};

/// @brief 3维向量
/// @tparam DATATYPE 
template <typename DATATYPE>
class Vector3
{
public:
    union
    {
        DATATYPE x, r;
    };
    union
    {
        DATATYPE y, g;
    };
    union
    {
        DATATYPE z, b;
    };

    Vector3(DATATYPE v=0):x(v),y(v),z(v){};
    Vector3(DATATYPE X,DATATYPE Y,DATATYPE Z=0):x(X),y(Y),z(Z){};
    Vector3(Vector2<DATATYPE> vec2,DATATYPE Z=0):x(vec2.x),y(vec2.y),z(Z){};
    Vector3(const Vector4<DATATYPE>& vec4):x(vec4.x),y(vec4.y),z(vec4.z){};

    ~Vector3(){}

    Vector3 normalized()const
    {
        auto l= length();
        return Vector3(x/l,y/l,z/l);
    }

    DATATYPE length()const
    {
        return std::sqrt(x*x+y*y+z*z);
    }
    
    Vector3 cross(const Vector3& vec3)const
    {
        return cross(*this,vec3);
    }
    double dot(const Vector3& vec3)const
    {
        return dot(*this,vec3);
    }

    template<class OtherType>
    Vector3<OtherType>conversion_type()const
    {
        return conversion_type(*this);
    }
    template<class OtherType>
    static Vector3<OtherType>conversion_type(const Vector3& vec3)
    {
        return Vector3<OtherType>(\
        static_cast<OtherType>(vec3.x),
        static_cast<OtherType>(vec3.y),
        static_cast<OtherType>(vec3.z));
    }

	Vector3 operator+(const Vector3& other)const
	{
		return Vector3(x + other.x, y + other.y,z+other.z);
	}
    Vector3 operator-()const
    {
        return Vector3(-x,-y,-z);
    }
	Vector3 operator-(const Vector3& other)const
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}
	Vector3 operator*(const DATATYPE& mul)const
	{
		return Vector3(x * mul, y * mul, z*mul);
	}
    Vector3 operator*(const Vector3& other)const
	{
		return Vector3(x * other.x, y * other.y, z*other.z);
	}
	Vector3 operator/(const DATATYPE& mul)const
	{
		return Vector3(x / mul, y / mul, z / mul);
	}
    Vector3 operator/(const Vector3& other)const
	{
		return Vector3(x / other.x, y / other.y, z/other.z);
	}
	Vector3 operator+=(const Vector3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}
	Vector3 operator-=(const Vector3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}
	Vector3 operator*=(const DATATYPE& mul)
	{
		x *= mul;
		y *= mul;
		z *= mul;
		return *this;
	}
    Vector3 operator*=(const Vector3 &other)
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }
    Vector3 operator/=(const DATATYPE &mul)
    {
        x /= mul;
        y /= mul;
        z /= mul;
        return *this;
    }
    Vector3 operator/=(const Vector3 &other)
    {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }
    bool operator==(const Vector3& other)const
	{
		return other.x == x && other.y == y && other.z == z;
	}
};

/// @brief 4维向量
/// @tparam DATATYPE 
template <typename DATATYPE>
class Vector4
{
public:
    union
    {
        DATATYPE x, r,left;
    };
    union
    {
        DATATYPE y, g,top;
    };
    union
    {
        DATATYPE z, b,right;
    };
    union
    {
        DATATYPE w,a,bottom;
    };

    Vector4(DATATYPE v=0):x(v),y(v),z(v),w(v){};
    Vector4(DATATYPE X,DATATYPE Y,DATATYPE Z=0,DATATYPE W=0):x(X),y(Y),z(Z),w(W){};
    Vector4(const Vector3<DATATYPE>& vec3,DATATYPE W=0):x(vec3.x),y(vec3.y),z(vec3.z),w(W){};

    DATATYPE squareLength()const
    {
        return x*x+y*y+z*z+w*w;
    }
    double length()const
    {
        return sqrt(squareLength());
    }
    Vector4 normalize()const
    {
        auto len=length();
        return Vector4(x/len,y/len,z/len,w/len);
    }

    bool operator==(const Vector4 &other) const
    {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }

    DATATYPE &operator[](int p)
    {
        switch (p)
        {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        case 3:
            return a;
        default:
            throw std::out_of_range("数组越界Vector4");
        }
    }
    const DATATYPE &operator[](int p) const
    {
        switch (p)
        {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        case 3:
            return a;
        default:
            throw std::out_of_range("数组越界Vector4");
        }
    }

    Vector4 operator+(const Vector4 &other) const
    {
        return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
    }
    Vector4 operator-() const
    {
        return Vector4(-x, -y, -z, -w);
    }
    Vector4 operator-(const Vector4 &other) const
    {
        return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
    }
    Vector4 operator*(const DATATYPE &mul) const
    {
        return Vector4(x * mul, y * mul, z * mul, w * mul);
    }
    Vector4 operator/(const DATATYPE &mul) const
    {
        return Vector4(x / mul, y / mul, z / mul, w / mul);
    }
    Vector4 operator+=(const Vector4 &other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }
    Vector4 operator-=(const Vector4 &other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }
    Vector4 operator*=(const DATATYPE &mul)
    {
        x *= mul;
        y *= mul;
        z *= mul;
        w *= mul;
        return *this;
    }
    Vector4 operator/=(const DATATYPE &mul)
    {
        x /= mul;
        y /= mul;
        z /= mul;
        w /= mul;
        return *this;
    }
};

using Size=Vector2<int>;
using Point=Vector2<int>;
using Vec2=Vector2<float>;
using iVec2=Vector2<int>;

using Vec3=Vector3<float>;

using Rectangle= Vector4<int>;
using RectangleF= Vector4<float>;
using Vec4=Vector4<float>;

/// @brief 颜色，继承自4维向量
class Color: public Vector4<float>
{
public:
    Color()
    {
        r=0;
        g=0;
        b=0;
        a=1.0f;
    }
    Color(float R,float G,float B,float A=1.0f)
    {
        // if (R > 1 || G > 1 || B > 1 || A > 1)
        // {
        //     R /= 255.0f;
        //     G /= 255.0f;
        //     B /= 255.0f;
        //     A /= 255.0f;
        // }
        r=R;
        g=G;
        b=B;
        a=A;
    }
    Color operator/(float m)const
    {
        return Color(r/m,g/m,b/m,a/m);
    }
    Color operator-(const Color &other) const
    {
        return Color(r - other.r, g - other.g, b - other.b, a - other.a);
    }
    Color operator+(const Color &other) const
    {
        return Color(r + other.r, g + other.g, b + other.b, a + other.a);
    }
    Color operator-() const
    {
        return Color(1.0f-r,1.0f-g,1.0f-b,1.0f);
    }
#ifdef _WIN32

    COLORREF to_win32_gdi_color()const
    {
        return RGB(r*255,g*255,b*255);
    }

#endif

};

/// @brief 4元数，继承自4维向量w放在最后
class Quaternion : public Vector4<float>
{
public:
    // 默认构造函数
    Quaternion() : Vector4<float>() {}

    // 构造函数
    Quaternion(float _w, float _x, float _y, float _z) : Vector4<float>(_x, _y, _z, _w) {}
    // 构造函数，用于从旋转轴(axis)和角度(angleInRadians)创建四元数
    Quaternion(const Vec3& axis, float angleInRadians)
    {
        // 确保传递进来的axis已经单位化，这里假设Vec3类有一个normalized()方法
        Vec3 normalizedAxis = axis.normalized();

        // 计算四元数的各分量
        float sinHalfAngle = std::sin(angleInRadians / 2.0f);
        float cosHalfAngle = std::cos(angleInRadians / 2.0f);

        // 实部
        w = cosHalfAngle;
        // 虚部对应于旋转轴的方向
        x = normalizedAxis.x * sinHalfAngle;
        y = normalizedAxis.y * sinHalfAngle;
        z = normalizedAxis.z * sinHalfAngle;
    }

    // 获取共轭四元数
    Quaternion conjugate() const
    {
        return Quaternion(this->w,-this->x, -this->y, -this->z);
    }
    // 四元数乘法
    Quaternion operator*(const Quaternion &other) const
    {
        Quaternion result;
        result.w = this->w * other.w - this->x * other.x - this->y * other.y - this->z * other.z;
        result.x = this->w * other.x + this->x * other.w + this->y * other.z - this->z * other.y;
        result.y = this->w * other.y - this->x * other.z + this->y * other.w + this->z * other.x;
        result.z = this->w * other.z + this->x * other.y - this->y * other.x + this->z * other.w;
        return result;
    }

    // 返回四元数的模
    float magnitude() const
    {
        return std::sqrt(this->w * this->w + this->x * this->x + this->y * this->y + this->z * this->z);
    }

    // 返回四元数的单位化
    Quaternion normalized() const
    {
        float mag = magnitude();
        return Quaternion(this->w / mag,this->x / mag, this->y / mag, this->z / mag);
    }
};


/// @brief 3*3矩阵
/// @tparam DATATYPE 
template<typename DATATYPE>
class Matrix3x3
{
public:
    DATATYPE 
    a11,a12,a13,
    a21,a22,a23,
    a31,a32,a33;

    Matrix3x3(DATATYPE*data):\
    a11(data[0]),a12(data[1]),a13(data[2]),\
    a21(data[3]),a22(data[4]),a23(data[5]),\
    a31(data[6]),a32(data[7]),a33(data[8])\
    {}
    Matrix3x3(DATATYPE multiple=0):\
    a11(multiple),  a12(0),         a13(0),\
    a21(0),         a22(multiple),  a23(0),\
    a31(0),         a32(0),         a33(multiple)\
    {}
    Matrix3x3(
        DATATYPE A11,DATATYPE A12,DATATYPE A13,
        DATATYPE A21,DATATYPE A22,DATATYPE A23,
        DATATYPE A31,DATATYPE A32,DATATYPE A33
    ):
    a11(A11),  a12(A12),  a13(A13),\
    a21(A21),  a22(A22),  a23(A23),\
    a31(A31),  a32(A32),  a33(A33)\
    {}
    ~Matrix3x3()
    {}
};

/// @brief 4*4矩阵
/// @tparam DATATYPE
template <typename DATATYPE>
class Matrix4x4
{
public:
    DATATYPE data[4][4];
    Matrix4x4(DATATYPE val = 0)
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                if (i == j || val != 1)
                    data[i][j] = val;
                else
                    data[i][j] = 0;
            }
        }
    }
    Matrix4x4(DATATYPE initialValues[4][4])
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                data[i][j] = initialValues[i][j];
            }
        }
    }
#ifdef USE_GLM
    Matrix4x4(const glm::mat4x4 &glmMat)
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                data[i][j] = glmMat[j][i];
            }
        }
    }
#endif
    static Matrix4x4 unit_matrix()
    {
        DATATYPE v[4][4] = {
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f};
        return v;
    }
    Matrix4x4<DATATYPE> inverse() const
    {
        Matrix4x4<DATATYPE> result;
        DATATYPE augmented[4][8];

        // Form the augmented matrix [A | I]
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                augmented[i][j] = data[i][j];
                augmented[i][j + 4] = (i == j) ? 1 : 0;
            }
        }

        // Perform Gauss-Jordan elimination
        for (int i = 0; i < 4; ++i)
        {
            // Search for maximum in this column
            DATATYPE maxEl = std::abs(augmented[i][i]);
            int maxRow = i;
            for (int k = i + 1; k < 4; ++k)
            {
                if (std::abs(augmented[k][i]) > maxEl)
                {
                    maxEl = std::abs(augmented[k][i]);
                    maxRow = k;
                }
            }

            // Swap maximum row with current row (column by column)
            for (int k = i; k < 8; ++k)
            {
                std::swap(augmented[maxRow][k], augmented[i][k]);
            }

            // Make sure the matrix is not singular
            if (augmented[i][i] == 0)
            {
                throw std::runtime_error("Matrix is not invertible");
            }

            // Make all rows below this one 0 in current column
            for (int k = i + 1; k < 4; ++k)
            {
                DATATYPE c = -augmented[k][i] / augmented[i][i];
                for (int j = i; j < 8; ++j)
                {
                    if (i == j)
                    {
                        augmented[k][j] = 0;
                    }
                    else
                    {
                        augmented[k][j] += c * augmented[i][j];
                    }
                }
            }
        }

        // Solve equation Ax = I for an upper triangular matrix
        for (int i = 3; i >= 0; --i)
        {
            for (int k = 4; k < 8; ++k)
            {
                augmented[i][k] /= augmented[i][i];
            }
            augmented[i][i] = 1;

            for (int rowModify = i - 1; rowModify >= 0; --rowModify)
            {
                for (int k = 4; k < 8; ++k)
                {
                    augmented[rowModify][k] -= augmented[i][k] * augmented[rowModify][i];
                }
                augmented[rowModify][i] = 0;
            }
        }

        // Extract the inverse matrix
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                result.data[i][j] = augmented[i][j + 4];
            }
        }

        return result;
    }
    DATATYPE &operator()(int i, int j)
    {
        return data[i][j];
    }
    DATATYPE *operator[](int row)
    {
        return data[row];
    }
    const DATATYPE *operator[](int row) const
    {
        return data[row];
    }
    Matrix4x4 operator+(const Matrix4x4 &other) const
    {
        Matrix4x4 result;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                result.data[i][j] = data[i][j] + other.data[i][j];
            }
        }
        return result;
    }
    Matrix4x4 operator-(const Matrix4x4 &other) const
    {
        Matrix4x4 result;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                result.data[i][j] = data[i][j] - other.data[i][j];
            }
        }
        return result;
    }
    Matrix4x4 operator*(const DATATYPE &scalar) const
    {
        Matrix4x4 result;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                result.data[i][j] = data[i][j] * scalar;
            }
        }
        return result;
    }
    Matrix4x4 operator*(const Matrix4x4 &other) const
    {
        Matrix4x4 result;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                for (int k = 0; k < 4; ++k)
                {
                    result.data[i][j] += data[i][k] * other.data[k][j];
                }
            }
        }
        return result;
    }
    Vector3<DATATYPE> operator*(const Vector3<DATATYPE> &other) const
    {
        auto v4=(*this)*Vector4<DATATYPE>(other,1);
        return Vector3<DATATYPE>(v4.x,v4.y,v4.z);
    }
    Vector4<DATATYPE> operator*(const Vector4<DATATYPE> &other) const
    {
        return Vector4<DATATYPE>(
            data[0][0] * other.x + data[0][1] * other.y + data[0][2] * other.z + data[0][3] * other.w,
            data[1][0] * other.x + data[1][1] * other.y + data[1][2] * other.z + data[1][3] * other.w,
            data[2][0] * other.x + data[2][1] * other.y + data[2][2] * other.z + data[2][3] * other.w,
            data[3][0] * other.x + data[3][1] * other.y + data[3][2] * other.z + data[3][3] * other.w);
    }
   
    // 将四元数转换为旋转矩阵
    // 假设四元数为单位四元数
    static Matrix4x4<float> to_rotation_matrix(const Quaternion &quaternion)
    {
        Matrix4x4<float> result;
        float xx = quaternion.x * quaternion.x;
        float xy = quaternion.x * quaternion.y;
        float xz = quaternion.x * quaternion.z;
        float xw = quaternion.x * quaternion.w;
        float yy = quaternion.y * quaternion.y;
        float yz = quaternion.y * quaternion.z;
        float yw = quaternion.y * quaternion.w;
        float zz = quaternion.z * quaternion.z;
        float zw = quaternion.z * quaternion.w;
        result.data[0][0] = 1 - 2 * (yy + zz);
        result.data[0][1] = 2 * (xy - zw);
        result.data[0][2] = 2 * (xz + yw);
        result.data[1][0] = 2 * (xy + zw);
        result.data[1][1] = 1 - 2 * (xx + zz);
        result.data[1][2] = 2 * (yz - xw);
        result.data[2][0] = 2 * (xz - yw);
        result.data[2][1] = 2 * (yz + xw);
        result.data[2][2] = 1 - 2 * (xx + yy);
        result.data[3][0] = 0;
        result.data[3][1] = 0;
        result.data[3][2] = 0;
        result.data[3][3] = 1;
        return result;
    }
    void translate(const Vector3<DATATYPE> &move)
    {
        *this = translate(*this, move);
    }
    static Matrix4x4 translate(const Matrix4x4 &mat4, const Vector3<DATATYPE> &move)
    {
        auto result = Matrix4x4::unit_matrix();
        result[0][3] = move.x;
        result[1][3] = move.y;
        result[2][3] = move.z;
        return mat4 * result;
    }
    void rotate(const Quaternion &rotation)
    {
        *this = rotate(*this, rotation);
    }
    void rotate(const VEC::Vec3 &axis, const DATATYPE &radian)
    {
        *this = rotate(*this, Quaternion(axis, radian));
    }
    static Matrix4x4 rotate(const Matrix4x4 &mat4, const Quaternion &rotation)
    {
        return mat4 * Matrix4x4::to_rotation_matrix(rotation);
    }
    void scale(const Vector3<DATATYPE> &scale_)
    {
        *this = scale(*this, scale_);
    }

    static Matrix4x4 scale(const Matrix4x4 &mat4, const Vector3<DATATYPE> &scale_)
    {
        Matrix4x4 result = mat4;
        result[0][0] *= scale_.x;
        result[1][0] *= scale_.x;
        result[2][0] *= scale_.x;
        result[3][0] *= scale_.x;
        result[0][1] *= scale_.y;
        result[1][1] *= scale_.y;
        result[2][1] *= scale_.y;
        result[3][1] *= scale_.y;
        result[0][2] *= scale_.z;
        result[1][2] *= scale_.z;
        result[2][2] *= scale_.z;
        result[3][2] *= scale_.z;
        return result;
    }
    Matrix4x4 transpose()
    {
        Matrix4x4 result;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                result[i][j] = data[j][i];
            }
        }
        return result;
    }
    static Matrix4x4 transpose(const Matrix4x4 &mat)
    {
        Matrix4x4 result;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                result[i][j] = mat[j][i];
            }
        }
        return result;
    }
};
using Mat4 = Matrix4x4<float>;

class Ray
{
public:
    VEC::Vec3 origin;
    VEC::Vec3 direction;

    Ray(const VEC::Vec3 &orig, const VEC::Vec3 &dir) : origin(orig), direction(dir) {}
    static bool ray_intersects_triangle(const Vec3 &orig, const Vec3 &dir, const Vec3 &v0, const Vec3 &v1, const Vec3 &v2, float &t, float &u, float &v)
    {
        const float EPSILON = 0.0000001;
        Vec3 edge1 = v1 - v0;
        Vec3 edge2 = v2 - v0;
        Vec3 h = cross(dir,edge2);
        float a = dot(edge1,h);

        if (a > -EPSILON && a < EPSILON)
            return false; // 射线与三角形平行

        float f = 1.0 / a;
        Vec3 s = orig - v0;
        u = f * dot(s,h);

        if (u < 0.0 || u > 1.0)
            return false;

        Vec3 q = cross(s,edge1);
        v = f * dot(dir,q);

        if (v < 0.0 || u + v > 1.0)
            return false;

        t = f * dot(edge2,q);

        if (t > EPSILON) // 射线相交三角形
            return true;

        return false; // 射线相交但在三角形之外
    }
};

class Transform
{
    Transform *parent = nullptr;
    std::vector<Transform *> childs;

public:
    VEC::Vec3 position;
    VEC::Quaternion rotation;
    VEC::Vec3 scale;
    void *userData = nullptr;

    // 默认构造函数
    Transform();
    Transform(const VEC::Vec3 &pos, const VEC::Vec3 &scale, const VEC::Quaternion &rot, Transform *parent = nullptr);
    // 平移
    void translate(const VEC::Vec3 &translation);

    // 旋转
    void rotate(const VEC::Quaternion &deltaRotation);
    void rotate(const VEC::Vec3 &axis,float radian);

    // 缩放
    void scaleBy(const VEC::Vec3 &scaleFactor);

    // 获取世界坐标
    VEC::Vec3 get_world_position() const;

    // 设置世界坐标
    void set_world_position(const VEC::Vec3 &worldPosition);
    void set_parent(Transform &Parent);
    void set_parent(Transform *Parent);
    int get_childs_count() const;
    void set_child(Transform &child, int pos = -1);
    void set_child(Transform *child, int pos = -1);
    Transform *get_child(int id) const;
    Transform *get_parent() const;
    bool is_child(Transform *aim) const;
    int get_child_id(Transform *child) const;
    void delete_child(Transform *child);

    // 通过相对坐标计算绝对坐标
    VEC::Vec3 calculate_absolute_position(const VEC::Vec3 &relativePosition) const;
    VEC::Vec3 calculate_parent_relative_position(const VEC::Vec3 &relativePosition) const;
    VEC::Vec3 calculate_relative_position(const VEC::Vec3 &relativePosition) const;

    Mat4 to_local_matrix() const;
    Mat4 to_world_matrix() const;
};

/// @brief 树节点
/// @tparam Data 
template <class Data>
class TreeNode
{
public:
    Data data;
    TreeNode *left = nullptr;
    TreeNode *right = nullptr;
    ~TreeNode()
    {
        erase(true);
        erase(false);
    }

    TreeNode *insort_left(const Data &data)
    {
        if (!left)
        {
            left = new TreeNode;
        }
        left->data = data;
        return left;
    }
    TreeNode *insort_right(const Data &data)
    {
        if (!right)
        {
            right = new TreeNode;
        }
        right->data = data;
        return right;
    }
    TreeNode *insort_empty(const Data &data)
    {
        if (!left)
            return insort_left(data);
        if (!right)
            return insort_right(data);
        return nullptr;
    }
    void erase(bool isRight)
    {
        if (isRight)
        {
            if (right)
                delete[] right;
        }
        else if (left)
            delete[] left;
    }
};
/// @brief 树父节点
/// @tparam Data 
template <class Data>
class BinaryTree
{
public:
    TreeNode<Data> *root;
    BinaryTree(TreeNode<Data> *Root = nullptr) : root(Root) {}
    ~BinaryTree()
    {
        if (root)
            delete[] root;
    }
    /// @brief 插入数据
    /// @param data 你的数据
    /// @param pos 插入位置，负数表示左边，0表示空位，正数表示右边，当指定位置有数据时会被替换，0会返回空
    /// @return 插入后的数据位置
    TreeNode<Data> *insort(const Data &data, char pos = 0)
    {
        if (!root)
            root = new TreeNode<Data>;
        if (pos < 0)
        {
            return root->insort_left(data);
        }
        else if (pos > 0)
        {
            return root->insort_right(data);
        }
        return root->insort_empty(data);
    }

    /// @brief 读取树数据，假为左，真为右
    /// @param orientations
    /// @param size
    /// @return
    TreeNode<Data> *get_node(bool *orientations, int size);
    /// @brief 每个比特对应左右，从第一个1后开始读取
    /// @param orientations
    /// @return
    TreeNode<Data> *get_node(int orientations);

    Data *get_data(bool *orientations, int size)
    {
        auto node = get_node(orientations, size);
        if (node)
            return node->data;
        return nullptr;
    }
};

static VEC::Mat4 perspective(float fov, float aspect, float nearPlane, float farPlane)
{
    VEC::Mat4 result;

    float tanHalfFov = std::tan(fov / 2.0f);

    result[0][0] = 1.0f / (aspect * tanHalfFov);
    result[1][1] = 1.0f / tanHalfFov;
    result[2][2] = -(farPlane + nearPlane) / (farPlane - nearPlane);
    result[2][3] = -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane);
    result[3][2] = -1.0f;

    return result;
}

static VEC::Mat4 ortho(float left, float right, float bottom, float top, float zNear = -1, float zFar = 1)
{
    VEC::Mat4 Result(1.0f);
		Result[0][0] = static_cast<float>(2) / (right - left);
		Result[1][1] = static_cast<float>(2) / (top - bottom);
		Result[2][2] = - static_cast<float>(2) / (zFar - zNear);
		Result[0][3] = - (right + left) / (right - left);
		Result[1][3] = - (top + bottom) / (top - bottom);
		Result[2][3] = - (zFar + zNear) / (zFar - zNear);
        
    return Result;
}


// 读取树数据，假为左，真为右
template <class Data>
TreeNode<Data> *BinaryTree<Data>::get_node(bool *orientations, int size)
{
    TreeNode<Data> *now = this->root;
    for (int i = 0; i < size; ++i)
    {
        if (orientations[i])
        {
            if (now->right)
            {
                now = now->right;
            }
            else
            {
                return nullptr;
            }
        }
        else
        {
            if (now->left)
            {
                now = now->left;
            }
            else
            {
                return nullptr;
            }
        }
    }
    return now;
}

template <class Data>
inline TreeNode<Data> *BinaryTree<Data>::get_node(int orientations)
{

    return nullptr;
}

static float angle_to_radian(float angle)
{
    return angle * 0.017453292f; // PI/180
}
static double angle_to_radian(double angle)
{
    return angle * 0.017453292519943295; // PI/180
}
static float radian_to_angle(float radian)
{
    return radian * 57.29578f; // 180/PI
}
static double radian_to_angle(double radian)
{
    return radian * 57.29577951308232; // 180/PI
}
static float angle_to_radian(int angle)
{
    return angle_to_radian(float(angle));
}

template <class dataType>
static dataType length(const Vector2<dataType> &vec)
{
    return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}
template <class dataType>
static dataType length(const Vector3<dataType> &vec)
{
    return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}
template <class dataType>
static dataType length(const Vector4<dataType> &vec)
{
    return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
}
template <class dataType>
static Vector2<dataType> normalize(const Vector2<dataType> &vec)
{
    dataType len = length(vec);
    if (len == 0)
        return vec;
    return vec / len;
}
template <class dataType>
static Vector3<dataType> normalize(const Vector3<dataType> &vec)
{
    dataType len = length(vec);
    if (len == 0)
        return vec;
    return vec / len;
}
template <class dataType>
static Vector4<dataType> normalize(const Vector4<dataType> &vec)
{
    dataType len = length(vec);
    if (len == 0)
        return vec;
    return vec / len;
}
template <class dataType>
static Vector3<dataType> cross(const Vector3<dataType> &vec1, const Vector3<dataType> &vec2)
{
    return Vector3<dataType>{
        vec1.y * vec2.z - vec1.z * vec2.y,
        vec1.z * vec2.x - vec1.x * vec2.z,
        vec1.x * vec2.y - vec1.y * vec2.x};
}
template <class dataType>
static dataType dot(const Vector3<dataType> &vec1, const Vector3<dataType> &vec2)
{
    return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}
static float angle_between(const Vec3 &a, const Vec3 &b)
{
    // 计算向量的点积
    float dotProduct = dot(a,b);

    // 计算向量的模长
    float lengthA = a.length();
    float lengthB = b.length();

    // 计算夹角的余弦值
    float cosTheta = dotProduct / (lengthA * lengthB);

    // 计算并返回夹角（单位：弧度）
    return std::acos(cosTheta);
}

#ifdef USE_GLM
static glm::vec2 to_glm(const Vec2& vec)
{
    return glm::vec2(vec.x,vec.y);
}
static glm::vec3 to_glm(const Vec3& vec)
{
    return glm::vec3(vec.x,vec.y,vec.z);
}

static glm::vec4 to_glm(const Vec4& vec)
{
    return glm::vec4(vec.x,vec.y,vec.z,vec.w);
}

static glm::mat4x4 to_glm(const Mat4& vec) {
    glm::mat4x4 result;
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            result[j][i] = vec.data[j][i];
        }
    }
    return result;
}
#endif

inline void Transform::set_world_position(const VEC::Vec3 &worldPosition)
{
    auto nowPos = get_world_position();
    position += worldPosition - nowPos;
}

inline void Transform::set_parent(Transform &Parent)
{
    return set_parent(&Parent);
}

inline void Transform::set_parent(Transform *Parent)
{
    if (Parent)
        Parent->set_child(this);
}

inline int Transform::get_childs_count() const
{
    return childs.size();
}

inline void Transform::set_child(Transform &child, int pos)
{
    set_child(&child, pos);
}

inline void Transform::set_child(Transform *child, int pos)
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

inline Transform *Transform::get_child(int id) const
{
    if (id < childs.size())
    {
        return childs[id];
    }
    return nullptr;
}

inline Transform *Transform::get_parent() const
{
    return parent;
}

inline bool Transform::is_child(Transform *aim) const
{
    for (auto T : childs)
    {
        if (T == aim)
            return true;
    }
    return false;
}

inline int Transform::get_child_id(Transform *child) const
{
    for (int i = 0; i < childs.size(); ++i)
    {
        if (childs[i] == child)
            return i;
    }
    return -1;
}

inline void Transform::delete_child(Transform *child)
{
    int id = get_child_id(child);
    childs.erase(childs.begin() + id);
}
inline VEC::Vec3 Transform::calculate_absolute_position(const VEC::Vec3 &relativePosition) const
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

inline VEC::Vec3 Transform::calculate_parent_relative_position(const VEC::Vec3 &relativePosition) const
{
    VEC::Vec3 scaledRelativePosition(relativePosition.x * scale.x, relativePosition.y * scale.y, relativePosition.z * scale.z);
    VEC::Quaternion rotatedPoint = rotation * VEC::Quaternion(0, scaledRelativePosition.x, scaledRelativePosition.y, scaledRelativePosition.z) * rotation.conjugate();
    VEC::Vec3 absolutePosition(position.x + rotatedPoint.x, position.y + rotatedPoint.y, position.z + rotatedPoint.z);
    return absolutePosition;
}
inline VEC::Vec3 Transform::calculate_relative_position(const VEC::Vec3 &absolutePosition) const
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

inline Mat4 Transform::to_local_matrix() const
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

inline Mat4 Transform::to_world_matrix() const
{
    if (parent)
    {
        return parent->to_world_matrix() * to_local_matrix();
    }
    return to_local_matrix();
}


Transform::Transform() : position(0, 0, 0), rotation(1, 0, 0, 0), scale(1, 1, 1) {}
inline Transform::Transform(const VEC::Vec3 &pos, const VEC::Vec3 &scale, const VEC::Quaternion &rot, Transform *parent_) : position(pos), scale(scale), rotation(rot), parent(parent_)
{
}
inline void Transform::translate(const VEC::Vec3 &translation)
{
    position.x += translation.x;
    position.y += translation.y;
    position.z += translation.z;
}
inline void Transform::rotate(const VEC::Quaternion &deltaRotation)
{
    rotation = rotation * deltaRotation;
}
inline void Transform::rotate(const VEC::Vec3 &axis, float radian)
{
    rotate(VEC::Quaternion(axis,radian));
}
inline void Transform::scaleBy(const VEC::Vec3 &scaleFactor)
{
    scale.x *= scaleFactor.x;
    scale.y *= scaleFactor.y;
    scale.z *= scaleFactor.z;
}
inline VEC::Vec3 Transform::get_world_position() const
{
    if (parent == nullptr)
        return position;
    return position + parent->get_world_position();
}



} // namespace VEC

} // namespace OneKeyEngine

#endif