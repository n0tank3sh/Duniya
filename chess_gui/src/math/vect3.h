#pragma once

#include "mathutils.h"

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

    Vect3();    
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
};
