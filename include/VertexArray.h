#pragma once

#include "VertexBuffer.h"
#include "VertexAttribute.h"

//forward declaration

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void addBuffer(const VertexBuffer& vb, const std::vector<VertexAttribute>& attributes);

	void bind() const;
	void unBind() const;

private:
	unsigned int m_Id;
};

