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
	friend std::istream& operator>>(std::istream& input, Mat& mat)
	{
		for(uint32_t i = 0; i < mat.sizet; i++)
		{
			input >> mat.buffer.get()[i];
		}
		return input;
	}
	friend std::ostream& operator<<(std::ostream& output, const Mat& mat)
	{
		for(uint32_t a = 0; a < mat.dimension.column; a++)
		{
			for(uint32_t b = 0; b < mat.dimension.row; b++)
			{
				output << mat.buffer.get()[a * mat.dimension.row + b] << " ";
			}
			output << std::endl;
		}
		return output;
	}
private:
    void GausianElimination();
};
