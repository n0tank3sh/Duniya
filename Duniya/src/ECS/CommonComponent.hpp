#pragma once
#include <Math/Mat.hpp>
#include <Math/Vect2.hpp>
#include <Math/Vect3.hpp>
#include <Math/Vect4.hpp>

struct Transform {
    Vect3 pos;
    Vect3 scale;
    Vect3 rotation;
};

namespace ComponentTypes {
constexpr uint32_t POINTLIGHT = 7;
constexpr uint32_t DIRLIGHT = 8;
};  // namespace ComponentTypes

struct LightColor {
    Vect3 specular, ambient, diffuse;
};

struct DirectionalLight {
    LightColor lightColor;
    Vect3 dir;
};

struct PointLight {
    LightColor lightColor;
    float constant, linear, quadratic;
    Vect3 pos;
};

struct Camera {
    float fov;
    float near;
    float far;
    Vect3 lookAt;
};

inline Mat GetRotationMatrix(Vect3 rotation) {
    Mat mat = DefaultMatrix::generateIdentityMatrix({4, 4});
    if (rotation.x != 0)
	mat *= DefaultMatrix::generateRollMatrix({4, 4}, rotation.x);
    if (rotation.y != 0)
	mat *= DefaultMatrix::generatePitchMatrix({4, 4}, rotation.y);
    if (rotation.z != 0)
	mat *= DefaultMatrix::generateYawMatrix({4, 4}, rotation.z);
    return mat;
}

inline Mat ConvertTranforToMatrix(Transform& transform) {
    Mat mat({4, 4});
    mat = {transform.scale.x,
	   .0f,
	   .0f,
	   transform.pos.x,
	   .0f,
	   transform.scale.y,
	   .0f,
	   transform.pos.y,
	   .0f,
	   .0f,
	   transform.scale.z,
	   transform.pos.z,
	   .0f,
	   .0f,
	   .0f,
	   1.0f};
    mat *= GetRotationMatrix(transform.rotation);
    return mat;
}
