#include "Application.h"
#include <Graphics/Renderer.h>
#include "Exception.h"
#include <cstdint>
#include <ECS/CommonComponent.h>
#include <ECS/GraphicsComponent.h>
#include <RendererSystem.h>
#include "AssetLoader.h"
#include "TestGame.h"
#include <filesystem>
#include <iterator>
#include <string>
#include <Renderer2DSystem.h>

namespace ComponentTypes
{
	constexpr uint32_t MAINTYPE = 0;
};

struct MainType
{
};


Application::Application()
{
    height = 900;
    width = 1400;
    title = "Hello Darkness my old friend";

    Graphics_API gpi = Graphics_API::OPENGL;
    uint32_t windowFlag =  SDL_WINDOW_OPENGL;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    window = SDL_CreateWindow(title.c_str(), 0, 0, width, height, windowFlag);
	if(window == NULL) throw CException(__LINE__, __FILE__, "Window Error", SDL_GetError());
	

    deviceContext = SDL_GL_CreateContext(window);
	if(deviceContext == NULL) throw CException(__LINE__, __FILE__, "GL Device Context", SDL_GetError());
	
	scene = new Scene();
	scene->LoadScene("justChecking.dick");
	char* c = getenv(CHESS_GUI_PATH);
	std::string paths;
	if(c != NULL) paths = c;
	std::string directory;
	int i = 0; 
	while(i < paths.size())
	{
		auto itr = paths.find(";", i);
		directory = paths.substr(i, itr);
		i = itr + i; 
	}

	manager = new SystemManager;
	manager->Add(RendererSystem::init(Graphics_API::OPENGL));
	manager->Add(Renderer2DSystem::init());
	manager->LoadScene(scene);
}

uint32_t Application::run()
{
	SDL_Event event;
	bool quit = false;
	float lastTime = 0, currentTime;
	while(!quit)
	{
		currentTime = SDL_GetTicks();
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
			{
			    quit = true;
			}
			else if(event.type == SDL_KEYDOWN)
			{
				auto keyboardEvent = new KeyboardEvent;
				keyboardEvent->event = event.key;
				(*manager->queryMessages)[EVENTS::KEYBOARD_EVENTS].push(std::unique_ptr<Message>(static_cast<Message*>(keyboardEvent)));
			}
		}
		manager->update(1/(currentTime - lastTime));
		SDL_GL_SwapWindow(window);
		lastTime = currentTime;
	}
	SDL_GL_DeleteContext(deviceContext);
	SDL_DestroyWindow(window);
	return 0;
}
