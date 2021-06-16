#define SDL_MAIN_HANDLED
#include <Application.h>
#include <SDL2/SDL.h>

int main(int argc, const char* argv[])
{
    uint32_t sdlInitFlag = SDL_INIT_VIDEO;
    SDL_Init(sdlInitFlag);
    uint32_t runFlag = 0;
    SDL_SetMainReady();
    runFlag = Application().run();
    SDL_Quit();
    return runFlag;
}
