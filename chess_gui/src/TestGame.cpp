#include "TestGame.h"
#include <SDLUtiliy.h>
#include <Renderer2DSystem.h>
#include <cstdlib>
#include <iostream>


void TestGame::LoadScene(Scene* scene)
{
	uint32_t entity = scene->PushDef();
	auto panel = scene->GetEntity(entity)->Emplace<Panel>(ComponentTypes::PANEL);
	panel->dimension = Vect4(0.5, 0.5, 0.5, 0.5);
	panel->color = Vect4(0.5, 1, 0.0, 0.5);
	panel->sideDist = .0f;
	messagingSystem->at(0x35).push_back(std::make_pair(0, nullptr));
}

void TestGame::Update(float deltaTime)
{
	//auto& messages = (*messagingSystem)[EVENTS::KEYBOARD_EVENTS];
	//while(!messages.empty())
	//{
	//	auto message = static_cast<KeyboardEvent*>(messages.front().second.get());
	//	switch(message->event.keysym.sym)
	//	{
	//		case SDLK_w:
	//			player->pos.z += .05f * deltaTime;
	//			break;
	//		case SDLK_a:
	//			player->pos.x -= .05f * deltaTime;
	//			break;
	//		case SDLK_d:
	//			player->pos.x += .05f * deltaTime;
	//			break;
	//		case SDLK_s:
	//			player->pos.z -= .05f * deltaTime;
	//			break;
	//	};
	//	messages.pop_front();
	//}
}
