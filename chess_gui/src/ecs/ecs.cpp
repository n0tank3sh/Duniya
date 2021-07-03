#include "ecs.h"
#include <random>
#include <unordered_map>
#include <fstream>
#include <ecs/commoncomponent.h>
#include <ecs/GraphicsComponent.h>

Scene::EntityManager::EntityManager(Scene* scene)
{
    owner = scene;
}

Scene::Scene()
{
	componentTypeMap.insert(std::make_pair(std::type_index(typeid(Transform)), ComponentType::TRANSFORM));
	componentTypeMap.insert(std::make_pair(std::type_index(typeid(Mesh)), ComponentType::MESH));
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
		std::unordered_map<std::type_index, ComponentType>& componentTypeMap)
{
    for(auto i: componentTypes)
    {
		ComponentType type = componentTypeMap[i.first];
        components.insert(std::make_pair(type, ComponentPtr(i.second.base)));
        components[type].base->Create();
    }
}
Scene::IComponentArray* Scene::ComponentManager::CreateComponentArray()
{
    return new IComponentArray(componentTypes, scene->componentTypeMap);
}


uint32_t Scene::Push()
{
    uint32_t entity = entityManager->CreateEntity();
    entities[entity] = std::unique_ptr<IComponentArray>(componentManager->CreateComponentArray());
    return entity;
}

Scene::IComponentArray* Scene::GetEntity(uint32_t entity)
{
    if(entities.find(entity) != entities.end())
        return entities[entity].get();
    else return nullptr;
}

void Scene::LoadScene(std::string filePath)
{
	std::ifstream fin(filePath, std::ifstream::binary);
	uint32_t entitySize;
	fin.read((char*)&entitySize, sizeof(uint32_t));
	for(uint32_t i = 0; i < entitySize; i++)
	{
		uint32_t entity;
		uint32_t totalSize;
		IComponentArray* componentArray = new IComponentArray();
		componentArray->componentTypes.resize(totalSize);
		fin.read((char*)&entity, sizeof(uint32_t));
		fin.read((char*)&totalSize, sizeof(uint32_t));
		fin.read((char*)componentArray->componentTypes.data(), sizeof(uint32_t) * totalSize);
		for(uint32_t itr = 0; itr < totalSize; itr++)
		{
			ComponentPtr componentPtr;
			if(componentArray->componentTypes[itr] == ComponentType::MESH)
			{
				componentPtr.base = new ComponentPtr::Impl<Mesh>();
				componentPtr.base->Create();
				Mesh *mesh = (Mesh*)componentPtr.base->GetPointer();
				uint32_t indexSize, vertexSize;
				fin.read((char*)&indexSize, sizeof(uint32_t));
				fin.read((char*)&vertexSize, sizeof(uint32_t));
				fin.read((char*)mesh->indicies->data(), mesh->indicies->size() * sizeof(uint32_t));
				fin.read((char*)mesh->verticies->data(), mesh->verticies->size() * sizeof(Vertex));
				fin.read((char*)&mesh->texture->sizet, sizeof(uint32_t));
				fin.read((char*)&mesh->texture->depth, sizeof(uint32_t));
				fin.read((char*)&mesh->texture->height, sizeof(uint32_t));
				fin.read((char*)&mesh->texture->width, sizeof(uint32_t));
				fin.read((char*)&mesh->texture->type, sizeof(int32_t));
				fin.read((char*)&mesh->texture->sizet, sizeof(uint32_t));
				fin.read((char*)mesh->texture->data, sizeof(uint8_t) * mesh->texture->sizet);
			}
			else if(componentArray->componentTypes[itr] == ComponentType::TRANSFORM)
			{
				componentPtr.base = new ComponentPtr::Impl<Transform>();
				fin.read((char*)componentPtr.base->GetPointer(), sizeof(Transform));
//				componentArray->components.insert(std::make_pair(std::type_index(typeid(Transform)), componentPtr));
			}
		}
	}
	fin.close();
}

void Scene::SaveScene(std::string filePath)
{	
	std::ofstream fout(filePath, std::ofstream::binary);
	uint32_t entitySize = entities.size();
	fout.write((char*)&entitySize, sizeof(uint32_t));
	for(auto& i: entities)
	{
		fout.write((char*)&i.first, sizeof(uint32_t));
		auto comptritr = i.second.get()->components.begin();
		auto types = i.second.get()->componentTypes.begin();
		uint32_t totalSize = i.second.get()->componentTypes.size();
		fout.write((char*)&totalSize, sizeof(uint32_t));
		fout.write((char*)i.second.get()->componentTypes.data(), i.second.get()->componentTypes.size() * sizeof(uint32_t));
		while(comptritr != i.second.get()->components.end() && types != i.second.get()->componentTypes.end())
		{
			if(*types == ComponentType::TRANSFORM)
			{
				Transform* transform = (Transform*)comptritr->second.base->GetPointer();
				fout.write((char*)transform, sizeof(Transform));
			}
			else if(*types == ComponentType::MESH)
			{
				Mesh* mesh = (Mesh*)comptritr->second.base->GetPointer();
				uint32_t indexSize = mesh->indicies->size(), vertexSize = mesh->verticies->size();
				fout.write((char*)&indexSize, sizeof(uint32_t));
				fout.write((char*)&vertexSize, sizeof(uint32_t));
				fout.write((char*)mesh->indicies->data(), indexSize * sizeof(uint32_t));
				fout.write((char*)mesh->verticies->data(), vertexSize * sizeof(Vertex));
				fout.write((char*)&mesh->texture->sizet, sizeof(uint32_t));
				fout.write((char*)&mesh->texture->depth, sizeof(uint32_t));
				fout.write((char*)&mesh->texture->height, sizeof(uint32_t));
				fout.write((char*)&mesh->texture->width, sizeof(uint32_t));
				fout.write((char*)&mesh->texture->type, sizeof(int32_t));
				fout.write((char*)&mesh->texture->sizet, sizeof(uint32_t));
				fout.write((char*)mesh->texture->data, sizeof(uint8_t) * mesh->texture->sizet);
			}
		}
		comptritr++;types++;
	}
	fout.close();
}

