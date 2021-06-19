#pragma once

#include "mathutils.h"
#include "vect2.h"
#include "vect3.h"


class Vect4
{
public:
    union
    {
        struct
        {
            float x, y, z, w;
        };
        float coordinates[4];
    };
public:

    Vect4(float x = .0f, float y = .0f, float z = .0f, float w = .0f);
    Vect4(Vect2 a, Vect2 b);
    Vect4(Vect2 a, float z, float w);
    Vect4(Vect3 a, float w);
    Vect4 operator+(const float& other) const;
    Vect4 operator-(const float& other) const;
    Vect4 operator*(const float& other) const;
    Vect4 operator/(const float& other) const;

    Vect4 operator+=(const float& other);
    Vect4 operator-=(const float& other);
    Vect4 operator*=(const float& other);
    Vect4 operator/=(const float& other);



    Vect4 operator+(const Vect4& other) const;
    Vect4 operator-(const Vect4& other) const;
    Vect4 operator*(const Vect4& other) const;
    Vect4 operator/(const Vect4& other) const;

    Vect4 operator+=(const Vect4& other);
    Vect4 operator-=(const Vect4& other);
    Vect4 operator*=(const Vect4& other);
    Vect4 operator/=(const Vect4& other);

    bool operator<(const Vect4& other) const;
    bool operator>(const Vect4& other) const;
    bool operator<=(const Vect4& other) const;
    bool operator>=(const Vect4& other) const;
    bool operator==(const Vect4& other) const;

    float dot(const Vect4& other) const;
    float cross(const Vect4& other) const;
    void normalize();
};

inline Vect4 Vect4::operator+(const float& other) const
{
    Vect4 temp(*this);
    temp.x += other;
    temp.y += other;
    temp.z += other;
    return temp;
}
inline Vect4 Vect4::operator-(const float& other) const
{
    Vect4 temp(*this);
    temp.x -= other;
    temp.y -= other;
    temp.z -= other;
    return temp;
}


inline Vect4 Vect4::operator*(const float& other) const
{
    Vect4 temp(*this);
    temp.x *= other;
    temp.y *= other;
    temp.z *= other;
    return temp;
}

inline Vect4 Vect4::operator/(const float& other) const
{
    Vect4 temp(*this);
    temp.x /= other;
    temp.y /= other;
    temp.z /= other;
    return temp;
}

inline Vect4 Vect4::operator+=(const float& other)
{
    *this = *this + other;
    return *this;
}

inline Vect4 Vect4::operator-=(const float& other)
{
    *this = *this - other;
    return *this;
}

inline Vect4 Vect4::operator*=(const float& other)
{
    *this = *this * other;
    return *this;
}

inline Vect4 Vect4::operator/=(const float& other)
{
    *this = *this / other;
    return *this;
}

inline Vect4 Vect4::operator+(const Vect4& other) const
{
    Vect4 temp(*this);
    temp.x = this->x + other.x;
    temp.y = this->y + other.y;
    temp.z = this->z + other.z;
    return temp;
}

inline Vect4 Vect4::operator-(const Vect4& other) const
{
    Vect4 temp(*this);
    temp.x = this->x - other.x;
    temp.y = this->y - other.y;
    temp.z = this->z - other.z;
    return temp;
}

inline Vect4 Vect4::operator*(const Vect4& other) const
{
    Vect4 temp(*this);
    temp.x = this->x * other.x;
    temp.y = this->y * other.y;
    temp.z = this->z * other.z;
    return temp;
}

inline Vect4 Vect4::operator/(const Vect4& other) const
{
    Vect4 temp(*this);
    temp.x = this->x / other.x;
    temp.y = this->y / other.y;
    temp.z = this->z / other.z;
    return temp;
}

inline Vect4 Vect4::operator+=(const Vect4& other)
{
    *this = *this + other;
    return *this;
}

inline Vect4 Vect4::operator-=(const Vect4& other)
{
    *this = *this - other;
    return *this;
}

inline Vect4 Vect4::operator*=(const Vect4& other)
{
    *this = *this * other;
    return *this;
}

inline Vect4 Vect4::operator/=(const Vect4& other)
{
    *this = *this / other;
    return *this;
}

inline bool Vect4::operator<(const Vect4& other) const
{
}
