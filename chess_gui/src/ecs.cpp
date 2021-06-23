#include "ecs.h"
#include <random>
#include <unordered_map>

Scene::EntityManager::EntityManager(Scene* scene)
{
    owner = scene;
}

Scene::Scene()
{
    entityManager = new EntityManager(this);
    componentManager = new ComponentManager();
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

Scene::IComponentArray::IComponentArray(std::unordered_map<std::type_index, ComponentPtr>& componentTypes)
{
    for(auto i: componentTypes)
    {
        components.insert(std::make_pair(i.first, ComponentPtr(i.second.base)));
        components[i.first].base->Create();
    }
}
Scene::IComponentArray* Scene::ComponentManager::CreateComponentArray()
{
    return new IComponentArray(componentTypes);
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
