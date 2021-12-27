#include "AssetLoader.h"
#include "ECS/ECS.h"
#include "Graphics/Renderer.h"
#include <Renderer2DSystem.h>
#include <Graphics/OpenGL/GLRenderer.h>
#include <queue>



Renderer2DSystem* Renderer2DSystem::singleton = nullptr;


Renderer2DSystem::Renderer2DSystem()
{
	messageID = 0x35;
	renderer = new GLRenderer();

	shaderStageHandler.reset(renderer->CreateShaderStage());	
	fontShaderStageHandler.reset(renderer->CreateShaderStage());

	std::unique_ptr<NativeShaderHandlerParent> vertShader(renderer->CreateShader(ShaderType::VERTEX)), 
											   fragShader(renderer->CreateShader(ShaderType::FRAGMENT)),
											   fontVertShader(renderer->CreateShader(ShaderType::VERTEX)),
											   fontFragShader(renderer->CreateShader(ShaderType::FRAGMENT));

	AssetLoader::GetSingleton()->LoadTextFile("Resource/Shaders/RectVert.glsl", vertShader->source);
	AssetLoader::GetSingleton()->LoadTextFile("Resource/Shaders/RectFrag.glsl", fragShader->source);
	AssetLoader::GetSingleton()->LoadTextFile("Resource/Shaders/FontFrag.glsl", fontFragShader->source);
	AssetLoader::GetSingleton()->LoadTextFile("Resource/Shaders/FontVert.glsl", fontVertShader->source);
	
	shaderStageHandler->shaderHandler.push_back(std::move(vertShader));
	shaderStageHandler->shaderHandler.push_back(std::move(fragShader));
	shaderStageHandler->Load();
	//VertexSpecification vertexSpecification;
	//vertexSpecification.push_back(std::make_pair("goat", 2));
	//layout = renderer->AddSpecification(vertexSpecification);
	//uint32_t vertCount = 4, indexCount = 6;
	//auto vert = new Vect2[vertCount];
	//vert[0] = {-1.f, -1.f};
	//vert[1] = {-1.f, 1.f};
	//vert[2] = {1.f, -1.f};
	//vert[3] = {1.f, 1.f}; 

	//auto& style = panel.vertBuffer.bufferStyle;
	//style.cpuFlags = GBuffer::GBufferStyle::CPUFlags::STATIC;
	//style.type = GBuffer::GBufferStyle::BufferType::VERTEX;
	//style.usage = GBuffer::GBufferStyle::Usage::DRAW;
	//panel.vertBuffer.count = vertCount;
	//panel.vertBuffer.data = scene->resourceBank->Push_Back(reinterpret_cast<uint8_t*>(vert), sizeof(Vect2) * vertCount);
	//panel.vertBuffer.sizet = vertCount * sizeof(Vect2);
	//renderer->LoadBuffer(&panel.vertBuffer);
	//renderer->SetLayout(layout);

}

Renderer2DSystem* Renderer2DSystem::Init()
{
	auto tmp = new Renderer2DSystem;
	return tmp;
}

Renderer2DSystem* Renderer2DSystem::GetSingleton()
{
	return singleton;
}

void Renderer2DSystem::LoadScene(Scene* scene)
{
	shaderStageHandler->Load();
	this->scene = scene;
	Scan();


}

void Renderer2DSystem::Scan()
{
	uint32_t numPanel = 0;
	panels.clear();
	for(int i = 0; i < scene->entities.size(); i++)
	{
		auto& componentList = scene->entities[i];
		if(componentList->Get(ComponentTypes::PANEL) != nullptr)
		{
			panels.push_back(i);
		}
	}
}

void Renderer2DSystem::Add(uint32_t entity)
{
	panels.push_back(entity);
}

void Renderer2DSystem::ProcessMessages()
{
	auto& messages = messagingSystem->at(messageID);
	while(!messages.empty())
	{
		auto message = std::move(messages.front());
		messages.pop_front();
		switch(message.first)
		{
			case 0:
				Scan();
				break;
			case 1:
				auto addMessage = dynamic_cast<AddMessage*>(message.second.get());
				Add(addMessage->entity);
				break;
		};
	}
}

void Renderer2DSystem::update(float deltaTime)
{
	ProcessMessages();
	shaderStageHandler->Load();
	for(auto i = 0; i < panels.size(); i++)
	{
		auto& componentList = scene->entities[panels[i]];
		if(componentList->Get(ComponentTypes::PANEL) != nullptr)
		{
			std::string panelIndex = "[" + std::to_string(i) + "]";
			auto panel = reinterpret_cast<Panel*>(componentList->Get(ComponentTypes::PANEL));
			renderer->Uniform4f(1, &panel->dimension, "panels" + panelIndex);
			renderer->Uniform4f(1, &panel->color, "panelColors" + panelIndex);
			renderer->Uniform1f(1, &panel->sideDist, "panelCorners" + panelIndex);
		}
		else
		{
			std::swap(panels[i], panels[panels.size() - 1]);
			panels.pop_back();
		}
	}
	
	if(panels.size() != 0)
	{
		renderer->Clear();
		renderer->DrawInstancedArrays(DrawPrimitive::TRIANGLES_STRIPS, nullptr, 4, panels.size());
	}
}
