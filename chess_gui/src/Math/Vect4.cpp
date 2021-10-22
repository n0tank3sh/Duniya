#include "Vect4.h"

Vect4::Vect4(float x, float y, float z, float w)
    :
        x(x), y(y), z(z), w(w)
{}

Vect4::Vect4(Vect2 a, float z, float w)
    :
        x(a.x), y(a.y), z(z), w(w)
{}

Vect4::Vect4(Vect2 a, Vect2 b)
    :
        x(a.x), y(a.y), z(b.x), w(b.y)
{}

Vect4::Vect4(Vect3 a, float w)
    :
        x(a.x), y(a.y), z(a.z), w(w)
{}


float Vect4::dot(const Vect4& other) const
{
	float game = .0f;
	for(uint32_t i = 0; i < 4; i++)
	{
		game += this->coordinates[i] * other.coordinates[i];
	}
	return game;
}


