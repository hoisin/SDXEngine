#include "stdafx.h"
#include "SDXSubMesh.h"

using namespace SDXEngine;


SDXSubMesh::SDXSubMesh() : m_materialID("")
{
}


SDXSubMesh::~SDXSubMesh()
{
}

//
//	LoadMeshData()
//		-- Loads mesh data into internal mesh class
//
SDXErrorId SDXEngine::SDXSubMesh::LoadMeshData(SDXEngine::SDXMeshData* data, SDXDirectX* directX)
{
	SDXErrorId error = SDXErrorId::SDX_ERROR_NONE;

	// Clear vertex buffer if already existing data
	if (m_vertices.GetBuffer())
		m_vertices.Release();

	// Load vertex data 
	error = m_vertices.LoadData(data, directX);
	if (error != SDXErrorId::SDX_ERROR_NONE)
		return error;

	// Clear index buffer if already existing data
	if (m_indices.GetBuffer())
		m_indices.Release();

	// Load index data
	error = m_indices.LoadData(data, directX);

	// Get material ID for mesh
	m_materialID = data->GetMaterialID();

	m_minVertexPos = data->GetMinVertexPos();
	m_maxVertexPos = data->GetMaxVertexPos();

	return error;
}

void SDXEngine::SDXSubMesh::SetMaterialID(const std::string & materialID)
{
	m_materialID = materialID;
}

const std::string & SDXEngine::SDXSubMesh::GetMaterialID() const
{
	return m_materialID;
}

SDXVertexBuffer* SDXEngine::SDXSubMesh::GetVertexBuffer()
{
	return &m_vertices;
}

SDXIndexBuffer* SDXEngine::SDXSubMesh::GetIndexBuffer()
{
	return &m_indices;
}

const XMFLOAT3& SDXEngine::SDXSubMesh::GetMinVertexPos() const
{
	return m_minVertexPos;
}

const XMFLOAT3& SDXEngine::SDXSubMesh::GetMaxVertexPos() const
{
	return m_maxVertexPos;
}

void SDXEngine::SDXSubMesh::Release()
{
	if (m_vertices.GetBuffer())
	{
		m_vertices.Release();
	}

	if (m_indices.GetBuffer())
	{
		m_indices.Release();
	}
}
