#include "RendererSystem.h"
#include "Graphics/opengl/GLRenderer.h"
#include <ecs/commoncomponent.h>
#include <AssetLoader.h>

RendererSystem* RendererSystem::singleton = nullptr;

RendererSystem::RendererSystem()
{
	animated = .0f;
}

RendererSystem* RendererSystem::init(Graphics_API graphicsAPI)
{
    singleton = new RendererSystem();
    singleton->renderer.reset(new GLRenderer());
	std::string vertexShaderSrc, fragShaderSrc;
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
void RendererSystem::CreateGBufferMesh(Mesh* mesh, GBuffer* iBuffer, GBuffer* vBuffer)
{
    GBuffer* vertexBuffer =  new GBuffer;
    GBuffer* indexBuffer = new GBuffer;

    vertexBuffer->bufferStyle.cpuFlags = GBuffer::GBufferStyle::CPUFlags::STATIC; 
    vertexBuffer->bufferStyle.type = GBuffer::GBufferStyle::BufferType::VERTEX;
    vertexBuffer->bufferStyle.usage = GBuffer::GBufferStyle::Usage::DRAW;
    vertexBuffer->data = mesh->verticies->data();
    vertexBuffer->count = mesh->verticies->size();
    vertexBuffer->sizet = mesh->verticies->size() * sizeof(Vertex);
    std::cout << vertexBuffer->sizet << " is the total Size" << std::endl;

    indexBuffer->bufferStyle.cpuFlags = GBuffer::GBufferStyle::CPUFlags::STATIC;
    indexBuffer->bufferStyle.type = GBuffer::GBufferStyle::BufferType::INDEX;
    indexBuffer->bufferStyle.usage = GBuffer::GBufferStyle::Usage::DRAW;
    indexBuffer->data = mesh->indicies->data();
    indexBuffer->count = mesh->indicies->size();
    indexBuffer->sizet = mesh->indicies->size() * sizeof(uint32_t);
    std::cout << indexBuffer->sizet << " is the total size " << std::endl;

    iBuffer = indexBuffer;
    vBuffer = vertexBuffer;
}
void RendererSystem::LoadScene(Scene* scene)
{
//	renderer->ClearColor(.0f, 1.f, 0.5f);
    this->scene = scene;
    Mesh* mesh;
	RendererStuff* rendererStuff;
	Texture* texture;
	std::cout << scene->entities.size() << std::endl;
    for(auto& i: scene->entities)
    {
		Scene::IComponentArray* componentArray;
		componentArray = i.second.get();
		auto meshItr = componentArray->components.find(ComponentType::MESH);
		if(meshItr != componentArray->components.end())
		{
		}
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
        	rendererStuff->texture->Bind();
        	Mat* mat = ConvertTranforToMatrix(*((Transform*)i.second->get<ComponentType::TRANSFORM>()));
        	renderer->UniformMat(*mat, "MVP");
        	renderer->Draw(rendererStuff->iBuffer);
		}
    }
	animated += .01f;
}
