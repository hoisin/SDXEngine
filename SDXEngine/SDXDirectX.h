//--------------------------------------------------------------------------
//
//	DirectX handler
//  DirectX11 support only currently
//
//--------------------------------------------------------------------------
#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")

#include <map>
#include <d3d11.h>
#include "SDXGfx.h"
#include "SDXErrors.h"

namespace SDXEngine
{
	class SDXDirectX
	{
	public:
		SDXDirectX();
		~SDXDirectX();

		// Swap chain setup
		void SetClientArea(UINT width, UINT height);
		void SetRefreshRate(UINT rate);
		void SetOutputWindow(HWND hWnd);
		void SetWindowed(bool IsWindowed);
		void SetUse4XMSAA(bool bUse, UINT quality = 0);

		//  Call methods in order
		SDXErrorId CreateDevice();
		SDXErrorId CreateSwapChain();
		SDXErrorId CreateRenderTargetView();
		SDXErrorId CreateDepthStencilBufferView();
		SDXErrorId BindOutputMerger();

		SDXErrorId Check4XMSAAQuality(UINT& quality) const;

		// Render functions
		void ClearRenderTargetView(float red = 0, float green = 0, float blue = 0);
		void ClearDepthStencilView();
		SDXErrorId SwapChainPresent();

		ID3D11Device* GetDevice() const;
		ID3D11DeviceContext* GetContext()const;
		IDXGISwapChain* GetSwapChain() const;

		ID3D11RenderTargetView* GetRenderTargetView() const;
		ID3D11DepthStencilView* GetDepthStencilView() const;

		float GetAspectRatio() const;
		UINT GetClientAreaWidth() const;
		UINT GetClientAreaHeight() const;

		SDXErrorId SetRasterState(const SDXRasterState& state);

		void ShutDown();

	private:
		ID3D11Device*			m_d3d11Device;
		ID3D11DeviceContext*	m_d3d11ImmediateContext;
		
		IDXGISwapChain*			m_dxSwapChain;

		ID3D11RenderTargetView* m_renderTargetView;
		ID3D11Texture2D*		m_depthStencilBuffer;
		ID3D11DepthStencilView* m_depthStencilView;

		UINT					m_width;
		UINT					m_height;
		UINT					m_refreshRate;
		HWND					m_hWnd;
		UINT					m_msaaQuality;
		bool					m_UseMsaa;
		bool					m_windowed;		
	};
}

