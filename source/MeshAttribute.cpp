#include "MeshAttribute.h"
#include "common.h"

bool MeshAttribute::fillAttribListfromProfile(	MeshAttributeProfile& profile,
												std::vector<MeshAttribute>& attributes)
{
	const int TRIO = 3;
	const int PAIR = 2;
	const int UNIT = 1;
	switch (profile)
	{
	case MeshAttributeProfile::POS_COL_TEX_NOR:
	{
		attributes.push_back(MeshAttribute(MeshAttribute::MeshDataType::_FLOAT_, TRIO));
		attributes.push_back(MeshAttribute(MeshAttribute::MeshDataType::_FLOAT_, TRIO));
		attributes.push_back(MeshAttribute(MeshAttribute::MeshDataType::_FLOAT_, TRIO));
		attributes.push_back(MeshAttribute(MeshAttribute::MeshDataType::_FLOAT_, TRIO));
		break;
	}
	case MeshAttributeProfile::POS_COL_NOR:
	{
		attributes.push_back(MeshAttribute(MeshAttribute::MeshDataType::_FLOAT_, TRIO));
		attributes.push_back(MeshAttribute(MeshAttribute::MeshDataType::_FLOAT_, TRIO));
		attributes.push_back(MeshAttribute(MeshAttribute::MeshDataType::_FLOAT_, TRIO));
		break;
	}
	case MeshAttributeProfile::POS_COL:
	{
		attributes.push_back(MeshAttribute(MeshAttribute::MeshDataType::_FLOAT_, TRIO));
		attributes.push_back(MeshAttribute(MeshAttribute::MeshDataType::_FLOAT_, TRIO));
		break;
	}
	case MeshAttributeProfile::POS_NOR:
	{
		attributes.push_back(MeshAttribute(MeshAttribute::MeshDataType::_FLOAT_, TRIO));
		attributes.push_back(MeshAttribute(MeshAttribute::MeshDataType::_FLOAT_, TRIO));
		break;
	}
	default:
		return false;
		break;
	}
	return true;
}

size_t MeshAttribute::computeVertexByteOffset(std::vector<MeshAttribute>& attributes)
{
	size_t tsize = 0;
	for each (auto atrb in attributes)
		tsize += atrb.size();
	return tsize;
}

size_t MeshAttribute::computeAttributeByteOffset(MeshAttribute& attribute)
{
	size_t _dsize = 0;
	switch (attribute.type())
	{
	case MeshDataType::_INT_:
		_dsize = sizeof(int);
		break;
	case MeshDataType::_FLOAT_:
		_dsize = sizeof(float);
		break;
	case MeshDataType::_BYTE_:
		_dsize = sizeof(signed char);
		break;
	case MeshDataType::_DOUBLE_:
		_dsize = sizeof(double);
		break;
	case MeshDataType::_SHORT_:
		_dsize = sizeof(short);
		break;
	case MeshDataType::_UBYTE_:
	//case MeshDataType::_GENERIC_:
		_dsize = sizeof(unsigned char);
		break;
	case MeshDataType::_INVALID_:
	default:
		break;
	}
	return attribute.count() * _dsize;
}

MeshAttribute::MeshAttribute()
{
	m_dataType = MeshDataType::_GENERIC_;
	m_count = 1;
	m_size = sizeof(unsigned char);
}

MeshAttribute::MeshAttribute(MeshDataType type, int count)
{
	m_dataType = type;
	m_count = count;
	m_size = computeAttributeByteOffset(*this);
}

void MeshAttribute::init(int id,
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