#include "Editor.hpp"
#include <assimp/scene.hpp>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <iostream>

Editor::Editor(uint32_t width, uint32_t height)
{
}

void Editor::Import(std::string filePath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath.c_str(),
			aiProcess_GenSmoothNormals |
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType
			);
	if(!scene)
	{
		std::cout << "Error loading file: (assimp:) " << importer.GetErrorString();
		return;
	}
}


int main()
{
	Editor().Run();
}
