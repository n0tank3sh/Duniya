#include "AssetLoader.h"
#include "ECS/GraphicsComponent.h"
#include "SDLUtiliy.h"
#include <array>
#include <cstdint>
#include <sstream>
#include <SDL2/SDL_image.h>
#include <Exception.h>
#include <stdexcept>
#include <string>
#include <fstream>
#include <iterator>

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
	BREAKPOINT;
	if(mesh == nullptr) mesh = new Mesh;
    std::ifstream fin;
    std::string line;
    std::vector<Vect4> aPos;
    std::vector<Vect3> vNormal;
	std::vector<Vect2> texCord;
	std::vector<Vertex> verticies;
	std::vector<uint32_t> indicies;
	fin.open(filePath, std::ios::in);
	if(!fin)
		throw std::runtime_error("Couldn't open the file: " + filePath);
    while(std::getline(fin, line))
    {
		if(!line.empty())
		{
			if(line[0] == 'v' && line.size() > 2)
			{
				if(line[1] == ' ')
				{
					auto cord = 0;
					auto prev = 1;
					auto next = 1;
					aPos.emplace_back();
					while(next <= line.size() && line[next] == ' ') next++;
					prev = next;
					while(next <= line.size())
					{
						if(((prev != next) && ((next == line.size())) || (line[next] == ' ' && line[next - 1] != ' ')))
						{
							aPos.back().coordinates[cord] = std::stof(line.substr(prev, next - prev)); cord++;
							prev = next + 1;
							if(cord == 3) break;
						}
						next++;
					}
					if(cord != 3) 
						throw std::runtime_error("It's not formatted correctly");
					aPos.back().w = 1.f;
				}
				else if(line[1] == 't')
				{					
					auto cord = 0;
					auto prev = 2;
					auto next = 2;
					texCord.emplace_back();
					while(next <= line.size() && line[next] == ' ') next++;
					prev = next;
					while(next <= line.size())
					{
						if(((prev != next) && ((next == line.size())) || (line[next] == ' ' && line[next - 1] != ' ')))
						{
							texCord.back().coordinates[cord] = std::stof(line.substr(prev, next - prev)); 
							cord++;
							prev = next + 1;
							if(cord == 2) break;
						}
						next++;
					}
					if(cord != 2) 
						throw std::runtime_error("It's not formatted correctly");
				}
				else if(line[1] == 'n')
				{					
					auto cord = 0;
					auto prev = 2;
					auto next = 2;
					vNormal.emplace_back();
					while(next <= line.size() && line[next] == ' ') next++;
					prev = next;
					while(next <= line.size())
					{
						if(((prev != next) && ((next == line.size())) || (line[next] == ' ' && line[next - 1] != ' ')))
						{
							vNormal.back().coordinates[cord] = std::stof(line.substr(prev, next - prev)); 
							cord++;
							prev = next + 1;
							if(cord == 3) break;
						}
						next++;
					}
					if(cord != 3) 
						throw std::runtime_error("It's not formatted correctly");
				}
			}
			else if(line[0] == 'f')
			{
				auto prev = 1;
				auto next = 1;
				while(next <= line.size() && line[next] == ' ') next++;
				prev = next;
				auto dif = verticies.size();
				while(next <= line.size())
				{
					if(((prev != next) && (next == line.size())) || (line[next] == ' ' && line[next - 1] != ' '))
					{
						auto nPrev = prev;
						std::array<int, 3> goat;
						auto count = 0;
						for(int i = prev; i < next; i++)
						{
							if(line[i] == '/')
							{
								goat[count] = stoi(line.substr(nPrev, i - nPrev));
								nPrev = i + 1;
								count++;
							}
						}
						verticies.push_back({aPos[goat[0]], vNormal[goat[1]], texCord[goat[2]]});
						indicies.push_back(verticies.size() - 1);
						if((verticies.size() - dif) == 3 && (next + 1) < line.size()) 
						{
							indicies.push_back(indicies[indicies.size() - 2]);
							indicies.push_back(indicies[indicies.size() - 2]);
						}
						prev = next + 1;
					}
					next++;
				}
			}
		}
    }
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

