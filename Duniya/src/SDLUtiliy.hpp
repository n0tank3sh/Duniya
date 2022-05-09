#pragma once
#include <SDL2/SDL.h>

#include <ECS/ECS.hpp>

#include "SDL_events.h"

struct KeyboardEvent : public Message {
    SDL_KeyboardEvent event;
};

struct MouseButtonEvent : public Message {
    SDL_MouseButtonEvent event;
};

struct MouseMotionEvent : public Message {
    SDL_MouseMotionEvent event;
};

#ifdef __unix__
#include <signal.h>
#define BREAKPOINT raise(SIGTRAP);
#endif

