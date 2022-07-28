#include "stdafx.h"
#include "SDXDirectX.h"

using namespace SDXEngine;

SDXDirectX::SDXDirectX()
{
	// These are values that can be 'set' via a config
	m_width = 800;
	m_height = 600;
	m_refreshRate = 60;
	m_msaaQuality = 0;
	m_UseMsaa = false;
	m_windowed = true;

	ZeroMemory(&m_rasterState, sizeof(m_rasterState));
	ZeroMemory(&m_depthStencilState, sizeof(m_depthStencilState));
}


SDXDirectX::~SDXDirectX()
{
}

SDXErrorId SDXEngine::SDXDirectX::Initialise(const SDXDirectXInfo& setupInfo)
{
	SDXErrorId error = SDXErrorId::SDX_ERROR_NONE;

	if (setupInfo.clientWidth == 0 || setupInfo.clientHeight == 0 ||
		setupInfo.hwnd == nullptr || setupInfo.refreshRate == 0)
		return SDXErrorId::SDX_ERROR_INVALIDDIRECTX_SETUPINFO;

	m_width = setupInfo.clientWidth;
	m_height = setupInfo.clientHeight;
	m_windowed = setupInfo.windowed;
	m_UseMsaa = setupInfo.useMsaa;
	m_hWnd = setupInfo.hwnd;
	m_refreshRate = setupInfo.refreshRate;
	m_msaaQuality = setupInfo.msaaQuality;

	error = InitWindowIndependentResources();
	if (IsError(error))
		return error;

	error = InitWindowDependentResources(m_hWnd);

	return error;
}

SDXErrorId SDXEngine::SDXDirectX::ReInitWindowDependentResources(const SDXDirectXInfo& setupInfo)
{
	SDXErrorId error = SDXErrorId::SDX_ERROR_NONE;
	
	if (setupInfo.clientWidth == 0 || setupInfo.clientHeight == 0 ||
		setupInfo.hwnd == nullptr || setupInfo.refreshRate == 0)
		return SDXErrorId::SDX_ERROR_INVALIDDIRECTX_SETUPINFO;

	m_dxSwapChain.Reset();
	m_dxSwapChain1.Reset();
	m_renderTargetView.Reset();
	m_depthStencilBuffer.Reset();
	m_depthStencilView.Reset();

	// Unbind otherwise cannot re-create the swap chain due to lock
	error = UnbindOutputMerger();
	if (IsError(error))
		return error;

	m_width = setupInfo.clientWidth;
	m_height = setupInfo.clientHeight;
	m_windowed = setupInfo.windowed;
	m_UseMsaa = setupInfo.useMsaa;
	m_hWnd = setupInfo.hwnd;
	m_refreshRate = setupInfo.refreshRate;
	m_msaaQuality = setupInfo.msaaQuality;

	error = InitWindowDependentResources(m_hWnd);
	return error;
}

void SDXEngine::SDXDirectX::ClearRenderTargetView(float red, float green, float blue)
{
	if (m_d3d11Context.Get())
	{
		float colours[4] = { 0 };
		colours[0] = red;
		colours[1] = green;
		colours[2] = blue;
		m_d3d11Context->ClearRenderTargetView(m_renderTargetView.Get(),
			colours);
	}
}

