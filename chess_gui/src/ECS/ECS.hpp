#pragma once

#include "ECS/SerializerSystem.hpp"
#include <Exception.hpp>
#include <list>
#include <queue>
#include <memory>
#include <iostream>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <string>
#include <memory>
#include <cstdint>
#include <Math/Vect2.hpp>
#include <thread>
#include "Logger.hpp"



using ComponentType = uint32_t;
namespace ComponentTypes
{
	constexpr uint32_t MESH = 0x1;
	constexpr uint32_t TRANSFORM = 0x2;
	constexpr uint32_t CHILDREN = 0x3;
	constexpr uint32_t MATERIAL = 0x4;
	constexpr uint32_t TEXTURE = 0x5;
	constexpr uint32_t RENDERERSTUFF = 0x6;
	constexpr uint32_t CAMERA = 0x7;

};


namespace EVENTS
{
	constexpr uint32_t MAIN_EVENTS = 2;
	constexpr uint32_t KEYBOARD_EVENTS = 3;
	constexpr uint32_t MOUSE_EVENTS = 4;
	constexpr uint32_t MOUSEMOTION_EVENT = 5;
};



using ComponentTypeMap = std::unordered_map<std::type_index, ComponentType>;

struct TypeNotFoundException : public CException
{
	TypeNotFoundException(uint32_t line, const char* file)
		:
			CException(line, file, "Type Not Found", "Cannot find the type you are referencing")
	{}
};

class ComponentPtr
{
	public:
		struct BaseImpl
		{
			virtual void* Create() { return nullptr;}
			virtual void* GetPointer() { std::cout << "Creating a base pointer " << std::endl; return nullptr;}
			virtual void CheckingToSeeBase(){std::cout << "If base \n";}
			virtual BaseImpl* Clone() { return new BaseImpl(*this);} 
			virtual ~BaseImpl() {} 
		};
		template<typename T>
			struct Impl : public BaseImpl
		{
			T* data;
			Impl();
			Impl(const Impl& imp);
			Impl(T* data);

			~Impl();

			void CheckingToSeeBase() override;         
			BaseImpl* Clone() override;
			void* Create() override;
			void* GetPointer() override;
		};
		BaseImpl* base;
	public:
		uint32_t entity;

		ComponentPtr();
		ComponentPtr(BaseImpl* baseImpl);
		ComponentPtr(const ComponentPtr& comptr);    

		~ComponentPtr();

		void* emplace(BaseImpl* baseImpl);

};

class ResourceBank
{
	public:
		class ResourcePtr
		{
			private:
				uint8_t* data;
				size_t size;
			public:
				ResourcePtr();
				ResourcePtr(size_t size);
				ResourcePtr(uint8_t* data, size_t size);			
				ResourcePtr(const ResourcePtr& ptr);
				ResourcePtr(ResourcePtr&& ptr) noexcept;

				~ResourcePtr() noexcept;

				ResourcePtr& operator=(const ResourcePtr& ptr);
				ResourcePtr& operator=(ResourcePtr&& ptr);

				uint8_t* Get();
				size_t GetSize();
		}; 
		//		std::vector<std::unique_ptr<char>> resouces;
		std::vector<ResourcePtr> resources;
		ResourceBank() = default;
		ResourceBank(ResourceBank&) = default;
		~ResourceBank() = default;
		uint32_t Push_Back(uint8_t* __ptr, size_t size)
		{
			resources.emplace_back(__ptr, size);
			return resources.size() - 1;
		}
};

class Scene
{
	public:
		class EntityManager
		{
			Scene* owner;
			public:
			EntityManager(Scene* scene);
			uint32_t CreateEntity();
			void DestroyEntity(uint32_t entity);
		};

		class IComponentArray
		{
			//std::unordered_map<std::type_index, ComponentPtr> components;
			std::vector<ComponentType> componentTypes;
			friend SerializerSystem;
			public:
			std::unordered_map<ComponentType, ComponentPtr> components;
			public:
			IComponentArray() = default;
			IComponentArray(std::unordered_map<std::type_index, ComponentPtr>& componentTypes, 
					std::unordered_map<std::type_index, ComponentType>& componentTypeMap, uint32_t entity);

			void* Get(uint32_t componentType);
			template<typename T>
				T* Get(uint32_t componentType);
			template<typename T>
				void Insert(uint32_t componentType, T* data);
			template<typename T> 
				T* Emplace(uint32_t componentType);
			void* Insert(ComponentType component, ComponentPtr::BaseImpl* base);
		};    
		struct IComponentArrayIterator
		{
		};

