#pragma once 

#include <cstdint>
#include <memory>
#include <vector>

using VertexSpecification = std::vector<std::string, uint32_t>;

struct Texture
{
    uint32_t width, height, sizet;
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



class Renderer
{
public:
    virtual void Draw(uint32_t count) = 0;
    virtual void LoadBuffer(GBuffer* gBuffer) = 0;
    virtual void SetLayout(VertexSpecification& specification) = 0;
    virtual void LoadTexture(Texture* texture) = 0;
    virtual void AddShader(std::string& source);
    virtual void Clear() = 0;
    virtual void ClearColor(uint8_t r, uint8_t g, uint8_t b) = 0;
    virtual void ClearDepth(float depthLevel) = 0;
};
