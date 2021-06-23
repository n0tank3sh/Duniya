#include "AssetLoader.h"
#include <sstream>
#include <SDL2/SDL_image.h>
#include <string>
#include <fstream>
#include <iterator>

AssetLoader* AssetLoader::singleton = nullptr;
bool AssetLoader::sdl_initialised = false;

AssetLoader* AssetLoader::init()
{
    singleton = new AssetLoader();
    return singleton;
}

AssetLoader* AssetLoader::GetSingleton() 
{
    return singleton;
}

void AssetLoader::LoadObj(std::string filePath, Mesh* mesh)
{
    uint32_t indexOfIndex = 0;
    if(mesh == nullptr) mesh = new Mesh();
    std::ifstream fin(filePath);
    std::string line;
    std::vector<Vect4> aPos;
    std::vector<Vect3> vNormal;
    std::vector<Vect3> texCord;
    while(std::getline(fin, line))
    {
        std::stringstream lineStream(line);
        std::string firstTok;
        lineStream >> firstTok;
        if(firstTok == "vt")
        {
            Vect3 texCoord;
            lineStream >> texCoord.x >> texCoord.y >> texCoord.z;
            texCord.push_back(texCoord);
        }
        else if(firstTok == "vn")
        {
            Vect3 vertNormal;
            lineStream >> vertNormal.x >> vertNormal.y >> vertNormal.z;
            vNormal.push_back(vertNormal);
        }
        else if(firstTok == "v")
        {
            Vect4 vertPos;
            lineStream >> vertPos.x >> vertPos.y >> vertPos.z >> vertPos.w;
            aPos.push_back(vertPos);
        }
        else if(firstTok == "f")
        {
            std::string vS;
            Vertex temp;
            bool normFlag = 0;
            do
            {
                vS.clear();
                lineStream >> vS;
                std::istringstream v(vS);
                uint32_t index;
                v >> index;
                temp.aPos = aPos[index - 1];
                v.ignore();
                if(v.peek() == '/')
                    normFlag = true;
                v >> index;
                temp.aNormal = vNormal[index - 1]; 
                v.ignore();
                if(v.eof())
                    temp.texCord = Vect3();
                else
                {
                    v >> index;
                    temp.texCord = texCord[index -1];
                }
                mesh->verticies.push_back(temp);
                mesh->indicies.push_back(indexOfIndex);
                indexOfIndex++;

            } while(lineStream.eof());
        }
    }
}

void AssetLoader::LoadTextureFile(std::string hPath, Texture* texture)
{
    if(!sdl_initialised)
    {
        uint32_t flags = IMG_INIT_PNG | IMG_INIT_JPG;
        uint32_t tempFlag = IMG_Init(flags);
        if(tempFlag != flags)
        {
            std::cout << "Can't initalize on sdl image " << std::endl;
            std::cout << "Reason given: " << IMG_GetError() << std::endl;
        }
    }
    uint32_t pixelFormat = SDL_PIXELFORMAT_RGBA8888;
    SDL_Surface* loadedImage = IMG_Load(hPath.c_str());
    if(loadedImage == nullptr) 
    {
        std::cout << "Can't load the image \n " 
         << "Reason Given: " << IMG_GetError() << std::endl;
    }
    SDL_Surface* formattedImage;
    formattedImage  = SDL_ConvertSurfaceFormat(loadedImage, pixelFormat, 0);
    if(formattedImage == NULL)
        std::cout << "Can't convert to different format \n"
            << "Reason Given: " << IMG_GetError() << std::endl;
    SDL_LockSurface(formattedImage);
    texture->type = Texture::Type::T2D;
    texture->width = formattedImage->w;
    texture->height = formattedImage->h;
    texture->sizet = formattedImage->h * formattedImage->pitch;

    texture->data = new uint8_t[texture->sizet];
    uint8_t* pixels  = (uint8_t*)formattedImage->pixels;
    std::copy(pixels, pixels + texture->sizet, texture->data);
    if(texture == nullptr) std::cout << "Texture is nullptr in the function " << std::endl;
}

void AssetLoader::LoadTextFile(std::string filePath, std::string& source)
{
    std::ifstream fin(filePath);
    std::istream_iterator<char> fileIterator(fin), eos;
    std::copy(fileIterator, eos, std::back_inserter(source));
}

