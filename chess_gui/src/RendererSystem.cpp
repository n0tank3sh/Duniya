#include "RendererSystem.h"
#include "Graphics/opengl/GLRenderer.h"

RendererSystem* RendererSystem::singleton = nullptr;

RendererSystem::RendererSystem()
{}

RendererSystem* RendererSystem::init(Graphics_API graphicsAPI)
{
    singleton = new RendererSystem();
    singleton->renderer.reset(renderer);
    return singleton;
}

RendererSystem* RendererSystem::GetSingleton()
{
    return singleton;
}

void RendererSystem::LoadScene(Scene* scene)
{
}

void RendererSystem::update(float deltaTime)
{
}
