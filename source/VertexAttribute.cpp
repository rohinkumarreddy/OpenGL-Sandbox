#include "VertexAttribute.h"
#include "common.h"

bool VertexAttribute::fillAttribListfromProfile(	VertexAttributeProfile& profile,
												std::vector<VertexAttribute>& attributes)
{
	const int TRIO = 3;
	const int PAIR = 2;
	const int UNIT = 1;
	switch (profile)
	{
	case VertexAttributeProfile::POS_COL_TEX_NOR:
	{
		attributes.push_back(VertexAttribute(VertexAttribute::VertexDataType::_FLOAT_, TRIO));
		attributes.push_back(VertexAttribute(VertexAttribute::VertexDataType::_FLOAT_, TRIO));
		attributes.push_back(VertexAttribute(VertexAttribute::VertexDataType::_FLOAT_, TRIO));
		attributes.push_back(VertexAttribute(VertexAttribute::VertexDataType::_FLOAT_, TRIO));
		break;
	}
	case VertexAttributeProfile::POS_COL_NOR:
	{
		attributes.push_back(VertexAttribute(VertexAttribute::VertexDataType::_FLOAT_, TRIO));
		attributes.push_back(VertexAttribute(VertexAttribute::VertexDataType::_FLOAT_, TRIO));
		attributes.push_back(VertexAttribute(VertexAttribute::VertexDataType::_FLOAT_, TRIO));
		break;
	}
	case VertexAttributeProfile::POS_TEX_NOR:
	{
		attributes.push_back(VertexAttribute(VertexAttribute::VertexDataType::_FLOAT_, TRIO));
		attributes.push_back(VertexAttribute(VertexAttribute::VertexDataType::_FLOAT_, TRIO));
		attributes.push_back(VertexAttribute(VertexAttribute::VertexDataType::_FLOAT_, TRIO));
		break;
	}
	case VertexAttributeProfile::POS_COL:
	{
		attributes.push_back(VertexAttribute(VertexAttribute::VertexDataType::_FLOAT_, TRIO));
		attributes.push_back(VertexAttribute(VertexAttribute::VertexDataType::_FLOAT_, TRIO));
		break;
	}
	case VertexAttributeProfile::POS_NOR:
	{
		attributes.push_back(VertexAttribute(VertexAttribute::VertexDataType::_FLOAT_, TRIO));
		attributes.push_back(VertexAttribute(VertexAttribute::VertexDataType::_FLOAT_, TRIO));
		break;
	}
	default:
		return false;
		break;
	}
	return true;
}

size_t VertexAttribute::computeVertexByteOffset(const std::vector<VertexAttribute>& attributes)
{
	size_t tsize = 0;
	for each (auto atrb in attributes)
		tsize += atrb.size();
	return tsize;
}

size_t VertexAttribute::computeAttributeByteOffset(VertexAttribute& attribute)
{
	size_t _dsize = 0;
	switch (attribute.type())
	{
	case VertexDataType::_INT_:
		_dsize = sizeof(int);
		break;
	case VertexDataType::_FLOAT_:
		_dsize = sizeof(float);
		break;
	case VertexDataType::_BYTE_:
		_dsize = sizeof(signed char);
		break;
	case VertexDataType::_DOUBLE_:
		_dsize = sizeof(double);
		break;
	case VertexDataType::_SHORT_:
		_dsize = sizeof(short);
		break;
	case VertexDataType::_UBYTE_:
	//case VertexDataType::_GENERIC_:
		_dsize = sizeof(unsigned char);
		break;
	case VertexDataType::_INVALID_:
	default:
		break;
	}
	return attribute.count() * _dsize;
}

VertexAttribute::VertexAttribute()
{
	m_dataType = VertexDataType::_GENERIC_;
	m_count = 1;
	m_size = sizeof(unsigned char);
}

VertexAttribute::VertexAttribute(VertexDataType type, int count)
{
	m_dataType = type;
	m_count = count;
	m_size = computeAttributeByteOffset(*this);
}

void VertexAttribute::init(int id,
						 const int _VERTEX_BYTE_SIZE_,
						 const int _ATTRIBUTE_OFFSET_)
{
	GLCALL(glEnableVertexAttribArray(id));
	/*
	* void glVertexAttribPointer(	GLuint index,
									GLint size,
									GLenum type,
									GLboolean normalized,
									GLsizei stride,
									const GLvoid * pointer);
	*/
	GLCALL(glVertexAttribPointer(	id,
									m_count,
									(int)m_dataType,//GL_FLOAT,
									GL_FALSE,
									_VERTEX_BYTE_SIZE_,
									(const void*)_ATTRIBUTE_OFFSET_ ) );
}