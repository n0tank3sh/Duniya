#pragma once 

#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>
#include <math/mat.h>

using VertexSpecification = std::vector<std::pair<std::string, uint32_t>>;


struct GBinder
{
    virtual void Bind() const noexcept  = 0;
    virtual void UnBind() const noexcept  = 0;
};

struct Texture
{
	Texture() = default;
	~Texture() = default;
    uint32_t width, height, depth,  sizet;
    enum class Type
    {
        T1D,
        T2D,
        T3D,
    } type;
    uint8_t* data;
    GBinder* binder;
    void Bind() const noexcept;
    void UnBind() const noexcept;
};

struct GBuffer
{
    struct GBufferStyle
    {
        enum class CPUFlags
        {
            STATIC, DYNAMIC, STREAM
        } cpuFlags;
        enum class Usage
        {
            DRAW, COPY, READ
        } usage;
        enum class BufferType
        {
            VERTEX, INDEX
        } type;
    } bufferStyle;
    uint32_t count;
    uint32_t sizet;
    void* data;
    std::unique_ptr<GBinder> gBinder;

    void Bind() const noexcept;
    void UnBind() const noexcept;

};

enum class ShaderType
{
    VERTEX, FRAGMENT
};



class Renderer
{
public:
	//Override function
    virtual void Draw(GBuffer* gBuffer) = 0;
    virtual void LoadBuffer(GBuffer* gBuffer) = 0;
    virtual void LoadTexture(Texture* texture) = 0;
    virtual void AddShader(ShaderType type, std::string& source) = 0;
    virtual void Uniform1f(const float& data, std::string name) = 0;
    virtual void Uniform2f(const Vect2& data, std::string name) = 0;
    virtual void Uniform3f(const Vect3& data, std::string name) = 0;
    virtual void Uniform4f(const Vect4& data, std::string name) = 0;
    virtual void UniformMat(const Mat& mat, std::string name) = 0;
    virtual void Clear() = 0;
    virtual void ClearColor(float r, float g, float b) = 0;
    virtual void ClearDepth(float depthLevel) = 0;
public:
    void SetLayout(VertexSpecification& specification);
protected:
    VertexSpecification specification;
};
