#include "Vect2.h"


Vect2::Vect2(float x, float y)
    :
        x(x), y(y)
{}

float Vect2::dot(const Vect2& other) const
{
    return this->x * other.x + this->y * other.y;
}

float Vect2::angle() const
{
    return std::cos(this->x);
}

float Vect2::distance() const
{
    float tempX = this->x * this->x;
    float tempY = this->y * this->y;
    return sqrtf(tempX + tempY);
}

void Vect2::normalize()
{
    *this/= this->distance();
}

