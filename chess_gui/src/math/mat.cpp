
#include "mat.h"
#include <initializer_list>
#include <cassert>
#include <string.h>

Mat::Mat(Dimension dimension)
{
	sizet = dimension.row * dimension.column;
    buffer.reset(new float[sizet]);
	memset(buffer.get(), 0, sizeof(float) * sizet);
    this->dimension = dimension;
}

Mat::Mat(Dimension dimension, float* otherBuffer)
{
    this->dimension = dimension;
    sizet = dimension.column * dimension.row;
    buffer.reset(new float[sizet]);
    std::copy(otherBuffer, otherBuffer + sizet, buffer.get());
}

Mat::Mat(Dimension s, const std::initializer_list<float> l)
{
	sizet = s.row * s.column;
	buffer.reset(new float[sizet]);
	auto itr = l.begin();
	for(uint32_t i = 0; i < sizet && itr != l.end(); i++, itr++)
	{
		buffer[i] = *itr;
	}
}

Mat::Mat(const Mat& other)
{
    sizet = other.sizet;
    dimension = other.dimension;
    buffer.reset(new float[sizet]);
    std::copy(other.buffer.get(), other.buffer.get() + sizet, buffer.get());
}

Mat& Mat::operator=(const Mat& other)
{
    sizet = other.sizet;
    dimension = other.dimension;
    buffer.reset(new float[sizet]);
    std::copy(other.buffer.get(), other.buffer.get() + sizet, buffer.get());
    return *this;
}

Mat& Mat::operator=(const std::initializer_list<float> l)
{
	uint32_t i = 0;
	auto itr = l.begin();
	for(; i < sizet && itr != l.end(); i++, itr++)
	{
		buffer[i] = *itr;
	}
	return *this;
}


float& Mat::operator[](MatIndex a)
{
    return buffer[a.x * dimension.row + a.y];
}

Mat Mat::operator+(const Mat& other)
{
	assert(sizet == other.sizet);
	Mat mat(dimension);
	for(uint32_t i = 0; i < sizet; i++)
	{
		mat.buffer[i] = buffer[i] + other.buffer[i];
	}
	return mat;
}

Mat Mat::operator-(const Mat& other)
{
	assert(sizet == other.sizet);
	Mat mat(dimension);
	for(uint32_t i = 0; i < sizet; i++)
	{
		mat.buffer[i] = buffer[i] - other.buffer[i];
	}
	return mat;
}

Mat Mat::operator*(const Mat& other)
{
    Mat mat(this->dimension);
    if(dimension.row != other.dimension.column)
    {
    }
    for(uint32_t i = 0; i < dimension.column; i++)
    {
        for(uint32_t j = 0; j < dimension.row; j++)
        {
            float& temp = mat.buffer[i * dimension.column + j];
            temp = 0;
            for(uint32_t a = 0; a < dimension.row; a++)
            {
                temp += buffer[i * dimension.row + a] * other.buffer[a * other.dimension.row + j];
            }

        }
    }
    return mat;
}

Mat Mat::operator*=(const Mat& other)
{
	*this = *this * other;
	return *this;
}

Mat Mat::operator+=(const Mat& other)
{
	*this = *this + other;
	return *this;
}
Mat Mat::operator-=(const Mat& other)
{
	*this = *this - other;
	return *this;
}

Mat DefaultMatrix::generateIdentityMatrix(Dimension s)
{
	Mat mat(s);
	for(uint32_t i = 0; i < s.row; i++)
	{
		mat.buffer[i * s.column + i] = 1.f;
	}
	return mat;
}

Mat DefaultMatrix::generateRollMatrix(Dimension s, float x)
{
	assert(s.row >= 2 && s.column >= 2);
	Mat mat = DefaultMatrix::generateIdentityMatrix(s);
	mat[{0, 0}] = cosf(x);
	mat[{0, 1}] = -sinf(x);
	mat[{1, 0}] = sinf(x);
	mat[{1, 1}] = cosf(x);
	return mat;
}
Mat DefaultMatrix::generatePitchMatrix(Dimension s, float y)
{
	assert(s.row >= 3 && s.column >= 3);
	Mat mat= DefaultMatrix::generateIdentityMatrix(s);
	mat[{0, 0}] = cosf(y);
	mat[{0, 2}] = sinf(y);
	mat[{2, 0}] = -sinf(y);
	mat[{2, 2}] = cosf(y);
	return mat;
}

Mat DefaultMatrix::generateYawMatrix(Dimension s, float z)
{
	assert(s.row >= 3 && s.column >= 3);
	Mat mat = DefaultMatrix::generateIdentityMatrix(s);
	mat[{1, 1}] = cosf(z);
	mat[{1, 2}] = -sinf(z);
	mat[{2, 1}] = sinf(z);
	mat[{2, 2}] = cosf(z);
	return mat;
}
