#pragma once

#include "mathutils.h"
#include <ios>
#include "vect2.h"

class Vect3
{
public:
    union
    {
        struct
        {
            float x, y, z;
        };
        float coordinates[3];
    };
public:

    Vect3(float x = .0f, float y = .0f, float z = .0f);
    Vect3(Vect2 other, float z);
    Vect3 operator+(const float& other) const;
    Vect3 operator-(const float& other) const;
    Vect3 operator*(const float& other) const;
    Vect3 operator/(const float& other) const;

    Vect3 operator+=(const float& other);
    Vect3 operator-=(const float& other);
    Vect3 operator*=(const float& other);
    Vect3 operator/=(const float& other);



    Vect3 operator+(const Vect3& other) const;
    Vect3 operator-(const Vect3& other) const;
    Vect3 operator*(const Vect3& other) const;
    Vect3 operator/(const Vect3& other) const;

    Vect3 operator+=(const Vect3& other);
    Vect3 operator-=(const Vect3& other);
    Vect3 operator*=(const Vect3& other);
    Vect3 operator/=(const Vect3& other);

    bool operator<(const Vect3& other) const;
    bool operator>(const Vect3& other) const;
    bool operator<=(const Vect3& other) const;
    bool operator>=(const Vect3& other) const;
    bool operator==(const Vect3& other) const;

    float dot(const Vect3& other) const;
    float cross(const Vect3& other) const;
    
    void normalize();
	friend std::istream& operator>>(std::istream &input, Vect3& a)
	{
		input >> a.x >> a.y >> a.z;
		return input;
	}
	friend std::ostream& operator<<(std::ostream &output, const Vect3& a)
	{
		output << a.x << " " << a.y << " " << a.z;
		return output;
	}
};

inline Vect3 Vect3::operator+(const float& other) const
{
    Vect3 temp(*this);
    temp.x += other;
    temp.y += other;
    temp.z += other;
    return temp;
}
inline Vect3 Vect3::operator-(const float& other) const
{
    Vect3 temp(*this);
    temp.x -= other;
    temp.y -= other;
    temp.z -= other;
    return temp;
}


inline Vect3 Vect3::operator*(const float& other) const
{
    Vect3 temp(*this);
    temp.x *= other;
    temp.y *= other;
    temp.z *= other;
    return temp;
}

inline Vect3 Vect3::operator/(const float& other) const
{
    Vect3 temp(*this);
    temp.x /= other;
    temp.y /= other;
    temp.z /= other;
    return temp;
}

inline Vect3 Vect3::operator+=(const float& other)
{
    *this = *this + other;
    return *this;
}

inline Vect3 Vect3::operator-=(const float& other)
{
    *this = *this - other;
    return *this;
}

inline Vect3 Vect3::operator*=(const float& other)
{
    *this = *this * other;
    return *this;
}

inline Vect3 Vect3::operator/=(const float& other)
{
    *this = *this / other;
    return *this;
}

inline Vect3 Vect3::operator+(const Vect3& other) const
{
    Vect3 temp(*this);
    temp.x = this->x + other.x;
    temp.y = this->y + other.y;
    temp.z = this->z + other.z;
    return temp;
}

inline Vect3 Vect3::operator-(const Vect3& other) const
{
    Vect3 temp(*this);
    temp.x = this->x - other.x;
    temp.y = this->y - other.y;
    temp.z = this->z - other.z;
    return temp;
}

inline Vect3 Vect3::operator*(const Vect3& other) const
{
    Vect3 temp(*this);
    temp.x = this->x * other.x;
    temp.y = this->y * other.y;
    temp.z = this->z * other.z;
    return temp;
}

inline Vect3 Vect3::operator/(const Vect3& other) const
{
    Vect3 temp(*this);
    temp.x = this->x / other.x;
    temp.y = this->y / other.y;
    temp.z = this->z / other.z;
    return temp;
}

inline Vect3 Vect3::operator+=(const Vect3& other)
{
    *this = *this + other;
    return *this;
}

inline Vect3 Vect3::operator-=(const Vect3& other)
{
    *this = *this - other;
    return *this;
}

inline Vect3 Vect3::operator*=(const Vect3& other)
{
    *this = *this * other;
    return *this;
}

inline Vect3 Vect3::operator/=(const Vect3& other)
{
    *this = *this / other;
    return *this;
}

inline bool Vect3::operator<(const Vect3& other) const
{
}
