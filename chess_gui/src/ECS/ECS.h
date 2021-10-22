#pragma once

#include <Exception.h>
#include <queue>
#include <memory>
#include <iostream>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <string>
#include <memory>



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
        Impl()
        {
            data = nullptr;
        }
        Impl(const Impl& imp)
        {
			data = nullptr;
        }
        void CheckingToSeeBase() override {std::cout << "If derived \n";}
        BaseImpl* Clone() override
        {
            return static_cast<BaseImpl*>(new Impl<T>(*this));
        }
        void* Create() override
        {
            data = new T();
			return data;
        }
        void* GetPointer() override
        {
			if(data == nullptr) std::cout << "Data is NULL " << std::endl;
            return data;
        }
        ~Impl()
        {
            if(data != nullptr) delete data;
        }
    };
    BaseImpl* base;
public:
	uint32_t entity;
    ComponentPtr()
    {
        base = nullptr;
		entity = 0;
    }

    ComponentPtr(BaseImpl* baseImpl) 
    {
        base = baseImpl->Clone();
		entity = 0;
    }
	void* emplace(BaseImpl* baseImpl);
    ComponentPtr(const ComponentPtr& comptr)
    {
		if(comptr.base != nullptr)
        base = comptr.base->Clone();
		else base = nullptr;
    }
    ~ComponentPtr() 
    {
		if(base != nullptr)
        delete base;
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
        uint32_t CreateEntity() const;
        void DestroyEntity(uint32_t& entity);
    };

    struct IComponentArray
    {
		std::unordered_map<ComponentType, ComponentPtr> components;
        //std::unordered_map<std::type_index, ComponentPtr> components;
		std::vector<ComponentType> componentTypes;
        IComponentArray() = default;
        IComponentArray(std::unordered_map<std::type_index, ComponentPtr>& componentTypes, std::unordered_map<std::type_index, ComponentType>& componentTypeMap, uint32_t entity);
        template<ComponentType T>
        void* get()
        {
            auto i = components.find(T);
            if(i == components.end()) return nullptr;
			void* basePointer = i->second.base->GetPointer();
			if(basePointer == nullptr) std::cout << "BasePointer is null" << std::endl;
			return basePointer;
        }
		void* insert(ComponentType component, ComponentPtr::BaseImpl* base)
		{
			return components[component].emplace(base);
		}
    };    

    struct ComponentManager
    {
		Scene* scene;
		ComponentManager(Scene* scene);
        std::unordered_map<std::type_index, ComponentPtr> componentTypes;
        IComponentArray* CreateComponentArray(uint32_t entity);
    };
	using Entities  = std::unordered_map<uint32_t,  std::unique_ptr<IComponentArray>>;
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
    template <typename T>
    void RegisterComponent()
    {
        componentManager->componentTypes.insert(std::make_pair
                	(std::type_index(typeid(T)), 
                	 ComponentPtr(new ComponentPtr::Impl<T>())
                	)
                );
    }
    template <typename T>
    void UnRegisterComponent()
    {
        componentManager->componentTypes.erase(std::type_index(typeid(T)));
    }

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
using QueryMessages= std::unordered_map<uint32_t, std::queue<std::unique_ptr<Message>>>;

struct System
{
    virtual void LoadScene(Scene* scene) = 0;
    virtual void update(float deltaTime) = 0;
	template<typename T> 
	static System* Create();
	std::weak_ptr<QueryMessages> messagingSystem;
	bool isSingelton;
};

struct SystemManager
{
	SystemManager();
	std::vector<std::unique_ptr<System>> systems;
	void Add(System* system);
	void LoadScene(Scene* scene);
	void update(float deltaTime);
	std::shared_ptr<QueryMessages> queryMessages;
};
