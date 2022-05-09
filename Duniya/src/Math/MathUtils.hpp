#pragma once

#include <math.h>

#include <cstdint>

template <typename T>
T lerp(const T& a, const T& b, const T& x) {
    return a + x * (b - a);
}
