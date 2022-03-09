#include "TestGame.h"
#include "AssetLoader.h"
#include "ECS/CommonComponent.h"
#include "ECS/ECS.h"
#include "ECS/GraphicsComponent.h"
#include "SDL_keycode.h"
#include <SDLUtiliy.h>
#include <Renderer2DSystem.h>
#include <cstdlib>
#include <iostream>


void TestGame::LoadScene(Scene* scene)
{
	//uint32_t entity = scene->PushDef();
	//auto panel = scene->GetEntity(entity)->Emplace<Panel>(ComponentTypes::PANEL);
	//panel->dimension = Vect4(0.5, 0.5, 0.5, 0.5);
	//panel->color = Vect4(0.5, 1, 0.0, 0.5);
	//panel->sideDist = .0f;
	//messagingSystem->at(0x35).push_back(std::make_pair(0, nullptr));

	uint32_t cube = scene->Push();
	AssetLoader::GetSingleton()->scene = scene;
	auto mesh = scene->GetEntity(cube)->Emplace<Mesh>(ComponentTypes::MESH);
	AssetLoader::GetSingleton()->LoadObj("Resource/Test/cube1.obj", mesh);
	auto cubeTransform = scene->GetEntity(cube)->Emplace<Transform>(ComponentTypes::TRANSFORM);
	cubeTransform->pos = Vect3();
	cubeTransform->rotation = Vect3();
	cubeTransform->scale = Vect3(0.5f, 0.5f, 0.5f);
	for(auto& entity: scene->entities)
	{
		if(entity->Get(ComponentTypes::CAMERA) != nullptr)
		{
			player = entity->Get<Transform>(ComponentTypes::TRANSFORM);
			camera = entity->Get<Camera>(ComponentTypes::CAMERA);
		}
	}
}

void TestGame::Update(float deltaTime)
{
	auto& messages = messagingSystem->at(0x0);
	while(!messages.empty())
	{
		auto message = static_cast<KeyboardEvent*>(messages.front().second.get());
		switch(message->event.keysym.sym)
		{
			case SDLK_w:
				player->pos.z += .5f;
				break;
			case SDLK_a:
				player->pos.x -= .5f;
				break;
			case SDLK_d:
				player->pos.x += .5f;
				break;
			case SDLK_s:
				player->pos.z -= .5f;
				break;
			case SDLK_q:
				player->pos.y += .5f;
				break;
			case SDLK_e:
				player->pos.y -= .5f;
				break;
			case SDLK_UP:
				player->rotation.y += .05f;
				break;
			case SDLK_DOWN:
				player->rotation.y -= .05f;
				break;
			case SDLK_LEFT:
				player->rotation.x -= .05f;
				break;
			case SDLK_RIGHT:
				player->rotation.x += .05f;
				break;
		};
		messages.pop_front();
	}
}
