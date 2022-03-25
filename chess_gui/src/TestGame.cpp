#include "TestGame.hpp"
#include "AssetLoader.hpp"
#include "ECS/CommonComponent.hpp"
#include "ECS/ECS.hpp"
#include "ECS/GraphicsComponent.hpp"
#include <SDLUtiliy.hpp>
#include <Renderer2DSystem.hpp>
#include <cstdlib>
#include <iostream>
#include <SDL_keycode.h>


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
	//mesh->drawPrimitive = DrawPrimitive::LINES;
	auto cubeTransform = scene->GetEntity(cube)->Emplace<Transform>(ComponentTypes::TRANSFORM);
	cubeTransform->pos = Vect3();
	cubeTransform->rotation = Vect3();
	cubeTransform->scale = Vect3(1.f, 1.f, 1.f);
	for(auto& entity: scene->entities)
	{
		if(entity->Get(ComponentTypes::CAMERA) != nullptr)
		{
			player = entity->Get<Transform>(ComponentTypes::TRANSFORM);
			camera = entity->Get<Camera>(ComponentTypes::CAMERA);
		}
	}
	uint32_t texPanel = scene->Push();
	auto text = scene->GetEntity(texPanel)->Emplace<std::string>(ComponentTypes::TEXTBOX);
	(*text) = "Hellow";
	auto hell = scene->GetEntity(texPanel)->Emplace<TextPanel>(ComponentTypes::TEXTPANEL); 
	hell->dimension.z = settings->NormalizeX(46);
	hell->dimension.w = settings->NormalizeY(32);
	messagingSystem->at(0x35).push_back(std::make_pair(0, nullptr));
}

void TestGame::Update(float deltaTime)
{
	auto& messages = messagingSystem->at(0x0);
	auto acceleration = Vect3();
	while(!messages.empty())
	{
		auto message = static_cast<KeyboardEvent*>(messages.front().second.get());
		switch(message->event.keysym.sym)
		{
			case SDLK_w:
				acceleration.z -= .5f;
				break;
			case SDLK_a:
				acceleration.x += .5f;
				break;
			case SDLK_d:
				acceleration.x -= .5f;
				break;
			case SDLK_s:
				acceleration.z += .5f;
				break;
			case SDLK_q:
				acceleration.y -= .5f;
				break;
			case SDLK_e:
				acceleration.y += .5f;
				break;
			case SDLK_UP:
				player->rotation.x -= .05f;
				break;
			case SDLK_DOWN:
				player->rotation.x = .05f;
				break;
			case SDLK_LEFT:
				player->rotation.y += .05f;
				break;
			case SDLK_RIGHT:
				player->rotation.y -= .05f;
				break;
		};
		messages.pop_front();
	}
	player->pos += acceleration * deltaTime * 10;
}
