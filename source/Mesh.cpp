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
	/* Choose shape */
	switch (type)
	{
	case shapeType::_TEAPOT_:
		shape = ShapeGenerator::makeTeapot();
		break;
	case shapeType::_ARROW_:
		shape = ShapeGenerator::makeArrow();
		break;
	case shapeType::_PLANE_:
		shape = ShapeGenerator::makePlane(100, 2);
		break;
	case shapeType::_CUBE_:
		shape = ShapeGenerator::makeCube();
		break;
	case shapeType::_TORUS_:
		shape = ShapeGenerator::makeTorus(50);
		break;
	case shapeType::_SPHERE_:
	default:
		shape = ShapeGenerator::makeSphere(50);
		break;
	}
	/* Create mesh */
	createMesh(	shape.vertices,
				shape.indices,
				shape.numVertices,
				shape.numIndices,
				VertexAttribute::VertexAttributeProfile::POS_COL_TEX_NOR);
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

	const size_t _VERTEX_BYTE_SIZE_ = VertexAttribute::computeVertexByteOffset(attributes);

	/* Vertex array object */
	m_pVAO = new VertexArray();

	/* Index buffer object */
	m_pIBO = new IndexBuffer(indices, numOfIndices);

	/* Vertex buffer object */
	m_pVBO = new VertexBuffer(vertices, _VERTEX_BYTE_SIZE_ * numOfVertices);

	m_pVAO->Bind();
	m_pIBO->Bind();
	m_pVBO->Bind();

	/* Vertex attributes */
	m_pVAO->AddBuffer(*m_pVBO, attributes);

	//Unbind Vertex array
	m_pVAO->UnBind();
}

#if 0
void Mesh::createMesh(Vertex* vertices,
					  unsigned short* indices,
					  unsigned int numOfVertices,
					  unsigned int numOfIndices,
					  bool hasTexture)
{
	m_idxCnt = numOfIndices;

	/* Vertex array object */
	m_pVAO = new VertexArray();

	/* Index buffer object */
	m_pIBO = new IndexBuffer(indices, numOfIndices);

	/* Vertex buffer object */
	m_pVBO = new VertexBuffer(vertices, sizeof(vertices[0]) * numOfVertices);

	/* Vertex attributes */
	GLCALL(glEnableVertexAttribArray(0));
	GLCALL(glEnableVertexAttribArray(1));
	GLCALL(glEnableVertexAttribArray(2));
	if (hasTexture)
	{
		GLCALL(glEnableVertexAttribArray(3));

		//attribute-0|position
		GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT,
									GL_FALSE, VERTEX_BYTE_SIZE, 0));
		//4th element is by default 1.0f

		//attribute-1|color
		GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT,
									GL_FALSE, VERTEX_BYTE_SIZE,
									(const void*)(3 * sizeof(float))));

		//attribute-2|texture coordinates
		GLCALL(glVertexAttribPointer(2, 3, GL_FLOAT,
									GL_FALSE, VERTEX_BYTE_SIZE,
									(const void*)(6 * sizeof(float))));

		//attribute-3|normal
		GLCALL(glVertexAttribPointer(3, 3, GL_FLOAT,
									GL_FALSE, VERTEX_BYTE_SIZE,
									(const void*)(9 * sizeof(float))));
	}
	else
	{
		//attribute-0|position
		GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT,
									GL_FALSE, NO_TEX_VERTEX_BYTE_SIZE, 0));
		//4th element is by default 1.0f

		//attribute-1|color
		GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT,
									GL_FALSE, NO_TEX_VERTEX_BYTE_SIZE,
									(const void*)(3 * sizeof(float))));

		//attribute-2|normal
		GLCALL(glVertexAttribPointer(2, 3, GL_FLOAT,
									GL_FALSE, NO_TEX_VERTEX_BYTE_SIZE,
									(const void*)(6 * sizeof(float))));
	}
	//Unbind Vertex array
	GLCALL(glBindVertexArray(0));

	/*
	* Attribute data can also be sent to shader
	* using glVertexAttrib3f(attribId, <data>)
	* without enabling the corresponding attrib-Array.
	* Note: can be used to switch from solid color & dynamic color etc.
	*/
}
#endif

void Mesh::renderMesh()
{
	/* Bind VAO */
	m_pVAO->Bind();
	/* Bind IBO */
	//GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO));
	/* Draw Mesh */
	GLCALL(glDrawElements(GL_TRIANGLES, m_idxCnt,
						  GL_UNSIGNED_SHORT, 0));
	/* Unbind IBO */
	//GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	/* Unbind VAO */
	m_pVAO->UnBind();
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