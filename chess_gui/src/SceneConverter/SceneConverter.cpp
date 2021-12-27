#include "SceneConverter.h"
#include "ECS/GraphicsComponent.h"
#include <assimp/Importer.hpp>
#include <assimp/light.h>
#include <assimp/material.h>
#include <assimp/vector3.h>
#include <cstdint>
#include <memory>
#include <chrono>
#include <thread>

SceneConverter* SceneConverter::singleton = nullptr;

SceneConverter* SceneConverter::init()
{
	singleton = new SceneConverter;
	return singleton;
}

template<typename T>
void ConVec3(Vect3& vect3, const T& vec)
{
	if(sizeof(Vect3) == sizeof(T))
	memcpy(&vect3, &vec, sizeof(T));
}

void SceneConverter::Import(std::string filePath, std::string resultedPath)
{
	Scene* resultedScene = new Scene;
	Assimp::Importer importer;
	auto timerStart = std::chrono::system_clock::now();
	uint32_t assimpFlag = aiProcess_GenSmoothNormals |
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices;
	const aiScene* scene = importer.ReadFile(filePath.c_str(),
			assimpFlag
			);
	auto timerEnd = std::chrono::system_clock::now();
	std::cout << "Time Taken importing: " << static_cast<std::chrono::duration<float>>(timerEnd - timerStart).count() << std::endl;
	if(scene->HasMeshes())
	{
		resultedScene->componentTypeMap.insert(std::make_pair(std::type_index(typeid(Mesh)), ComponentTypes::MESH));
	}
	if(scene->HasMaterials())
	{
		resultedScene->componentTypeMap.insert(std::make_pair(std::type_index(typeid(Material)), ComponentTypes::MATERIAL));
	}
	if(scene->HasTextures())
	{
		resultedScene->componentTypeMap.insert(std::make_pair(std::type_index(typeid(Texture)), ComponentTypes::TEXTURE));
	}
	if(scene->HasLights())
	{
		resultedScene->componentTypeMap.insert(std::make_pair(std::type_index(typeid(PointLight)), ComponentTypes::POINTLIGHT));
		resultedScene->componentTypeMap.insert(std::make_pair(std::type_index(typeid(DirectionalLight)), ComponentTypes::DIRLIGHT));
	}
	if(scene->HasCameras())
		resultedScene->componentTypeMap.insert(std::make_pair(std::type_index(typeid(Camera)), ComponentTypes::CAMERA));

	ProcessNodes(scene->mRootNode, resultedScene, scene);

	for(auto& i: resultedScene->entities)
	{
		for(auto& j: i.get()->components)
		{
			if(j.first == ComponentTypes::MESH)
			{
				Mesh* mesh = reinterpret_cast<Mesh*>(j.second.base->GetPointer());
			}
		}
	}
	resultedScene->SaveScene(resultedPath);
	importer.FreeScene();
}

void SceneConverter::ProcessLightColor(const aiLight* light, LightColor& color)
{
	ConVec3(color.ambient, light->mColorAmbient);
	ConVec3(color.specular, light->mColorSpecular);
	ConVec3(color.diffuse, light->mColorDiffuse);
}

void SceneConverter::ProcessMeshes(aiMesh* mesh, Scene* scene, const aiScene* queryScene, const uint32_t& entity)
{
	Mesh* resultedMesh = (Mesh*)scene->entities[entity]->components[ComponentTypes::MESH].emplace(new ComponentPtr::Impl<Mesh>);
	resultedMesh->verticiesIndex = scene->resourceBank->Push_Back(reinterpret_cast<uint8_t*>(new Vertex[mesh->mNumVertices]), 
															sizeof(Vertex) * mesh->mNumVertices);
	resultedMesh->vertexCount = mesh->mNumVertices;
	auto verticies = reinterpret_cast<Vertex*>(scene->resourceBank->resources[resultedMesh->verticiesIndex].Get());
	for(uint32_t i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.aPos.x = mesh->mVertices[i].x; vertex.aPos.y = mesh->mVertices[i].y;
		vertex.aPos.z = mesh->mVertices[i].z;
		vertex.aPos.w = 1.f;
		vertex.aNormal = *((Vect3*)&mesh->mNormals[i]);
		vertex.texCord = *((Vect3*)&mesh->mTextureCoords[0][i]);
		verticies[i] = vertex;
	}
	if(mesh->HasFaces())
	{
		resultedMesh->indexCount = mesh->mNumFaces * 3;
		resultedMesh->indiciesIndex = scene->resourceBank->Push_Back(reinterpret_cast<uint8_t*>(new uint32_t[resultedMesh->indexCount]),  
														resultedMesh->indexCount * sizeof(uint32_t));
		auto indicies = reinterpret_cast<uint32_t*>(scene->resourceBank->resources[resultedMesh->indiciesIndex].Get());
		
		for(uint32_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace* face = &mesh->mFaces[i];
			for(uint32_t j = 0; j < 3; j++)
			{
				indicies[i * 3 + j] = face->mIndices[j];
			}
		}
	}
	if(mesh->mMaterialIndex)
	{
		ProcessMaterial(queryScene->mMaterials[mesh->mMaterialIndex], scene, queryScene, entity);
	}
}

