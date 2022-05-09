#include <ECS/CommonComponent.hpp>
#include <ECS/ECS.hpp>
#include <ECS/GraphicsComponent.hpp>
#include <memory>

struct ShadpeGenerator {
    Scene* scene;
    Scene* GetScene();
    Mesh GenerateCube();
    Mesh GenerateSphere(uint32_t divisorX = 4, uint32_t divisorY = 1);
    Mesh GeneratePlane(uint32_t divisorX, uint32_t divisorY);
};
