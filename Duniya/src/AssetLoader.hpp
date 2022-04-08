#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <map>
#include <ECS/GraphicsComponent.hpp>
#include <ECS/CommonComponent.hpp>
#include <ECS/ECS.hpp>


class AssetLoader
{
private:
    AssetLoader() = default;
private:
	class ObjLoader
	{
	private:
		template<typename T>
		void RepeatInsert(std::istringstream& line, T& var, uint32_t count)
		{
			for(int i = 0; i < count; i++)
				line >> var;
		}
	private:
		std::ifstream fin;
		std::string lineStore;
		std::vector<Vect4> aPos;
		std::vector<Vect3> vNormal;
		std::vector<Vect2> texCord;
		std::map<std::array<int, 3>, int> repeatCheck;
	public:
		void SetFile(const std::string& fileName);
		void Interpret(std::vector<Vertex>& vertex, std::vector<uint32_t>& indicies, DrawPrimitive& drawPrimitve);
	};
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
