#include <ECS/ECS.h>
#include <ECS/CommonComponent.h>


struct TestGame : public System
{
	TestGame() :
		player(nullptr)
	{
		messageID = 0x25;
	}
	void LoadScene(Scene* scene) override;
	void update(float deltaTime) override;
	Transform* player;
};
