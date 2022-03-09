#include "Vect3.h"
Vect3::Vect3(float x, float y, float z)
    :
        x(x), y(y), z(z)
{}

Vect3::Vect3(Vect2 other, float z)
    :
        x(other.x), y(other.y), z(z)
{}

float Vect3::distance() const
{
	return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
}
void Vect3::normalize()
{
	float distance = this->distance();
	for(uint32_t i = 0; i < 3; i++)
	{
		this->coordinates[i] /= distance;
	}
}


float Vect3::dot(const Vect3& other) const
{
	float game = 0.f;
	for(uint32_t i = 0; i < 3; i++)
	{
		game += this->coordinates[i] * other.coordinates[i];
	}
	return game;
}
Vect3 Vect3::cross(const Vect3& other) const
{
	Vect3 game;
	game.x = y * other.z - z * other.y;
	game.y = z * other.x - x * other.z;
	game.z = x * other.y - y * other.x;
	return game;
}
