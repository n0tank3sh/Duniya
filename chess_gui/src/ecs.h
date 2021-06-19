#pragma once

#include <queue>
#include <memory>
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
        virtual void Create();
        virtual void* GetPointer();
    };
    template<typename T>
    class Impl : public BaseImpl
    {
        T* data;
        Impl()
        {
            data = nullptr;
        }
        virtual void Create() override
        {
            data = new T;
        }
        virtual void* GetPointer() override
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
        base = baseImpl;
    }
    ~ComponentPtr()
    {
        delete base;
    }
};

class Scene
{
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
            return static_cast<T>(components[std::type_index(typeid(T))].base->GetPointer()); 
        }
    };    

    struct ComponentManager
    {
        std::unordered_map<std::type_index, ComponentPtr> componentTypes;
        IComponentArray* CreateComponentArray();
    };
public:
    std::unordered_map<uint32_t, std::unique_ptr<IComponentArray>> entities;
    EntityManager entityManager;
    ComponentManager componentManager;
public:
    void Push();
    IComponentArray* GetEntity(uint32_t entity);
    template <typename T>
    void RegisterComponent()
    {
        componentManager.componentTypes.insert(std::make_pair
                (std::type_index(typeid(T)), 
                 ComponentPtr(static_cast<ComponentPtr::BaseImpl>(new ComponentPtr::Impl<T>))
                 )
                );
    }
    template <typename T>
    void UnRegisterComponent()
    {
        componentManager.componentTypes.erase(std::type_index(typeid(T)));
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
