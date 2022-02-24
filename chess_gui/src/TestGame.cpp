#include "TestGame.h"
#include "AssetLoader.h"
<<<<<<< HEAD
#include "ECS/ECS.h"
=======
#include "ECS/CommonComponent.h"
#include "ECS/ECS.h"
#include "ECS/GraphicsComponent.h"
>>>>>>> 1656a13 (Removing unncessary module)
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
<<<<<<< HEAD
	uint32_t cube = scene->Push();
	AssetLoader::GetSingleton()->scene = scene;
	auto mesh = scene->GetEntity(cube)->Emplace<Mesh>(ComponentTypes::MESH);
	AssetLoader::GetSingleton()->LoadObj("Resource/Test/cube.obj", mesh);
=======
	//uint32_t cube = scene->Push();
	//AssetLoader::GetSingleton()->scene = scene;
	//auto mesh = scene->GetEntity(cube)->Emplace<Mesh>(ComponentTypes::MESH);
	//AssetLoader::GetSingleton()->LoadObj("Resource/Test/cube.obj", mesh);
	//auto cubeTransform = scene->GetEntity(cube)->Emplace<Transform>(ComponentTypes::TRANSFORM);
	//cubeTransform->pos = Vect3();
	//cubeTransform->rotation = Vect3();
	//cubeTransform->scale = Vect3(0.5f, 0.5f, 0.5f);
	uint32_t triangle = scene->Push();
	auto mesh = scene->GetEntity(triangle)->Emplace<Mesh>(ComponentTypes::MESH);
	auto verticies = new Vertex[3];
	verticies[0] = {.aPos = Vect4(0, 1.f, 0.f, 1.f), .texCord = Vect2(1.f, 1.f) };
	verticies[1] = {.aPos = Vect4(1.f, 0, 0.f, 1.f), .texCord = Vect2(0.5f, 0.5f) };
	verticies[2] = {.aPos = Vect4(-1.f, 0, 0, 1.f), .texCord = Vect2(0.35f, 0) }; 
	auto indicies = new uint32_t[3];
	for(int i = 0; i < 3; i++)
	{
		indicies[i] = i;
	}
	mesh->verticiesIndex = scene->resourceBank->Push_Back(reinterpret_cast<uint8_t*>(verticies), sizeof(Vertex) * 3);
	mesh->indiciesIndex = scene->resourceBank->Push_Back(reinterpret_cast<uint8_t*>(indicies), sizeof(uint32_t) * 3);
	mesh->indexCount = 3;
	mesh->vertexCount = 3;
	auto triangleTransform = scene->GetEntity(triangle)->Emplace<Transform>(ComponentTypes::TRANSFORM);
	triangleTransform->scale = Vect3(0.5f, 0.5f, 0.5f);
>>>>>>> 1656a13 (Removing unncessary module)
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
