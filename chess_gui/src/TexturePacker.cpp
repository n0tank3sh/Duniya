
#include "TexturePacker.hpp"
#include "ECS/ECS.hpp"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <queue>
#include <array>
#include <stdexcept>
#include <string>
#include <vector>
#include <freetype2/ft2build.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include FT_FREETYPE_H

TexturePacker::TexturePacker(uint32_t width, uint32_t height, Scene* scene)
	:
	width(width), height(height), scene(scene)
{}

void TexturePacker::AddTexture(uint32_t entity)
{
	auto k = scene->GetEntity(entity);
	if(k == nullptr)
		throw std::exception();
	auto l = reinterpret_cast<Texture*>(k->Get(ComponentTypes::TEXTURE));
	if(l == nullptr)
		throw std::exception();

	if(datas.count(l->data))
	{
		datas[l->data].push_back(entity);
		return;
	}
	datas[l->data].push_back(entity);
	switch(heuristic)
	{
		case Heuristic::Col:
			rects.push({l->width, l->height, l->data});
			break;
		case Heuristic::Row:
			rects.push({l->height, l->width, l->data});
			break;
	};
}

void TexturePacker::SetScene(Scene* scene)
{
	this->scene = scene;
}
Scene* TexturePacker::GetScene()
{
	if(scene == nullptr)
		throw std::runtime_error("Texture Packer scene is not set");
	return this->scene;
}

//template<typename T, typename K>
//void TexturePacker::Util(T& rects, K& guide, uint8_t* data, Heuristic heuristic)
//{
//	while(!rects.empty())
//	{
//		auto k = rects.top(); rects.pop();
//		auto tmpData = scene->resourceBank->resources[k[2]].Get();
//		int j = 0;
//		auto width = 0u;
//		auto height = 0u;
//		switch(heuristic)
//		{
//			case Heuristic::Col:
//				width = k[0];
//				height = k[1];
//				break;
//			case Heuristic::Row:
//				width = k[1];
//				height = k[0];
//				break;
//		}
//		while((guide[j] + width) > colSize)
//			j++;
//
//		for(int i = 0; i < height; i++)
//		{
//			if((i + j) >= rowSize)
//				throw std::exception();
//			auto plusMargin = data + guide[i] + margin + (colSize * (j + i + margin));
//			std::copy(tmpData, tmpData + width, plusMargin); 
//		}
//	}
//}


template <typename T, typename U>
uint8_t* PacKToBuffer(std::priority_queue<std::pair<T, T>, U>& rects, std::vector<Vect4>& uvs, 
		std::function<uint8_t*(const U&)>& retrieveFunction, std::function<uint64_t(const U&)>& toUV,
		const uint64_t& width, const uint64_t& height, uint8_t channel)
{
	auto atlasPtr = new uint8_t[width * height * channel];
	std::vector<std::vector<bool>> guide(width, std::vector<bool>(height, false));
	while(!rects.empty())
	{
		auto k = rects.top();
		rects.pop();
		auto glympBitmap = retrieveFunction(k.second);
		auto chWidth = k.first.first;
		auto chHeight = k.first.second;
		int start = -1;
		int32_t alignment = -1;
		int32_t margin = 3;
		auto mWidth = chWidth + 2 * margin;
		auto mHeight = chHeight + 2 * margin;
		for(int i = 0; i < height - mHeight; i++)
		{
			for(int j = 0; j < width - mWidth; j++)
			{
				start = i;
				alignment = j;
				int a = 0;
				int b = 0;
				for(; a < mHeight; a++)
				{
					b = 0;
					for(; b < mWidth; b++)
					{
						if(guide[i + a][b + j])
						{
							start = -1;
							alignment = -1;
							break;
						}
					}
					if(guide[i + a][b + j]) break;
				}
				if(a == mHeight && b == mWidth)
				{
					i = height;
					j = width;
				}
			} 
		}
		//if((end - j) < chHeight) break;
		if(start == -1 || alignment == -1)
			return nullptr;
		start += margin;
		alignment += margin;
		for(int i = 0; i < chHeight; i++)
		{
			auto already = chWidth * i;
			auto alreadyData = width * (start + i);
			std::copy(glympBitmap + already, glympBitmap + already + chWidth, atlasPtr + alreadyData  + alignment);
			for(int a = 0; a < chWidth; a++)
				guide[start + i][alignment + a] = true;
		}
		auto high = (float)chHeight/(float)height;
		uvs[toUV(k.second)] = Vect4((float)alignment/(float)width, (float)start/(float)height + high, 
				(float)chWidth/(float)width, -high); 
	}
}
uint32_t TexturePacker::Pack()
{
	auto data = (uint8_t*)std::malloc(height * width);
	std::unordered_map<uint32_t, uint8_t*> update;
	std::vector<int> guide(height, 0);
	while(!rects.empty())
	{
		auto k = rects.top(); rects.pop();
		auto tmpData = scene->resourceBank->resources[k[2]].Get();
		int j = 0;
		auto texWidth = 0u;
		auto texHeight = 0u;
		switch(heuristic)
		{
			case Heuristic::Col:
				texWidth = k[0];
				texHeight = k[1];
				break;
			case Heuristic::Row:
				texWidth = k[1];
				texHeight = k[0];
				break;
		}
		while((guide[j] + texWidth) > width)
			j++;

		for(int i = 0; i < texHeight; i++)
		{
			if((i + j) >= height)
				throw std::exception();
			auto plusMargin = data + guide[i] + margin + (width * (j + i + margin));
			std::copy(tmpData, tmpData + texWidth, plusMargin); 
		}
	}
	return 0;
}

