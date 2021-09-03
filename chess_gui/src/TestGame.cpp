#include "TestGame.h"
#include <SDLUtiliy.h>
#include <iostream>


void TestGame::LoadScene(Scene* scene)
{
	for(auto itr = scene->entities.begin(); itr != scene->entities.end(); itr++)
	{
		if(itr->second->components.find(ComponentTypes::CAMERA) != itr->second->components.end())
		{
			player = (Transform*)itr->second->get<ComponentTypes::TRANSFORM>();
		}
	}
}

void TestGame::update(float deltaTime)
{
	auto& messages = (*messagingSystem.lock())[EVENTS::KEYBOARD_EVENTS];
	while(!messages.empty())
	{
		auto message = static_cast<KeyboardEvent*>(messages.front().get());
		switch(message->event.keysym.sym)
		{
			case SDLK_w:
				player->pos.z += .05f * deltaTime;
				break;
			case SDLK_a:
				player->pos.x -= .05f * deltaTime;
				break;
			case SDLK_d:
				player->pos.x += .05f * deltaTime;
				break;
			case SDLK_s:
				player->pos.z -= .05f * deltaTime;
				break;
		};
		messages.pop();
	}
}
