#pragma once
#include <ECS/ECS.hpp>
#include <vector>

struct RigidBody {
    float mass;
};

struct CollisionBox2D {
    float x, y, length, width;
};

struct CollisionBox3D {
    float x, y, z, length, width, height;
};

namespace ComponentTypes {
enum : uint32_t { RIGIDBODY = 93, COLLISIONBOX2D, COLLISIONBOX3D };
};

class PhysicsSystem : public System {
   public:
    PhysicsSystem();
    ~PhysicsSystem() = default;
    void LoadScene(Scene* scene) override;
    void Update(float deltaTime) override;

   private:
    std::vector<std::list<Scene::Entities>> collisionBoxes;
};
