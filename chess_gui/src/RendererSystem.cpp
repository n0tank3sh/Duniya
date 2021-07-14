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
		transformDefault.buffer.get()[i * 4 + i] = 1.f;
	}
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
    }
}

void RendererSystem::update(float deltaTime)
{
//	renderer->ClearColor(.0f, 1.f, 0.5f);
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
			try
			{
				mat = ConvertTranforToMatrix(*((Transform*)i.second->get<ComponentType::TRANSFORM>()));
			}
			catch(TypeNotFoundException& e)
			{
//				std::cout << "Are we using this " << std::endl;
				mat = &transformDefault;
//				for(uint32_t i = 0; i < mat->sizet; i++)
//				{
//					std::cout << mat->buffer.get()[i] << " ";
//					if((i % mat->dimension.row) == 0) std::cout << std::endl;
//				}
			}
        	//renderer->UniformMat(*mat, "MVP");
			
        	renderer->Draw(rendererStuff->iBuffer);
		}
    }
	animated += .01f;
}
