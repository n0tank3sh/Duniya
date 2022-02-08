#include "RendererSystem.h"
#include "Application.h"
#include "ECS/CommonComponent.h"
#include "ECS/ECS.h"
#include "Graphics/Renderer.h"
#include "Graphics/OpenGL/GLRenderer.h"
#include <AssetLoader.h>
#include <chrono>
#include <iterator>
#include <string>

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
	camera = new Scene::IComponentArray;
	transformDefault.buffer.get()[4 * 4 - 1] = 1.f;
	camera->Insert<Camera>(ComponentTypes::CAMERA, nullptr);
	auto transform = camera->Emplace<Transform>(ComponentTypes::TRANSFORM);
	transform->pos = Vect3(.0f, .0f, 1.f);
	renderer = std::unique_ptr<Renderer>(new GLRenderer);
	if(renderer.get() == nullptr) std::cout << "Renderer is nullptr" << std::endl;
	mainShaderStage.reset(renderer->CreateShaderStage());	
	std::unique_ptr<NativeShaderHandlerParent> fragShader(renderer->CreateShader(ShaderType::FRAGMENT)), 
											   vertShader(renderer->CreateShader(ShaderType::VERTEX));
	if(AssetLoader::GetSingleton() == nullptr)
		AssetLoader::init();
	AssetLoader* assLoader = AssetLoader::GetSingleton();


	assLoader->LoadTextFile("Resource/Shaders/VertexShader.glsl", vertShader->source);
	assLoader->LoadTextFile("Resource/Shaders/FragmentShader.glsl", fragShader->source);

	mainShaderStage->shaderHandler.push_back(std::move(vertShader));
	mainShaderStage->shaderHandler.push_back(std::move(fragShader));
	VertexSpecification specification  = { {"pos", 4}, {"aNormal", 3}, {"texCoord", 3}};
	mainShaderStage->Load();
	layout = renderer->AddSpecification(specification);

}

RendererSystem* RendererSystem::init(Graphics_API graphicsAPI)
{
    singleton = new RendererSystem();
	return singleton;
}

RendererSystem* RendererSystem::GetSingleton()
{
    return singleton;
}

void RendererSystem::LoadLightColor(const LightColor& color, std::string name)
{
	renderer->Uniform3f(1, &color.ambient, name + ".ambient");
	renderer->Uniform3f(1, &color.specular, name + ".specular");
	renderer->Uniform3f(1, &color.diffuse, name + ".diffuse");
}


