//--------------------------------------------------------------------------
//
//	Procedurally generates simple shapes.
//	Data generated is callers responsibility to manage.
//
//--------------------------------------------------------------------------
#pragma once

#include "SDXGfx.h"
#include "SDXMeshData.h"

namespace SDXEngine
{
	class SDXMeshGenerator
	{
	public:
		SDXMeshGenerator();
		~SDXMeshGenerator();

		SDXMeshData* GenerateTriangle(float size, SDXVertexType type, const XMFLOAT3& color = XMFLOAT3(0, 0, 0));
		SDXMeshData* GeneratePlane(UINT size, SDXVertexType type, UINT subdivision = 1, const XMFLOAT3& color = XMFLOAT3(0, 0, 0));
		SDXMeshData* GenerateQuad();
		SDXMeshData* GenerateCube(UINT size, SDXVertexType type, UINT subdivision = 1, const XMFLOAT3& color = XMFLOAT3(0,0,0));
		SDXMeshData* GenerateSphere();
	};
}
