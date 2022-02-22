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
    Vect2 texCord;
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
    uint32_t verticiesIndex;
    uint32_t indiciesIndex;
};

struct Texture
{
	uint32_t width, height, channels, data;
	enum Format
	{
		RGBA, RGB, R
	} format;
};

