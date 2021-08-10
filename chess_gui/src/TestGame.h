#include <ecs/ecs.h>
#include <ecs/commoncomponent.h>


struct TestGame : public System
{
	TestGame() :
		player(nullptr)
	{}
	void LoadScene(Scene* scene) override;
	void update(float deltaTime) override;
	Transform* player;
};
