#pragma once
#include <math/vect2.h>
#include <math/vect3.h>
#include <math/vect4.h>
#include <vector>
#include "SerializerSystem.h"
#include <cassert>

struct Vertex
{
    Vect4 aPos;
    Vect3 aNormal;
    Vect3 texCord;
};
struct Material
{
	Vect3 diffuse;
	Vect3 spectacular;
	Vect3 ambient;
	float shininess;
};
struct Mesh
{
    std::vector<Vertex>* verticies;
    std::vector<uint32_t>* indicies;
};

template<>
void SerializerSystem::Serialize<Mesh>(const Mesh& var)
{
	CHECKOS;
	assert(var.verticies != nullptr);
	assert(var.indicies  != nullptr);
	uint32_t meshSizes[2] = { (uint32_t)var.verticies->size(), (uint32_t)var.indicies->size()};	
	os->write((char*)meshSizes, sizeof(uint32_t) * 2);
	os->write((char*)var.verticies->data(), sizeof(Vertex) * var.verticies->size());
	os->write((char*)var.indicies->data(), sizeof(uint32_t) * var.indicies->size());
}

template<>
void SerializerSystem::Deserialize<Mesh>(Mesh& var)
{
	CHECKIS;
	if(var.verticies == nullptr) var.verticies = new std::vector<Vertex>();
	if(var.indicies == nullptr) var.indicies = new std::vector<uint32_t>();
	uint32_t meshSizes[2];
	is->read((char*)meshSizes, sizeof(uint32_t) * 2);
	for(uint32_t i = 0; i < meshSizes[0]; i ++)
	{
		Vertex vertex;
		is->read((char*)&vertex, sizeof(Vertex));
	}
	for(uint32_t i = 0; i < meshSizes[1]; i++)
	{
		uint32_t temp;
		is->read((char*)&temp, sizeof(uint32_t));
	}
}



struct Texture
{
	Texture() = default;
	~Texture() = default;
    uint32_t width, height, depth,  sizet;
    enum class Type : uint32_t
    {
        T1D,
        T2D,
        T3D,
    } type;
    uint8_t* data;
};
template<>
void SerializerSystem::Serialize<Texture>(const Texture& var)
{
	CHECKOS;
	os->write((char*)&var.width, sizeof(uint32_t));
	os->write((char*)&var.height, sizeof(uint32_t));
	os->write((char*)&var.depth, sizeof(uint32_t));
	os->write((char*)&var.type, sizeof(uint32_t));
	os->write((char*)&var.sizet, sizeof(uint32_t));
	os->write((char*)var.data, var.sizet);
}

template<>
void SerializerSystem::Deserialize<Texture>(Texture& var)
{
	CHECKIS;
	is->read((char*)&var.width, sizeof(uint32_t));
	is->read((char*)&var.height, sizeof(uint32_t));
	is->read((char*)&var.depth, sizeof(uint32_t));
	is->read((char*)&var.type, sizeof(uint32_t));
	is->read((char*)&var.sizet, sizeof(uint32_t));
	is->read((char*)var.data, var.sizet);

}
