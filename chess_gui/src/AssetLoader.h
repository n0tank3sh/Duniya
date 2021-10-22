#pragma once
#include <iostream>
#include <SDL.h>
#include <string>
#include <ECS/GraphicsComponent.h>
#include <ECS/CommonComponent.h>


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
public:
    static AssetLoader* singleton;
    static bool sdl_initialised;
};
