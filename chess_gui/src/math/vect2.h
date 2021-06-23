#pragma once

#include "mathutils.h"

class Vect2
{
public:
    union
    {
        struct
        {
            float x, y;
        };
        float coordinates[2];
    };
public:

    Vect2(float x = .0f, float y  = .0f);
    Vect2 operator+(const float& other) const;
    Vect2 operator-(const float& other) const;
    Vect2 operator*(const float& other) const;
    Vect2 operator/(const float& other) const;

    Vect2 operator+=(const float& other);
    Vect2 operator-=(const float& other);
    Vect2 operator*=(const float& other);
    Vect2 operator/=(const float& other);



    Vect2 operator+(const Vect2& other) const;
    Vect2 operator-(const Vect2& other) const;
    Vect2 operator*(const Vect2& other) const;
    Vect2 operator/(const Vect2& other) const;

    Vect2 operator+=(const Vect2& other);
    Vect2 operator-=(const Vect2& other);
    Vect2 operator*=(const Vect2& other);
    Vect2 operator/=(const Vect2& other);

    bool operator<(const Vect2& other) const;
    bool operator>(const Vect2& other) const;
    bool operator<=(const Vect2& other) const;
    bool operator>=(const Vect2& other) const;
    bool operator!=(const Vect2& other) const;
    bool operator==(const Vect2& other) const;

    float dot(const Vect2& other) const;
    float angle() const;
    float distance() const;
    void normalize();
};

inline Vect2 Vect2::operator+(const float& other) const
{
    Vect2 temp(*this);
    temp.x += other;
    temp.y += other;
    return temp;
}
inline Vect2 Vect2::operator-(const float& other) const
{
    Vect2 temp(*this);
    temp.x -= other;
    temp.y -= other;
    return temp;
}


inline Vect2 Vect2::operator*(const float& other) const
{
    Vect2 temp(*this);
    temp.x *= other;
    temp.y *= other;
    return temp;
}

inline Vect2 Vect2::operator/(const float& other) const
{
    Vect2 temp(*this);
    temp.x /= other;
    temp.y /= other;
    return temp;
}

inline Vect2 Vect2::operator+=(const float& other)
{
    *this = *this + other;
    return *this;
}

inline Vect2 Vect2::operator-=(const float& other)
{
    *this = *this - other;
    return *this;
}

inline Vect2 Vect2::operator*=(const float& other)
{
    *this = *this * other;
    return *this;
}

inline Vect2 Vect2::operator/=(const float& other)
{
    *this = *this / other;
    return *this;
}

inline Vect2 Vect2::operator+(const Vect2& other) const
{
    Vect2 temp(*this);
    temp.x = this->x + other.x;
    temp.y = this->y + other.y;
    return temp;
}

inline Vect2 Vect2::operator-(const Vect2& other) const
{
    Vect2 temp(*this);
    temp.x = this->x - other.x;
    temp.y = this->y - other.y;
    return temp;
}

inline Vect2 Vect2::operator*(const Vect2& other) const
{
    Vect2 temp(*this);
    temp.x = this->x * other.x;
    temp.y = this->y * other.y;
    return temp;
}

inline Vect2 Vect2::operator/(const Vect2& other) const
{
    Vect2 temp(*this);
    temp.x = this->x / other.x;
    temp.y = this->y / other.y;
    return temp;
}

inline Vect2 Vect2::operator+=(const Vect2& other)
{
    *this = *this + other;
    return *this;
}

inline Vect2 Vect2::operator-=(const Vect2& other)
{
    *this = *this - other;
    return *this;
}

inline Vect2 Vect2::operator*=(const Vect2& other)
{
    *this = *this * other;
    return *this;
}

inline Vect2 Vect2::operator/=(const Vect2& other)
{
    *this = *this / other;
    return *this;
}

inline bool Vect2::operator<(const Vect2& other) const
{
    if(x == other.x)
    {
        return y < other.y;
    }
    return x < other.y;
}

inline bool Vect2::operator>(const Vect2& other) const
{
    if(x == other.x)
    {
        return y < other.y;
    }
    return x > other.y;
}

inline bool Vect2::operator>=(const Vect2& other) const
{
    if(x == other.x)
    {
        return y <= other.y;
    } 
    return x > other.y;
}

inline bool Vect2::operator<=(const Vect2& other) const
{
    if(x == other.x)
    {
        return y <= other.y;
    }
    return x < other.x;
}


inline bool Vect2::operator!=(const Vect2& other) const
{
    return x != other.x || y != other.y;
}

inline bool Vect2::operator==(const Vect2& other) const
{
    return !(*this == other);
}
