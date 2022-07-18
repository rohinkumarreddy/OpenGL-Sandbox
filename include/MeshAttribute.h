#pragma once

#include <vector>

//forward declaration
struct Vertex;

class MeshAttribute
{
public:
	enum class MeshDataType {
		_INVALID_	= -1,
		_INT_		= 0x1404,
		_FLOAT_		= 0x1406,
		_BYTE_		= 0x1400,
		_SHORT_		= 0x1402,
		_DOUBLE_	= 0x140A,
		_GENERIC_	= 0x1401,
		_UBYTE_		= 0x1401
	};

	enum class MeshAttributeProfile
	{
		POS_COL_TEX_NOR,
		POS_COL_NOR,
		POS_NOR,
		POS_COL
	};

	static bool fillAttribListfromProfile(	MeshAttributeProfile& profile,
											std::vector<MeshAttribute>& attributes );

	static size_t computeVertexByteOffset(std::vector<MeshAttribute>& attributes);

	static size_t computeAttributeByteOffset(MeshAttribute& attribute);

	MeshAttribute();

	MeshAttribute(MeshDataType type, int count = 1);

	const int GLtype() { return (int)m_dataType; }
	inline const MeshDataType type() { return m_dataType; }
	inline const int count() { return m_count; }
	inline const size_t size() { return m_size; }
	inline const size_t perElementSize() { return m_size/m_count; }

private:
	MeshDataType m_dataType;
	int m_count;
	size_t m_size;
};