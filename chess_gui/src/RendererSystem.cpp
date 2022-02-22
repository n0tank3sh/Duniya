#include "RendererSystem.h"
#include "Application.h"
#include "ECS/CommonComponent.h"
#include "ECS/ECS.h"
#include "Graphics/Renderer.h"
#include "Graphics/OpenGL/GLRenderer.h"
#include "Math/Mat.h"
#include "Math/Vect3.h"
#include <AssetLoader.h>
#include <chrono>
#include <cmath>
#include <iterator>
#include <stdexcept>
#include <string>

RendererSystem* RendererSystem::singleton = nullptr;

RendererSystem::RendererSystem()
	:
		transformDefault({4, 4})
{
	animated = .0f;
	transformDefault = DefaultMatrix::generateIdentityMatrix({4, 4});
	camera = new Scene::IComponentArray;
	transformDefault.buffer.get()[4 * 4 - 1] = 1.f;
	camera->Insert<Camera>(ComponentTypes::CAMERA, nullptr);
	auto transform = camera->Emplace<Transform>(ComponentTypes::TRANSFORM);
	transform->pos = Vect3(.0f, .0f, 1.f);
	renderer = std::unique_ptr<Renderer>(new GLRenderer);
	mainShaderStage.reset(renderer->CreateShaderStage());	
	std::unique_ptr<NativeShaderHandlerParent> fragShader(renderer->CreateShader(ShaderType::FRAGMENT)), 
		vertShader(renderer->CreateShader(ShaderType::VERTEX));
	if(AssetLoader::GetSingleton() == nullptr)
		AssetLoader::init();
	auto assetLoader = AssetLoader::GetSingleton();


	assetLoader->LoadTextFile("Resource/Shaders/VertexShader.glsl", vertShader->source);
	assetLoader->LoadTextFile("Resource/Shaders/FragmentShader.glsl", fragShader->source);

	mainShaderStage->shaderHandler.push_back(std::move(vertShader));
	mainShaderStage->shaderHandler.push_back(std::move(fragShader));
	VertexSpecification specification  = { {"pos", 4}, {"aNormal", 3}, {"texCoord", 2}};

	defaultMaterial.ambient = Vect3() + 1.f;
	defaultMaterial.diffuse = Vect3() +  1.f;
	defaultMaterial.shininess = 0.0f;
	defaultMaterial.spectacular = Vect3() + 1.f;

	mainShaderStage->Load();
	layout = renderer->AddSpecification(specification);

}

