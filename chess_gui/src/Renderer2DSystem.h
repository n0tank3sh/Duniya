#pragma once
#include "Application.h"
#include <cstdint>
#include <ecs/ecs.h>
#include <Graphics/Renderer.h>


namespace ComponentTypes
{
		enum : uint32_t
	   {
		   PANEL = 20,
		   BACKGROUNDCOLOR
	   };
};

struct Panel
{
	float x;
	float y;
	float width;
	float height;
};

struct Button
{
	uint32_t id;
};

struct BackgroundColor
{
	Vect3 color;
};


class Renderer2DSystem : public System
{
public:
	void LoadScene(Scene* scene) override;
	void update(float deltaTime) override;
	friend class RendererSystem;
	static Renderer2DSystem* init();
	static Renderer2DSystem* GetSingleton();
private:
	Renderer2DSystem();
public:
	static Renderer2DSystem* singleton;
	Renderer* renderer;
private:
	std::unique_ptr<ShaderStageHandler> shaderStageHandler;
	Scene* scene;
	uint32_t layout;
	struct 
	{
		GBuffer* vertBuffer;
		GBuffer* iBuffer;
	} panel;
};
