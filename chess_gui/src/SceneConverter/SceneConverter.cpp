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
	ProcessNodes(scene->mRootNode, resultedScene, scene);
	resultedScene->SaveScene(resultedPath);
	importer.FreeScene();
}

void SceneConverter::ProcessMeshes(aiMesh* mesh, Scene* scene, const aiScene* queryScene)
{
	uint32_t entity = scene->entityManager->CreateEntity();
	scene->entities.insert(std::make_pair(entity, std::unique_ptr<Scene::IComponentArray>(new Scene::IComponentArray)));
	ComponentPtr& componentPtr = scene->entities[entity]->components[ComponentType::MESH];
	componentPtr.base = new ComponentPtr::Impl<Mesh>();
	componentPtr.base->Create();
	Mesh* resultedMesh = (Mesh*)componentPtr.base->GetPointer();
	resultedMesh->indicies =new std::vector<uint32_t>();
	resultedMesh->verticies = new std::vector<Vertex>();
	if(mesh->mNumVertices != mesh->mNumUVComponents[0]) std::cout << "The larger size will be: " << (mesh->mNumVertices == std::max(mesh->mNumVertices, mesh->mNumUVComponents[0])? "vertices size ": "uv size") << std::endl;
	if(mesh->HasTextureCoords(0)) std::cout << " Mesh have the uv channel " << std::endl;
	if(sizeof(Vect3) != sizeof(aiColor3D)) std::cout << "Size fo Vect3 is not equals to sizeo of aiColor3D" << std::endl;
	if(!mesh->HasNormals()) std::cout << "Doesn't has the normals " << std::endl;
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
			const aiFace* face = &mesh->mFaces[i];
			if(mesh->mFaces == nullptr) std::cout << "Faces is null" << std::endl;
			for(uint32_t j = 0; j < face->mNumIndices; j++)
			{
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
//		ComponentPtr& componentPtr = scene->components[ComponentType::MATERIAL];
//		componentPtr.base->Create();
//		Material* resultedMaterial = reinterpret_cast<Material*>(componentPtr.base->GetPointer());
//		material->Get(AI_MATKEY_COLOR_SPECULAR, *(aiColor3D*)&resultedMaterial->spectacular);
//		material->Get(AI_MATKEY_COLOR_DIFFUSE, resultedMaterial->diffuse.coordinates);
//		material->Get(AI_MATKEY_COLOR_AMBIENT, resultedMaterial->ambient.coordinates);
	//	material->Get(AI_MATKEY_SHININESS, &resultedMaterial->shininess);
	//	aiGetMaterialFloat(AI_MATKEY_SHININESS, &resultedMaterial->shininess);
	}
}

void SceneConverter::ProcessNodes(aiNode* node, Scene* scene, const aiScene* queryScene)
{
	std::cout << "Reach here " << std::endl;
	std::cout << "the size of the root nodes " << node->mNumMeshes << std::endl;
	std::cout << "the total number of children " << node->mNumChildren << std::endl;
	std::cout << "the size fo the child nodes " << node->mChildren[0]->mNumMeshes << std::endl;
	for(uint32_t i = 0; i < node->mNumMeshes; i++)
	{
		std::cout << "Going through the loop" << std::endl;
		ProcessMeshes(queryScene->mMeshes[node->mMeshes[i]], scene, queryScene);
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
