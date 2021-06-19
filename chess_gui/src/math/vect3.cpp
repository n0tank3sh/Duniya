#include "vect3.h"
Vect3::Vect3(float x, float y, float z)
    :
        x(x), y(y), z(z)
{}

Vect3::Vect3(Vect2 other, float z)
    :
        x(other.x), y(other.y), z(z)
{}
