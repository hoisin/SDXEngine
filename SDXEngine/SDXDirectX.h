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
#include <d3d11_1.h>
#include <wrl.h>
#include "SDXGfx.h"
#include "SDXErrors.h"
#include "SDXRasterState.h"

using namespace Microsoft::WRL;

namespace SDXEngine
{
	// DirectX info setup
	struct SDXDirectXInfo
	{
		UINT clientWidth = 0;
		UINT clientHeight = 0;
		HWND hwnd = nullptr;
		UINT refreshRate = 0;		// For DX11 swapchain, not required for DX11_1 swapchain (SwapChain1)
		UINT msaaQuality = 0;
		bool windowed = true;
		bool useMsaa = false;

		SDXDirectXInfo()
		{
			// Optional, set as defaults
			refreshRate = 60;
			windowed = true;
			useMsaa = false;
		}
	};

	class SDXDirectX
	{
	public:
		SDXDirectX();
		~SDXDirectX();

		SDXErrorId Initialise(const SDXDirectXInfo& setupInfo);
		SDXErrorId ReInitWindowDependentResources(const SDXDirectXInfo& setupInfo);

		// Render functions
		void ClearRenderTargetView(float red = 0, float green = 0, float blue = 0);
		void ClearDepthStencilView();
		SDXErrorId SwapChainPresent();

		ComPtr<ID3D11Device> GetDevice() const;
		ComPtr<ID3D11DeviceContext> GetContext()const;
		ComPtr<IDXGISwapChain> GetSwapChain() const;

		ComPtr<ID3D11RenderTargetView> GetRenderTargetView() const;
		ComPtr<ID3D11DepthStencilView> GetDepthStencilView() const;

		float GetAspectRatio() const;
		UINT GetClientAreaWidth() const;
		UINT GetClientAreaHeight() const;
		HWND GetHwnd() const;
		UINT GetRefreshRate() const;
		UINT GetMSAAQuality() const;
		bool GetIsWindowed() const;
		bool GetUseMSAA() const;

		SDXErrorId SetRasterState(const SDXRasterState& state);		
		SDXErrorId SetDepthStencilState(const D3D11_DEPTH_STENCIL_DESC& state);

		const D3D11_RASTERIZER_DESC& GetRasterState() const;
		const D3D11_DEPTH_STENCIL_DESC& GetDepthStencilState() const;

		void ShutDown();

	private:
		SDXErrorId InitWindowIndependentResources();
		SDXErrorId InitWindowDependentResources(HWND hWnd);

		SDXErrorId CreateDevice();
		SDXErrorId CreateSwapChain();
		SDXErrorId CreateRenderTargetView();
		SDXErrorId CreateDepthStencilBufferView();
		SDXErrorId BindOutputMerger();

		SDXErrorId SetTextureSamplerState(D3D11_SAMPLER_DESC* descArray, int numSamplers);

		SDXErrorId Check4XMSAAQuality(UINT& quality) const;

		SDXErrorId UnbindOutputMerger();

	private:
		ComPtr<ID3D11Device>			m_d3d11Device = nullptr;
		ComPtr<ID3D11DeviceContext>		m_d3d11Context = nullptr;
		ComPtr<ID3D11Device1>			m_d3d11Device1 = nullptr;
		ComPtr<ID3D11DeviceContext1>	m_d3d11Context1 = nullptr;
		
		ComPtr<IDXGISwapChain>			m_dxSwapChain = nullptr;
		ComPtr<IDXGISwapChain1>			m_dxSwapChain1 = nullptr;

		ComPtr<ID3D11RenderTargetView>	m_renderTargetView = nullptr;
		ComPtr<ID3D11Texture2D>			m_depthStencilBuffer = nullptr;
		ComPtr<ID3D11DepthStencilView>  m_depthStencilView = nullptr;

		D3D11_RASTERIZER_DESC			m_rasterState;
		D3D11_DEPTH_STENCIL_DESC		m_depthStencilState;

		D3D_FEATURE_LEVEL				m_featureLevel = D3D_FEATURE_LEVEL_11_0;

		UINT					m_width = 0;
		UINT					m_height = 0;
		UINT					m_refreshRate = 0;
		HWND					m_hWnd = nullptr;
		UINT					m_msaaQuality = 0;
		bool					m_UseMsaa = false;
		bool					m_windowed = false;		
	};
}

