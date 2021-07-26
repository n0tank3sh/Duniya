
#include "Application.h"
#include <Graphics/Renderer.h>
#include "Exception.h"
#include <ecs/commoncomponent.h>
#include <ecs/GraphicsComponent.h>
#include <RendererSystem.h>
#include "AssetLoader.h"


Application::Application()
{
    height = 600;
    width = 1000;
    title = "Hello Darkness my old friend";

    Graphics_API gpi = Graphics_API::OPENGL;
    uint32_t windowFlag =  SDL_WINDOW_OPENGL;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    window = SDL_CreateWindow(title.c_str(), 0, 0, width, height, windowFlag);
	if(window == NULL) printf("Can't create window reason: %s", SDL_GetError());
	

    deviceContext = SDL_GL_CreateContext(window);
	if(deviceContext == NULL) printf("%s", SDL_GetError());
	RendererSystem::init(gpi);
	

//	const GLubyte* version;
//	version = glGetString(GL_VERSION);
//	std::cout << "The version of opengl is : " << version << std::endl;
//	glClearColor(.0f, .5f, 0.25f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT);
//	SDL_GL_SwapWindow(window);

	scene = new Scene();
//	scene->RegisterComponent<Transform>();
//	scene->RegisterComponent<Mesh>();
//	Scene::IComponentArray* ica = scene->GetEntity(scene->Push());
//	
//	AssetLoader::init();
//	Mesh* mesh = (Mesh*)ica->get<ComponentType::MESH>();
//	Transform* transform = (Transform*)ica->get<ComponentType::TRANSFORM>();
//	if(mesh == nullptr) std::cout << "Error found in Mesh" << std::endl;
//	AssetLoader::GetSingleton()->LoadObj("Resource/Test/Goat.obj", mesh);
//	mesh->texture = new Texture();
//	AssetLoader::GetSingleton()->LoadTextureFile("Resource/Test/Tex.png", mesh->texture);
//	if(mesh->texture == nullptr) std::cout << "Texture is nullptr after Load Texture " << std::endl;
//	
//	transform->pos = Vect3();
//	transform->rotation = Vect3();
//	transform->scale = Vect3(.25f, .25f, .25f);
//	std::cout  << scene->entities.size() << std::endl;
//	scene->SaveScene("justChecking.dick");
	scene->LoadScene("justChecking.dick");

	RendererSystem::GetSingleton()->LoadScene(scene);
}

uint32_t Application::run()
{
    SDL_Event event;
    bool quit = false;
	float lastTime = 0, currentTime;
    while(!quit)
    {
		currentTime = SDL_GetTicks();
        RendererSystem::GetSingleton()->update(currentTime - lastTime);
        SDL_GL_SwapWindow(window);
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                quit = true;
            }
        }
		lastTime = currentTime;
    }
	SDL_GL_DeleteContext(deviceContext);
	SDL_DestroyWindow(window);
    return 0;
}
