#include "stdafx.h"
#include "SDXMeshData.h"

using namespace SDXEngine;

SDXMeshData::SDXMeshData() : m_vertexData(nullptr), m_indexData(nullptr), m_vertexCount(0), m_indexCount(0), m_vertexType(SDXVERTEX_TYPE_UNKNOWN)
{
}

SDXMeshData::~SDXMeshData()
{
	Destroy();
}

SDXErrorId SDXEngine::SDXMeshData::CreateVertexArray(SDXVertexType type, UINT count)
{
	// Should not run this again if already created. Create another mesh data instance.
	if (m_vertexData)
		return SDX_ERROR_MESHDATA_VERTEXARRAY_ALREADY_CREATED;

	if (count == 0)
		return SDX_ERROR_MESHDATA_CREATE_VERTEXARRAY_COUNT_ZERO;

	switch (type)
	{
	case SDXVERTEX_TYPE_PC:
		m_vertexData = new SDXVertexPC[count];
		break;

	case SDXVERTEX_TYPE_PNT:
		m_vertexData = new SDXVertexPNT[count];
		break;

	default:
		return SDX_ERROR_MESHDATA_UNKNOWN_VERTEXTYPE;
	}

	m_vertexCount = count;
	m_vertexType = type;

	return SDX_ERROR_NONE;
}

SDXErrorId SDXEngine::SDXMeshData::CreateIndexArray(UINT count)
{
	if (m_indexData)
		return SDX_ERROR_MESHDATA_INDEXARRAY_ALREADY_CREATED;

	if (count == 0)
		return SDX_ERROR_MESHDATA_CREATE_INDEXARRAY_COUNT_ZERO;

	m_indexCount = count;
	m_indexData = new int[m_indexCount];
	return SDX_ERROR_NONE;
}

SDXVertex * SDXEngine::SDXMeshData::GetVertexData() const
{
	return m_vertexData;
}

int * SDXEngine::SDXMeshData::GetIndexData() const
{
	return m_indexData;
}

SDXVertexType SDXEngine::SDXMeshData::GetVertexType() const
{
	return m_vertexType;
}

int SDXEngine::SDXMeshData::GetVertexCount() const
{
	return m_vertexCount;
}

int SDXEngine::SDXMeshData::GetIndexCount() const
{
	return m_indexCount;
}

void SDXEngine::SDXMeshData::Destroy()
{
	m_vertexCount = 0;
	m_indexCount = 0;
	m_vertexType = SDXVERTEX_TYPE_UNKNOWN;

	if (m_indexData)
	{
		delete[] m_indexData;
		m_indexData = nullptr;
	}

	if (m_vertexData)
	{
		delete[] m_vertexData;
		m_vertexData = nullptr;
	}
}
