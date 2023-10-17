#include "Vector4.h"

#include "Vector3.h"
#include "MathHelpers.h"
#include "Macros.h"

#include <cmath>
#include <cassert>

namespace dae
{
    Vector4::Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w)
    {
    }

    Vector4::Vector4(const Vector3& v, float _w) : x(v.x), y(v.y), z(v.z), w(_w)
    {
    }

    float Vector4::Magnitude() const
    {
#if QRSQRT
        const float magnitudeSq {x * x + y * y + z * z + w * w};
        return 1.0f / Q_rsqrt(magnitudeSq);
#else
        return std::sqrt(x * x + y * y + z * z + w * w);
#endif
    }

    float Vector4::SqrMagnitude() const
    {
        return x * x + y * y + z * z + w * w;
    }

    float Vector4::Normalize()
    {
#if QRSQRT
        const float m {Q_rsqrt(x * x + y * y + z * z + w * w)};
        x *= m;
        y *= m;
        z *= m;
        w *= m;
        return m;
#else
        const float m{Magnitude()};
        x /= m;
        y /= m;
        z /= m;
        w /= m;
        return m;
#endif
    }

    Vector4 Vector4::Normalized() const
    {
#if QRSQRT
        const float m {Q_rsqrt(x * x + y * y + z * z + w * w)};
        return {x * m, y * m, z * m, w * m};
#else
        const float m{Magnitude()};
        return {x / m, y / m, z / m, w / m};
#endif
    }

    float Vector4::Dot(const Vector4& v1, const Vector4& v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
    }

#pragma region Operator Overloads
    Vector4 Vector4::operator*(float scale) const
    {
        return {x * scale, y * scale, z * scale, w * scale};
    }

    Vector4 Vector4::operator+(const Vector4& v) const
    {
        return {x + v.x, y + v.y, z + v.z, w + v.w};
    }

    Vector4 Vector4::operator-(const Vector4& v) const
    {
        return {x - v.x, y - v.y, z - v.z, w - v.w};
    }

    Vector4& Vector4::operator+=(const Vector4& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;
        return *this;
    }

    float& Vector4::operator[](int index)
    {
        assert(index <= 3 && index >= 0);

        if (index == 0)return x;
        if (index == 1)return y;
        if (index == 2)return z;
        return w;
    }

    float Vector4::operator[](int index) const
    {
        assert(index <= 3 && index >= 0);

        if (index == 0)return x;
        if (index == 1)return y;
        if (index == 2)return z;
        return w;
    }

#pragma endregion
    std::ostream& operator<<(std::ostream& os, const Vector4& v)
    {
        return os << '[' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ']';
    }
}
