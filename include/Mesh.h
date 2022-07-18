#pragma once

#include <vector>
#include "MeshAttribute.h"

//forward declaration
struct Vertex;

class Mesh
{
public:
	enum class shapeType
	{
		_TEAPOT_ = 0,
		_ARROW_,
		_PLANE_,
		_CUBE_,
		_TORUS_,
		_SPHERE_
	};
	Mesh();
	~Mesh();
	void createMesh(shapeType type);
	void createMesh(Vertex* vertices,
					unsigned short* indices,
					unsigned int numOfVertices,
					unsigned int numOfIndices,
					bool hasTexture = true);
	void createMesh(void* vertices,
					unsigned short* indices,
					unsigned int numOfVertices,
					unsigned int numOfIndices,
					std::vector<MeshAttribute>& attributes);
	void createMesh(void* vertices,
					unsigned short* indices,
					unsigned int numOfVertices,
					unsigned int numOfIndices,
					MeshAttribute::MeshAttributeProfile attributeProfile);
	/*void createMesh(Vertex* vertices,
					unsigned short* indices,
					unsigned int numOfVertices,
					unsigned int numOfIndices,
					std::vector<MeshAttribute>& attributes);*/
	void renderMesh();
	void clearMesh();

private:
	unsigned int m_VAO, m_VBO, m_IBO;
	int m_idxCnt;

};