void SDXEngine::SDXDirectX::ClearDepthStencilView()
{
	if (m_d3d11Context.Get())
	{
		m_d3d11Context->ClearDepthStencilView(m_depthStencilView.Get(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}

SDXErrorId SDXEngine::SDXDirectX::SwapChainPresent()
{
	if (!m_dxSwapChain)
		return SDXErrorId::SDX_ERROR_SWAPCHAIN_NOT_CREATED;

	if (SUCCEEDED(m_dxSwapChain->Present(0, 0)))
	{
		m_d3d11Context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
		return SDXErrorId::SDX_ERROR_NONE;
	}

	return SDXErrorId::SDX_ERROR_SWAPCHAIN_PRESENT_FAILED;
}

ComPtr<ID3D11Device> SDXEngine::SDXDirectX::GetDevice() const
{
	return m_d3d11Device;
}

ComPtr<ID3D11DeviceContext> SDXEngine::SDXDirectX::GetContext() const
{
	return m_d3d11Context;
}

ComPtr<IDXGISwapChain> SDXEngine::SDXDirectX::GetSwapChain() const
{
	return m_dxSwapChain;
}

ComPtr<ID3D11RenderTargetView> SDXEngine::SDXDirectX::GetRenderTargetView() const
{
	return m_renderTargetView;
}

ComPtr<ID3D11DepthStencilView> SDXEngine::SDXDirectX::GetDepthStencilView() const
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

HWND SDXEngine::SDXDirectX::GetHwnd() const
{
	return m_hWnd;
}

UINT SDXEngine::SDXDirectX::GetRefreshRate() const
{
	return m_refreshRate;
}

UINT SDXEngine::SDXDirectX::GetMSAAQuality() const
{
	return m_msaaQuality;
}

bool SDXEngine::SDXDirectX::GetIsWindowed() const
{
	return m_windowed;
}

bool SDXEngine::SDXDirectX::GetUseMSAA() const
{
	return m_UseMsaa;
}

SDXErrorId SDXEngine::SDXDirectX::SetRasterState(const SDXRasterState& state)
{
	if (m_d3d11Device.Get() == nullptr || m_d3d11Context.Get() == nullptr)
		return SDXErrorId::SDX_ERROR_DEVICE_NOT_CREATED;

	ComPtr<ID3D11RasterizerState> rasterState;

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

	if(FAILED(m_d3d11Device->CreateRasterizerState(&rasterizerState, rasterState.ReleaseAndGetAddressOf())))
		return SDXErrorId::SDX_ERROR_RASTERSTATECREATE_FAILED;

	// Copy
	m_rasterState = rasterizerState;

	m_d3d11Context->RSSetState(rasterState.Get());

	return SDXErrorId::SDX_ERROR_NONE;
}

SDXErrorId SDXEngine::SDXDirectX::SetDepthStencilState(const D3D11_DEPTH_STENCIL_DESC& state)
{
	if (m_d3d11Device == nullptr || m_d3d11Context == nullptr)
		return SDXErrorId::SDX_ERROR_DEVICE_NOT_CREATED;
		
	HRESULT result;
	ComPtr<ID3D11DepthStencilState> depthStencil;
	result = m_d3d11Device->CreateDepthStencilState(&state, depthStencil.ReleaseAndGetAddressOf());
	if (FAILED(result))
		return SDXErrorId::SDX_ERROR_DEPTHSTENCILCREATE_FAILED;

	// Copy
	m_depthStencilState = state;

	m_d3d11Context->OMSetDepthStencilState(depthStencil.Get(), 1);
	return SDXErrorId();
}

const D3D11_RASTERIZER_DESC& SDXEngine::SDXDirectX::GetRasterState() const
{
	return m_rasterState;
}

const D3D11_DEPTH_STENCIL_DESC& SDXEngine::SDXDirectX::GetDepthStencilState() const
{
	return m_depthStencilState;
}

void SDXEngine::SDXDirectX::ShutDown()
{
	m_depthStencilView = nullptr;
	m_depthStencilBuffer = nullptr;
	m_renderTargetView = nullptr;
	m_dxSwapChain1 = nullptr;
	m_dxSwapChain = nullptr;
	m_d3d11Context1 = nullptr;
	m_d3d11Device1 = nullptr;
	m_d3d11Context = nullptr;
	m_d3d11Device = nullptr;
}

/// <summary>
/// Initialises window independent resources such as:
///		- Device
///		- Context
///		- Raster state
///		- Sampler state
/// </summary>
/// <returns>SDXError</returns>
SDXErrorId SDXEngine::SDXDirectX::InitWindowIndependentResources()
{
	SDXErrorId error = CreateDevice();
	if (IsError(error))
		return error;

	SDXRasterState defaultState;
	error = SetRasterState(defaultState);
	if (IsError(error))
		return error;

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	error = SetTextureSamplerState(&samplerDesc, 1);
	if (IsError(error))
		return error;

	return error;
}

SDXErrorId SDXEngine::SDXDirectX::InitWindowDependentResources(HWND hWnd)
{
	SDXErrorId error;

	error = CreateSwapChain();
	if (IsError(error))
		return error;

	error = CreateRenderTargetView();
	if (IsError(error))
		return error;

	error = CreateDepthStencilBufferView();
	if (IsError(error))
		return error;

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	error = SetDepthStencilState(depthStencilDesc);
	if (IsError(error))
		return error;

	error = BindOutputMerger();

	return error;
}

SDXErrorId SDXEngine::SDXDirectX::CreateDevice()
{
	// If device created already
	if (m_d3d11Device.Get() || m_d3d11Context.Get())
		return SDXErrorId::SDX_ERROR_DEVICE_ALREADY_EXIST;

	// For direct2D support
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	// Enable D3D debug when running in debug
#if defined (DEBUG) | defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	HRESULT result = D3D11CreateDevice(
		0,							// Default adapter
		D3D_DRIVER_TYPE_HARDWARE,
		0,							// no software device
		createDeviceFlags,
		levels, ARRAYSIZE(levels),	// default feature level array
		D3D11_SDK_VERSION,
		&m_d3d11Device,
		&m_featureLevel,
		m_d3d11Context.ReleaseAndGetAddressOf());

	if (FAILED(result))
		return SDXErrorId::SDX_ERROR_DEVICE_CREATE_FAILED;

	// Support only directX 11 at the moment
	if (m_featureLevel < D3D_FEATURE_LEVEL_11_0)
		return SDXErrorId::SDX_ERROR_DEVICE_DIRECTX11_NOTSUPPORTED;

	// Check if DirectX 11.1 is supported
	if (SUCCEEDED(m_d3d11Device.As(&m_d3d11Device1)))
	{
		m_d3d11Context.As(&m_d3d11Context1);
	}

	return SDXErrorId::SDX_ERROR_NONE;
}

SDXErrorId SDXEngine::SDXDirectX::CreateSwapChain()
{
	if (m_d3d11Device.Get() == nullptr || m_d3d11Context.Get() == nullptr)
		return SDXErrorId::SDX_ERROR_DEVICE_NOT_CREATED;

	if (!m_hWnd)
		return SDXErrorId::SDX_ERROR_SWAPCHAIN_NO_SET_OUTPUTWINDOW;

	if (m_dxSwapChain.Get())
		return SDXErrorId::SDX_ERROR_SWAPCHAIN_ALREADY_EXIST;

	UINT quality;
	SDXErrorId error = Check4XMSAAQuality(quality);
	if (IsError(error))
		return error;

	if (m_msaaQuality > quality)
		return SDXErrorId::SDX_ERROR_SWAPCHAIN_INVALID_MSAAQUALITY;

	// Get the COM factory to create our swapchain
	HRESULT result;
	ComPtr<IDXGIFactory1> dxgiFactory;
	ComPtr<IDXGIDevice> dxgiDevice;
	result = m_d3d11Device.As(&dxgiDevice);
	if (FAILED(result))
		return SDXErrorId::SDX_ERROR_SWAPCHAIN_CREATE_FAILED;

	ComPtr<IDXGIAdapter> adapter;
	result = dxgiDevice->GetAdapter(&adapter);
	if (FAILED(result))
		return SDXErrorId::SDX_ERROR_SWAPCHAIN_CREATE_FAILED;

	result = adapter->GetParent(IID_PPV_ARGS(&dxgiFactory));
	if (FAILED(result))
		return SDXErrorId::SDX_ERROR_SWAPCHAIN_CREATE_FAILED;

	// Try to get dxgiFactory2
	ComPtr<IDXGIFactory2> dxgiFactory2;
	result = dxgiFactory.As(&dxgiFactory2);
	
	if (dxgiFactory2)
	{
		// DirectX 11.1 or later
		DXGI_SWAP_CHAIN_DESC1 sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Width = m_width;
		sd.Height = m_height;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 2;
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		if (m_UseMsaa)
		{
			sd.SampleDesc.Count = 4;
			sd.SampleDesc.Quality = m_msaaQuality - 1;
		}
		else
		{
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
		}
	
		result = dxgiFactory2->CreateSwapChainForHwnd(m_d3d11Device.Get(), m_hWnd, &sd, NULL, NULL, m_dxSwapChain1.ReleaseAndGetAddressOf());
		if (FAILED(result))
			return SDXErrorId::SDX_ERROR_SWAPCHAIN_CREATE_FAILED;

		result = m_dxSwapChain1.As(&m_dxSwapChain);
		if (FAILED(result))
			return SDXErrorId::SDX_ERROR_SWAPCHAIN_CREATE_FAILED;
	}
	else
	{
		// DirectX 11
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferDesc.Width = m_width;
		sd.BufferDesc.Height = m_height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = m_refreshRate;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = m_hWnd;
		sd.BufferCount = 2;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;	// No multi-sampling
		sd.Windowed = m_windowed;

		if (m_UseMsaa)
		{
			sd.SampleDesc.Count = 4;
			sd.SampleDesc.Quality = m_msaaQuality - 1;
		}
		else
		{
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
		}

		result = dxgiFactory->CreateSwapChain(m_d3d11Device.Get(), &sd, m_dxSwapChain.ReleaseAndGetAddressOf());
		if (FAILED(result))
			return SDXErrorId::SDX_ERROR_SWAPCHAIN_CREATE_FAILED;
	}

	return SDXErrorId::SDX_ERROR_NONE;
}

SDXErrorId SDXEngine::SDXDirectX::CreateRenderTargetView()
{
	if (m_dxSwapChain.Get() == nullptr)
		return SDXErrorId::SDX_ERROR_SWAPCHAIN_NOT_CREATED;

	if (m_renderTargetView.Get())
		return SDXErrorId::SDX_ERROR_RENDERTARGETVIEW_ALREADY_EXIST;

	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	HRESULT result = m_dxSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(backBuffer.ReleaseAndGetAddressOf()));

	if (SUCCEEDED(result))
	{
		result = m_d3d11Device->CreateRenderTargetView(backBuffer.Get(), 0,
			&m_renderTargetView);

		if (SUCCEEDED(result))
			return SDXErrorId::SDX_ERROR_NONE;
	}

	return SDXErrorId::SDX_ERROR_RENDERTARGETVIEW_CREATE_FAILED;
}

SDXErrorId SDXEngine::SDXDirectX::CreateDepthStencilBufferView()
{
	if (m_d3d11Device.Get() == nullptr || m_d3d11Context.Get() == nullptr)
		return SDXErrorId::SDX_ERROR_DEVICE_NOT_CREATED;

	if (m_depthStencilBuffer.Get() || m_depthStencilView.Get())
		return SDXErrorId::SDX_ERROR_DEPTHSTENCIL_ALREADY_EXIST;

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.Width = m_width;
	depthStencilDesc.Height = m_height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

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
		0, m_depthStencilBuffer.ReleaseAndGetAddressOf());

	if (SUCCEEDED(result))
	{
		result = m_d3d11Device->CreateDepthStencilView(m_depthStencilBuffer.Get(),
			0, m_depthStencilView.ReleaseAndGetAddressOf());

		if (SUCCEEDED(result))
			return SDXErrorId::SDX_ERROR_NONE;
	}

	return SDXErrorId::SDX_ERROR_DEPTHSTENCIL_NOT_CREATED;
}

