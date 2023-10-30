#include "Vector3.h"

#include "Vector4.h"
#include "MathHelpers.h"
#include "Macros.h"

#include <cmath>
#include <cassert>

namespace dae
{
    const Vector3 Vector3::UnitX = Vector3{1, 0, 0};
    const Vector3 Vector3::UnitY = Vector3{0, 1, 0};
    const Vector3 Vector3::UnitZ = Vector3{0, 0, 1};
    const Vector3 Vector3::Zero = Vector3{0, 0, 0};

    Vector3::Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
    {
    }

    Vector3::Vector3(const Vector4& v) : x(v.x), y(v.y), z(v.z)
    {
    }

    Vector3::Vector3(const Vector3& from, const Vector3& to) : x(to.x - from.x), y(to.y - from.y), z(to.z - from.z)
    {
    }

    float Vector3::Magnitude() const
    {
#if QRSQRT
        const float magnitudeSq {x * x + y * y + z * z};
        return 1.0f / Q_rsqrt(magnitudeSq);
#else
        return std::sqrt(x * x + y * y + z * z);
#endif
    }

    float Vector3::SqrMagnitude() const
    {
        return x * x + y * y + z * z;
    }

    float Vector3::Normalize()
    {
#if QRSQRT
        const float m {Q_rsqrt(x * x + y * y + z * z)};
        x *= m;
        y *= m;
        z *= m;
        return m;
#else
        const float m{Magnitude()};
        x /= m;
        y /= m;
        z /= m;
        return m;
#endif
    }

    Vector3 Vector3::Normalized() const
    {
#if QRSQRT
        const float m {Q_rsqrt(x * x + y * y + z * z)};
        return {x * m, y * m, z * m};
#else
        const float m{Magnitude()};
        return {x / m, y / m, z / m};
#endif
    }

    float Vector3::Dot(const Vector3& v1, const Vector3& v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2)
    {
        Vector3 temp;
        temp.x = v1.y * v2.z - v1.z * v2.y;
        temp.y = v1.z * v2.x - v1.x * v2.z;
        temp.z = v1.x * v2.y - v1.y * v2.x;
        return temp;
    }

    Vector3 Vector3::Project(const Vector3& v1, const Vector3& v2)
    {
        return (v2 * (Dot(v1, v2) / Dot(v2, v2)));
    }

    Vector3 Vector3::Reject(const Vector3& v1, const Vector3& v2)
    {
        return (v1 - v2 * (Dot(v1, v2) / Dot(v2, v2)));
    }

    Vector3 Vector3::Reflect(const Vector3& v1, const Vector3& v2)
    {
        return v1 - (2.f * Vector3::Dot(v1, v2) * v2);
    }

    // implementation of barycentric coordinates
    Vector3 Vector3::Lico(float f1, const Vector3& v1, float f2, const Vector3& v2, float f3, const Vector3& v3)
    {
        Vector3 result;
        result.x = f1 * v1.x + f2 * v2.x + f3 * v3.x;
        result.y = f1 * v1.y + f2 * v2.y + f3 * v3.y;
        result.z = f1 * v1.z + f2 * v2.z + f3 * v3.z;
        return result;
    }

    Vector3 Vector3::Max(const Vector3& v1, const Vector3& v2)
    {
        return {std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z)};
    }

    Vector3 Vector3::Min(const Vector3& v1, const Vector3& v2)
    {
        return {std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z)};
    }

    Vector4 Vector3::ToPoint4() const
    {
        return {x, y, z, 1};
    }

    Vector4 Vector3::ToVector4() const
    {
        return {x, y, z, 0};
    }

#pragma region Operator Overloads
    Vector3 Vector3::operator*(float scale) const
    {
        return {x * scale, y * scale, z * scale};
    }

    Vector3 Vector3::operator/(float scale) const
    {
        return {x / scale, y / scale, z / scale};
    }

    Vector3 Vector3::operator+(const Vector3& v) const
    {
        return {x + v.x, y + v.y, z + v.z};
    }

    Vector3 Vector3::operator-(const Vector3& v) const
    {
        return {x - v.x, y - v.y, z - v.z};
    }

    Vector3 Vector3::operator-() const
    {
        return {-x, -y, -z};
    }

    Vector3& Vector3::operator*=(float scale)
    {
        x *= scale;
        y *= scale;
        z *= scale;
        return *this;
    }

    Vector3& Vector3::operator/=(float scale)
    {
        x /= scale;
        y /= scale;
        z /= scale;
        return *this;
    }

    Vector3& Vector3::operator-=(const Vector3& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vector3& Vector3::operator+=(const Vector3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    float& Vector3::operator[](int index)
    {
        assert(index <= 2 && index >= 0);

        if (index == 0) return x;
        if (index == 1) return y;
        return z;
    }

    float Vector3::operator[](int index) const
    {
        assert(index <= 2 && index >= 0);

        if (index == 0) return x;
        if (index == 1) return y;
        return z;
    }

    std::ostream& operator<<(std::ostream& os, const Vector3& v)
    {
        os << '[' << v.x << ", " << v.y << ", " << v.z << "]";
        return os;
    }
#pragma endregion
}
