#include "ecs.h"
#include <random>
#include <unordered_map>
#include <fstream>
#include <ecs/commoncomponent.h>
#include <ecs/GraphicsComponent.h>
#include <Exception.h>
#include "SerializerSystem.h"


//Serializer system Impl
SerializerSystem* SerializerSystem::singleton = nullptr;

SerializerSystem::SerializerSystem()
{
	is = nullptr;
	os = nullptr;
}

void SerializerSystem::SetIStream(std::istream& is)
{
	this->is = &is;
}

void SerializerSystem::SetOStream(std::ostream& os)
{
	this->os = &os;
}

SerializerSystem* SerializerSystem::init()
{
	if(singleton == nullptr)
	singleton = new SerializerSystem(); 
	return singleton;
}

template<class T>
void SerializerSystem::Serialize(const T& var)
{
	CHECKOS;
	os->write((char*)&var, sizeof(T));
}

template<class T>
void SerializerSystem::Deserialize(T& var)
{
	CHECKIS;
	is->read((char*)&var, sizeof(T));
}

template<>
void SerializerSystem::Serialize<Scene::IComponentArray>(const Scene::IComponentArray& var)
{
	auto comptritr = var.components.begin();
	uint32_t totalSize = var.components.size();
	os->write((char*)&totalSize, sizeof(uint32_t));
	while(comptritr != var.components.end())
	{
		os->write((char*)&comptritr->first, sizeof(uint32_t));
		if(comptritr->first == ComponentType::TRANSFORM)
		{
			Transform* transform = (Transform*)comptritr->second.base->GetPointer();
			SerializerSystem::singleton->Serialize<Transform>(*transform);
		}
		else if(comptritr->first == ComponentType::MESH)
		{
			Mesh* mesh = (Mesh*)comptritr->second.base->GetPointer();
			SerializerSystem::singleton->Serialize<Mesh>(*mesh);
		}
		else if(comptritr->first == ComponentType::TEXTURE)
		{
			Texture* texture = (Texture*)comptritr->second.base->GetPointer();
			SerializerSystem::singleton->Serialize<Texture>(*texture);
		}
	comptritr++;
	}
}

template<>
void SerializerSystem::Deserialize<Scene::IComponentArray>(Scene::IComponentArray& var)
{
	uint32_t totalSize;
	is->read((char*)&totalSize, sizeof(uint32_t));
	//std::cout << "Totatl Size: " << totalSize << std::endl;
	//in.read((char*)componentArray->componentTypes.data(), sizeof(uint32_t) * totalSize);
	for(uint32_t itr = 0; itr < totalSize; itr++)
	{
		std::cout << "Is it even entering here " << std::endl;
		ComponentType componentTypeTemp;
		is->read((char*)&componentTypeTemp, sizeof(uint32_t));
		ComponentPtr& componentPtr= var.components[componentTypeTemp];
		if(componentTypeTemp == ComponentType::MESH)
		{
			componentPtr.base = new ComponentPtr::Impl<Mesh>();
			componentPtr.base->Create();
			Mesh *mesh = (Mesh*)componentPtr.base->GetPointer();
			SerializerSystem::singleton->Deserialize<Mesh>(*mesh);
		}
		else if(componentTypeTemp == ComponentType::TEXTURE)
		{				
			componentPtr.base = new ComponentPtr::Impl<Texture>();
			componentPtr.base->Create();
			Texture* texture = (Texture*)componentPtr.base->GetPointer();
			SerializerSystem::singleton->Deserialize<Texture>(*texture);
		}
		else if(componentTypeTemp == ComponentType::TRANSFORM)
		{
			componentPtr.base = new ComponentPtr::Impl<Transform>();
			componentPtr.base->Create();
			SerializerSystem::singleton->Deserialize<Transform>(*reinterpret_cast<Transform*>(componentPtr.base->GetPointer()));
			//componentArray->components.insert(std::make_pair(ComponentType::TRANSFORM, componentPtr));
			//componentArray->components.insert(std::make_pair(std::type_index(typeid(Transform)), componentPtr));
		}
	}
}

template<>
void SerializerSystem::Serialize<ComponentTypeMap>(const ComponentTypeMap& var)
{
	uint32_t typeMapSize = var.size();
	os->write((char*)&typeMapSize, sizeof(uint32_t));
	for(auto& i: var)
	{
		std::string temp;
		if(i.first == std::type_index(typeid(Transform)))
			temp = "Transform";
		else if(i.first == std::type_index(typeid(Mesh)))
			temp = "Mesh";
		uint32_t tempSize = temp.size();
		os->write((char*)&tempSize, sizeof(uint32_t));
		os->write(temp.data(), tempSize);
		os->write((char*)&i.second, sizeof(uint32_t));
	}
}

template<>
void SerializerSystem::Deserialize<ComponentTypeMap>(ComponentTypeMap& var)
{
	uint32_t typeMapSize;
	is->read((char*)&typeMapSize, sizeof(uint32_t));
	
	while(typeMapSize--)
	{
		uint32_t tempSize;
		is->read((char*)&tempSize, sizeof(uint32_t));
		char* typeNameChar = new char[tempSize];
		uint32_t componentTypeUI;
		std::cout << "The temp size: " << tempSize << std::endl;
		is->read(typeNameChar, tempSize);
		is->read((char*)&componentTypeUI, sizeof(uint32_t));
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
		if(var.find(*typeIndex) == var.end())
		var.insert(std::make_pair(*typeIndex, static_cast<ComponentType>(componentTypeUI)));
		delete[] typeNameChar;
		typeNameChar = nullptr;
	}
}

// ecs

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

	
	SerializerSystem::init();
	SerializerSystem::singleton->SetIStream(fin);
	uint32_t entitySize;
	fin.read((char*)&entitySize, sizeof(uint32_t));
	SerializerSystem::singleton->Deserialize<ComponentTypeMap>(componentTypeMap);
	for(uint32_t i = 0; i < entitySize; i++)
	{
		std::cout << "Are we entering here " << std::endl;
		uint32_t entity;
		IComponentArray* componentArray = new IComponentArray();
		fin.read((char*)&entity, sizeof(uint32_t));
		SerializerSystem::singleton->Deserialize<IComponentArray>(*componentArray);
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
	SerializerSystem::init();
	SerializerSystem::singleton->SetOStream(fout);
	SerializerSystem::singleton->Serialize<ComponentTypeMap>(componentTypeMap);	
	for(auto& i: entities)
	{
		fout.write((char*)&i.first, sizeof(uint32_t));
		SerializerSystem::singleton->Serialize<IComponentArray>(*i.second.get());
	}
	fout.close();
}

