#pragma once
#include <assimp/camera.h>
#include <ecs/ecs.h>
#include <ecs/commoncomponent.h>
#include <ecs/GraphicsComponent.h>
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
	void ProcessMeshes(aiMesh* mesh, Scene* scene, const aiScene* queryScene, const uint32_t& entity);
	void ProcessMaterial(aiMaterial* material, Scene* scene, const aiScene* queryScene, uint32_t entity);
	void ProcessTexture(aiTexture* texture, Scene* scene, const aiScene* queryScene, uint32_t entity);
	void ProcessLight(Scene* scene, const aiScene* queryScene);
	void ProcessCamera(aiCamera* camera, Scene* scene, const aiScene* queryScene);
	void ProcessLightColor(const aiLight* light, LightColor& color);
	void ProcessTransform(const aiNode* node, Scene* scene, const uint32_t& entity);
	void ProcessNodes(aiNode* node, Scene* scene, const aiScene* queryScene);
public:
	static SceneConverter* singleton;
};
