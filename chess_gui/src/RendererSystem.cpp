#include "RendererSystem.h"
#include "Graphics/opengl/GLRenderer.h"
#include <Graphics/GraphicsComponent.h>
#include <commoncomponent.h>

RendererSystem* RendererSystem::singleton = nullptr;

RendererSystem::RendererSystem()
{}

RendererSystem* RendererSystem::init(Graphics_API graphicsAPI)
{
    singleton = new RendererSystem();
    singleton->renderer.reset(new GLRenderer());
    return singleton;
}

RendererSystem* RendererSystem::GetSingleton()
{
    return singleton;
}

void RendererSystem::LoadScene(Scene* scene)
{
    this->scene = scene;
    Mesh* mesh;
    for(auto& i: scene->entities)
    {
        mesh = i.second->get<Mesh>();
        renderer->LoadBuffer(mesh->vertexBuffer);
        renderer->LoadBuffer(mesh->indexBuffer);
        renderer->LoadTexture(mesh->texture);
    }
}

void RendererSystem::update(float deltaTime)
{
    Mesh* mesh;
    for(auto& i: scene->entities)
    {
        mesh = i.second->get<Mesh>();
        mesh->vertexBuffer->Bind();
        mesh->indexBuffer->Bind();
        mesh->texture->Bind();
        Mat* mat = ConvertTranforToMatrix(*(i.second->get<Transform>()));
        renderer->UniformMat(*mat, "MVP");
        renderer->Draw(mesh->indexBuffer);
    }
}
