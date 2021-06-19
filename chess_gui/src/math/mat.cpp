
#include "mat.h"

Mat::Mat(Dimension dimension)
{
    buffer.reset(new float[dimension.column * dimension.row]);
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
    for(uint32_t i = 0; i < dimension.row; i++)
    {
        for(uint32_t j = 0; j < dimension.column; j++)
        {
            float& temp = mat.buffer.get()[i * dimension.row + j];
            temp = 0;
            for(uint32_t a = 0; a < dimension.row; a++)
            {
                temp += buffer.get()[i * dimension.row + a] * other.buffer.get()[a * other.dimension.row + j];
            }

        }
    }
    return mat;
}
