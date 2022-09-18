#include "Mesh.h"
#include <GL\glew.h>
#include "common.h"
#include "Vertex.h"
#include "ShapeGenerator.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

Mesh::Mesh()
	:
	m_pVAO(nullptr),
	m_pVBO(nullptr),
	m_pIBO(nullptr),
	m_idxCnt(0)
{
}

Mesh::~Mesh()
{
	/* GLCALL results in error here */
	clearMesh();
}

void Mesh::createMesh(shapeType type)
{
	/* Shape object */
	ShapeData shape;
	ShapeGenerator::ShapeAttributeProfile profile = ShapeGenerator::ShapeAttributeProfile::POS_TEX_NOR;
	/* Choose shape */
	switch (type)
	{
	case shapeType::_TEAPOT_:
		shape = ShapeGenerator::makeTeapot(profile);
		break;
	case shapeType::_ARROW_:
		shape = ShapeGenerator::makeArrow(profile);
		break;
	case shapeType::_PLANE_:
		shape = ShapeGenerator::makePlane(profile, 20, 2);
		break;
	case shapeType::_CUBE_:
		shape = ShapeGenerator::makeCube(profile);
		break;
	case shapeType::_TORUS_:
		shape = ShapeGenerator::makeTorus(profile, 50);
		break;
	case shapeType::_SPHERE_:
	default:
		shape = ShapeGenerator::makeSphere(profile, 50);
		break;
	}
	/* Create mesh */
	createMesh(	shape.vertices,
				shape.indices,
				shape.numVertices,
				shape.numIndices,
				VertexAttribute::VertexAttributeProfile::POS_TEX_NOR);
	/* Clean up */
	shape.cleanUp();
}

void Mesh::createMesh(void* vertices,
					  unsigned short* indices,
					  unsigned int numOfVertices,
					  unsigned int numOfIndices,
					  VertexAttribute::VertexAttributeProfile attributeProfile)
{
	std::vector<VertexAttribute> attributes;
	if(VertexAttribute::fillAttribListfromProfile(attributeProfile, attributes) == true)
		createMesh(vertices, indices, numOfVertices, numOfIndices, attributes);
}

void Mesh::createMesh(void* vertices,
					  unsigned short* indices,
					  unsigned int numOfVertices,
					  unsigned int numOfIndices,
					  std::vector<VertexAttribute>& attributes)
{
	m_idxCnt = numOfIndices;
	//std::cout << "[Debug]: createMesh chk 1\n";

	const size_t _VERTEX_BYTE_SIZE_ = VertexAttribute::computeVertexByteOffset(attributes);

	//std::cout << "[Debug]: createMesh chk 2\n";
	/* Vertex array object */
	m_pVAO = new VertexArray();
	//std::cout << "[Debug]: createMesh chk 3\n";

	/* Index buffer object */
	m_pIBO = new IndexBuffer(indices, numOfIndices);

	//std::cout << "[Debug]: createMesh chk 4\n" << "numofVertices " << numOfVertices;
	//std::cout << "\n_VERTEX_BYTE_SIZE_ " << _VERTEX_BYTE_SIZE_ << "\n";
	//std::cout << "\nfloat size " << sizeof(float) << "\n";

	/* Vertex buffer object */
	m_pVBO = new VertexBuffer(vertices, _VERTEX_BYTE_SIZE_ * numOfVertices);
	//std::cout << "[Debug]: createMesh chk 5\n";

	m_pVAO->bind();
	m_pIBO->bind();
	m_pVBO->bind();
	//std::cout << "[Debug]: createMesh chk 6\n";

	/* Vertex attributes */
	m_pVAO->addBuffer(*m_pVBO, attributes);
	//std::cout << "[Debug]: createMesh chk 7\n";

	//Unbind Vertex array
	m_pVAO->unBind();
	//std::cout << "[Debug]: createMesh chk 8\n";
}

void Mesh::createMesh(Vertex* vertices,
					  unsigned short* indices,
					  unsigned int numOfVertices,
					  unsigned int numOfIndices,
					  bool hasTexture)
{
	std::vector<VertexAttribute> attributes;
	VertexAttribute::VertexAttributeProfile profile;
	if (hasTexture)
	{
		profile = VertexAttribute::VertexAttributeProfile::POS_COL_TEX_NOR;
	}
	else
	{
		profile = VertexAttribute::VertexAttributeProfile::POS_COL_NOR;
	}
	if (VertexAttribute::fillAttribListfromProfile(profile, attributes) == true)
		createMesh(vertices, indices, numOfVertices, numOfIndices, attributes);

	/*
	* Attribute data can also be sent to shader
	* using glVertexAttrib3f(attribId, <data>)
	* without enabling the corresponding attrib-Array.
	* Note: can be used to switch from solid color & dynamic color etc.
	*/
}

void Mesh::renderMesh()
{
	/* Bind VAO */
	m_pVAO->bind();
	/* Bind IBO */
	//GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO));
	/* Draw Mesh */
	GLCALL(glDrawElements(GL_TRIANGLES, m_idxCnt,
						  GL_UNSIGNED_SHORT, 0));
	/* Unbind IBO */
	//GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	/* Unbind VAO */
	m_pVAO->unBind();
}

void Mesh::clearMesh()
{
	//UnBind causes error here
	if (m_pIBO != nullptr)
	{
		delete m_pIBO;
		m_pIBO = nullptr;
	}

	if (m_pVBO != nullptr)
	{
		delete m_pVBO;
		m_pVBO = nullptr;
	}

	if (m_pVAO != nullptr)
	{
		delete m_pVAO;
		m_pVAO = nullptr;
	}

	m_idxCnt = 0;
}