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
void RendererSystem::CreateGBufferMesh(Mesh* mesh)
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

    mesh->indexBuffer = indexBuffer;
    mesh->vertexBuffer = vertexBuffer;
}
void RendererSystem::LoadScene(Scene* scene)
{
//	renderer->ClearColor(.0f, 1.f, 0.5f);
    this->scene = scene;
    Mesh* mesh;
	std::cout << scene->entities.size() << std::endl;
    for(auto& i: scene->entities)
    {
        mesh = (Mesh*)i.second->get<ComponentType::MESH>();
		if(mesh->vertexBuffer == nullptr && mesh->indexBuffer == nullptr)
			this->CreateGBufferMesh(mesh);
        renderer->LoadBuffer(mesh->vertexBuffer);
        renderer->LoadBuffer(mesh->indexBuffer);
        renderer->LoadTexture(mesh->texture);
		std::cout << "The index buffer size is %3 : " << (mesh->indicies->size() % 3 == 0?"TRUE": "FALSE") << std::endl;
    }
}

void RendererSystem::update(float deltaTime)
{
//	renderer->ClearColor(.0f, 1.f, 0.5f);


	renderer->ClearColor(std::abs(std::cos(animated)/100 * deltaTime) , std::abs(std::sin(animated)/10 * deltaTime)  , .5f);
	renderer->Clear();
    Mesh* mesh;
    for(auto& i: scene->entities)
    {
        mesh = (Mesh*)i.second->get<ComponentType::MESH>();
        mesh->vertexBuffer->Bind();
        mesh->indexBuffer->Bind();
        mesh->texture->Bind();
        Mat* mat = ConvertTranforToMatrix(*((Transform*)i.second->get<ComponentType::TRANSFORM>()));
        renderer->UniformMat(*mat, "MVP");
        renderer->Draw(mesh->indexBuffer);
    }
	animated += .01f;
}
