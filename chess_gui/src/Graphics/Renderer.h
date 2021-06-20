#pragma once 

#include <cstdint>
#include <memory>
#include <vector>
#include <math/mat.h>

using VertexSpecification = std::vector<std::pair<std::string, uint32_t>>;


struct GBinder
{
    virtual void Bind() const noexcept = 0;
    virtual void UnBind() const noexcept = 0;
};

struct Texture
{
    uint32_t width, height, depth,  sizet;
    enum class Type
    {
        T1D,
        T2D,
        T3D,
    } type;
    uint8_t* data;
    Texture() = default;
    ~Texture() = default;
    GBinder* binder;
    void Bind() 
    {
        binder->Bind();
    }
    void UnBind()
    {
        binder->UnBind();
    }
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
    std::unique_ptr<void> data;
    std::unique_ptr<GBinder> gBinder;
    void Bind() const noexcept
    {
        gBinder->Bind();
    }
    void UnBind() const noexcept
    {
        gBinder->UnBind();
    }
};

enum class ShaderType
{
    VERTEX, FRAGMENT
};



class Renderer
{
public:
    virtual void Draw(GBuffer* gBuffer) = 0;
    virtual void LoadBuffer(GBuffer* gBuffer) = 0;
    void SetLayout(VertexSpecification& specification);
    virtual void LoadTexture(Texture* texture) = 0;
    virtual void AddShader(ShaderType type, std::string& source) = 0;
    virtual void Uniform1f(const float& data, std::string name) = 0;
    virtual void Uniform2f(const Vect2& data, std::string name) = 0;
    virtual void Uniform3f(const Vect3& data, std::string name) = 0;
    virtual void Uniform4f(const Vect4& data, std::string name) = 0;
    virtual void UniformMat(const Mat& mat, std::string name) = 0;
    virtual void Clear() = 0;
    virtual void ClearColor(uint8_t r, uint8_t g, uint8_t b) = 0;
    virtual void ClearDepth(float depthLevel) = 0;
protected:
    VertexSpecification specification;
};
