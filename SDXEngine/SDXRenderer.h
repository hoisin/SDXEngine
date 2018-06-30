//--------------------------------------------------------------------------
//
//	Renderer class
//
//--------------------------------------------------------------------------
#pragma once

#include "SDXDirectX.h"
#include "SDXDirect2D.h"
#include "SDXCameraFP.h"

namespace SDXEngine
{
	struct SDXDirectXInfo
	{
		UINT clientWidth;
		UINT clientHeight;
		HWND hwnd;
		UINT refreshRate;
		bool windowed;
		bool useMsaa;

		SDXDirectXInfo()
		{
			// Client must set these
			clientWidth = 0;
			clientHeight = 0;
			hwnd = nullptr;

			// Optional, set as defaults
			refreshRate = 60;
			windowed = true;
			useMsaa = false;
		}
	};

	// Testing for vertex shader uniform
	typedef struct _constantBufferStruct {
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	} ConstantBufferStruct;

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
		SDXErrorId CreateShaders();
		SDXErrorId CreateCube();
		SDXErrorId CreateViewAndPerspective();

		void UpdateTest();
		void RenderCube();

		// Delete this
		typedef struct _constantBufferStruct {
			DirectX::XMFLOAT4X4 world;
			DirectX::XMFLOAT4X4 view;
			DirectX::XMFLOAT4X4 projection;
		} ConstantBufferStruct;

	private:
		SDXErrorId CreateVertexShader();
		SDXErrorId CreatePixelShader();
		SDXErrorId BindConstants();

	private:
		SDXDirectX m_directX;
		SDXDirect2D m_direct2D;

		// Testing
		ID3D11VertexShader*		m_vertexShader;
		ID3D11PixelShader*		m_pixelShader;
		ID3D11InputLayout*		m_inputLayout;
		ID3D11Buffer*			m_constantBuffer;

		ID3D11Buffer*			m_vertexBuffer;
		ID3D11Buffer*			m_indexBuffer;
		int						m_indexCount;

		ConstantBufferStruct    m_worldViewProj;
		int						m_testFrameCount;
	};
}

