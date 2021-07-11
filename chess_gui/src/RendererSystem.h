#include <ecs/ecs.h>
#include <Graphics/Renderer.h>
#include <Application.h>

struct RendererStuff
{
	GBuffer* iBuffer;
	GBuffer* vBuffer;
	GBuffer* texture;
};

class RendererSystem : public System
{
private:
    RendererSystem();
public:
    static RendererSystem* init(Graphics_API graphicsAPI);
    static RendererSystem* GetSingleton();
	void CreateGBufferMesh(Mesh* mesh, GBuffer* iBuffer, GBuffer* vBuffer);
    ~RendererSystem();
    void LoadScene(Scene* scene) override;
    void update(float deltaTime) override;
private:
    std::unique_ptr<Renderer> renderer;
    Scene* scene;
    static RendererSystem* singleton;
	float animated;
};
