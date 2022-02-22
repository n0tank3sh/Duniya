#include "AssetLoader.h"
#include "ECS/ECS.h"
#include "Graphics/Renderer.h"
#include "Math/Vect4.h"
#include <Renderer2DSystem.h>
#include <Graphics/OpenGL/GLRenderer.h>
#include <exception>
#include <filesystem>
#include <fstream>
#include <queue>
#include <stdexcept>
#include <SDLUtiliy.h>




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
	resolution.x = 1400;
	resolution.y = 900; 
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
	TexturePacker texturePacker(256, 256);
	texturePacker.SetScene(scene);
	std::string fontFile("/home/mightygoat/B/Projects/chess_gui/chess_gui/Resource/Fonts/myFont.otf"); 
	if(!std::filesystem::exists(fontFile))
	{
		throw std::runtime_error("Font File doesn't exist");
	}
	if(texturePacker.PackFont(fontFile, defaultFont))
	{
		throw std::runtime_error("Couldn't pack font");
	}
	renderer->SetResourceBank(scene->resourceBank);
	renderer->LoadTexture(&defaultFont.texture, &defaultFont.gBuffer);
	this->scene = scene;
	texts.push_back(scene->PushDef());
	auto& panel = *scene->GetEntity(texts.back())->Emplace<TextPanel>(ComponentTypes::TEXTPANEL);
	auto& str = *scene->GetEntity(texts.back())->Emplace<std::string>(ComponentTypes::TEXTBOX);
	str = "hello world";
	panel.dimension = Vect4(0, 0, (float)(32 * str.size())/resolution.x * 2, (float)32/resolution.y * 2);
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

void Renderer2DSystem::Update(float deltaTime)
{
	ProcessMessages();
	shaderStageHandler->Load();
	renderer->Enable(Options::BLEND);
	renderer->Disable(Options::DEPTH_TEST);
	renderer->Disable(Options::FACE_CULL);
	
	renderer->ClearColor(0.f, 0.f, 0.f);
	renderer->Clear();
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
	goat = 0;
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
			//auto uv = Vect4(0, 0, 1, 1);
			auto glyphPos = Vect4(curPos, temp.pos/resolution * 2);
			glyphPos.y = panel.dimension.y;
			auto luft = "[" + std::to_string(goat) + "]";
			renderer->Uniform4f(1, &glyphPos, "pos" + luft);
			renderer->Uniform4f(1, &uv, "uvs" + luft);
			//renderer->Uniform4f(1, &panel.dimension, "boxPositions" + luft);
			goat++; 
			if(goat == batchSize)                                       
			{
				renderer->DrawInstancedArrays(DrawPrimitive::TRIANGLES_STRIPS, nullptr, 4, goat);
				goat = 0;
			}
			advanceY = std::max(temp.advance.y, advanceY); 
			curPos.x += lerp(0.f, 2.f, (float)temp.advance.x / resolution.x);
			if(curPos.x > panelPos.x + panelPos.z)
			{
				curPos.y -= lerp(0.f, 2.f, (float)advanceY / resolution.y);
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
