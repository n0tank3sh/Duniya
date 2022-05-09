#include <ECS/CommonComponent.hpp>
#include <ECS/ECS.hpp>

struct TestGame : public System {
    TestGame() : player(nullptr) { messageID = 0x25; }
    void LoadScene(Scene* scene) override;
    void Update(float deltaTime) override;
    Transform* player;
    Camera* camera;
    Scene* scene;
    uint32_t textPanel;
};
