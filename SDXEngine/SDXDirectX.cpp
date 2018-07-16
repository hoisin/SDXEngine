#include "stdafx.h"
#include "SDXDirectX.h"

using namespace SDXEngine;

SDXDirectX::SDXDirectX() : m_d3d11Device(nullptr), m_d3d11ImmediateContext(nullptr),
	m_dxSwapChain(nullptr), m_renderTargetView(nullptr), m_depthStencilBuffer(nullptr),
	m_depthStencilView(nullptr), m_hWnd(nullptr)
{
	// These are values that can be 'set' via a config
	m_width = 800;
	m_height = 600;
	m_refreshRate = 60;
	m_msaaQuality = 0;
	m_UseMsaa = false;
	m_windowed = true;
}


SDXDirectX::~SDXDirectX()
{
	ShutDown();
}

void SDXEngine::SDXDirectX::SetClientArea(UINT width, UINT height)
{
	m_width = width;
	m_height = height;
}

void SDXEngine::SDXDirectX::SetRefreshRate(UINT rate)
{
	m_refreshRate = rate;
}

void SDXEngine::SDXDirectX::SetOutputWindow(HWND hWnd)
{
	m_hWnd = hWnd;
}

void SDXEngine::SDXDirectX::SetWindowed(bool IsWindowed)
{
	m_windowed = IsWindowed;
}

void SDXEngine::SDXDirectX::SetUse4XMSAA(bool bUse, UINT quality)
{
	m_UseMsaa = bUse;
	m_msaaQuality = quality;
}

SDXErrorId SDXEngine::SDXDirectX::CreateDevice()
{
	// If device created already
	if (m_d3d11Device || m_d3d11ImmediateContext)
		return SDX_ERROR_DEVICE_ALREADY_EXIST;

	// For direct2D support
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	// Enable D3D debug when running in debug
#if defined (DEBUG) | defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;
	HRESULT result = D3D11CreateDevice(
		0,							// Default adapter
		D3D_DRIVER_TYPE_HARDWARE,
		0,							// no software device
		createDeviceFlags,
		0, 0,						// default feature level array
		D3D11_SDK_VERSION,
		&m_d3d11Device,
		&featureLevel,
		&m_d3d11ImmediateContext);

	if (FAILED(result))
		return SDX_ERROR_DEVICE_CREATE_FAILED;

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
		return SDX_ERROR_DEVICE_DIRECTX11_NOTSUPPORTED;

	return SDX_ERROR_NONE;
}

SDXErrorId SDXEngine::SDXDirectX::CreateSwapChain()
{
	if (!m_d3d11Device || !m_d3d11ImmediateContext)
		return SDX_ERROR_DEVICE_NOT_CREATED;

	if (!m_hWnd)
		return SDX_ERROR_SWAPCHAIN_NO_SET_OUTPUTWINDOW;

	if (m_dxSwapChain)
		return SDX_ERROR_SWAPCHAIN_ALREADY_EXIST;

	// Describe the swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc.Width = m_width;
	swapChainDesc.BufferDesc.Height = m_height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = m_refreshRate;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = m_hWnd;
	swapChainDesc.Windowed = m_windowed;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	if (m_UseMsaa)
	{
		swapChainDesc.SampleDesc.Count = 4;
		swapChainDesc.SampleDesc.Quality = m_msaaQuality - 1;
	}
	else
	{
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	}

	// Setup factories to create the swap chain
	IDXGIDevice* dxgiDevice = nullptr;
	HRESULT result = m_d3d11Device->QueryInterface(__uuidof(IDXGIDevice),
		reinterpret_cast<void**>(&dxgiDevice));

	if (SUCCEEDED(result))
	{
		IDXGIAdapter* dxgiAdapter = nullptr;
		result = dxgiDevice->GetParent(__uuidof(IDXGIAdapter),
			reinterpret_cast<void**>(&dxgiAdapter));

		if (SUCCEEDED(result))
		{
			IDXGIFactory* dxgiFactory = nullptr;
			result = dxgiAdapter->GetParent(__uuidof(IDXGIFactory),
				reinterpret_cast<void**>(&dxgiFactory));
			
			if (SUCCEEDED(result))
			{
				result = dxgiFactory->CreateSwapChain(m_d3d11Device,
					&swapChainDesc, &m_dxSwapChain);

				if (SUCCEEDED(result))
				{
					// Release un-required COM 
					dxgiDevice->Release();
					dxgiAdapter->Release();
					dxgiFactory->Release();
					return SDX_ERROR_NONE;
				}
				// On failed
				dxgiFactory->Release();
			}
			// On failed
			dxgiFactory->Release();
		}
		// On failed
		dxgiDevice->Release();
	}
	return SDX_ERROR_SWAPCHAIN_CREATE_FAILED;
}

