
#include "Application.h"
#include <Graphics/Renderer.h>
#include <Graphics/opengl/GLRenderer.h>

Application::Application()
{
    Graphics_API gpi = Graphics_API::OPENGL;
    Renderer* renderer;
    uint32_t windowFlag;
    switch(gpi)
    {
        case Graphics_API::OPENGL:
            windowFlag = SDL_WINDOW_OPENGL;
    };
    window = SDL_CreateWindow(title.c_str(), 0, 0, width, height, 
}

uint32_t Application::run()
{
    return 0;
}
