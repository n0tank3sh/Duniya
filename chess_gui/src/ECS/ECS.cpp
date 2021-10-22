#include "ECS.h"
#include <random>
#include <unordered_map>
#include <fstream>
#include <ECS/CommonComponent.h>
#include <ECS/GraphicsComponent.h>
#include <Exception.h>
#include "SerializerSystem.h"



void* ComponentPtr::emplace(BaseImpl* impl)
{
	this->base = impl;
	return base->Create();
}
Scene::EntityManager::EntityManager(Scene* scene)
{
    owner = scene;
}

Scene::Scene()
{
	componentTypeMap.insert(std::make_pair(std::type_index(typeid(Transform)), ComponentTypes::TRANSFORM));
	componentTypeMap.insert(std::make_pair(std::type_index(typeid(Mesh)), ComponentTypes::MESH));
    entityManager = new EntityManager(this);
    componentManager = new ComponentManager(this);
}

uint32_t Scene::EntityManager::CreateEntity() const
{
    std::random_device rd;
    std::uniform_int_distribution<uint32_t> distrib;
    uint32_t entity = distrib(rd); 
    while(owner->entities.find(entity) != owner->entities.end())
    {
        entity = distrib(rd);
    }
    return entity;
}

void Scene::EntityManager::DestroyEntity(uint32_t& entity)
{
    if(owner->entities.find(entity) != owner->entities.end())
    {
        owner->entities.erase(entity);
    }
}

Scene::IComponentArray::IComponentArray(std::unordered_map<std::type_index, ComponentPtr>& componentTypes, 
		std::unordered_map<std::type_index, ComponentType>& componentTypeMap, uint32_t entity)
{
    for(auto& i: componentTypes)
    {
		ComponentType type = componentTypeMap[i.first];
        components.insert(std::move(std::make_pair(type, ComponentPtr(i.second.base))));
		components[type].entity = entity;
        components[type].base->Create();
    }
}

Scene::ComponentManager::ComponentManager(Scene* scene)
{
	this->scene = scene;
}

Scene::IComponentArray* Scene::ComponentManager::CreateComponentArray(uint32_t entity)
{
    return new IComponentArray(componentTypes, scene->componentTypeMap, entity);
}

uint32_t Scene::Push()
{
    uint32_t entity = entityManager->CreateEntity();
    entities[entity] = std::unique_ptr<IComponentArray>(componentManager->CreateComponentArray(entity));
    return entity;
}

Scene::IComponentArray* Scene::GetEntity(uint32_t entity)
{
    if(entities.find(entity) != entities.end())
        return entities[entity].get();
    else return nullptr;
}

uint32_t Scene::PushDef()
{
	auto tmp = entityManager->CreateEntity();
	entities[tmp].reset(new IComponentArray);
	return tmp;
}

void Scene::LoadScene(std::string filePath)
{
	std::ifstream fin(filePath, std::ifstream::binary);
	if(!fin.is_open()) throw CException(__LINE__, __FILE__, "File Exception", filePath + " not found");
	SerializerSystem::init();
	SerializerSystem::singleton->SetIStream(fin);
	SerializerSystem::singleton->Deserialize<ComponentTypeMap>(componentTypeMap);
	SerializerSystem::singleton->Deserialize<Entities>(entities);
	fin.close();
}

void Scene::SaveScene(std::string filePath)
{	
	std::ofstream fout(filePath, std::ofstream::binary);
	SerializerSystem::init();
	SerializerSystem::singleton->SetOStream(fout);
	SerializerSystem::singleton->Serialize<ComponentTypeMap>(componentTypeMap);	
	SerializerSystem::singleton->Serialize<Entities>(entities);
	fout.close();
}

SystemManager::SystemManager()
{
	queryMessages = std::shared_ptr<QueryMessages>(new QueryMessages);
}

void SystemManager::Add(System* system)
{
	system->messagingSystem = queryMessages;
	systems.emplace_back();
	systems.back().reset(system);
}

void SystemManager::LoadScene(Scene* scene)
{
	for(auto itr = systems.begin(); itr != systems.end(); itr++)
	{
		itr->get()->LoadScene(scene);
	}
}

void SystemManager::update(float deltaTime)
{
	for(auto itr = systems.begin(); itr != systems.end(); itr++)
	{
		itr->get()->update(deltaTime);
	}
}