SDXErrorId SDXEngine::SDXDirectX::CreateRenderTargetView()
{
	if (!m_dxSwapChain)
		return SDX_ERROR_SWAPCHAIN_NOT_CREATED;
	
	if (m_renderTargetView)
		return SDX_ERROR_RENDERTARGETVIEW_ALREADY_EXIST;

	ID3D11Texture2D* backBuffer = nullptr;
	HRESULT result = m_dxSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&backBuffer));

	if (SUCCEEDED(result))
	{
		result = m_d3d11Device->CreateRenderTargetView(backBuffer, 0,
			&m_renderTargetView);

		// Release COM
		backBuffer->Release();
		backBuffer = nullptr;

		if (SUCCEEDED(result))
			return SDX_ERROR_NONE;
	}

	return SDX_ERROR_RENDERTARGETVIEW_CREATE_FAILED;
}

SDXErrorId SDXEngine::SDXDirectX::CreateDepthStencilBufferView()
{
	if (!m_d3d11Device || !m_d3d11ImmediateContext)
		return SDX_ERROR_DEVICE_NOT_CREATED;

	if (m_depthStencilBuffer || m_depthStencilView)
		return SDX_ERROR_DEPTHSTENCIL_ALREADY_EXIST;

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = m_width;
	depthStencilDesc.Height = m_height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	if (m_UseMsaa)
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = m_msaaQuality - 1;
	}
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	HRESULT result = m_d3d11Device->CreateTexture2D(&depthStencilDesc,
		0, &m_depthStencilBuffer);

	if (SUCCEEDED(result))
	{
		result = m_d3d11Device->CreateDepthStencilView(m_depthStencilBuffer,
			0, &m_depthStencilView);

		if (SUCCEEDED(result))
			return SDX_ERROR_NONE;
	}

	return SDX_ERROR_DEPTHSTENCIL_NOT_CREATED;
}

SDXErrorId SDXEngine::SDXDirectX::BindOutputMerger()
{
	if (!m_d3d11Device || !m_d3d11ImmediateContext)
		return SDX_ERROR_DEVICE_NOT_CREATED;

	if (!m_renderTargetView)
		return SDX_ERROR_RENDERTARGETVIEW_NOT_CREATED;

	if (!m_depthStencilView)
		return SDX_ERROR_DEPTHSTENCIL_NOT_CREATED;

	m_d3d11ImmediateContext->OMSetRenderTargets(1,
		&m_renderTargetView, m_depthStencilView);

	// Just going to always use the full client area
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = static_cast<float>(m_width);
	vp.Height = static_cast<float>(m_height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 0.0f;

	m_d3d11ImmediateContext->RSSetViewports(1,
		&vp);

	return SDX_ERROR_NONE;
}

SDXErrorId SDXEngine::SDXDirectX::Check4XMSAAQuality(UINT & quality) const
{
	if (!m_d3d11Device || !m_d3d11ImmediateContext)
		return SDX_ERROR_DEVICE_NOT_CREATED;

	HRESULT error = m_d3d11Device->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &quality);

	if(SUCCEEDED(error))
		return SDX_ERROR_NONE;

	return SDX_ERROR_CHECKMSAA_FAILED;
}

