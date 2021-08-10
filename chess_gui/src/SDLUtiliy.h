#pragma once
#include <SDL2/SDL.h>
#include <ecs/ecs.h>


struct KeyboardEvent : public Message
{
	SDL_KeyboardEvent event; 
};

struct MouseEvent : public Message
{
	SDL_MouseButtonEvent buttonEvent;
	SDL_MouseMotionEvent motionEvent;
};

