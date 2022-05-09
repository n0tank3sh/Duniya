#include "SDL_error.h"
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include <Application.hpp>

#include "Exception.hpp"

int main(int argc, const char* argv[]) {
    SDL_SetMainReady();
    uint32_t sdlInitFlag = SDL_INIT_VIDEO | SDL_INIT_EVENTS;
    if (SDL_Init(sdlInitFlag) < 0) {
	printf("Can't init sdl reason: \n%s", SDL_GetError());
    }
    uint32_t runFlag = 0;
    try {
	runFlag = Application().run();
    } catch (CException& e) {
	if (!SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, e.GetType().c_str(),
				      e.what(), NULL)) {
	    std::cout << SDL_GetError() << std::endl;
	}
	// std::cout << e.GetType() << std::endl
	//<< e.what() << std::endl;
    }
    SDL_Quit();
    return runFlag;
}
