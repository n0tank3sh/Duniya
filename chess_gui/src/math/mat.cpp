
#include "mat.h"
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

float& Mat::operator[](MatIndex a)
{
    return buffer.get()[a.x * dimension.row + a.y];
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
            float& temp = mat.buffer.get()[i * dimension.column + j];
            temp = 0;
            for(uint32_t a = 0; a < dimension.row; a++)
            {
                temp += buffer.get()[i * dimension.row + a] * other.buffer.get()[a * other.dimension.row + j];
            }

        }
    }
    return mat;
}
