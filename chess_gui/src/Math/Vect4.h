#pragma once

#include "MathUtils.h"
#include "Vect2.h"
#include "Vect3.h"


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
	Vect4 cross(const Vect4& other) const;
    void normalize();
	friend std::istream& operator>>(std::istream &input, Vect4& a)
	{
		input >> a.x >> a.y >> a.z >> a.w;
		return input;
	}
	friend std::ostream& operator<<(std::ostream &output, const Vect4& a)
	{
		output << a.x << " " << a.y << " " << a.z << " " << a.w;
		return output;
	}
public:
	inline Vect4 normalized()
	{
		auto tmp = *this;
		tmp.normalize();
		return tmp;
	}
	inline static float dot(const Vect4& first, const Vect4& other)
	{
		auto temp = first;
		return temp.dot(other);
	}
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
	if(this->x == other.x)
	{
		if(this->y == other.y)
		{
			if(this->z == other.z)
			{
				return this->w < other.w;
			}
			else return this->z < other.z;
		}
		else return this->y < other.y;
	}
	else return this->x < other.x;
}
