#include "AssetLoader.hpp"

#include <SDL2/SDL_image.h>
#include <assimp/scene.h>

#include <Exception.hpp>
#include <array>
#include <codecvt>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "ECS/GraphicsComponent.hpp"
#include "SDLUtiliy.hpp"
#include "SDL_pixels.h"

void AssetLoader::ObjLoader::SetFile(const std::string& fileName) {
    fin.open(fileName);
    if (!fin.is_open()) throw std::runtime_error("file not found");
}

void AssetLoader::ObjLoader::Interpret(std::vector<Vertex>& verticies,
				       std::vector<uint32_t>& indicies,
				       DrawPrimitive& drawPrimitive) {
    uint32_t indexSize = 0;
    std::string line;
    drawPrimitive = DrawPrimitive::TRIANGLES;
    while (!fin.eof()) {
	std::string temp;
	std::getline(fin, temp);
	std::istringstream line(temp);
	if (line.peek() == 'v') {
	    line.ignore();
	    if (line.peek() == ' ') {
		aPos.emplace_back();
		RepeatInsert(line, aPos.back(), 3);
		aPos.back().coordinates[3] = 1.f;
	    } else {
		if (line.peek() == 't') {
		    line.ignore();
		    texCord.emplace_back();
		    RepeatInsert(line, texCord.back(), 2);
		} else if (line.peek() == 'n') {
		    line.ignore();
		    vNormal.emplace_back();
		    RepeatInsert(line, vNormal.back(), 3);
		}
	    }
	} else if (line.peek() == 'f') {
	    line.ignore();
	    std::array<int, 3> tempFace;
	    for (int i = 0; i < 3; i++) {
		tempFace.fill(0);
		for (int j = 0; j < 3; j++) {
		    line >> tempFace[j];
		    tempFace[j] -= 1;
		    line.ignore();
		}
		auto check = repeatCheck.find(tempFace);
		if (check == repeatCheck.end()) {
		    verticies.push_back({aPos[tempFace[0]],
					 vNormal[tempFace[2]],
					 texCord[tempFace[1]]});
		    indicies.push_back(indexSize);
		    repeatCheck.insert(std::make_pair(tempFace, indexSize));
		    indexSize++;
		} else
		    indicies.push_back(check->second);
	    }

	    if (!line.eof()) {
		indicies.push_back(indicies[indicies.size() - 2]);
		indicies.push_back(indicies[indicies.size() - 2]);
		tempFace.fill(0);
		for (int j = 0; j < 3; j++) {
		    line >> tempFace[j];
		    tempFace[j] -= 1;
		    line.ignore();
		}
		auto check = repeatCheck.find(tempFace);
		if (check == repeatCheck.end()) {
		    verticies.push_back({aPos[tempFace[0]],
					 vNormal[tempFace[2]],
					 texCord[tempFace[1]]});
		    indicies.push_back(indexSize);
		    repeatCheck.insert(std::make_pair(tempFace, indexSize));
		    indexSize++;
		} else
		    indicies.push_back(check->second);
	    }
	}
    }
}
AssetLoader* AssetLoader::singleton = nullptr;
bool AssetLoader::sdl_initialised = false;

AssetLoader* AssetLoader::init() {
    singleton = new AssetLoader();
    singleton->scene = nullptr;
    return singleton;
}

AssetLoader* AssetLoader::GetSingleton() { return singleton; }

Scene* AssetLoader::GetScene() {
    if (scene == nullptr)
	throw std::runtime_error("AssetLoader\nScene is not set");
    return scene;
}

void AssetLoader::LoadObj(std::string filePath, Mesh* mesh) {
    std::vector<Vertex> verticies;
    std::vector<uint32_t> indicies;
    ObjLoader objLoader;
    objLoader.SetFile(filePath);
    objLoader.Interpret(verticies, indicies, mesh->drawPrimitive);

    mesh->vertexCount = verticies.size();
    auto vertexData = new Vertex[mesh->vertexCount];
    if (indicies.size() == verticies.size()) {
	mesh->indexCount = 0;
    } else {
	mesh->indexCount = indicies.size();
	auto indiciesData = new uint32_t[mesh->indexCount];
	std::copy(indicies.begin(), indicies.end(), indiciesData);
	mesh->indiciesIndex = scene->resourceBank->Push_Back(
	    reinterpret_cast<uint8_t*>(indiciesData),
	    sizeof(uint32_t) * mesh->indexCount);
    }
    std::copy(verticies.begin(), verticies.end(), vertexData);
    mesh->verticiesIndex =
	scene->resourceBank->Push_Back(reinterpret_cast<uint8_t*>(vertexData),
				       sizeof(Vertex) * mesh->vertexCount);
}

Texture::Format GetFormat(uint32_t sdlFormat) {
    Texture::Format ans;
    switch (sdlFormat) {
	case SDL_PIXELFORMAT_RGB888:
	    ans = Texture::RGB;
	    break;
	case SDL_PIXELFORMAT_RGBA8888:
	    ans = Texture::RGBA;
	    break;
	case SDL_PIXELFORMAT_INDEX8:
	    ans = Texture::R;
	    break;
	default:
	    throw std::runtime_error("We don't support that image format");
    };
    return ans;
}

void AssetLoader::LoadTextureFile(std::string hPath, Texture* texture) {
    if (!sdl_initialised) {
	uint32_t flags = IMG_INIT_PNG | IMG_INIT_JPG;
	uint32_t tempFlag = IMG_Init(flags);
	if (tempFlag != flags) {
	    std::cout << "Can't initalize on sdl image " << std::endl;
	    std::cout << "Reason given: " << IMG_GetError() << std::endl;
	}
    }
    uint32_t pixelFormat = SDL_PIXELFORMAT_RGBA8888;
    SDL_Surface* loadedImage = IMG_Load(hPath.c_str());
    if (loadedImage == nullptr) {
	std::cout << "Can't load the image \n "
		  << "Reason Given: " << IMG_GetError() << std::endl;
    }
    SDL_Surface* formattedImage;
    formattedImage = SDL_ConvertSurfaceFormat(loadedImage, pixelFormat, 0);
    if (formattedImage == NULL)
	std::cout << "Can't convert to different format \n"
		  << "Reason Given: " << IMG_GetError() << std::endl;
    SDL_LockSurface(formattedImage);
    texture->width = formattedImage->w;
    texture->height = formattedImage->h;

    texture->data = scene->resourceBank->Push_Back(
	reinterpret_cast<uint8_t*>(formattedImage->pixels),
	texture->width * texture->height * formattedImage->pitch);
    if (texture == nullptr)
	std::cout << "Texture is nullptr in the function " << std::endl;
}

void AssetLoader::LoadTextFile(std::string filePath, std::string& source) {
    std::ifstream fin;
    fin.open(filePath.c_str(), std::ios::in);
    if (!fin)
	throw CException(
	    __LINE__, __FILE__, "Text file Loader",
	    std::string("Couldn't open the file path: ") + filePath);
    std::stringstream ss;
    ss << fin.rdbuf();
    source = ss.str();
}

