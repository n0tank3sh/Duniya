#pragma once
#include <cinttypes>
#include <ECS/ECS.h>
#include <ECS/GraphicsComponent.h>
#include <unordered_set>
#include "Graphics/Renderer.h"

template<typename T>
struct RectComp
{
	bool operator()(const T& a, const T& b) const
	{
		return a[0] < b[0];
	}
};

namespace ComponentTypes
{
enum  : uint32_t 
	{
		FONTDICT = 0x59
	};
};

struct Glyph
{
	Vect4 uv;
	Vect2 pos;
	Vect2 advance;
};

struct FontDict
{
	Glyph glyps[127];
	Texture texture;
	GBuffer gBuffer;
};

class TexturePacker
{
private:
	uint32_t height, width, margin;
	Scene* scene;
	Logger* logger;
	std::priority_queue<std::array<uint32_t, 3>, std::vector<std::array<uint32_t, 3>>, RectComp<std::array<uint32_t, 3>>> rects;
	std::unordered_map<uint32_t, std::vector<uint32_t>> datas;

public:
	enum class Heuristic
	{
		Row, Col
	} heuristic;
private:
	template<typename T, typename K>
	void Util(T& rects, K& guide, uint8_t* data, Heuristic heuristic = Heuristic::Col);
public:
	TexturePacker(uint32_t width = 0, uint32_t height = 0, Scene* scene = nullptr);
	void SetColumnSize(uint32_t colSize = 0);
	void SetRowSize(uint32_t rowSize = 0);
	void SetScene(Scene* scene);
	Scene* GetScene();
	void AddTexture(uint32_t id);
	int32_t PackFont(std::string fontFile, FontDict& dict);
	uint32_t Pack();
};
