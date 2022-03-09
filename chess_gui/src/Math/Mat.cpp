
#include "Mat.h"
#include <initializer_list>
#include <cassert>
#include <stdexcept>
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
            float& temp = mat.Get(i, j);
            temp = 0.f;
            for(uint32_t a = 0; a < dimension.row; a++)
            {
                temp += Get(i, a) * other.Get(a, j);
            }

        }
    }
    return mat;
}

Vect2 Mat::operator*(const Vect2& other)
{
	Vect2 ans;
	if(dimension.row != 2 || dimension.column != 2)
		throw std::runtime_error("incompatible matrix dimension with type");
	for(int i = 0; i < dimension.row; i++)
	{
		for(int j = 0; j < dimension.column; j++)
		{
			ans.coordinates[i] += Get(i, j) * other.coordinates[j];
		}
	}
	return ans;
}
Vect3 Mat::operator*(const Vect3& other)
{
	Vect3 ans;
	if(dimension.row != 3 || dimension.column != 3)
		throw std::runtime_error("incompatible matrix dimension with type");
	for(int i = 0; i < dimension.row; i++)
	{
		for(int j = 0; j < dimension.column; j++)
		{
			ans.coordinates[i] += Get(i, j) * other.coordinates[j];
		}
	}
	return ans;
}

Vect4 Mat::operator*(const Vect4& other)
{
	Vect4 ans;
	if(dimension.row != 4 || dimension.column != 4)
		throw std::runtime_error("incompatible matrix dimension with type");
	for(int i = 0; i < dimension.row; i++)
	{
		for(int j = 0; j < dimension.column; j++)
		{
			ans.coordinates[i] += Get(i, j) * other.coordinates[j];
		}
	}
	return ans;
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

float& Mat::Get(const uint32_t& a, const uint32_t& b)
{
	if(a < dimension.row && b < dimension.column)  
		return buffer.get()[a * dimension.column + b];
	else
		throw std::out_of_range("indices out of range");
}

const float& Mat::Get(const uint32_t& a, const uint32_t& b) const
{
	if(a < dimension.row && b < dimension.column)  
		return buffer.get()[a * dimension.column + b];
	else
		throw std::out_of_range("indices out of range");
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
