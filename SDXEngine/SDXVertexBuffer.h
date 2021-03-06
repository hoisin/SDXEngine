//--------------------------------------------------------------------------
//
// Vertex buffer
//
//--------------------------------------------------------------------------
#pragma once

#include "SDXGfx.h"
#include "SDXDirectX.h"
#include "SDXMeshData.h"

namespace SDXEngine
{
	class SDXVertexBuffer
	{
	public:
		SDXVertexBuffer();
		~SDXVertexBuffer();

		SDXErrorId LoadData(SDXMeshData* meshData, SDXDirectX* inDirectX);
		ID3D11Buffer* GetBuffer();
		int GetCount() const;
		SDXVertexType GetType() const;

		void Release();

	private:
		ID3D11Buffer * m_buffer;
		int m_count;
		SDXVertexType m_type;
	};
}
