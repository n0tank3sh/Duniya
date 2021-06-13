#pragma once 

#include <cstdint>
#include <memory>

struct Texture
{
    uint32_t width, height, sizet;
    std::unique_ptr<uint8_t> data;
};

struct GBuffer
{
    virtual void Bind();
    virtual void UnBind();
};
class Renderer
{
public:
    virtual void Draw(uint32_t count) = 0;
    virtual GBuffer* CreateBuffer(const void * data, uint32_t sizet) = 0;
    virtual void LoadTexture(Texture* texture) = 0;
    virtual void AddShader(std::string& source);
    virtual void Clear() = 0;
    virtual void ClearColor(uint8_t r, uint8_t g, uint8_t b);
    virtual void ClearDepth(float depthLevel);
};
