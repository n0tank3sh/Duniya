#include <ecs/GraphicsComponent.h>
#include <ecs/ecs.h>
#include "Graphics/Renderer.h"
#include "Application.h"

class RendererSystem : public System
{
private:
    RendererSystem();
public:
    static RendererSystem* init(Graphics_API graphicsAPI);
    static RendererSystem* GetSingleton();
	void CreateGBufferMesh(Mesh* mesh);
    ~RendererSystem();
    void LoadScene(Scene* scene) override;
    void update(float deltaTime) override;
private:
    std::unique_ptr<Renderer> renderer;
    Scene* scene;
    static RendererSystem* singleton;
	float animated;
};
