#include "AssetLoader.h"
#include "ECS/GraphicsComponent.h"
#include "SDLUtiliy.h"
#include <array>
#include <codecvt>
#include <cstdint>
#include <sstream>
#include <SDL2/SDL_image.h>
#include <Exception.h>
#include <stdexcept>
#include <string>
#include <fstream>
#include <iterator>
#include <map>
#include <unordered_map>



void AssetLoader::ObjLoader::SetFile(const std::string& fileName)
{
	fin.open(fileName);
	if(!fin.is_open())
		throw std::runtime_error("file not found");

}


void AssetLoader::ObjLoader::Interpret(std::vector<Vertex>& verticies, std::vector<uint32_t>& indicies)
{
	uint32_t indexSize = 0;
	std::string line;	
	while(!fin.eof())
	{
		std::string temp;
		std::getline(fin, temp);
		std::istringstream line(temp);
		if(line.peek() == 'v')
		{
			line.ignore();
			if(line.peek() == ' ')
			{
				aPos.emplace_back();
				RepeatInsert(line, aPos.back(), 3);
				aPos.back().coordinates[3] = 1.f;
			}
			else
			{
				if(line.peek() == 't')
				{
					line.ignore();
					texCord.emplace_back();
					RepeatInsert(line, texCord.back(), 2);	
				}
				else if(line.peek() == 'n')
				{
					line.ignore();
					vNormal.emplace_back();
					RepeatInsert(line, vNormal.back(), 3);
				}
			}
		}
		else if(line.peek() == 'f')
		{
			line.ignore();
			std::array<int, 3> tempFace;
			for(int i = 0; i < 3; i++)
			{
				tempFace.fill(0);
				for(int j = 0; j < 3; j++)
				{
					line >> tempFace[j]; 
					tempFace[j] -= 1;
					line.ignore();
				}			
				auto check = repeatCheck.find(tempFace);
				if(check == repeatCheck.end())
				{
					verticies.push_back({aPos[tempFace[0]], vNormal[tempFace[2]], texCord[tempFace[1]]});
					indicies.push_back(indexSize);
					repeatCheck.insert(std::make_pair(tempFace, indexSize));
					indexSize++;
				}
				else
					indicies.push_back(check->second);
			}

			if(!line.eof())
			{
				indicies.push_back(indicies[indicies.size() - 2]);
				indicies.push_back(indicies[indicies.size() - 2]);
				tempFace.fill(0);
				for(int j = 0; j < 3; j++)
				{
					line >> tempFace[j]; 
					tempFace[j] -= 1;
					line.ignore();
				}
				auto check = repeatCheck.find(tempFace);
				if(check == repeatCheck.end())
				{
					verticies.push_back({aPos[tempFace[0]], vNormal[tempFace[2]], texCord[tempFace[1]]});
					indicies.push_back(indexSize);
					repeatCheck.insert(std::make_pair(tempFace, indexSize));
					indexSize++;
				}
				else
					indicies.push_back(check->second);

			}
		}
	}
}
AssetLoader* AssetLoader::singleton = nullptr;
bool AssetLoader::sdl_initialised = false;

AssetLoader* AssetLoader::init()
{
	singleton = new AssetLoader();
	singleton->scene = nullptr;
	return singleton;
}

AssetLoader* AssetLoader::GetSingleton() 
{
	return singleton;
}

Scene* AssetLoader::GetScene()
{
	if(scene == nullptr)
		throw std::runtime_error("AssetLoader\nScene is not set");
	return scene;
}

void AssetLoader::LoadObj(std::string filePath, Mesh* mesh)
{
	std::vector<Vertex> verticies;
	std::vector<uint32_t> indicies;
	ObjLoader objLoader;
	objLoader.SetFile(filePath);
	objLoader.Interpret(verticies, indicies);
	
	mesh->vertexCount = verticies.size();
	mesh->indexCount = indicies.size();
	auto vertexData = new uint8_t[sizeof(Vertex) * mesh->vertexCount];
	auto indiciesData = new uint8_t[sizeof(uint32_t) * mesh->indexCount];
	std::copy(verticies.begin(), verticies.end(), reinterpret_cast<Vertex*>(vertexData));
	std::copy(indicies.begin(), indicies.end(), reinterpret_cast<uint32_t*>(indiciesData));
	mesh->vertexCount = verticies.size();
	mesh->indexCount = indicies.size();
	mesh->verticiesIndex = scene->resourceBank->Push_Back(vertexData, sizeof(Vertex) * mesh->vertexCount);
	mesh->indiciesIndex = scene->resourceBank->Push_Back(indiciesData, sizeof(uint32_t) * mesh->indexCount);
}

//void AssetLoader::LoadTextureFile(std::string hPath, Texture* texture)
//{
//    if(!sdl_initialised)
//    {
//        uint32_t flags = IMG_INIT_PNG | IMG_INIT_JPG;
//        uint32_t tempFlag = IMG_Init(flags);
//        if(tempFlag != flags)
//        {
//            std::cout << "Can't initalize on sdl image " << std::endl;
//            std::cout << "Reason given: " << IMG_GetError() << std::endl;
//        }
//    }
//    uint32_t pixelFormat = SDL_PIXELFORMAT_RGBA8888;
//    SDL_Surface* loadedImage = IMG_Load(hPath.c_str());
//    if(loadedImage == nullptr) 
//    {
//        std::cout << "Can't load the image \n " 
//         << "Reason Given: " << IMG_GetError() << std::endl;
//    }
//    SDL_Surface* formattedImage;
//    formattedImage  = SDL_ConvertSurfaceFormat(loadedImage, pixelFormat, 0);
//    if(formattedImage == NULL)
//        std::cout << "Can't convert to different format \n"
//            << "Reason Given: " << IMG_GetError() << std::endl;
//    SDL_LockSurface(formattedImage);
//    texture->type = Texture::Type::T2D;
//    texture->width = formattedImage->w;
//    texture->height = formattedImage->h;
//    texture->sizet = formattedImage->h * formattedImage->pitch;
//
//    texture->data = new uint8_t[texture->sizet];
//    uint8_t* pixels  = (uint8_t*)formattedImage->pixels;
//    std::copy(pixels, pixels + texture->sizet, texture->data);
//    if(texture == nullptr) std::cout << "Texture is nullptr in the function " << std::endl;
//}

void AssetLoader::LoadTextFile(std::string filePath, std::string& source)
{
	std::ifstream fin;
	fin.open(filePath.c_str(), std::ios::in);
	if(!fin) throw CException(__LINE__, __FILE__, "Text file Loader", std::string("Couldn't open the file path: ") + filePath);
	std::stringstream ss;
	ss << fin.rdbuf();
	source = ss.str();
}

