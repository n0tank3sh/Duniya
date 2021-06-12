#pragma once

#include "mathutils.h"

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

    Vect4();    
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
