
#include <Graphics/Renderer.h>
#include <Exception.h>
#include <glad/glad.h>

class GLException : public CException 
{
public:
    GLException(int line, const char* file, uint32_t errorCode, std::string functionName);
    virtual std::string GetOriginalString() const noexcept override;
    virtual std::string GetType() const noexcept override;
};

struct GLBuffer : public GBuffer
{
    uint32_t rendererID;
    uint32_t target;
};

// All the classes for buffer handling
struct GLVertexBinder : public GBinder
{
    virtual void Bind() const noexcept override;
    virtual void UnBind() const noexcept override;
    uint32_t rendererID;
};

struct GLIndexBinder : public GBinder
{
    virtual void Bind() const noexcept override;
    virtual void UnBind() const noexcept override;
    uint32_t rendererID;
};

struct GLTextureBinder : public GBinder
{
    virtual void Bind() const noexcept override;
    virtual void UnBind() const noexcept override;
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
