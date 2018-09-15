//--------------------------------------------------------------------------
//
// Sub-mesh class.
//
//--------------------------------------------------------------------------
#pragma once

#include <string>

#include "SDXVertexBuffer.h"
#include "SDXIndexBuffer.h"

namespace SDXEngine
{
	class SDXSubMesh
	{
	public:
		SDXSubMesh();
		~SDXSubMesh();

		SDXErrorId LoadMeshData(SDXEngine::SDXMeshData* data, SDXDirectX* directX);

		void SetMaterialID(const std::string& materialID);
		const std::string& GetMaterialID() const;
		SDXVertexBuffer* GetVertexBuffer();
		SDXIndexBuffer* GetIndexBuffer();

	private:
		std::string m_materialID;

		SDXVertexBuffer m_vertices;
		SDXIndexBuffer m_indices;
	};

}