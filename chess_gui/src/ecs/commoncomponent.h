#pragma once
#include <math/vect2.h>
#include <math/vect3.h>
#include <math/vect4.h>
#include <math/mat.h>

struct Transform
{
    Vect3 pos;
    Vect3 scale;
    Vect3 rotation;
};

struct PointLight
{
	Vect3 color;
};

inline Mat* ConvertTranforToMatrix(Transform& transform)
{
    float trans[] = 
    {
        transform.scale.x, .0f, .0f, transform.pos.x,
        .0f, transform.scale.y, .0f, transform.pos.y,
        .0f, .0f, transform.scale.z,  transform.pos.z,
        .0f, .0f, .0f, 1.0f
    };
    Mat* mat = new Mat({4, 4}, trans);
    return mat;
}