void RendererSystem::LoadLights()
{
	uint32_t numPointLights = 0, numDirLights = 0;
	for(auto i: lights)
	{
		if(scene->entities[i]->Get(ComponentTypes::POINTLIGHT) != nullptr)
		{
			auto pointLight = reinterpret_cast<PointLight*>(scene->entities[i]->Get(ComponentTypes::POINTLIGHT));
			std::string pointLightName("pointLight[");
			pointLightName += std::to_string(numPointLights) + "]";
			renderer->Uniform3f(1, &pointLight->pos, pointLightName + ".pos");
			LoadLightColor(pointLight->lightColor, pointLightName + ".lightColor");
			renderer->Uniform1f(1, &pointLight->constant, pointLightName + ".constant");
			renderer->Uniform1f(1, &pointLight->linear, pointLightName + ".linear");
			renderer->Uniform1f(1, &pointLight->quadratic, ".quadratic");
			numPointLights++;
		}
		if(scene->entities[i]->Get(ComponentTypes::DIRLIGHT) != nullptr)
		{
			auto dirLight = reinterpret_cast<DirectionalLight*>(scene->entities[i]->Get(ComponentTypes::DIRLIGHT));
			std::string dirLightName("dirLight[");
			dirLightName += std::to_string(numDirLights) + "]";
			renderer->Uniform3f(1, &dirLight->dir, dirLightName + ".dir");
			numDirLights++;
		}
	}
	renderer->Uniform1u(1, &numPointLights, "numPointLights");
	renderer->Uniform1u(1, &numDirLights, "numDirLights");
}
void RendererSystem::CreateGBufferMesh(Mesh* mesh, GBuffer* indexBuffer, GBuffer* vertexBuffer)
{
	vertexBuffer->bufferStyle.cpuFlags = GBuffer::GBufferStyle::CPUFlags::STATIC; 
    vertexBuffer->bufferStyle.type = GBuffer::GBufferStyle::BufferType::VERTEX;
    vertexBuffer->bufferStyle.usage = GBuffer::GBufferStyle::Usage::DRAW;
    //vertexBuffer->data = scene->resourceBank->resources[mesh->verticiesIndex].Get();
    vertexBuffer->count = mesh->vertexCount;
    vertexBuffer->sizet = mesh->vertexCount * sizeof(Vertex);

    indexBuffer->bufferStyle.cpuFlags = GBuffer::GBufferStyle::CPUFlags::STATIC;
    indexBuffer->bufferStyle.type = GBuffer::GBufferStyle::BufferType::INDEX;
    indexBuffer->bufferStyle.usage = GBuffer::GBufferStyle::Usage::DRAW;
    //indexBuffer->data = scene->resourceBank->resources[mesh->indiciesIndex].Get();
    indexBuffer->count = mesh->indexCount;
    indexBuffer->sizet = mesh->indexCount * sizeof(uint32_t);
}
void RendererSystem::LoadScene(Scene* scene)
{
//	renderer->ClearColor(.0f, 1.f, 0.5f);
	mainShaderStage->Load();
    this->scene = scene;
    Mesh* mesh;
	Texture* texture;
	bool foundCamera = false;
	renderer->SetLayout(layout);
    for(int itr = 0; itr < scene->entities.size(); itr++)
    {
		auto& i = scene->entities[itr];
		Scene::IComponentArray* componentArray;
		componentArray = i.get();
		//Mesh* mesh = (Mesh*)componentArray->components.find(ComponentTypes::MESH)->second.base->GetPointer();
		auto mesh = componentArray->Get<Mesh>(ComponentTypes::MESH);
		//componentArray->components.insert(std::make_pair(ComponentTypes::RENDERERSTUFF, ComponentPtr(new ComponentPtr::Impl<RendererStuff>())));
		//componentArray->components[ComponentTypes::RENDERERSTUFF].base->Create();
		//RendererStuff* rendererStuff = (RendererStuff*)componentArray->components[ComponentTypes::RENDERERSTUFF].base->GetPointer();
		auto rendererStuff = componentArray->Emplace<RendererStuff>(ComponentTypes::RENDERERSTUFF);
		//rendererStuff->iBuffer = new GBuffer;
		//rendererStuff->vBuffer = new GBuffer;
		//CreateGBufferMesh(mesh, rendererStuff->iBuffer, rendererStuff->vBuffer);
		//renderer->LoadBuffer(rendererStuff->iBuffer);
		//renderer->LoadBuffer(rendererStuff->vBuffer);
		//renderer->SetLayout(layout);
		//if(meshItr != componentArray->components.end())
		//{
		//}
		Vertex* vertex = reinterpret_cast<Vertex*>(scene->resourceBank->resources[rendererStuff->vBuffer.data].Get());
		auto findingCamera = componentArray->Get(ComponentTypes::CAMERA);
		if(findingCamera != nullptr)
		{
			foundCamera = true;
			mainCamera = itr;
		}
		if(componentArray->Get(ComponentTypes::DIRLIGHT) == nullptr || 
				componentArray->Get(ComponentTypes::POINTLIGHT) == nullptr		  
				)
			lights.push_back(itr);

    }
	if(!foundCamera)
	{
		uint32_t entity = scene->entityManager->CreateEntity();
		scene->entities[entity] = std::unique_ptr<Scene::IComponentArray>(camera);
		mainCamera = entity;
	}
	LoadLights();
}

