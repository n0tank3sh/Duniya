
#include <Graphics/Renderer.hpp>
#include <Exception.hpp>
#include <glad/glad.h>
#include <vector>

class GLException : public CException 
{
public:
    GLException(int line, const char* file, GLenum errorCode, std::string functionName);
    std::string CheckError() const  noexcept;
	const char* what() const noexcept override;
private:
    uint32_t errorCode;
    std::string functionName;
};

// All the classes for buffer handling
struct GLVertexBinder : public GBinder
{
    GLVertexBinder(uint32_t pvao, uint32_t rendererID = 0);
    void Bind() const noexcept override;
    void UnBind() const noexcept override;
	uint32_t pvao;
    uint32_t rendererID;
};

struct GLIndexBinder : public GBinder
{
    GLIndexBinder(uint32_t pvao, uint32_t rendererID = 0);
    void Bind() const noexcept override;
    void UnBind() const noexcept override;
	uint32_t pvao;
    uint32_t rendererID;
};

struct GLTextureBinder : public GBinder
{
    GLTextureBinder(uint32_t pvao, uint32_t rendererID = 0, GLenum target = GL_TEXTURE_2D);
    void Bind() const noexcept override;
    void UnBind() const noexcept override;
    uint32_t rendererID;
	uint32_t pvao;
    GLenum target;
};

class GLRenderer : public Renderer
{
public:
    GLRenderer();
	GLenum GetOption(Options option);
	GLenum GetUniformLocation(const uint32_t& shaderProgram, std::string& uniformName);
    void Draw(DrawPrimitive drawPrimitive, GBuffer* gBuffer) override;
	void DrawInstanced(DrawPrimitive drawPrimitive, GBuffer* gBuffer, uint32_t numInstanced) override;
	void DrawBuffer(DrawPrimitive drawPrimitive, GBuffer* gBuffer) override;
	void DrawArrays(DrawPrimitive drawPrimitive, GBuffer* gBuffer, uint32_t numElement) override;
	void DrawInstancedArrays(DrawPrimitive drawPrimitive, GBuffer* gBuffer, uint32_t numElement, uint32_t numInstanced) override;
    void LoadBuffer(GBuffer* gBuffer) override;
    void LoadTexture(Texture* texture, GBuffer* gbuffer) override;
	void Enable(Options option) override;
	void Disable(Options option) override;
    void Clear() override;
    void ClearColor(float r, float g, float b) override;
    void ClearDepth(float depthLevel) override;
	void Uniform1u(const uint32_t count, const uint32_t* data, std::string name) override;
	void Uniform1i(const uint32_t count, const int32_t* data, std::string name) override;
    void Uniform1f(const uint32_t count, const float* data, std::string name) override;
    void Uniform2f(const uint32_t count, const Vect2* data, std::string name) override;
    void Uniform3f(const uint32_t count, const Vect3* data, std::string name) override;
    void Uniform4f(const uint32_t count, const Vect4* data, std::string name) override;
    void UniformMat(const uint32_t count, const Mat* mat, std::string name) override;
	void UseShaderStage(ShaderStageHandler* shaderStagerHandler) override;
	void SetLayout(const uint32_t layout) override;
	void WireFrameMode(bool) override;
	uint32_t AddSpecification(VertexSpecification& vertexSpecification) override;
	NativeShaderHandlerParent* CreateShader(ShaderType type) override;
	ShaderStageHandler * CreateShaderStage() override;
private:
    static void LoadGladGL();
    void FinalizeVertexSpecification();
    void UseCurrentShaderProgram();
	GLenum GetDrawTarget(DrawPrimitive drawPrimitive);
private:
	static bool gladLoaded;
	uint32_t pvao;
private:
	std::vector<std::pair<uint32_t, std::vector<uint32_t>>> vaos;
public:
	uint32_t shaderProgram;
};

template<>
struct NativeShaderHandler<GLRenderer> : public NativeShaderHandlerParent
{
	uint32_t shader;
	NativeShaderHandler(ShaderType shaderType);
	~NativeShaderHandler();
	void Load() override;
	void UnLoad() override;
};

template<>
struct NativeShaderStageHandler<GLRenderer> : public ShaderStageHandler
{
	bool linked;
	uint32_t program;
	GLRenderer* renderer;
	NativeShaderStageHandler();
	~NativeShaderStageHandler();
	void Load() override;
	void UnLoad() override;
};


