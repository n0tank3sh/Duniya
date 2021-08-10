#include <ecs/ecs.h>
#include <Graphics/Renderer.h>
#include <Application.h>
#include <ecs/commoncomponent.h>
#include <ecs/GraphicsComponent.h>
#include <ecs/ecs.h>

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
	Mat transformDefault;
	uint32_t mainCamera;
	Scene::IComponentArray* camera;
	std::vector<uint32_t> lights;
private:
	Mat SetUpCamera(uint32_t entity, Vect3& pos, const Vect3& temp );
	void LoadMaterial(Scene::Entities::iterator& itr);
	void LoadLights();
	void LoadTransform(Scene::Entities::iterator& itr);
	void lookAt(Vect3& from, Vect3& forward, Vect3& up, Vect3& right, Mat& mat);
};