RendererSystem* RendererSystem::init(Graphics_API graphicsAPI)
{
	singleton = new RendererSystem();
	singleton->scene = nullptr;
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

Scene* RendererSystem::GetScene()
{
	if(this->scene == nullptr)
		throw std::runtime_error("RendererSystem\nscene is not set");
	return this->scene;
}


void RendererSystem::LoadLights()
{
	uint32_t numPointLights = 0, numDirLights = 0;
	auto scene = GetScene();
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

void RendererSystem::SetupDefaultTexture()
{
	auto data = new uint8_t[4];
	memset(data, 1, 4);
	defaultTexture.data = GetScene()->resourceBank->Push_Back(data, 4);
	defaultTexture.format = Texture::RGBA;
	defaultTexture.width = 1;
	defaultTexture.height = 1;
	renderer->LoadTexture(&defaultTexture, &defaultTextureGBuffer); 
}

void RendererSystem::CreateGBufferMesh(Mesh* mesh, GBuffer* indexBuffer, GBuffer* vertexBuffer)
{
	vertexBuffer->bufferStyle.cpuFlags = GBuffer::GBufferStyle::CPUFlags::STATIC; 
	vertexBuffer->bufferStyle.type = GBuffer::GBufferStyle::BufferType::VERTEX;
	vertexBuffer->bufferStyle.usage = GBuffer::GBufferStyle::Usage::DRAW;
	vertexBuffer->count = mesh->vertexCount;
	vertexBuffer->sizet = mesh->vertexCount * sizeof(Vertex);
	vertexBuffer->data = mesh->verticiesIndex;

	indexBuffer->bufferStyle.cpuFlags = GBuffer::GBufferStyle::CPUFlags::STATIC;
	indexBuffer->bufferStyle.type = GBuffer::GBufferStyle::BufferType::INDEX;
	indexBuffer->bufferStyle.usage = GBuffer::GBufferStyle::Usage::DRAW;
	indexBuffer->count = mesh->indexCount;
	indexBuffer->sizet = mesh->indexCount * sizeof(uint32_t);
	indexBuffer->data = mesh->indiciesIndex;
}

void RendererSystem::CreateRendererStuff(Mesh* mesh, RendererStuff* rendererStuff)
{
	if(rendererStuff == nullptr)
		rendererStuff  = new RendererStuff;
	CreateGBufferMesh(mesh, &rendererStuff->iBuffer, &rendererStuff->vBuffer);
	renderer->LoadBuffer(&rendererStuff->vBuffer);
	renderer->LoadBuffer(&rendererStuff->iBuffer);
}

void RendererSystem::LoadScene(Scene* scene)
{
	//	renderer->ClearColor(.0f, 1.f, 0.5f);
	renderer->SetResourceBank(scene->resourceBank);
	mainShaderStage->Load();
	this->scene = scene;
	Mesh* mesh;
	Texture* texture;
	bool foundCamera = false;
	for(int itr = 0; itr < scene->entities.size(); itr++)
	{
		auto& i = scene->entities[itr];
		Scene::IComponentArray* componentArray;
		componentArray = i.get();
		auto mesh = componentArray->Get<Mesh>(ComponentTypes::MESH);
		if(mesh != nullptr)
		{
			if(componentArray->Get<RendererStuff>(ComponentTypes::RENDERERSTUFF) == nullptr)
			{
				CreateRendererStuff(mesh, componentArray->Emplace<RendererStuff>(ComponentTypes::RENDERERSTUFF));
			}
		}
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
	SetupDefaultTexture();
	if(!foundCamera)
	{
		uint32_t entity = scene->entityManager->CreateEntity();
		scene->entities[entity] = std::unique_ptr<Scene::IComponentArray>(camera);
		mainCamera = entity;
	}
	LoadLights();
}

Mat RendererSystem::SetUpCamera(uint32_t entity)
{
	auto scene = GetScene();
	auto pos = scene->GetEntity(entity)->Get<Transform>(ComponentTypes::TRANSFORM)->pos;
	auto camera = scene->GetEntity(entity)->Get<Camera>(ComponentTypes::CAMERA);
	return LookAt(pos, camera->lookAt) * SetupPerspective(*camera) ;
}

Mat RendererSystem::LookAt(const Vect3& pos, const Vect3& dir)
{
	return DefaultMatrix::generateIdentityMatrix({4, 4});
}

Mat RendererSystem::SetupPerspective(Camera& camera)
{
	Mat mat({4, 4});
	mat.Get(0, 0) = 1/tan(camera.fov/2) * resoltuion.x / resoltuion.y;
	mat.Get(1, 1) = 1/tan(camera.fov/2);
	mat.Get(2, 2) = camera.far/(camera.far - camera.near);
	mat.Get(3, 2) = -(camera.far * camera.near)/(camera.far - camera.near);
	mat.Get(2, 3) = 1;
	return mat;
}

void RendererSystem::LoadMaterial(Scene::Entities::iterator& itr)
{
	auto material = (*itr)->Get<Material>(ComponentTypes::MATERIAL);
	if(material == nullptr)
	{				
		material = &defaultMaterial;
	}
	renderer->Uniform3f(1, &material->diffuse, "material.diffuse");
	renderer->Uniform3f(1, &material->spectacular, "material.spectacular");
	renderer->Uniform3f(1, &material->ambient, "material.ambient");
	renderer->Uniform1f(1, &material->shininess, "material.shininess");
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
		transform = (*itr)->Emplace<Transform>(ComponentTypes::TRANSFORM);
		transform->scale = Vect3(.5f, .5f, .5f);
	}
	*mat *= SetUpCamera(mainCamera);
	renderer->UniformMat(1, mat, "MVP");
}

void RendererSystem::Update(float deltaTime)
{
	renderer->Clear();
	RendererStuff* rendererStuff;
	mainShaderStage->Load();
	auto scene = GetScene();
	for(auto itr = scene->entities.begin(); itr != scene->entities.end(); itr++)
	{
		if((*itr)->Get(ComponentTypes::MESH) != nullptr)
		{
			auto rendererStuff = (*itr)->Get<RendererStuff>(ComponentTypes::RENDERERSTUFF);
			if( rendererStuff == nullptr)
			{
				CreateRendererStuff((*itr)->Get<Mesh>(ComponentTypes::MESH), (*itr)->Emplace<RendererStuff>(ComponentTypes::RENDERERSTUFF));
				rendererStuff = (*itr)->Get<RendererStuff>(ComponentTypes::RENDERERSTUFF);
			}
			renderer->Bind(rendererStuff->vBuffer);
			renderer->SetLayout(layout);
			renderer->Bind(rendererStuff->iBuffer);
			if((*itr)->Get(ComponentTypes::TEXTURE) != nullptr)
				renderer->Bind(rendererStuff->texture);
			else
				renderer->Bind(defaultTextureGBuffer);
			LoadMaterial(itr);
			LoadTransform(itr);
			renderer->Draw(DrawPrimitive::TRIANGLES, &rendererStuff->iBuffer);
		}

	}
	animated += .01f;
}
