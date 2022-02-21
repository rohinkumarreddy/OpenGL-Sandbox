#pragma once

//forward declaration
struct Vertex;

class Mesh
{
public:
	Mesh();

	void createMesh(Vertex* vertices,
					unsigned short* indices,
					unsigned int numOfVertices,
					unsigned int numOfIndices);
	void renderMesh();
	void clearMesh();

	~Mesh();

private:
	unsigned int m_VAO, m_VBO, m_IBO;
	int m_idxCnt;

};