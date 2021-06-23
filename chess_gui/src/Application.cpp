
#include "Application.h"
#include <Graphics/Renderer.h>
#include "Exception.h"
#include <commoncomponent.h>
#include <Graphics/GraphicsComponent.h>
#include <RendererSystem.h>
#include "AssetLoader.h"

Application::Application()
{
    height = 600;
    width = 1000;
    title = "Hello Darkness my old friend";
    Graphics_API gpi = Graphics_API::OPENGL;
    uint32_t windowFlag =  SDL_WINDOW_OPENGL;
    window = SDL_CreateWindow(title.c_str(), 0, 0, width, height, windowFlag);
    deviceContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, deviceContext);    
    RendererSystem::init(gpi);
    scene = new Scene();
    scene->RegisterComponent<Transform>();
    scene->RegisterComponent<Mesh>();
    Scene::IComponentArray* ica = scene->GetEntity(scene->Push());
    AssetLoader::init();
    Mesh* mesh = ica->get<Mesh>();
    Transform* transform = ica->get<Transform>();
    if(mesh == nullptr) std::cout << "Error found in Mesh" << std::endl;
    AssetLoader::GetSingleton()->LoadObj("Resource/Test/Goat.obj", mesh);
    CreateGBufferMesh(mesh);
    mesh->texture = new Texture();
    AssetLoader::GetSingleton()->LoadTextureFile("Resource/Test/Tex.png", mesh->texture);
    if(mesh->texture == nullptr) std::cout << "Texture is nullptr after Load Texture " << std::endl;
    transform->pos = Vect3();
    transform->rotation = Vect3();
    transform->scale = Vect3(1.f, 1.f, 1.f);
    std::cout  << scene->entities.size() << std::endl;
    RendererSystem::GetSingleton()->LoadScene(scene);
}

uint32_t Application::run()
{
    SDL_Event event;
    bool quit = false;
    while(!quit)
    {
        RendererSystem::GetSingleton()->update(0.0f);
        SDL_GL_SwapWindow(window);
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                quit = true;
            }
        }
    }
    return 0;
}
