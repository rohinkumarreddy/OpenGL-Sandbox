#pragma once

class IndexBuffer
{
public:
	IndexBuffer(const unsigned short* data, unsigned int count);
	~IndexBuffer();

	void bind() const;
	void unBind() const;
	inline unsigned int getCount() const { return m_count; }
private:
	unsigned int m_Id;
	unsigned int m_count;
};