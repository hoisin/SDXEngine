//--------------------------------------------------------------------------
//
// Intermediate storage class for mesh data
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
		unsigned int* GetIndexData() const;

		SDXVertexType GetVertexType() const;
		int GetVertexCount() const;
		int GetIndexCount() const;

		void SetMaterialID(const std::string& id);
		const std::string& GetMaterialID() const;

		void SetMinVertexPos(const XMFLOAT3& pos);
		void SetMaxVertexPos(const XMFLOAT3& pos);
		const XMFLOAT3& GetMinVertexPos() const;
		const XMFLOAT3& GetMaxVertexPos() const;

		void Destroy();

	private:
		// Array pointers
		SDXVertex * m_vertexData;
		unsigned int* m_indexData;

		int m_vertexCount;
		int m_indexCount;
		SDXVertexType m_vertexType;

		std::string m_materialID;
		XMFLOAT3 m_minVertexPos;
		XMFLOAT3 m_maxVertexPos;
	};
}