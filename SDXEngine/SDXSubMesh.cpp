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
	SDXErrorId error = SDX_ERROR_NONE;

	// Clear vertex buffer if already existing data
	if (m_vertices.GetBuffer())
		m_vertices.Release();

	// Load vertex data 
	error = m_vertices.LoadData(data, directX);
	if (error != SDX_ERROR_NONE)
		return error;

	// Clear index buffer if already existing data
	if (m_indices.GetBuffer())
		m_indices.Release();

	// Load index data
	error = m_indices.LoadData(data, directX);

	// Get material ID for mesh
	m_materialID = data->GetMaterialID();

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
