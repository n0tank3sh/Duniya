#include <ECS/ECS.h>
#include <ECS/CommonComponent.h>


struct TestGame : public System
{
	TestGame() :
		player(nullptr)
	{}
	void LoadScene(Scene* scene) override;
	void update(float deltaTime) override;
	Transform* player;
};
