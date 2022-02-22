#pragma once
#include <iostream>
#include <SDL.h>
#include <string>
#include <ECS/GraphicsComponent.h>
#include <ECS/CommonComponent.h>
#include <ECS/ECS.h>


class AssetLoader
{
private:
    AssetLoader() = default;
public:
    static AssetLoader* init();
    static AssetLoader* GetSingleton();
    void LoadObj(std::string filePath, Mesh* mesh);
    void LoadTextureFile(std::string filePath, Texture* texture);
    void LoadTextFile(std::string filePath, std::string& fileSource);
	Scene* scene;
private:
	Scene* GetScene();
public:
    static AssetLoader* singleton;
    static bool sdl_initialised;
};