int32_t TexturePacker::PackFont(std::string fontFile, FontDict& dict, uint32_t fontSize)
{
	//std::priority_queue<std::array<int32_t, 3>, std::vector<std::array<int32_t, 3>>, RectComp<std::array<int32_t, 3>>> rects;
	auto scene = GetScene();
	std::priority_queue<std::array<int32_t, 3>, std::vector<std::array<int32_t, 3>>> rects;
	std::vector<std::vector<bool>> guide(height, std::vector<bool>(width, false));
	auto data = new uint8_t[height * width];
	std::memset(data, 0x0, height * width);
	FT_Library library;
	FT_Init_FreeType(&library);
	FT_Face face = nullptr;
	{
		auto error = FT_New_Face(library, fontFile.c_str(), 0, &face);
		switch(error)
		{
			case FT_Err_Unknown_File_Format:
				throw std::runtime_error("unknown font file format");
				break;
			case 0:
				break;
			default:
				throw std::runtime_error("something went wrong during font file \n error no:" + std::to_string(error));
		};
	}
	if(face == nullptr)
		std::runtime_error("face is null");
	FT_Set_Pixel_Sizes(face, fontSize, 0);
	dict.fontSize = 32;
	auto glyph = face->glyph;
	uint8_t* buffers[127];
	{
		uint32_t chIndex = 0;
		char c = FT_Get_First_Char(face, &chIndex);
		while(chIndex)
		{
			FT_Load_Glyph(face, chIndex, FT_LOAD_RENDER);
			auto glymp = face->glyph;
			rects.push({(int)glymp->bitmap.width, (int)glymp->bitmap.rows, (int32_t)c});
			c = FT_Get_Next_Char(face, c, &chIndex);
		}
	}
	while(!rects.empty())
	{
		auto k = rects.top();
		rects.pop();
		char ch = k[2];
		FT_Load_Char(face, ch, FT_LOAD_RENDER);
		auto glympBitmap = glyph->bitmap.buffer;
		auto chWidth = glyph->bitmap.width;
		auto chHeight = glyph->bitmap.rows;
		int start = -1;
		int32_t alignment = -1;
		int32_t margin = 3;
		auto mWidth = chWidth + 2 * margin;
		auto mHeight = chHeight + 2 * margin;
		for(int i = 0; i < height - mHeight; i++)
		{
			for(int j = 0; j < width - mWidth; j++)
			{
				start = i;
				alignment = j;
				int a = 0;
				int b = 0;
				for(; a < mHeight; a++)
				{
					b = 0;
					for(; b < mWidth; b++)
					{
						if(guide[i + a][b + j])
						{
							start = -1;
							alignment = -1;
							break;
						}
					}
					if(guide[i + a][b + j]) break;
				}
				if(a == mHeight && b == mWidth)
				{
					i = height;
					j = width;
				}
			} 
		}
		//if((end - j) < chHeight) break;
		if(start == -1 || alignment == -1)
			return -1;
		start += margin;
		alignment += margin;
		for(int i = 0; i < chHeight; i++)
		{
			auto already = chWidth * i;
			auto alreadyData = width * (start + i);
			std::copy(glympBitmap + already, glympBitmap + already + chWidth, data + alreadyData  + alignment);
			for(int a = 0; a < chWidth; a++)
				guide[start + i][alignment + a] = true;
		}
		auto high = (float)chHeight/(float)height;
		dict.glyps[ch].uv = Vect4((float)alignment/(float)width, (float)start/(float)height + high, 
				(float)chWidth/(float)width, -high); 
		
		dict.glyps[ch].bearings.x = (float)glyph->metrics.horiBearingX/64;
		dict.glyps[ch].bearings.y = (float)glyph->metrics.horiBearingY/64;
		dict.glyps[ch].pos = Vect2(chWidth, chHeight);
		dict.glyps[ch].advance.x = (float)glyph->advance.x / 64.f;
		dict.glyps[ch].advance.y = (float)glyph->advance.y / 64.f;
	}
	dict.texture.height = height;
	dict.texture.width = width;
	dict.texture.format = Texture::Format::R;
	dict.texture.channels = 1;
	dict.texture.data = scene->resourceBank->Push_Back(data, width * height);
	//uint32_t entity =  scene->Push();
	//auto fontDict = scene->GetEntity(entity)->Emplace<FontDict>(0);
	//std::copy(&dict, &dict + sizeof(FontDict), fontDict); 
	stbi_write_png("fontBitmap.png", width, height, 1, (*scene->resourceBank).resources[dict.texture.data].Get(), width);
	return 0;
}

