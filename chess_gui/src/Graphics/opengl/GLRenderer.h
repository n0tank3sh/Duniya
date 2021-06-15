
#include <Graphics/Renderer.h>
#include <glad/glad.h>

class GLRenderer : public Renderer
{
public:
    GLRenderer(void* GlProcAddressLoader);
    void Draw(uint32_t count) override;
    GBuffer* CreateBuffer(const void * data, uint32_t sizet) override;
    void LoadTexture(Texture* texture) override;
    void AddShader(std::string& source) override;
    void Clear() override;
    void ClearColor(uint8_t r, uint8_t g, uint8_t b) override;
    void ClearDepth(float depthLevel) override;
};
