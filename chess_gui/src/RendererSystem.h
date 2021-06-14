#include "ecs.h"
#include "Graphics/Renderer.h"

enum class Graphics_API
{
    DIRECTX_9,
    DIRECTX_10,
    DIRECTX_11,
    OPENGL,
};
class RendererSystem : public System
{
    RendererSystem(Graphics_API graphicsAPI);
    ~RendererSystem();
    virtual void LoadScene(Scene* scene);
    virtual void update(float deltaTime);
private:
    Renderer* renderer;

};
