#include "Mesh.h"
#include <GL\glew.h>
#include "common.h"
#include "Vertex.h"
#include "ShapeGenerator.h"

Mesh::Mesh()
	:
	m_VAO(0),
	m_VBO(0),
	m_IBO(0),
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
				MeshAttribute::MeshAttributeProfile::POS_COL_TEX_NOR);
	/* Clean up */
	shape.cleanUp();
}

void Mesh::createMesh(void* vertices,
					  unsigned short* indices,
					  unsigned int numOfVertices,
					  unsigned int numOfIndices,
					  MeshAttribute::MeshAttributeProfile attributeProfile)
{
	std::vector<MeshAttribute> attributes;
	if(MeshAttribute::fillAttribListfromProfile(attributeProfile, attributes) == true)
		createMesh(vertices, indices, numOfVertices, numOfIndices, attributes);
}

void Mesh::createMesh(void* vertices,
					  unsigned short* indices,
					  unsigned int numOfVertices,
					  unsigned int numOfIndices,
					  std::vector<MeshAttribute>& attributes)
{
	m_idxCnt = numOfIndices;

	const size_t _VERTEX_BYTE_SIZE_ = MeshAttribute::computeVertexByteOffset(attributes);

	/* Vertex array object */
	GLCALL(glGenVertexArrays(1, &m_VAO));
	GLCALL(glBindVertexArray(m_VAO));

	/* Index buffer object */
	GLCALL(glGenBuffers(1, &m_IBO));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO));
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
						sizeof(indices[0]) * numOfIndices,
						indices, GL_STATIC_DRAW));

	/* Vertex buffer object */
	GLCALL(glGenBuffers(1, &m_VBO));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
	GLCALL(glBufferData(GL_ARRAY_BUFFER,
						_VERTEX_BYTE_SIZE_ * numOfVertices,
						vertices, GL_STATIC_DRAW));

	/* Vertex attributes */
	{
		size_t _ATTRIBUTE_OFFSET_ = 0;
		int atrbId = 0;
		for each (auto atrb in attributes)
		{
			atrb.init(atrbId, _VERTEX_BYTE_SIZE_, _ATTRIBUTE_OFFSET_);
			_ATTRIBUTE_OFFSET_ += atrb.size();
			atrbId++;
		}
	}

	//Unbind Vertex array
	GLCALL(glBindVertexArray(0));
}

void Mesh::createMesh(Vertex* vertices,
					  unsigned short* indices,
					  unsigned int numOfVertices,
					  unsigned int numOfIndices,
					  bool hasTexture)
{
	m_idxCnt = numOfIndices;

	/* Vertex array object */
	GLCALL(glGenVertexArrays(1, &m_VAO));
	GLCALL(glBindVertexArray(m_VAO));

	/* Index buffer object */
	GLCALL(glGenBuffers(1, &m_IBO));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO));
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
						sizeof(indices[0]) * numOfIndices,
						indices, GL_STATIC_DRAW));

	/* Vertex buffer object */
	GLCALL(glGenBuffers(1, &m_VBO));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
	GLCALL(glBufferData(GL_ARRAY_BUFFER,
						sizeof(vertices[0]) * numOfVertices,
						vertices, GL_STATIC_DRAW));

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

void Mesh::renderMesh()
{
	/* Bind VAO */
	GLCALL(glBindVertexArray(m_VAO));
	/* Bind IBO */
	//GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO));
	/* Draw Mesh */
	GLCALL(glDrawElements(GL_TRIANGLES, m_idxCnt,
						  GL_UNSIGNED_SHORT, 0));
	/* Unbind IBO */
	//GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	/* Unbind VAO */
	GLCALL(glBindVertexArray(0));
}

void Mesh::clearMesh()
{
	if (m_IBO != 0)
	{
		glDeleteBuffers(1, &m_IBO);
		m_IBO = 0;
	}

	if (m_VBO != 0)
	{
		glDeleteBuffers(1, &m_VBO);
		m_VBO = 0;
	}

	if (m_VAO != 0)
	{
		glDeleteVertexArrays(1, &m_VAO);
		m_VAO = 0;
	}

	m_idxCnt = 0;
}