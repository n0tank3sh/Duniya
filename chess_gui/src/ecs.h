#pragma once

#include "Exception.h"
#include <queue>
#include <memory>
#include <iostream>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <string>
#include <memory>

class ComponentPtr
{
public:
    struct BaseImpl
    {
        virtual void Create() { }
        virtual void* GetPointer() { std::cout << "Creating a base pointer " << std::endl; return nullptr;}
        virtual void CheckingToSeeBase(){}
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
            data = imp.data;
        }
        void CheckingToSeeBase() override {std::cout << "If derived \n";}
        BaseImpl* Clone() override
        {
            return static_cast<BaseImpl*>(new Impl<T>(*this));
        }
        void Create() override
        {
            data = new T();
        }
        void* GetPointer() override
        {
            return data;
        }
        ~Impl()
        {
            if(data != nullptr) delete data;
        }
    };
    BaseImpl* base;
public:
    ComponentPtr()
    {
        base = nullptr;
    }

    ComponentPtr(BaseImpl* baseImpl) 
    {
        base = baseImpl->Clone();
    }
    ComponentPtr(const ComponentPtr& comptr)
    {
        base = comptr.base->Clone();
    }
    ~ComponentPtr() 
    {
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

    class IComponentArray
    {
        std::unordered_map<std::type_index, ComponentPtr> components;
    public:
        IComponentArray();
        IComponentArray(std::unordered_map<std::type_index, ComponentPtr>& componentTypes);
        template<typename T>
        T* get()
        {
            auto i = components.find(std::type_index(typeid(T)));
            if(i == components.end()) throw CException(__LINE__, __FILE__, "IComponentArray", "Couldn't find the Component");
            return static_cast<T*>(components[std::type_index(typeid(T))].base->GetPointer()); 
        }
    };    

    struct ComponentManager
    {
        std::unordered_map<std::type_index, ComponentPtr> componentTypes;
        IComponentArray* CreateComponentArray();
    };
public:
    std::unordered_map<uint32_t, std::unique_ptr<IComponentArray>> entities;
    EntityManager* entityManager;
    ComponentManager* componentManager;
public:
    Scene();
    uint32_t Push();
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

    // (TODO): Scene loading and unloading
    void LoadScene(std::string filePath);
    std::string SaveScene(std::string filePath);
};

struct System
{
    virtual void LoadScene(Scene* scene) = 0;
    virtual void update(float deltaTime) = 0;
};
