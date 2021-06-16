#pragma once

#include <cstdint>
#include <string>
#include <SDL2/SDL.h>

enum class Graphics_API
{
    OPENGL
};

class Application
{
public:
    Application();
    uint32_t run();
private:
    SDL_Window* window;
    uint32_t height;
    uint32_t width;
    std::string title;
    Graphics_API graphicsAPI;
    void* deviceContext;
};
