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
		return SDX_ERROR_MESH_MESHDATA_NULL;

	if (directX == nullptr)
		return SDX_ERROR_MESH_DIRECTX_NULL;

	m_subMeshes.push_back(SDXSubMesh());
	SDXSubMesh* mesh = &m_subMeshes[(int)m_subMeshes.size() - 1];

	SDXErrorId error = SDX_ERROR_NONE;

	mesh->SetMaterialID(meshData->GetMaterialID());
	error = mesh->GetVertexBuffer()->LoadData(meshData, directX);
	error = mesh->GetIndexBuffer()->LoadData(meshData, directX);

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
