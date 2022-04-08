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
	this->scene = scene;
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
	textPanel = scene->Push();
	auto text = scene->GetEntity(textPanel)->Emplace<Text>(ComponentTypes::TEXTBOX);
	text->str = "Hellow";
	text->color = Vect3(1.f, 1.f, 0.f);
	text->scale = 16;
	auto hell = scene->GetEntity(textPanel)->Emplace<TextPanel>(ComponentTypes::TEXTPANEL); 
	hell->dimension.x = -1.f;
	hell->dimension.y = 1.f;
	hell->dimension.z = settings->NormalizeX(200);
	hell->dimension.w = settings->NormalizeY(32);
	hell->dimension.y -= hell->dimension.w;
	auto dirLightEntity = scene->Push();
	auto dirLight = scene->GetEntity(dirLightEntity)->Emplace<DirectionalLight>(ComponentTypes::DIRLIGHT); 
	dirLight->dir= Vect3(1.f, 0.f, 0.f);
	//dirLight->constant = 1.f;
	dirLight->lightColor.ambient = Vect3(1.f, .3f, 0.3f);
	dirLight->lightColor.diffuse = Vect3(0.1, .5f, 0.3f);
	dirLight->lightColor.specular = Vect3(0.1, 1.f, 0.3f);
	messagingSystem->at(0x35).push_back(std::make_pair(0, nullptr));
	(*messagingSystem)[0x15].push_back(std::make_pair(0, nullptr));
}

void TestGame::Update(float deltaTime)
{
	auto& messages = messagingSystem->at(0x0);
	auto acceleration = Vect4();
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
		};
		messages.pop_front();
	}
	auto mouseMessages = messagingSystem->find(0x1); 
	if(mouseMessages != messagingSystem->end())
	{
		for(auto& message : mouseMessages->second)
		{
			switch(message.first)
			{
				case EVENTS::MOUSEMOTION_EVENT:
					auto mouseMotionEvent = reinterpret_cast<MouseMotionEvent*>(message.second.get());
					auto ny = (settings->NormalizeX(mouseMotionEvent->event.xrel));
					auto nx = (settings->NormalizeY(mouseMotionEvent->event.yrel));
					player->rotation.y += ny * deltaTime * 10;
					player->rotation.x += nx * deltaTime * 10;
					break;
			};
		}
	}
	auto text = scene->GetEntity(textPanel)->Get<std::string>(ComponentTypes::TEXTBOX);
	(*text) = "fps: " + std::to_string(settings->fps) + "\nHello";
	acceleration = GetRotationMatrix(player->rotation) * acceleration * deltaTime * 10;
	player->pos += Vect3(acceleration.x, acceleration.y, acceleration.z);
}
