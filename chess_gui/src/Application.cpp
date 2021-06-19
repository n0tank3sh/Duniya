
#include "Application.h"
#include <Graphics/Renderer.h>
#include <Graphics/opengl/GLRenderer.h>
#include <RendererSystem.h>

Application::Application()
{
    Graphics_API gpi = Graphics_API::OPENGL;
    Renderer* renderer;
    uint32_t windowFlag;
    switch(gpi)
    {
        case Graphics_API::OPENGL:
            windowFlag = SDL_WINDOW_OPENGL;
            deviceContext = SDL_GL_CreateContext(window);
            break;
    };
    RendererSystem::init(gpi);
    window = SDL_CreateWindow(title.c_str(), 0, 0, width, height, windowFlag);
}

uint32_t Application::run()
{
    while(true)
    {
        RendererSystem::GetSingleton()->update(0.0f);
        SDL_GL_SwapWindow(window);
    }
    return 0;
}
