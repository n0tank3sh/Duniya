#pragma once 

#include <cstdint>
#include <memory>
#include <vector>

using VertexSpecification = std::vector<std::pair<std::string, uint32_t>>;

struct Texture
{
    uint32_t width, height, depth,  sizet;
    enum class Type
    {
        T1D,
        T2D,
        T3D,
    } type;
    std::unique_ptr<uint8_t> data;
};

struct GBinder
{
    virtual void Bind() const noexcept = 0;
    virtual void UnBind() const noexcept = 0;
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
    virtual void Clear() = 0;
    virtual void ClearColor(uint8_t r, uint8_t g, uint8_t b) = 0;
    virtual void ClearDepth(float depthLevel) = 0;
protected:
    VertexSpecification specification;
};
