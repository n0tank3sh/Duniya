#pragma once
#include <ECS/ECS.hpp>
#include <ECS/CommonComponent.hpp>
#include <ECS/GraphicsComponent.hpp>
#include <assimp/camera.h>
#include <SDL.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>



class SceneConverter
{
private:
	SceneConverter() = default;
public:
	static SceneConverter* init();
	void Import(std::string filePath, std::string resultedPath);
	void Export(std::string filePath);
private:
	Scene* scene;
	void ProcessMeshes(aiMesh* mesh, const aiScene* queryScene, const uint32_t& entity);
	void ProcessMaterial(aiMaterial* material, const aiScene* queryScene, uint32_t entity);
	void ProcessTexture(aiTexture* texture, const aiScene* queryScene, uint32_t entity);
	void ProcessLight(const aiScene* queryScene);
	void ProcessCamera(aiCamera* camera, const aiScene* queryScene);
	void ProcessLightColor(const aiLight* light, LightColor& color);
	void ProcessTransform(const aiNode* node, const uint32_t& entity);
	void ProcessNodes(aiNode* node, const aiScene* queryScene);
public:
	static SceneConverter* singleton;
};