Mat RendererSystem::SetUpCamera(uint32_t entity, Vect3& pos, const Vect3& temp = Vect3(0, 1, 0))
{
	Mat mat({4, 4});
	Transform* transform = (Transform*)scene->entities[entity]->Get(ComponentTypes::TRANSFORM);
	Vect3 forward = transform->pos - pos;
	Vect3 right = Vect3::cross(Vect3::normalize(temp), forward);
	Vect3 up = Vect3::cross(forward, right);
	lookAt(transform->pos, forward, up, right, mat);
	return mat;
}

void RendererSystem::lookAt(Vect3& from, Vect3& forward, Vect3& up, Vect3& right, Mat& mat)
{
	mat[{0, 0}] = right.x;
	mat[{0, 1}] = right.y;
	mat[{0, 2}] = right.z;
	mat[{1, 0}] = up.x;
	mat[{1, 1}] = up.y;
	mat[{1, 2}] = up.z;
	mat[{2, 0}] = forward.x; 
	mat[{2, 1}] = forward.y;
	mat[{2, 2}] = forward.z;
	//mat[{3, 0}] = from.x;
	//mat[{3, 1}] = from.y;
	//mat[{3, 2}] = from.z;
	mat[{3, 3}] = 1.f;
}

void RendererSystem::LoadMaterial(Scene::Entities::iterator& itr)
{
	float materialProvided = .0f;
	Material* material = (Material*)(*itr)->Get(ComponentTypes::MATERIAL);
	if((*itr)->Get(ComponentTypes::MATERIAL) != nullptr)
	{
		materialProvided = 1.f;
	}
	if(materialProvided != .0f)
	{				
		renderer->Uniform3f(1, &material->diffuse, "material.diffuse");
		renderer->Uniform3f(1, &material->spectacular, "material.spectacular");
		renderer->Uniform3f(1, &material->ambient, "material.ambient");
		renderer->Uniform1f(1, &material->shininess, "material.shininess");
	}
	renderer->Uniform1f(1, &materialProvided, "materialProvided");
}

void RendererSystem::LoadTransform(Scene::Entities::iterator& itr)
{
	Mat* mat = nullptr;
	Transform* transform = static_cast<Transform*>((*itr)->Get(ComponentTypes::TRANSFORM));
	if(transform != nullptr)
	{
		mat = ConvertTranforToMatrix(*transform);
	}
	else
	{
		mat = &transformDefault;
		//ComponentPtr& comptr = (*itr)->Get(ComponentTypes::TRANSFORM);
		//comptr.base = new ComponentPtr::Impl<Transform>;
		//transform =  (Transform*)comptr.base->Create();
		transform = (*itr)->Emplace<Transform>(ComponentTypes::TRANSFORM);
		transform->scale = Vect3(.5f, .5f, .5f);
//		comptr.entity = std::distance(scene->entities.begin(), itr);
	}
	*mat *= SetUpCamera(mainCamera, transform->pos);
	renderer->UniformMat(1, mat, "MVP");
}

void RendererSystem::Update(float deltaTime)
{
	renderer->Clear();
    RendererStuff* rendererStuff;
	mainShaderStage->Load();
	renderer->SetLayout(layout);
    for(auto itr = scene->entities.begin(); itr != scene->entities.end(); itr++)
    {
		auto rendererStuffitr = (*itr)->Get(ComponentTypes::RENDERERSTUFF);
		if( rendererStuffitr != nullptr)
		{
        	rendererStuff = (RendererStuff*)rendererStuffitr;
        	renderer->Bind(rendererStuff->vBuffer);
        	renderer->Bind(rendererStuff->iBuffer);
        	//rendererStuff->texture->Bind();		
			LoadMaterial(itr);
			LoadTransform(itr);
        	renderer->Draw(DrawPrimitive::TRIANGLES, &rendererStuff->iBuffer);
		}

	}
    
	animated += .01f;
}
