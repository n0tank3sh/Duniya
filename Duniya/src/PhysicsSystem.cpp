#include <PhysicsSystem.hpp>

PhysicsSystem::PhysicsSystem() { collisionBoxes.resize(1000); }

void PhysicsSystem::LoadScene(Scene* scene) {
    for (auto& entities : scene->entities) {
	auto collisionBox3D =
	    entities->Get<CollisionBox3D>(ComponentTypes::COLLISIONBOX3D);
	if (collisionBox3D != nullptr) {
	}
    }
}