		struct ComponentManager
		{
			Scene* scene;
			ComponentManager(Scene* scene);
			std::unordered_map<std::type_index, ComponentPtr> componentTypes;
			IComponentArray* CreateComponentArray(uint32_t entity);
		};
		using Entities  = std::vector<std::unique_ptr<IComponentArray>>;
		using EntitiesItr = Entities::iterator;
	public:
		ComponentTypeMap componentTypeMap;
		Entities entities;
		EntityManager* entityManager;
		ComponentManager* componentManager;
	public:
		Scene();
		uint32_t Push();
		uint32_t PushDef();
		void Merge(Scene* scene);
		IComponentArray* GetEntity(uint32_t entity);
		ResourceBank* resourceBank;

		template <typename T>
			void RegisterComponent();    
		template <typename T>
			void UnRegisterComponent();

		void LoadScene(std::string filePath);
		void SaveScene(std::string filePath);
};

struct Children
{
	Scene::Entities entities;
};

struct Message
{
	Message() = default;
	virtual ~Message() {};
};
using QueryMessages= std::unordered_map<uint32_t, std::deque<std::pair<uint32_t, std::unique_ptr<Message>>>>;

class Setting
{
	public:
		float fps;
		Vect2 resolution;
		float NormalizeX(const float& x) const;
		float NormalizeY(const float& y) const;
		Vect2 Normalize(const Vect2& point) const;
		float GetAspectRatio() const;
};
struct System
{

	uint32_t messageID;
	Logger* logger;
	virtual void LoadScene(Scene* scene) = 0;
	virtual void Update(float deltaTime) = 0;
	template<typename T> 
		static System* Create();
	QueryMessages* messagingSystem;
	Setting* settings;
	bool isSingelton;
};

struct SystemManager
{
	SystemManager();
	std::vector<System*> systems;
	Logger* logger;
	void Add(System* system);
	void LoadScene(Scene* scene);
	void update(float deltaTime);
	void AddQueryMessageBlock(uint32_t messageID);
	std::unique_ptr<QueryMessages> queryMessages;
	std::unique_ptr<Setting> settings;
};

struct ThreadPool
{
	private:
		std::vector<std::thread> threads;
		SystemManager* systemManager;
		Scene* scene;
	public:
		ThreadPool(SystemManager* systemManager);
		void LoadScene(Scene* scene);
		void Run();
		~ThreadPool();
};


// Impl definition for avoiding link error stupid c++
	template<typename T>
ComponentPtr::Impl<T>::Impl()
	:
		data(nullptr)
{}

	template<typename T>
ComponentPtr::Impl<T>::Impl(const Impl& imp)
	:
		data(nullptr)
{}

	template<typename T>
ComponentPtr::Impl<T>::Impl(T* data)
	:
		data(data)
{}

	template<typename T>
void ComponentPtr::Impl<T>::CheckingToSeeBase()  
{
	std::cout << "If derived \n";
}

	template<typename T>
ComponentPtr::BaseImpl* ComponentPtr::Impl<T>::Clone() 
{
	return static_cast<BaseImpl*>(new Impl<T>(*this));
}

	template<typename T>
void* ComponentPtr::Impl<T>::Create() 
{
	data = new T();
	return data;
}

	template<typename T>
void* ComponentPtr::Impl<T>::GetPointer() 
{
	if(data == nullptr) std::cout << "Data is NULL " << std::endl;
	return data;
}

	template<typename T>
ComponentPtr::Impl<T>::~Impl()
{
	if(data != nullptr) delete data;
}

	template<typename T>
void Scene::IComponentArray::Insert(uint32_t componentType, T* data)
{
	components[componentType].emplace(new ComponentPtr::Impl<T>(data));
}

	template<typename T>
T* Scene::IComponentArray::Get(uint32_t componentType)
{
	if(components.find(componentType) != components.end())
		return reinterpret_cast<T*>(components[componentType].base->GetPointer());
	return nullptr;
}

	template<typename T>
T* Scene::IComponentArray::Emplace(uint32_t componentType)
{
	auto compImpl = new ComponentPtr::Impl<T>();
	components[componentType].emplace(compImpl);
	return reinterpret_cast<T*>(compImpl->Create());
}


	template <typename T>
void Scene::RegisterComponent()
{
	componentManager->componentTypes.insert(std::make_pair
			(std::type_index(typeid(T)), 
			 ComponentPtr(new ComponentPtr::Impl<T>())
			)
			);
}

	template <typename T>
void Scene::UnRegisterComponent()
{
	componentManager->componentTypes.erase(std::type_index(typeid(T)));
}


