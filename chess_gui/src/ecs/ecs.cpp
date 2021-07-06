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

Scene::ComponentManager::ComponentManager(Scene* scene)
{
	this->scene = scene;
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
	uint32_t typeMapSize;
	fin.read((char*)&typeMapSize, sizeof(uint32_t));
	
	while(typeMapSize--)
	{
		uint32_t tempSize;
		fin.read((char*)&tempSize, sizeof(uint32_t));
		char* typeNameChar = new char[tempSize];
		uint32_t componentTypeUI;
		std::cout << "The temp size: " << tempSize << std::endl;
		fin.read(typeNameChar, tempSize);
		fin.read((char*)&componentTypeUI, sizeof(uint32_t));
		std::type_index* typeIndex = nullptr;
		std::string typeName(typeNameChar);
		std::cout <<"The type name is: " <<  typeName << std::endl;
		if(typeName == "Transform")
		{
			typeIndex = new std::type_index(typeid(Transform));
		}
		else if(typeName == "Mesh")
		{
			typeIndex = new std::type_index(typeid(Mesh));
		}
		if(typeIndex == nullptr) std::cout << "TypeIndex is nullptr" << std::endl; 
		if(*typeIndex == std::type_index(typeid(Mesh))) std::cout << "yes it is the Mesh we are the talking about " << std::endl;
		if(componentTypeMap.find(*typeIndex) == componentTypeMap.end())
		componentTypeMap.insert(std::make_pair(*typeIndex, static_cast<ComponentType>(componentTypeUI)));
		delete[] typeNameChar;
		typeNameChar = nullptr;
	}
	for(uint32_t i = 0; i < entitySize; i++)
	{
		std::cout << "Are we entering here " << std::endl;
		uint32_t entity;
		uint32_t totalSize;
		IComponentArray* componentArray = new IComponentArray();
		//componentArray->componentTypes.resize(totalSize);
		fin.read((char*)&entity, sizeof(uint32_t));
		fin.read((char*)&totalSize, sizeof(uint32_t));
		std::cout << "Entity " << entity << std::endl;
		std::cout << "Totatl Size: " << totalSize << std::endl;
		//fin.read((char*)componentArray->componentTypes.data(), sizeof(uint32_t) * totalSize);
		for(uint32_t itr = 0; itr < totalSize; itr++)
		{
			std::cout << "Is it even entering here " << std::endl;
			ComponentType componentTypeTemp;
			fin.read((char*)&componentTypeTemp, sizeof(uint32_t));
			ComponentPtr& componentPtr = componentArray->components[componentTypeTemp];
			if(componentTypeTemp == ComponentType::MESH)
			{
				componentPtr.base = new ComponentPtr::Impl<Mesh>();
				componentPtr.base->Create();
				Mesh *mesh = (Mesh*)componentPtr.base->GetPointer();
				uint32_t indexSize, vertexSize;
				fin.read((char*)&indexSize, sizeof(uint32_t));
				fin.read((char*)&vertexSize, sizeof(uint32_t));
				mesh->indicies = new std::vector<uint32_t>();
				mesh->verticies = new std::vector<Vertex>();
				while(indexSize--)
				{
					uint32_t indexData;
					fin.read((char*)&indexData, sizeof(uint32_t));
					mesh->indicies->push_back(indexData);
				}
				while(vertexSize--)
				{
					Vertex vertex;
					fin.read((char*)&vertex, sizeof(Vertex));
					mesh->verticies->push_back(vertex);
				}
				mesh->texture =new Texture;
				fin.read((char*)&mesh->texture->sizet, sizeof(uint32_t));
				fin.read((char*)&mesh->texture->depth, sizeof(uint32_t));
				fin.read((char*)&mesh->texture->height, sizeof(uint32_t));
				fin.read((char*)&mesh->texture->width, sizeof(uint32_t));
				uint32_t textureType;
				fin.read((char*)&textureType, sizeof(int32_t));
				mesh->texture->type = static_cast<Texture::Type>(textureType);
				mesh->texture->data = new uint8_t[mesh->texture->sizet];
				fin.read((char*)mesh->texture->data, sizeof(uint8_t) * mesh->texture->sizet);
				std::cout << "The size of the vertex size is: " << mesh->verticies->size() << std::endl;
			}
			else if(componentTypeTemp == ComponentType::TRANSFORM)
			{
				componentPtr.base = new ComponentPtr::Impl<Transform>();
				componentPtr.base->Create();
				fin.read((char*)(Transform*)componentPtr.base->GetPointer(), sizeof(Transform));
				//componentArray->components.insert(std::make_pair(ComponentType::TRANSFORM, componentPtr));
//				componentArray->components.insert(std::make_pair(std::type_index(typeid(Transform)), componentPtr));
			}
		}
		componentArray->components[ComponentType::MESH].base->CheckingToSeeBase();
		entities[entity] = std::unique_ptr<IComponentArray>(componentArray);
	}
	fin.close();
}

void Scene::SaveScene(std::string filePath)
{	
	std::ofstream fout(filePath, std::ofstream::binary);
	uint32_t entitySize = entities.size();
	fout.write((char*)&entitySize, sizeof(uint32_t));
	std::cout << "Checking if fout is open " << fout.is_open() << std::endl;
	uint32_t typeMapSize = componentTypeMap.size();
	fout.write((char*)&typeMapSize, sizeof(uint32_t));
	for(auto& i: componentTypeMap)
	{
		std::string temp;
		if(i.first == std::type_index(typeid(Transform)))
			temp = "Transform";
		else if(i.first == std::type_index(typeid(Mesh)))
			temp = "Mesh";
		uint32_t tempSize = temp.size();
		fout.write((char*)&tempSize, sizeof(uint32_t));
		fout.write(temp.data(), tempSize);
		fout.write((char*)&i.second, sizeof(uint32_t));
	}
	for(auto& i: entities)
	{
		fout.write((char*)&i.first, sizeof(uint32_t));
		auto comptritr = i.second.get()->components.begin();
		uint32_t totalSize = i.second.get()->components.size();
		fout.write((char*)&totalSize, sizeof(uint32_t));
		while(comptritr != i.second.get()->components.end())
		{
			fout.write((char*)&comptritr->first, sizeof(uint32_t));
			if(comptritr->first == ComponentType::TRANSFORM)
			{
				Transform* transform = (Transform*)comptritr->second.base->GetPointer();
				fout.write((char*)transform, sizeof(Transform));
			}
			else if(comptritr->first == ComponentType::MESH)
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
				fout.write((char*)&mesh->texture->type, sizeof(uint32_t));
				fout.write((char*)mesh->texture->data, sizeof(uint8_t) * mesh->texture->sizet);
			}
		comptritr++;
		}
	}
	fout.close();
}

