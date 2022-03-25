#include "AssetLoader.hpp"
#include "ECS/ECS.hpp"
#include "Graphics/Renderer.hpp"
#include "Math/Vect4.hpp"
#include <Renderer2DSystem.hpp>
#include <Graphics/OpenGL/GLRenderer.hpp>
#include <exception>
#include <filesystem>
#include <fstream>
#include <queue>
#include <stdexcept>
#include <SDLUtiliy.hpp>




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
	fontShaderStageHandler->shaderHandler.push_back(std::move(fontVertShader));
	fontShaderStageHandler->shaderHandler.push_back(std::move(fontFragShader));
	fontShaderStageHandler->Load();
	shaderStageHandler->Load();
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
	TexturePacker texturePacker(256, 256, scene);
	std::string fontFile("Resource/Fonts/myFont.otf"); 
	if(!std::filesystem::exists(fontFile))
	{
		throw std::runtime_error("Font File doesn't exist");
	}
	if(texturePacker.PackFont(fontFile, defaultFont, 32))
	{
		throw std::runtime_error("Couldn't pack font");
	}
	renderer->SetResourceBank(scene->resourceBank);
	renderer->LoadTexture(&defaultFont.texture, &defaultFont.gBuffer);
	this->scene = scene;
	Scan();
}

void Renderer2DSystem::LoadFontFile(std::string fontFile)
{
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
		if(componentList->Get(ComponentTypes::TEXTPANEL) != nullptr)
		{
			texts.push_back(i);
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


void Renderer2DSystem::LoadPanels()
{	
	uint32_t goat = 0;
	for(auto i = 0; i < panels.size(); i++)
	{
		auto& componentList = scene->entities[panels[i]];
		if(componentList->Get(ComponentTypes::PANEL) != nullptr)
		{
			std::string panelIndex = "[" + std::to_string(goat) + "]";
			goat++;
			auto panel = reinterpret_cast<Panel*>(componentList->Get(ComponentTypes::PANEL));
			renderer->Uniform4f(1, &panel->dimension, "panels" + panelIndex);
			renderer->Uniform4f(1, &panel->color, "panelColors" + panelIndex);
			renderer->Uniform1f(1, &panel->sideDist, "panelCorners" + panelIndex);
			if(goat == 50)
			{
				renderer->DrawInstancedArrays(DrawPrimitive::TRIANGLES_STRIPS, nullptr, 4, goat);
				goat = 0;
			}
		}
		else
		{
			std::swap(panels[i], panels[panels.size() - 1]);
			panels.pop_back();
		}
	}
	if(goat != 0)
		renderer->DrawInstancedArrays(DrawPrimitive::TRIANGLES_STRIPS, nullptr, 4, goat);
}

void Renderer2DSystem::LoadFontGlyph()
{	
	uint32_t goat = 0;
	fontShaderStageHandler->Load();
	uint32_t batchSize = 10;
	renderer->Bind(defaultFont.gBuffer);
	//Vect4 uv(0.f, 0.f, 1.f, 1.f);
	for(int i = 0; i < texts.size(); i++)
	{
		auto& text = *scene->GetEntity(texts[i])->Get<std::string>(ComponentTypes::TEXTBOX);
		auto& panel = *scene->GetEntity(texts[i])->Get<TextPanel>(ComponentTypes::TEXTPANEL);
		auto curPos = Vect2(panel.dimension.x, panel.dimension.y + panel.dimension.w);
		auto& panelPos = panel.dimension;
		auto advanceY = 0.f;
		for(auto e: text)
		{
			auto& temp = defaultFont.glyps[e];
			auto uv = temp.uv;
			auto glyphPos = Vect4(curPos, settings->Normalize(temp.pos));
			glyphPos.y -= settings->NormalizeY(defaultFont.fontSize);
			auto luft = "[" + std::to_string(goat) + "]";
			renderer->Uniform4f(1, &glyphPos, "pos" + luft);
			renderer->Uniform4f(1, &uv, "uvs" + luft);
			renderer->Uniform4f(1, &panel.dimension, "boxPositions" + luft);
			goat++; 
			if(goat == batchSize)                                       
			{
				renderer->DrawInstancedArrays(DrawPrimitive::TRIANGLES_STRIPS, nullptr, 4, goat);
				goat = 0;
			}
			advanceY = std::max(temp.advance.y, advanceY); 
			curPos.x += settings->NormalizeX(temp.advance.x); 			
			if(curPos.x > panelPos.x + panelPos.z)
			{
				curPos.y -= settings->NormalizeY(temp.advance.y); 			
			}
			if(curPos.y < panelPos.y)
				break;
		}
	}
	if(goat != 0)
	{
		renderer->DrawInstancedArrays(DrawPrimitive::TRIANGLES_STRIPS, nullptr, 4, goat);
	}
}

void Renderer2DSystem::Update(float deltaTime)
{
	ProcessMessages();
	shaderStageHandler->Load();
	renderer->Enable(Options::BLEND);
	renderer->Disable(Options::DEPTH_TEST);
	renderer->Disable(Options::FACE_CULL);
	
	//renderer->ClearColor(0.f, 0.f, 0.f);
	//renderer->Clear();
	LoadPanels();
	LoadFontGlyph();

}