void SDXEngine::SDXDirectX::ClearRenderTargetView(float red, float green, float blue)
{
	if (m_d3d11ImmediateContext)
	{
		float colours[4] = { 0 };
		colours[0] = red;
		colours[1] = green;
		colours[2] = blue;
		m_d3d11ImmediateContext->ClearRenderTargetView(m_renderTargetView,
			colours);
	}
}

void SDXEngine::SDXDirectX::ClearDepthStencilView()
{
	if (m_d3d11ImmediateContext)
	{
		m_d3d11ImmediateContext->ClearDepthStencilView(m_depthStencilView,
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}

SDXErrorId SDXEngine::SDXDirectX::SwapChainPresent()
{
	if (!m_dxSwapChain)
		return SDX_ERROR_SWAPCHAIN_NOT_CREATED;

	if (FAILED(m_dxSwapChain->Present(0, 0)))
		return SDX_ERROR_SWAPCHAIN_PRESENT_FAILED;

	return SDX_ERROR_NONE;
}

ID3D11Device * SDXEngine::SDXDirectX::GetDevice() const
{
	return m_d3d11Device;
}

ID3D11DeviceContext * SDXEngine::SDXDirectX::GetContext() const
{
	return m_d3d11ImmediateContext;
}

IDXGISwapChain * SDXEngine::SDXDirectX::GetSwapChain() const
{
	return m_dxSwapChain;
}

ID3D11RenderTargetView * SDXEngine::SDXDirectX::GetRenderTargetView() const
{
	return m_renderTargetView;
}

ID3D11DepthStencilView * SDXEngine::SDXDirectX::GetDepthStencilView() const
{
	return m_depthStencilView;
}

float SDXEngine::SDXDirectX::GetAspectRatio() const
{
	return (static_cast<float>(m_width) / static_cast<float>(m_height));
}

UINT SDXEngine::SDXDirectX::GetClientAreaWidth() const
{
	return m_width;
}

UINT SDXEngine::SDXDirectX::GetClientAreaHeight() const
{
	return m_height;
}

SDXErrorId SDXEngine::SDXDirectX::SetRasterState(const SDXRasterState& state)
{
	if (!m_d3d11Device || !m_d3d11ImmediateContext)
		return SDX_ERROR_DEVICE_NOT_CREATED;

	ID3D11RasterizerState * rasterState;;

	D3D11_RASTERIZER_DESC rasterizerState;
	rasterizerState.FillMode = state.IsWireFrame() ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
	rasterizerState.CullMode = state.GetCullMode() == SDX_CULL_BACK ? D3D11_CULL_BACK : D3D11_CULL_NONE;
	rasterizerState.FrontCounterClockwise = state.IsFronCounterClockwise();
	rasterizerState.DepthBias = state.IsDepthClip();
	rasterizerState.DepthBiasClamp = state.GetDepthBiasClamp();
	rasterizerState.SlopeScaledDepthBias = state.GetSlopeScaledDepthBias();
	rasterizerState.DepthClipEnable = state.IsDepthClip();
	rasterizerState.ScissorEnable = state.IsScissor();
	rasterizerState.MultisampleEnable = state.IsMultiSample();
	rasterizerState.AntialiasedLineEnable = state.IsAntialisedLine();

	m_d3d11Device->CreateRasterizerState(&rasterizerState, &rasterState);
	m_d3d11ImmediateContext->RSSetState(rasterState);

	rasterState->Release();

	return SDX_ERROR_NONE;
}

void SDXEngine::SDXDirectX::ShutDown()
{
	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = nullptr;
	}

	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = nullptr;
	}

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = nullptr;
	}

	if (m_dxSwapChain)
	{
		m_dxSwapChain->Release();
		m_dxSwapChain = nullptr;
	}

	if (m_d3d11ImmediateContext)
	{
		m_d3d11ImmediateContext->Release();
		m_d3d11ImmediateContext = nullptr;
	}

	if (m_d3d11Device)
	{
		m_d3d11Device->Release();
		m_d3d11Device = nullptr;
	}
}
