#include "stdafx.h"
#include "SDXSubMesh.h"

using namespace SDXEngine;


SDXSubMesh::SDXSubMesh()
{
}


SDXSubMesh::~SDXSubMesh()
{
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
