#pragma once
#include <Math/Vect2.h>
#include <Math/Vect3.h>
#include <Math/Vect4.h>
#include <Math/Mat.h>

struct Transform
{
    Vect3 pos;
    Vect3 scale;
    Vect3 rotation;
};

namespace ComponentTypes
{
	constexpr uint32_t POINTLIGHT = 7;
	constexpr uint32_t DIRLIGHT = 8;
};

struct LightColor
{
	Vect3 specular, ambient, diffuse;
};

struct DirectionalLight
{
	LightColor lightColor;
	Vect3 dir;
};

struct PointLight
{
	LightColor lightColor;
	float constant, linear, quadratic;
	Vect3 pos;
};

struct Camera
{
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
	*mat *= DefaultMatrix::generateRollMatrix({4, 4}, transform.rotation.x) * DefaultMatrix::generatePitchMatrix({4, 4}, transform.rotation.y) * DefaultMatrix::generateYawMatrix({4, 4}, transform.rotation.z);
    return mat;
}
