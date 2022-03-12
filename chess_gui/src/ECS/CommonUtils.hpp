#pragma once
#include <Math/MathUtils.hpp>
#include "CommonComponent.hpp"
#include "GraphicsComponent.hpp"
#include "ECS.hpp"


inline void CreatePlane()
{
	const float gridX = 10, gridY = 10,  height = 100, width = 100;
	const auto rows = width/gridX, cols = height/gridY;
	auto luft = new Vertex[(uint32_t)rows * cols];
	for(float i = 0.f; i < cols; i++)
	{
		for(float j = 0.f; j < width/gridX; j++)
		{
		}
	}
}


