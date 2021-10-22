#pragma once
#include <Math/Vect2.h>
#include <Math/Vect3.h>
#include <Math/Vect4.h>
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
	uint32_t vertexCount;
	uint32_t indexCount;
    Vertex* verticies;
    uint32_t* indicies;
};

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

