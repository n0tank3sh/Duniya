#pragma once

#include <cstdint>
#include <math.h>


template<typename T>
T lerp(const T& a, const T& b, const T& x)
{
	return a + x * (b - a);
}
