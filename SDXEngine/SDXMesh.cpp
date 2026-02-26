#include "stdafx.h"
#include "SDXMesh.h"

using namespace SDXEngine;

SDXMesh::SDXMesh()
{
}


SDXMesh::~SDXMesh()
{
}

SDXErrorId SDXEngine::SDXMesh::AddSubMesh(SDXMeshData* meshData, SDXDirectX* directX)
{
	if (meshData == nullptr)
		return SDXErrorId::SDX_ERROR_MESH_MESHDATA_NULL;

	if (directX == nullptr)
		return SDXErrorId::SDX_ERROR_MESH_DIRECTX_NULL;

	m_subMeshes.push_back(SDXSubMesh());
	SDXSubMesh* mesh = &m_subMeshes[(int)m_subMeshes.size() - 1];

	SDXErrorId error = mesh->LoadMeshData(meshData, directX);

	if (error == SDXErrorId::SDX_ERROR_NONE)
	{
		auto subMeshMin = mesh->GetMinVertexPos();
		auto subMeshMax = mesh->GetMaxVertexPos();

		if (subMeshMax.x > m_maxVertexPos.x) m_maxVertexPos.x = subMeshMax.x;
		if (subMeshMax.y > m_maxVertexPos.y) m_maxVertexPos.y = subMeshMax.y;
		if (subMeshMax.z > m_maxVertexPos.z) m_maxVertexPos.z = subMeshMax.z;

		if (subMeshMin.x < m_minVertexPos.x) m_minVertexPos.x = subMeshMin.x;
		if (subMeshMin.y < m_minVertexPos.y) m_minVertexPos.y = subMeshMin.y;
		if (subMeshMin.z < m_minVertexPos.z) m_minVertexPos.z = subMeshMin.z;
	}

	return error;
}

SDXSubMesh * SDXEngine::SDXMesh::GetSubMesh(int index)
{
	SDXSubMesh* mesh = nullptr;
	if (index < (int)m_subMeshes.size())
		mesh = &m_subMeshes[index];

	return mesh;
}

int SDXEngine::SDXMesh::GetTotalSubMeshes() const
{
	return (int)m_subMeshes.size();
}

void SDXEngine::SDXMesh::Release()
{
	for (int i = 0; i < static_cast<int>(m_subMeshes.size()); i++)
	{
		m_subMeshes[i].Release();
	}
}
