//--------------------------------------------------------------------------
//
// Index buffer
//
//--------------------------------------------------------------------------
#pragma once

#include "SDXGfx.h"
#include "SDXDirectX.h"
#include "SDXMeshData.h"

namespace SDXEngine
{
	class SDXIndexBuffer
	{
	public:
		SDXIndexBuffer();
		~SDXIndexBuffer();

		SDXErrorId LoadData(SDXMeshData* meshData, SDXDirectX* inDirectX);
		ID3D11Buffer* GetBuffer() const;

		int GetCount() const;

		void Release();

	private:
		ID3D11Buffer * m_buffer;
		int m_count;
	};
}

