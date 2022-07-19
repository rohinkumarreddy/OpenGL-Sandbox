#include "IndexBuffer.h"
#include "common.h"

IndexBuffer::IndexBuffer(const unsigned short* data, unsigned int count)
    :m_count(count),
    m_Id(0)
{
    ASSERT(sizeof(unsigned short) == sizeof(GLushort));

    GLCALL(glGenBuffers(1, &m_Id));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id));
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                        count * sizeof(unsigned short),
                        data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    //GLCALL causes error here
	glDeleteBuffers(1, &m_Id);
    m_Id = 0;
}

void IndexBuffer::bind() const
{
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id));
}

void IndexBuffer::unBind() const
{
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
