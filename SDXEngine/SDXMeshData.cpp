#include "stdafx.h"
#include "SDXMeshData.h"

using namespace SDXEngine;

SDXMeshData::SDXMeshData() : m_vertexData(nullptr), m_indexData(nullptr), m_vertexCount(0), m_indexCount(0),
	m_vertexType(SDXVERTEX_TYPE_UNKNOWN), m_materialID(""), m_minVertexPos(XMFLOAT3(0,0,0)), m_maxVertexPos(XMFLOAT3(0,0,0))
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
	m_indexData = new unsigned int[m_indexCount];
	return SDX_ERROR_NONE;
}

SDXVertex * SDXEngine::SDXMeshData::GetVertexData() const
{
	return m_vertexData;
}

unsigned int * SDXEngine::SDXMeshData::GetIndexData() const
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

void SDXEngine::SDXMeshData::SetMaterialID(const std::string & id)
{
	m_materialID = id;
}

const std::string & SDXEngine::SDXMeshData::GetMaterialID() const
{
	return m_materialID;
}

void SDXEngine::SDXMeshData::SetMinVertexPos(const XMFLOAT3 & pos)
{
	m_minVertexPos = pos;
}

void SDXEngine::SDXMeshData::SetMaxVertexPos(const XMFLOAT3 & pos)
{
	m_maxVertexPos = pos;
}

const XMFLOAT3 & SDXEngine::SDXMeshData::GetMinVertexPos() const
{
	return m_minVertexPos;
}

const XMFLOAT3 & SDXEngine::SDXMeshData::GetMaxVertexPos() const
{
	return m_maxVertexPos;
}

void SDXEngine::SDXMeshData::Destroy()
{
	m_vertexCount = 0;
	m_indexCount = 0;
	m_vertexType = SDXVERTEX_TYPE_UNKNOWN;
	m_materialID = "";
	m_minVertexPos = XMFLOAT3(0, 0, 0);
	m_maxVertexPos = XMFLOAT3(0, 0, 0);

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