void SceneConverter::ProcessTexture(aiTexture* texture, Scene* scene, const aiScene* queryScene, uint32_t entity)
{
}

void SceneConverter::ProcessMaterial(aiMaterial* material, Scene* scene, const aiScene* queryScene, uint32_t entity)
{
	aiShadingMode shadingMode;
	material->Get(AI_MATKEY_SHADING_MODEL, shadingMode);
	if(shadingMode == aiShadingMode_Gouraud)
	{
		Material* resultedMaterial = static_cast<Material*>(scene->entities[entity]->components[ComponentTypes::MATERIAL].emplace(new ComponentPtr::Impl<Material>));
		material->Get(AI_MATKEY_COLOR_SPECULAR, resultedMaterial->spectacular.coordinates);
		material->Get(AI_MATKEY_COLOR_DIFFUSE, resultedMaterial->diffuse.coordinates);
		material->Get(AI_MATKEY_COLOR_AMBIENT, resultedMaterial->ambient.coordinates);
		material->Get(AI_MATKEY_SHININESS_STRENGTH, resultedMaterial->shininess);
	}
}

void SceneConverter::ProcessNodes(aiNode* node, Scene* scene, const aiScene* queryScene)
{
	for(uint32_t i = 0; i < node->mNumMeshes; i++)
	{
		auto entity = scene->entityManager->CreateEntity();
		scene->entities.push_back(std::move(std::unique_ptr<Scene::IComponentArray>(new Scene::IComponentArray)));
		ProcessTransform(node, scene, entity);
		ProcessMeshes(queryScene->mMeshes[node->mMeshes[i]], scene, queryScene, entity);
	}
	for(uint32_t i = 0; i < node->mNumChildren; i++)
	{
		ProcessNodes(node->mChildren[i], scene, queryScene);
	}
}

void SceneConverter::ProcessTransform(const aiNode* node, Scene* scene, const uint32_t& entity)
{
	auto goatTransform = node->mTransformation;
	auto transform = (Transform*)scene->entities[entity]->components[ComponentTypes::TRANSFORM].emplace(new ComponentPtr::Impl<Transform>);
	aiVector3t<float> pos, scale, rotation;
	goatTransform.Decompose(scale, rotation, pos);
	ConVec3(transform->pos, pos);
	ConVec3(transform->rotation, rotation);
	ConVec3(transform->scale, scale);
}

void SceneConverter::ProcessLight(Scene* scene, const aiScene* queryScene)
{
	for(uint32_t i = 0; i < queryScene->mNumLights; i++)
	{
		const auto light = queryScene->mLights[i];
		auto entity = scene->entityManager->CreateEntity();
		scene->entities[entity].reset(new Scene::IComponentArray);
		auto LightComponent = scene->entities[entity].get();
		LightColor color;
		ProcessLightColor(light, color);	
		if(light->mType == aiLightSource_POINT)
		{
			PointLight* pointLight = (PointLight*)LightComponent->components[ComponentTypes::POINTLIGHT].emplace(new ComponentPtr::Impl<PointLight>);
			ConVec3(pointLight->pos, light->mPosition);
			pointLight->lightColor = color;
			pointLight->constant = light->mAttenuationConstant;
			pointLight->linear = light->mAttenuationLinear;
			pointLight->quadratic = light->mAttenuationQuadratic;
		}
		else if(light->mType == aiLightSource_DIRECTIONAL)
		{
			DirectionalLight* dirLight = (DirectionalLight*)LightComponent->components[ComponentTypes::DIRLIGHT].emplace(new ComponentPtr::Impl<DirectionalLight>);
			ConVec3(dirLight->dir, light->mDirection);
			dirLight->lightColor = color;
		}
	}
}


int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		std::cerr << " We need two arguments " << std::endl;
		return -1;
	}
	SceneConverter::init();
	SceneConverter::singleton->Import(argv[1], argv[2]);
	return 0;
}
