#pragma once
#include <ostream>

namespace dae
{
    struct Vector3;

    struct Vector4
    {
        float x {0.0f};
        float y {0.0f};
        float z {0.0f};
        float w {0.0f};

        Vector4() = default;
        Vector4(float _x, float _y, float _z, float _w);
        Vector4(const Vector3& v, float _w);

        float Magnitude() const;
        float SqrMagnitude() const;
        float Normalize();
        Vector4 Normalized() const;

        static float Dot(const Vector4& v1, const Vector4& v2);

        // operator overloading
        Vector4 operator*(float scale) const;
        Vector4 operator+(const Vector4& v) const;
        Vector4 operator-(const Vector4& v) const;
        Vector4& operator+=(const Vector4& v);
        float& operator[](int index);
        float operator[](int index) const;

    public:
        friend std::ostream& operator<<(std::ostream& os, const Vector4& v);
    };
}
