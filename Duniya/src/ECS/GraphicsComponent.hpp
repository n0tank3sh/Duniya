#pragma once
#include <Math/Vect2.hpp>
#include <Math/Vect3.hpp>
#include <Math/Vect4.hpp>
#include <vector>
#include "SerializerSystem.hpp"
#include <cassert>
#include "CommonComponent.hpp"

struct Vertex
{
	Vect4 aPos;
	Vect3 aNormal;
	Vect2 texCord;
};
struct Material
{
	LightColor color;
	float shininess;
};	

enum class DrawPrimitive : uint32_t
{
	TRIANGLES, POINTS, TRIANGLES_ADJACENCIES, TRIANGLES_STRIPS, QUADS, LINES
};

struct Mesh
{
	uint32_t vertexCount;
	uint32_t indexCount;
	uint32_t verticiesIndex;
	uint32_t indiciesIndex;
	DrawPrimitive drawPrimitive;
};

struct Texture
{
	uint32_t width, height, channels, data;
	enum Format
	{
		RGBA, RGB, R
	} format;
};

