#include "VertexBuffer.h"
#include "common.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
    :m_Id(0)
{
    GLCALL(glGenBuffers(1, &m_Id));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_Id));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    //GLCALL causes error here
    glDeleteBuffers(1, &m_Id);
    m_Id = 0;
}

void VertexBuffer::bind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_Id));
}

void VertexBuffer::unBind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
