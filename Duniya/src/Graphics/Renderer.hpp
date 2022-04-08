#pragma once 

#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>
#include <Math/Mat.hpp>
#include <ECS/ECS.hpp>
#include <ECS/GraphicsComponent.hpp>

using VertexSpecification = std::vector<std::pair<std::string, uint32_t>>;


struct GBinder
{
    virtual void Bind() const noexcept  = 0;
    virtual void UnBind() const noexcept  = 0;
};


enum class Options
{
	BLEND,
	DEPTH_TEST,
	FACE_CULL,
	WIREFRAME_MODE
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
    uint32_t  data;
	uint32_t bindNo;
};

enum class ShaderType
{
    VERTEX, FRAGMENT
};


struct NativeShaderHandlerParent
{
	ShaderType type;
	std::string source;
	virtual ~NativeShaderHandlerParent(){}
	virtual void Load() = 0;
	virtual void UnLoad() = 0;
}; template<typename T> struct NativeShaderHandler : public NativeShaderHandlerParent
{
};
struct ShaderStageHandler
{
	std::vector<std::unique_ptr<NativeShaderHandlerParent>> shaderHandler;
	virtual ~ShaderStageHandler(){}
	virtual void Load() = 0;
	virtual void UnLoad() = 0;
};

template<typename t>
struct NativeShaderStageHandler : ShaderStageHandler
{
};



class Renderer
{
protected:
	std::vector<std::unique_ptr<GBinder>> binders;
	ResourceBank* resourceBank;
public:
	//Override function
	void SetResourceBank(ResourceBank* resourceBank);
	void Bind(GBuffer& buffer);
	void UnBind(GBuffer& buffer);
    virtual void Draw(DrawPrimitive drawPrimitive, GBuffer* gBuffer) = 0;
	virtual void DrawInstanced(DrawPrimitive drawPrimitive, GBuffer* gBuffer, uint32_t numInstanced) = 0;
	virtual void DrawBuffer(DrawPrimitive drawPrimitive, GBuffer* gBuffer) = 0;
	virtual void DrawArrays(DrawPrimitive drawPrimitive, GBuffer* gBuffer, uint32_t numElement) = 0;
	virtual void DrawInstancedArrays(DrawPrimitive drawPrimitive, GBuffer* gBuffer, uint32_t numElement, uint32_t numInstanced) = 0;
    virtual void LoadBuffer(GBuffer* gBuffer) = 0;
    virtual void LoadTexture(Texture* texture, GBuffer* gBuffer) = 0;
	virtual void Enable(Options option) = 0;
	virtual void Disable(Options option) = 0;
    virtual void Uniform1f(const uint32_t count, const float* data, std::string name) = 0;
    virtual void Uniform2f(const uint32_t count, const Vect2* data, std::string name) = 0;
    virtual void Uniform3f(const uint32_t count, const Vect3* data, std::string name) = 0;
	virtual void Uniform1u(const uint32_t count, const uint32_t* data, std::string name) = 0;
	virtual void Uniform1i(const uint32_t count, const int32_t* data, std::string name) = 0;
    virtual void Uniform4f(const uint32_t count, const Vect4* data, std::string name) = 0;
    virtual void UniformMat(const uint32_t count, const Mat* mat, std::string name) = 0;
	virtual void UseShaderStage(ShaderStageHandler* shaderStageHandler) = 0;
	virtual void SetLayout(const uint32_t layout) = 0;
    virtual void Clear() = 0;
    virtual void ClearColor(float r, float g, float b) = 0;
    virtual void ClearDepth(float depthLevel) = 0;
	virtual void WireFrameMode(bool) = 0;
	virtual NativeShaderHandlerParent* CreateShader(ShaderType type) = 0;
	virtual ShaderStageHandler* CreateShaderStage() = 0;
	virtual uint32_t AddSpecification(VertexSpecification& vertexSpecification) = 0;
};
