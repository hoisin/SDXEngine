//--------------------------------------------------------------------------
//
// Mesh class containing sub-meshes
//
//--------------------------------------------------------------------------
#pragma once

#include "SDXErrors.h"
#include "SDXSubMesh.h"
#include <vector>

namespace SDXEngine
{
	class SDXMesh
	{
	public:
		SDXMesh();
		~SDXMesh();

		SDXErrorId AddSubMesh(SDXMeshData* meshData, SDXDirectX* directX);
		SDXSubMesh* GetSubMesh(int index);

		int GetTotalSubMeshes() const;

		void Release();

	private:
		std::vector<SDXSubMesh> m_subMeshes;
	};
}
