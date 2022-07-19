#pragma once

#include <vector>
#include "VertexAttribute.h"

//forward declaration
struct Vertex;
class VertexArray;
class VertexBuffer;
class IndexBuffer;

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
#if 0
	void createMesh(Vertex* vertices,
					unsigned short* indices,
					unsigned int numOfVertices,
					unsigned int numOfIndices,
					bool hasTexture = true);
#endif
	void createMesh(void* vertices,
					unsigned short* indices,
					unsigned int numOfVertices,
					unsigned int numOfIndices,
					std::vector<VertexAttribute>& attributes);
	void createMesh(void* vertices,
					unsigned short* indices,
					unsigned int numOfVertices,
					unsigned int numOfIndices,
					VertexAttribute::VertexAttributeProfile attributeProfile);
	/*void createMesh(Vertex* vertices,
					unsigned short* indices,
					unsigned int numOfVertices,
					unsigned int numOfIndices,
					std::vector<MeshAttribute>& attributes);*/
	void renderMesh();
	void clearMesh();

private:
	VertexArray* m_pVAO;
	VertexBuffer* m_pVBO;
	IndexBuffer* m_pIBO;
	int m_idxCnt;

};