SDXErrorId SDXEngine::SDXDirectX::BindOutputMerger()
{
	if (m_d3d11Device == nullptr || m_d3d11Context == nullptr)
		return SDXErrorId::SDX_ERROR_DEVICE_NOT_CREATED;

	if (m_renderTargetView.Get() == nullptr)
		return SDXErrorId::SDX_ERROR_RENDERTARGETVIEW_NOT_CREATED;

	if (m_depthStencilView == nullptr)
		return SDXErrorId::SDX_ERROR_DEPTHSTENCIL_NOT_CREATED;

	m_d3d11Context->OMSetRenderTargets(1,
		m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

	// Just going to always use the full client area
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = static_cast<float>(m_width);
	vp.Height = static_cast<float>(m_height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 0.0f;

	m_d3d11Context->RSSetViewports(1, &vp);

	return SDXErrorId::SDX_ERROR_NONE;
}

SDXErrorId SDXEngine::SDXDirectX::SetTextureSamplerState(D3D11_SAMPLER_DESC* descArray, int numSamplers)
{
	std::vector<ComPtr<ID3D11SamplerState>> states;
	
	for (int i = 0; i < numSamplers; i++)
	{
		states.push_back(ComPtr <ID3D11SamplerState>());
		if (FAILED(m_d3d11Device->CreateSamplerState(&descArray[i], states[i].ReleaseAndGetAddressOf())))
			return SDXErrorId::SDX_ERROR_SAMPLERSTATECREATE_FAILED;
	}

	m_d3d11Context->PSSetSamplers(0, numSamplers, states.front().GetAddressOf());

	return SDXErrorId::SDX_ERROR_NONE;
}

SDXErrorId SDXEngine::SDXDirectX::Check4XMSAAQuality(UINT& quality) const
{
	if (m_d3d11Device.Get() == nullptr || m_d3d11Context.Get() == nullptr)
		return SDXErrorId::SDX_ERROR_DEVICE_NOT_CREATED;

	HRESULT error = m_d3d11Device->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &quality);

	if (SUCCEEDED(error))
		return SDXErrorId::SDX_ERROR_NONE;

	return SDXErrorId::SDX_ERROR_CHECKMSAA_FAILED;
}

SDXErrorId SDXEngine::SDXDirectX::UnbindOutputMerger()
{
	if (m_d3d11Context)
	{
		ID3D11RenderTargetView* pNull[1] = { nullptr };
		m_d3d11Context->OMSetRenderTargets(1,
			pNull, nullptr);

		return SDXErrorId::SDX_ERROR_NONE;
	}

	return SDXErrorId::SDX_ERROR_DEVICE_NOT_CREATED;
}
