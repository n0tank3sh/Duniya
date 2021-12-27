#pragma once
#include <cstdint>
#include <ECS/ECS.h>
#include <Graphics/Renderer.h>


namespace ComponentTypes
{
		enum : uint32_t
	   {
		   PANEL = 20,
	   };
};

struct Panel
{
	Vect4 dimension;
	Vect4 color;
	float sideDist;
};

struct Button
{
	uint32_t id;
};



class Renderer2DSystem : public System
{
	struct AddMessage : public Message
	{
		uint32_t entity;
		AddMessage(uint32_t entity);
	};
public:
	void LoadScene(Scene* scene) override;
	void update(float deltaTime) override;
	friend class RendererSystem;
	static Renderer2DSystem* Init();
	static Renderer2DSystem* GetSingleton();
private:
	Renderer2DSystem();
public:
	static Renderer2DSystem* singleton;
	Renderer* renderer;
private:
	std::unique_ptr<ShaderStageHandler> shaderStageHandler;
	std::unique_ptr<ShaderStageHandler> fontShaderStageHandler;
	void ProcessMessages();
	void Scan();
	void Add(uint32_t);
	Scene* scene;
	uint32_t layout;
	std::vector<uint32_t> panels;
};
