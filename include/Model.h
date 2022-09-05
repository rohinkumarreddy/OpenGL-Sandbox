#pragma once

#include <vector>
#include <string>

#include "vendor\assimp\include\assimp\Importer.hpp"
#include "vendor\assimp\include\assimp\scene.h"
#include "vendor\assimp\include\assimp\postprocess.h"

//forward declarations
class Mesh;
class Texture;

class Model
{
public:
	Model();

	void LoadModel(const std::string& fileName);
	void RenderModel();
	void ClearModel();

	~Model();

private:

	void LoadNode(aiNode* node, const aiScene* scene);
	void LoadMesh(aiMesh* mesh, const aiScene* scene);
	void LoadMaterials(const aiScene* scene);

	std::vector<Mesh*> meshList;
	std::vector<Texture*> textureList;
	std::vector<unsigned int> meshToTex;
};