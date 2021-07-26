#include "RendererSystem.h"
#include "Graphics/opengl/GLRenderer.h"
#include <ecs/commoncomponent.h>
#include <AssetLoader.h>

RendererSystem* RendererSystem::singleton = nullptr;

RendererSystem::RendererSystem()
	:
		transformDefault({4, 4})
{
	animated = .0f;
	for(uint32_t i = 0; i < 4; i++)
	{
		transformDefault.buffer.get()[i * 4 + i] = .5f;
	}
	transformDefault.buffer.get()[4 * 4 - 1] = 1.f;
}

RendererSystem* RendererSystem::init(Graphics_API graphicsAPI)
{
    singleton = new RendererSystem();
    singleton->renderer.reset(new GLRenderer());
	std::string vertexShaderSrc, fragShaderSrc;
	if(AssetLoader::GetSingleton() == nullptr)
	AssetLoader::init();
	AssetLoader* assLoader = AssetLoader::GetSingleton();
	assLoader->LoadTextFile("Resource/Shaders/VertexShader.glsl", vertexShaderSrc);
	assLoader->LoadTextFile("Resource/Shaders/FragmentShader.glsl", fragShaderSrc);
	singleton->renderer->AddShader(ShaderType::VERTEX, vertexShaderSrc);
	singleton->renderer->AddShader(ShaderType::FRAGMENT, fragShaderSrc);
	VertexSpecification specification  = { {"pos", 4}, {"aNormal", 3}, {"texCoord", 3}};
	singleton->renderer->SetLayout(specification);
    return singleton;
}

RendererSystem* RendererSystem::GetSingleton()
{
    return singleton;
}
void RendererSystem::CreateGBufferMesh(Mesh* mesh, GBuffer* indexBuffer, GBuffer* vertexBuffer)
{
	vertexBuffer->bufferStyle.cpuFlags = GBuffer::GBufferStyle::CPUFlags::STATIC; 
    vertexBuffer->bufferStyle.type = GBuffer::GBufferStyle::BufferType::VERTEX;
    vertexBuffer->bufferStyle.usage = GBuffer::GBufferStyle::Usage::DRAW;
    vertexBuffer->data = mesh->verticies;
    vertexBuffer->count = mesh->vertexCount;
    vertexBuffer->sizet = mesh->vertexCount * sizeof(Vertex);

    indexBuffer->bufferStyle.cpuFlags = GBuffer::GBufferStyle::CPUFlags::STATIC;
    indexBuffer->bufferStyle.type = GBuffer::GBufferStyle::BufferType::INDEX;
    indexBuffer->bufferStyle.usage = GBuffer::GBufferStyle::Usage::DRAW;
    indexBuffer->data = mesh->indicies;
    indexBuffer->count = mesh->indexCount;
    indexBuffer->sizet = mesh->indexCount * sizeof(uint32_t);
}
void RendererSystem::LoadScene(Scene* scene)
{
//	renderer->ClearColor(.0f, 1.f, 0.5f);
    this->scene = scene;
    Mesh* mesh;
	Texture* texture;
	std::cout << "Entities size: " << scene->entities.size() << std::endl;
    for(auto& i: scene->entities)
    {
		Scene::IComponentArray* componentArray;
		componentArray = i.second.get();
		Mesh* mesh = (Mesh*)componentArray->components.find(ComponentType::MESH)->second.base->GetPointer();
		componentArray->components.insert(std::make_pair(ComponentType::RENDERERSTUFF, ComponentPtr(new ComponentPtr::Impl<RendererStuff>())));
		componentArray->components[ComponentType::RENDERERSTUFF].base->Create();
		RendererStuff* rendererStuff = (RendererStuff*)componentArray->components[ComponentType::RENDERERSTUFF].base->GetPointer();
		rendererStuff->iBuffer = new GBuffer;
		rendererStuff->vBuffer = new GBuffer;
		CreateGBufferMesh(mesh, rendererStuff->iBuffer, rendererStuff->vBuffer);
		renderer->LoadBuffer(rendererStuff->iBuffer);
		renderer->LoadBuffer(rendererStuff->vBuffer);
		//if(meshItr != componentArray->components.end())
		//{
		//}
		Vertex* vertex = static_cast<Vertex*>(rendererStuff->vBuffer->data);
		std::cout << "Vertex Buffer: " << std::endl;
		for(uint32_t i = 0; i < rendererStuff->vBuffer->count; i++)
		{
			std::cout << vertex[i].aPos << " " << vertex[i].aNormal << " " << vertex[i].texCord << std::endl;
		}
		std::cout << "Index Buffer: " << std::endl;
		for(uint32_t i = 0; i < rendererStuff->iBuffer->count; i++)
			std::cout << static_cast<uint32_t*>(rendererStuff->iBuffer->data)[i] << " ";
		std::cout << std::endl;

    }
}

void RendererSystem::update(float deltaTime)
{
//	renderer->ClearColor(.0f, 1.f, 0.5f);
	//std::cout << "Entering the renderering " << std::endl;
	renderer->ClearColor(std::abs(std::cos(animated)/100 * deltaTime) , std::abs(std::sin(animated)/10 * deltaTime)  , .5f);
	renderer->Clear();
    RendererStuff* rendererStuff;
    for(auto& i: scene->entities)
    {
		auto rendererStuffitr = i.second->components.find(ComponentType::RENDERERSTUFF);
		if( rendererStuffitr != i.second->components.end())
		{
        	rendererStuff = (RendererStuff*)rendererStuffitr->second.base->GetPointer();
        	rendererStuff->vBuffer->Bind();
        	rendererStuff->iBuffer->Bind();
        	//rendererStuff->texture->Bind();
			Mat* mat = nullptr;
			Transform* transform = static_cast<Transform*>(i.second->get<ComponentType::TRANSFORM>());
			if(transform != nullptr)
			mat = ConvertTranforToMatrix(*transform);
			else
			{
				mat = &transformDefault;
		//		std::cout << "Loading the default mvp" << std::endl;
		//		std::cout << "Size of the default transform " << mat->sizet << std::endl;
			}
		//	std::cout << *mat << std::endl;
        	renderer->UniformMat(*mat, "MVP");
			
        	renderer->Draw(rendererStuff->iBuffer);
		}
    }
	animated += .01f;
}
