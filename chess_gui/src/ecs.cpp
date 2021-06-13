#include "ecs.h"
#include <random>
#include <unordered_map>

Scene::EntityManager::EntityManager(Scene* scene)
{
    owner = scene;
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
        components[i.first] = i.second;
        components[i.first].base->Create();
    }
}
Scene::IComponentArray* Scene::ComponentManager::CreateComponentArray()
{
    return new IComponentArray(componentTypes);
}


void Scene::Push()
{
    entities[entityManager.CreateEntity()] = std::unique_ptr<IComponentArray>(componentManager.CreateComponentArray());
}

Scene::IComponentArray* Scene::GetEntity(uint32_t entity)
{
    if(entities.find(entity) != entities.end())
        return entities[entity].get();
}
