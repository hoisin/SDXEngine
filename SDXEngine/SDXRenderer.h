//--------------------------------------------------------------------------
//
//	Renderer class
//
//--------------------------------------------------------------------------
#pragma once

#include "SDXDirectX.h"
#include "SDXDirect2D.h"
#include "SDXCameraFP.h"

#include "SDXRasterState.h"

#include "SDXSubMesh.h"

namespace SDXEngine
{
	struct SDXDrawItem
	{
		XMFLOAT3 worldPos;
		XMFLOAT3 rotation;

		SDXSubMesh* submesh;
	};

	class SDXRenderer
	{
	public:
		SDXRenderer();
		~SDXRenderer();

		SDXErrorId Initialise(const SDXDirectXInfo& info);
		void BeginDraw();
		void EndDraw();

		void UpdateProjectionMatrix(const XMFLOAT4X4& proj);
		void UpdateViewMatrix(const XMFLOAT4X4& view);

		void RenderText(UINT x, UINT y, const std::string& text);

		// Tester methods
		SDXErrorId CreateCube();
		SDXErrorId CreateViewAndPerspective();

		void UpdateTest();
		void RenderCube();

		void EnableWireFrame(bool bEnable);

		SDXDirectX* GetDirectX();

	private:
		SDXDirectX m_directX;
		SDXDirect2D m_direct2D;

		// Testing
		ID3D11Buffer*			m_vertexBuffer;
		ID3D11Buffer*			m_indexBuffer;
		int						m_indexCount;

		ConstantBufferStruct    m_worldViewProj;
		int						m_testFrameCount;

		SDXRasterState m_fillState;
		SDXRasterState m_wireFrame;
	};
}

