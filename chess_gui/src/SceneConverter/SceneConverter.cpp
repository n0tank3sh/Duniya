#include "SceneConverter.h"
#include <ecs/commoncomponent.h>
#include <ecs/GraphicsComponent.h>
SceneConverter* SceneConverter::singleton = nullptr;

SceneConverter* SceneConverter::init()
{
	singleton = new SceneConverter;
	return singleton;
}

void SceneConverter::Import(std::string filePath, std::string resultedPath)
{
	std::cout <<  filePath << std::endl;
	Scene* resultedScene = new Scene;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath.c_str(),
			aiProcess_GenSmoothNormals |
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices
			);
	if(scene->HasMeshes())
	{
		resultedScene->componentTypeMap.insert(std::make_pair(std::type_index(typeid(Mesh)), ComponentType::MESH));
	}
	if(scene->HasMaterials())
	{
		resultedScene->componentTypeMap.insert(std::make_pair(std::type_index(typeid(Material)), ComponentType::MATERIAL));
	}
	if(scene->HasTextures())
	{
		resultedScene->componentTypeMap.insert(std::make_pair(std::type_index(typeid(Texture)), ComponentType::TEXTURE));
	}
	for(auto& i: resultedScene->componentTypeMap)
	{
		std::cout << i.first.name() << " " << static_cast<uint32_t>(i.second) << std::endl;
	}
	ProcessNodes(scene->mRootNode, resultedScene, scene);
	for(auto& i: resultedScene->entities)
	{
		for(auto& j: i.second.get()->components)
		{
			if(j.first == ComponentType::MESH)
			{
				Mesh* mesh = reinterpret_cast<Mesh*>(j.second.base->GetPointer());
				for(auto k: *(mesh->indicies))
				{
					std::cout << k << " ";
				}
				std::cout << std::endl;
			}
		}
	}
	resultedScene->SaveScene(resultedPath);
	importer.FreeScene();
}

void SceneConverter::ProcessMeshes(aiMesh* mesh, Scene* scene, const aiScene* queryScene)
{
	uint32_t entity = scene->entityManager->CreateEntity();
	scene->entities.insert(std::move(std::make_pair(entity, std::unique_ptr<Scene::IComponentArray>(new Scene::IComponentArray))));
	ComponentPtr& componentPtr = scene->entities[entity]->components[ComponentType::MESH];
	componentPtr.base = new ComponentPtr::Impl<Mesh>();
	componentPtr.base->Create();
	Mesh* resultedMesh = (Mesh*)componentPtr.base->GetPointer();
	resultedMesh->indicies =new std::vector<uint32_t>();
	resultedMesh->verticies = new std::vector<Vertex>();
	for(uint32_t i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.aPos.x = mesh->mVertices[i].x;
		vertex.aPos.y = mesh->mVertices[i].y;
		vertex.aPos.z = mesh->mVertices[i].z;
		vertex.aPos.w = 1.f;
		vertex.aNormal = *((Vect3*)&mesh->mNormals[i]);
		vertex.texCord = *((Vect3*)&mesh->mTextureCoords[0][i]);
		resultedMesh->verticies->push_back(vertex);
	}
	if(mesh->HasFaces())
	{
		std::cout  << "Total number of the faces: " << mesh->mNumFaces << std::endl;
		for(uint32_t i = 0; i < mesh->mNumFaces; i++)
		{
			std::cout << ((mesh->mFaces + sizeof(aiFace) * i)->mIndices == nullptr?" indicies is nullptr " : " indicies is not nullptr ") << std::endl;
			std::cout << "the face number: " << i << std::endl;
			aiFace* face = &mesh->mFaces[i];
		//	if(&mesh->mFaces[i] == nullptr) std::cout << "Faces is null" << std::endl;
			for(uint32_t j = 0; j < 3; j++)
			{
				std::cout << "Index of indicies: " << j << std::endl;
				resultedMesh->indicies->push_back(face->mIndices[j]);
			}
		}
	}
}

void SceneConverter::ProcessTexture(aiTexture* texture, Scene* scene, const aiScene* queryScene)
{
}

void SceneConverter::ProcessMaterial(aiMaterial* material, Scene* scene, const aiScene* queryScene)
{

	if(true)
	{
	//	ComponentPtr& componentPtr = scene->components[ComponentType::MATERIAL];
	//	componentPtr.base->Create();
	//	Material* resultedMaterial = reinterpret_cast<Material*>(componentPtr.base->GetPointer());
	//	material->Get(AI_MATKEY_COLOR_SPECULAR, *(aiColor3D*)&resultedMaterial->spectacular);
	//	material->Get(AI_MATKEY_COLOR_DIFFUSE, resultedMaterial->diffuse.coordinates);
	//	material->Get(AI_MATKEY_COLOR_AMBIENT, resultedMaterial->ambient.coordinates);
	//	material->Get(AI_MATKEY_SHININESS, &resultedMaterial->shininess);
	//	aiGetMaterialFloat(AI_MATKEY_SHININESS, &resultedMaterial->shininess);
	}
}

void SceneConverter::ProcessNodes(aiNode* node, Scene* scene, const aiScene* queryScene)
{
	for(uint32_t i = 0; i < node->mNumMeshes; i++)
	{
		std::cout << "Going through the loop" << std::endl;
		ProcessMeshes(queryScene->mMeshes[node->mMeshes[i]], scene, queryScene);
	}
	for(uint32_t i = 0; i < node->mNumChildren; i++)
	{
		ProcessNodes(node->mChildren[i], scene, queryScene);
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
