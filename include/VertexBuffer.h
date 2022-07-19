#pragma once

class VertexBuffer
{
public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();
	void bind() const;
	void unBind() const;
private:
	unsigned int m_Id;
};
