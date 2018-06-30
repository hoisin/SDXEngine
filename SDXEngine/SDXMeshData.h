//--------------------------------------------------------------------------
//
// Stores mesh data 
//
//--------------------------------------------------------------------------
#pragma once

#include "SDXGfx.h"
#include "SDXErrors.h"

namespace SDXEngine
{
	class SDXMeshData
	{
	public:
		SDXMeshData();
		~SDXMeshData();

		SDXErrorId CreateVertexArray(SDXVertexType type, UINT count);
		SDXErrorId CreateIndexArray(UINT count);

		SDXVertex* GetVertexData() const;
		int* GetIndexData() const;

		SDXVertexType GetVertexType() const;
		int GetVertexCount() const;
		int GetIndexCount() const;

		void Destroy();

	private:
		// Array pointers
		SDXVertex * m_vertexData;
		int* m_indexData;

		int m_vertexCount;
		int m_indexCount;
		SDXVertexType m_vertexType;
	};
}