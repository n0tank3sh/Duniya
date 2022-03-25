#include "ECS.hpp"
#include <memory>
#include <random>
#include <condition_variable>
#include <thread>
#include <unordered_map>
#include <fstream>
#include <ECS/CommonComponent.hpp>
#include <ECS/GraphicsComponent.hpp>
#include <Exception.hpp>
#include "SerializerSystem.hpp"



void* ComponentPtr::emplace(BaseImpl* impl)
{
	this->base = impl;
	return base->Create();
}
ComponentPtr::ComponentPtr()
{
	base = nullptr;
	entity = 0;
}

ComponentPtr::ComponentPtr(BaseImpl* baseImpl) 
{
	base = baseImpl->Clone();
	entity = 0;
}

ComponentPtr::ComponentPtr(const ComponentPtr& comptr)
{
	if(comptr.base != nullptr)
		base = comptr.base->Clone();
	else base = nullptr;
}
ComponentPtr::~ComponentPtr() 
{
	if(base != nullptr)
		delete base;
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
	resourceBank = new ResourceBank();
} 
uint32_t Scene::EntityManager::CreateEntity() 
{
	owner->entities.emplace_back();
	return owner->entities.size() - 1;
}

void Scene::EntityManager::DestroyEntity(uint32_t entity)
{
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
void* Scene::IComponentArray::Get(uint32_t componentType)
{
	auto i = components.find(componentType);
	if(i == components.end()) return nullptr;
	void* basePointer = i->second.base->GetPointer();
	if(basePointer == nullptr) std::cout << "BasePointer is null" << std::endl;
	return basePointer;
}


void* Scene::IComponentArray::Insert(ComponentType component, ComponentPtr::BaseImpl* base)
{
	return components[component].emplace(base);
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
	if(entity >= 0 && entity < entities.size())
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
	logger = new Logger;
	queryMessages.reset(new QueryMessages);
	settings.reset(new Setting); 
}

void SystemManager::AddQueryMessageBlock(uint32_t messageID)
{
	queryMessages->insert(std::make_pair(messageID, std::deque<std::pair<uint32_t, std::unique_ptr<Message>>>()));
}
void SystemManager::Add(System* system)
{
	system->messagingSystem = queryMessages.get();
	system->logger = logger;
	system->settings = settings.get();
	AddQueryMessageBlock(system->messageID);
	systems.push_back(system);
}

void SystemManager::LoadScene(Scene* scene)
{
	for(auto itr = systems.begin(); itr != systems.end(); itr++)
	{
		(*itr)->LoadScene(scene);
	}
}

void SystemManager::update(float deltaTime)
{
	for(auto itr = systems.begin(); itr != systems.end(); itr++)
	{
		(*itr)->Update(deltaTime);
	}
	logger->Paste();
}


float Setting::NormalizeX(const float& x) const
{
	return lerp(0.f, 1.f, x/resolution.x) * 2;
}

float Setting::NormalizeY(const float& y) const
{
	return lerp(0.f, 1.f, y/resolution.y) * 2;
}

Vect2 Setting::Normalize(const Vect2& point) const
{
	return Vect2(NormalizeX(point.x), NormalizeY(point.y));
}

float Setting::GetAspectRatio() const
{
	return resolution.x / resolution.y;
}


ThreadPool::ThreadPool(SystemManager* systemManager)
{
	this->systemManager = systemManager;
	threads.resize(std::thread::hardware_concurrency());
}

void ThreadPool::Run() 
{
	std::mutex moduleRunnerMutex;
	auto ModuleRunner = [](System* system, Scene* scene) {
		system->Update(0);
	};
}

ResourceBank::ResourcePtr::ResourcePtr()
	:
		data(nullptr), size(0)
{}

ResourceBank::ResourcePtr::ResourcePtr(size_t size)
	:
		size(size)
{
	if(size == 0) data = nullptr;
	else data = new uint8_t[size];
}
ResourceBank::ResourcePtr::ResourcePtr(uint8_t* data, size_t size)
	:
		data(data), size(size)
{}
ResourceBank::ResourcePtr::ResourcePtr(const ResourceBank::ResourcePtr& ptr)
{
	size = ptr.size;
	if(ptr.data != nullptr)
	{
		data = new uint8_t[ptr.size];
		std::copy(ptr.data, ptr.data + size, data);
	}
	else data = nullptr;
}

ResourceBank::ResourcePtr::ResourcePtr(ResourceBank::ResourcePtr&& ptr) noexcept
{
	this->size = std::move(ptr.size);
	this->data = std::move(ptr.data);
	ptr.data = nullptr;
	ptr.size = 0;
}

ResourceBank::ResourcePtr& ResourceBank::ResourcePtr::operator=(const ResourceBank::ResourcePtr& ptr)
{
	size = ptr.size;
	if(ptr.data != nullptr)
	{
		data = new uint8_t[ptr.size];
		std::copy(ptr.data, ptr.data + size, data);
	}
	else data = nullptr;
	return *this;
}

ResourceBank::ResourcePtr& ResourceBank::ResourcePtr::operator=(ResourceBank::ResourcePtr&& ptr)
{
	if(this->data != nullptr)
		delete this->data;
	this->data = ptr.data;
	this->size = ptr.size;
	ptr.data = nullptr;
	ptr.size = 0;
	return *this;
}
ResourceBank::ResourcePtr::~ResourcePtr() noexcept
{
	if(data != nullptr)
		delete[] data;
}
uint8_t* ResourceBank::ResourcePtr::Get()
{
	return data;
}

size_t ResourceBank::ResourcePtr::GetSize()
{
	return size;
}
