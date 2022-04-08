#include "ShapeGenerator.hpp"
#include <cmath>


Mesh ShadpeGenerator::GenerateCube()
{
	Mesh mesh;
	auto verticies = new Vertex[8];
	auto indicies = new uint32_t[36];
	for(int i = 0; i < 8; i++)
	{
		auto k = i;
		if(i >= 4)
		{
			k -= 4;
			verticies[i].aPos = Vect4(k & 1, -1.f, (k >> 1) & 1, 1.f);
		}
		else
		{
			verticies[i].aPos = Vect4(k & 1, -1.f, (k >> 1) & 1, 1.f);
		}
		verticies[i].aPos.x = lerp(-1.f, 1.f, verticies[i].aPos.x);
		verticies[i].aPos.z = lerp(-1.f, 1.f, verticies[i].aPos.z); 
	}
	indicies = {};
	return mesh;
}

Mesh ShadpeGenerator::GenerateSphere(uint32_t divisorX, uint32_t divisorY)
{
	Mesh mesh;
	mesh.vertexCount = 2 + divisorY * divisorX;
	auto verticies = new Vertex[mesh.vertexCount];
	float distribX = (M_PI * 2)/divisorX, distribY = (M_PI * 2)/divisorY;
	verticies[0].aPos = Vect4(0, 1.f, 0.0, 1.f);
	for(uint32_t i = 0; i < divisorY; i++)
	{
		for(uint32_t j = 0; j < divisorX; j++)
		{
			auto tmpY = distribY * i;
			auto tmpX = distribX * j;
			verticies[i * divisorX + 1 + j].aPos =  Vect4(std::sin(tmpX), std::cos(-M_PI + tmpY), std::cos(tmpX), 1.f);
		}
	}
	verticies[mesh.vertexCount - 1].aPos = Vect4(0, 1.f, 0.0, 1.f);
	return mesh;
}
