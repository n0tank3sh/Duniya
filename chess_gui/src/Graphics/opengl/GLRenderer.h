
#include <Graphics/Renderer.h>
#include <Exception.h>
#include <glad/glad.h>

class GLException : public CException 
{
public:
    GLException(int line, const char* file, GLenum errorCode, std::string functionName);
    std::string GetOriginalString() noexcept override;
    std::string GetType() const noexcept override;
private:
    uint32_t errorCode;
    std::string functionName;
};

// All the classes for buffer handling
struct GLVertexBinder : public GBinder
{
    GLVertexBinder() = default;
    GLVertexBinder(uint32_t rendererID);
    void Bind() const noexcept override;
    void UnBind() const noexcept override;
    uint32_t rendererID;
};

struct GLIndexBinder : public GBinder
{
    GLIndexBinder() = default;
    GLIndexBinder(uint32_t rendererID);
    void Bind() const noexcept override;
    void UnBind() const noexcept override;
    uint32_t rendererID;
};

struct GLTextureBinder : public GBinder
{
    GLTextureBinder() = default;
    GLTextureBinder(uint32_t rendererID);
    GLTextureBinder(uint32_t rendererID, GLenum target);
    void Bind() const noexcept override;
    void UnBind() const noexcept override;
    uint32_t rendererID;
    GLenum target;
};

class GLRenderer : public Renderer
{
public:
    GLRenderer();
    void Draw(GBuffer* gBuffer) override;
    void LoadBuffer(GBuffer* gBuffer) override;
    void LoadTexture(Texture* texture) override;
    void AddShader(ShaderType type, std::string& source) override;
    void Clear() override;
    void ClearColor(uint8_t r, uint8_t g, uint8_t b) override;
    void ClearDepth(float depthLevel) override;
    void Uniform1f(const float& data, std::string name) override;
    void Uniform2f(const Vect2& data, std::string name) override;
    void Uniform3f(const Vect3& data, std::string name) override;
    void Uniform4f(const Vect4& data, std::string name) override;
    void UniformMat(const Mat& mat, std::string name) override;
private:
    void LoadGladGL();
    void FinalizeVertexSpecification();
    void UseCurrentShaderProgram();
private:
    struct ShaderProgram
    {
        bool Linked;
        uint32_t id;
    } shaderProgram;
    std::vector<uint32_t> shaders;
};
