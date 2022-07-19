#include "VertexArray.h"
#include "common.h"

VertexArray::VertexArray()
	:m_Id(0)
{
	GLCALL(glGenVertexArrays(1, &m_Id));
}

VertexArray::~VertexArray()
{
	//GLCALL causes error here
	glDeleteVertexArrays(1, &m_Id);
}

void VertexArray::AddBuffer(const VertexBuffer& vb,
							const std::vector<VertexAttribute>& attributes)
{
	Bind();
	vb.Bind();

	const size_t _VERTEX_BYTE_SIZE_ = VertexAttribute::computeVertexByteOffset(attributes);

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
}

void VertexArray::Bind() const
{
	GLCALL(glBindVertexArray(m_Id));
}

void VertexArray::UnBind() const
{
	GLCALL(glBindVertexArray(0));
}
