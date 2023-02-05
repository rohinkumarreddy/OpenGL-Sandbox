#include "Model.h"
#include "common.h"
#include "Mesh.h"
#include "Texture.h"
#include <iostream>

Model::Model()
{
}

void Model::RenderModel()
{
	for (size_t i = 0; i < meshList.size(); i++)
	{
		unsigned int materialIndex = meshToTex[i];

		if (materialIndex < textureList.size() && textureList[materialIndex])
		{
			textureList[materialIndex]->activate();
		}

		meshList[i]->renderMesh();
	}
}

void Model::LoadModel(const std::string& fileName)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	//std::cout << "[Debug]: load chk 1\n";

	if (!scene)
	{
		printf("Model (%s) failed to load: %s", fileName.c_str(), importer.GetErrorString());
		return;
	}
	//std::cout << "[Debug]: load chk 2\n";

	LoadNode(scene->mRootNode, scene);

	//std::cout << "[Debug]: load chk 3\n";

	LoadMaterials(scene);

	//std::cout << "[Debug]: load chk 4\n";
}

void Model::LoadNode(aiNode* node, const aiScene* scene)
{
	//std::cout << "[Debug]: loadNode chk 1\n";
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	//std::cout << "[Debug]: loadNode chk 2\n";

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene);
	}

	//std::cout << "[Debug]: loadNode chk 3\n";
}

void Model::LoadMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<GLfloat> vertices;
	std::vector<unsigned short> indices;

	//std::cout << "[Debug]: loadMesh chk 1\n";

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.insert(vertices.end(),
						{ mesh->mVertices[i].x,
						  mesh->mVertices[i].y,
						  mesh->mVertices[i].z });
		if (mesh->mTextureCoords[0])
		{
			vertices.insert(vertices.end(),
							{ mesh->mTextureCoords[0][i].x,
							  mesh->mTextureCoords[0][i].y,
							  mesh->mTextureCoords[0][i].z });
		}
		else {
			vertices.insert(vertices.end(), { 0.0f, 0.0f, 0.0f });
		}
		vertices.insert(vertices.end(),
						{ mesh->mNormals[i].x,
						  mesh->mNormals[i].y,
						  mesh->mNormals[i].z });
	}

	//std::cout << "[Debug]: loadMesh chk 2\n";

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	//std::cout << "[Debug]: loadMesh chk 3\n";

	Mesh* newMesh = new Mesh();
	if (vertices.size() % 9)
	{
		std::cout << "[Debug]: " << "vertices size " << vertices.size() << "\n";
		std::cout << "[Debug]: " << "vertices size % 9 " << vertices.size()%9 << "\n";
	}
	newMesh->createMesh(&vertices[0],
						&indices[0],
						vertices.size()/9,
						indices.size(),
						VertexAttribute::VertexAttributeProfile::POS_TEX_NOR);

	//std::cout << "[Debug]: loadMesh chk 4\n";

	meshList.push_back(newMesh);
	meshToTex.push_back(mesh->mMaterialIndex);
	//std::cout << "[Debug]: loadMesh chk 5\n";
}

void Model::LoadMaterials(const aiScene* scene)
{
	textureList.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];

		textureList[i] = nullptr;

		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				int idx = std::string(path.data).rfind("\\");
				std::string filename = std::string(path.data).substr(idx + 1);

				std::string texPath = std::string("Textures/") + filename;
				
				//std::cout << texPath << "\n";

				textureList[i] = new Texture(texPath.c_str());
				//std::cout << "chk1" << "\n";

				if (!textureList[i]->load())
				{
					printf("Failed to load texture at: %s\n", texPath.c_str());
					delete textureList[i];
					textureList[i] = nullptr;
				}
				//std::cout << "chk2" << "\n";
			}
		}

		if (!textureList[i])
		{
			textureList[i] = new Texture("Textures/plain.png");//brick.png
			textureList[i]->load();
		}
	}
}

void Model::ClearModel()
{
	for (size_t i = 0; i < meshList.size(); i++)
	{
		if (meshList[i])
		{
			delete meshList[i];
			meshList[i] = nullptr;
		}
	}

	for (size_t i = 0; i < textureList.size(); i++)
	{
		if (textureList[i])
		{
			delete textureList[i];
			textureList[i] = nullptr;
		}
	}
}

Model::~Model()
{
}
