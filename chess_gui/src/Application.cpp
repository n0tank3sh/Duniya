#include "Application.hpp"
#include <Graphics/Renderer.hpp>
#include "ECS/ECS.hpp"
#include "Exception.hpp"
#include <cstdint>
#include <ECS/CommonComponent.hpp>
#include <ECS/GraphicsComponent.hpp>
#include <RendererSystem.hpp>
#include "AssetLoader.hpp"
#include "SDLUtiliy.hpp"
#include "SDL_events.h"
#include "TestGame.hpp"
#include <filesystem>
#include <iterator>
#include <string>
#include <Renderer2DSystem.hpp>

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
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    window = SDL_CreateWindow(title.c_str(), 0, 0, width, height, windowFlag);
	{
		int32_t width, height;
		SDL_GetWindowSize(window, &width, &height);
		std::cout << "Width: " << width << " height: " << height << std::endl;
	}
	if(window == NULL) throw CException(__LINE__, __FILE__, "Window Error", SDL_GetError());
	

    deviceContext = SDL_GL_CreateContext(window);
	if(deviceContext == NULL) throw CException(__LINE__, __FILE__, "GL Device Context", SDL_GetError());
	
	scene = new Scene();
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
	manager->settings->resolution = Vect2(width, height);
	manager->AddQueryMessageBlock(0x0);
	manager->Add(RendererSystem::init(Graphics_API::OPENGL));
	manager->Add(Renderer2DSystem::Init());
	manager->Add(new TestGame);
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
		KeyboardEvent* keyboardEvent;
		MouseButtonEvent* mouseButtonEvent;
		MouseMotionEvent* mouseMotionEvent;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_QUIT:
			    quit = true;
				break;
			case SDL_KEYDOWN:
				keyboardEvent = new KeyboardEvent;
				keyboardEvent->event = event.key;
				(*manager->queryMessages)[0x0].push_back(std::make_pair(EVENTS::KEYBOARD_EVENTS, std::unique_ptr<Message>(keyboardEvent)));
				break;
			case SDL_MOUSEBUTTONDOWN: 
				mouseButtonEvent = new MouseButtonEvent;
				mouseButtonEvent->event = event.button;
				(*manager->queryMessages)[0x1].push_back(std::make_pair(EVENTS::MOUSE_EVENTS, std::unique_ptr<Message>(mouseButtonEvent))); 
				break;
			case SDL_MOUSEMOTION:
				mouseMotionEvent = new MouseMotionEvent;
				mouseMotionEvent->event = event.motion;
				(*manager->queryMessages)[0x1].push_back(std::make_pair(EVENTS::MOUSEMOTION_EVENT, std::unique_ptr<Message>(mouseMotionEvent)));
				break;
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
