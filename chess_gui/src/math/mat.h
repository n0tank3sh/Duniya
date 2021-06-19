#pragma once

#include "mathutils.h"
#include "vect2.h"
#include "vect3.h"
#include "vect4.h"
#include <memory>

struct Dimension
{
    uint32_t row;
    uint32_t column;
};

struct MatIndex
{
    uint32_t x, y;
};

struct Mat
{
    Mat(Dimension dimension = {1, 1});
    Mat(Dimension dimension, float* otherBuffer);

    Mat(const Mat& other);
    Mat& operator=(const Mat& other);

    float& operator[](MatIndex a);

    Mat operator*(const Mat& other);
    Mat operator*(const Vect2& other);
    Mat operator*(const Vect3& other);
    Mat operator*(const Vect4& other);
    Mat operator+(const Mat& other);
    Mat operator-(const Mat& other);
    Mat dot(const Mat& other);
    
    // return the determinants of the Matrix
    float Deter();
    // Invering Matrix
    Mat& inverse();
    std::unique_ptr<float> buffer;
    Dimension dimension;
    uint32_t sizet;
private:
    void GausianElimination();
};
