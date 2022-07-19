#pragma once

class IndexBuffer
{
public:
	IndexBuffer(const unsigned short* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void UnBind() const;
	inline unsigned int getCount() const { return m_count; }
private:
	unsigned int m_Id;
	unsigned int m_count;
};