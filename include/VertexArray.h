#pragma once

#include "VertexBuffer.h"
#include "VertexAttribute.h"

//forward declaration

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const std::vector<VertexAttribute>& attributes);

	void Bind() const;
	void UnBind() const;

private:
	unsigned int m_Id;
};

