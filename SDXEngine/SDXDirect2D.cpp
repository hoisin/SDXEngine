#include "stdafx.h"
#include "SDXDirect2D.h"

using namespace SDXEngine;

SDXDirect2D::SDXDirect2D() : m_directX(nullptr), 
	m_writeFactory(nullptr), 
	m_device(nullptr), 
	m_context(nullptr), 
	m_textFormat(nullptr), 
	m_textLayout(nullptr), 
	m_targetBitmap(nullptr)
{
}

SDXDirect2D::~SDXDirect2D()
{
}

SDXErrorId SDXEngine::SDXDirect2D::Initialise(SDXDirectX* pDX)
{
	if (pDX == nullptr)
		return SDX_ERROR_DIRECT2D_NULLPTR_SET;

	m_directX = pDX;
	SDXErrorId error = SDX_ERROR_NONE;
	error = CreateDevice();
	if (IsError(error))
		return error;

	error = CreateBitmapRenderTarget();
	if (IsError(error))
		return error;

	error = InitialiseTextFormats();
	if (IsError(error))
		return error;

	m_bInit = true;
	return error;
}

SDXErrorId SDXEngine::SDXDirect2D::RenderText(UINT x, UINT y, const std::string & text)
{
	if (!m_bInit)
		return SDX_ERROR_DIRECT2D_NOT_INITIALISED;

	if (m_context == nullptr)
		return SDX_ERROR_DIRECT2D_DEVICE_NOT_CREATED;

	// Default brush colour yellow, expand for other colours later
	if (m_textFormat == nullptr || m_colorBrush == nullptr)
		return SDX_ERROR_DIRECT2D_TEXTFORMAT_NOT_SETUP;

	HRESULT result = S_OK;
	if (text != m_textToDraw)
	{
		// Create text
		std::wstring drawText = StringToWideString(text);
		result = m_writeFactory->CreateTextLayout(drawText.c_str(), static_cast<UINT32>(drawText.size()), m_textFormat.Get(),
			static_cast<float>(m_directX->GetClientAreaWidth()), static_cast<float>(m_directX->GetClientAreaHeight()),
			&m_textLayout);

		if (SUCCEEDED(result))
		{
			m_textLayout->GetMetrics(&m_textMetrics);
			m_textToDraw = text;
		}
	}

	if (SUCCEEDED(result))
	{
		// Draw the text
		m_context->BeginDraw();
		m_context->DrawTextLayout(D2D1::Point2F(static_cast<float>(x), static_cast<float>(y)), m_textLayout.Get(), m_colorBrush.Get());
		m_context->EndDraw();
		if (SUCCEEDED(result))
			return SDX_ERROR_NONE;
	}

	return SDX_ERROR_DIRECT2D_DRAWTEXT_FAILED;
}

SDXErrorId SDXEngine::SDXDirect2D::SetRenderText(const std::string text)
{
	SDXErrorId error = SDX_ERROR_NONE;

	if (!m_bInit)
		return SDX_ERROR_DIRECT2D_NOT_INITIALISED;

	if (m_context == nullptr)
		return SDX_ERROR_DIRECT2D_DEVICE_NOT_CREATED;

	// Default brush colour yellow, expand for other colours later
	if (m_textFormat == nullptr || m_colorBrush == nullptr)
		return SDX_ERROR_DIRECT2D_TEXTFORMAT_NOT_SETUP;

	HRESULT result = S_OK;
	if (text != m_textToDraw)
	{
		// Create text
		std::wstring drawText = std::wstring(text.begin(), text.end());
		HRESULT result = m_writeFactory->CreateTextLayout(drawText.c_str(), static_cast<UINT32>(drawText.size()), m_textFormat.Get(),
			static_cast<float>(m_directX->GetClientAreaWidth()), static_cast<float>(m_directX->GetClientAreaHeight()),
			m_textLayout.ReleaseAndGetAddressOf());

		if (SUCCEEDED(result))
		{
			m_textLayout->GetMetrics(&m_textMetrics);
			m_textToDraw = text;
			return SDX_ERROR_NONE;
		}
	}

	return SDX_ERROR_DIRECT2D_SETUPTEXTFORMAT_FAILED;
}

SDXErrorId SDXEngine::SDXDirect2D::RenderText(UINT x, UINT y)
{
	if (!m_bInit)
		return SDX_ERROR_DIRECT2D_NOT_INITIALISED;

	if (m_textLayout.Get())
	{
		// Draw the text
		m_context->BeginDraw();
		m_context->DrawTextLayout(D2D1::Point2F(static_cast<float>(x), static_cast<float>(y)), m_textLayout.Get(), m_colorBrush.Get());
		HRESULT result = m_context->EndDraw();
		if (SUCCEEDED(result))
			return SDX_ERROR_NONE;
	}

	return SDX_ERROR_DIRECT2D_DRAWTEXT_FAILED;
}

float SDXEngine::SDXDirect2D::GetRenderTextWidth() const
{
	return m_textMetrics.width;
}

float SDXEngine::SDXDirect2D::GetRenderTextHeight() const
{
	return m_textMetrics.height;
}

void SDXEngine::SDXDirect2D::ShutDown()
{
	m_targetBitmap = nullptr;
	m_textLayout = nullptr;
	m_colorBrush = nullptr;
	m_context = nullptr;
	m_device = nullptr;
	m_writeFactory = nullptr;
}

SDXErrorId SDXEngine::SDXDirect2D::CreateDevice()
{
	if (m_directX == nullptr)
		return SDX_ERROR_DIRECT2D_NOT_SET_DIRECTX;

	// Check directX device not null
	if (m_directX->GetDevice() == nullptr)
		return SDX_ERROR_DEVICE_NOT_CREATED;

	if (m_device || m_context)
		return SDX_ERROR_DIRECT2D_DEVICE_ALREADY_CREATED;

	// Create the DWrite factory
	HRESULT result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown * *>(m_writeFactory.ReleaseAndGetAddressOf()));
	if (FAILED(result))
		return SDX_ERROR_DIRECT2D_DEVICE_CREATE_FAILED;

	// Get the dxgi device
	ComPtr<IDXGIDevice> dxgiDevice = nullptr;
	result = m_directX->GetDevice()->QueryInterface(__uuidof(IDXGIDevice),
		reinterpret_cast<void**>(dxgiDevice.ReleaseAndGetAddressOf()));
	if (FAILED(result))
		return SDX_ERROR_DIRECT2D_DEVICE_CREATE_FAILED;

	// Create the Direct2D factory
	D2D1_FACTORY_OPTIONS options;
#if defined (DEBUG) | defined(_DEBUG)
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#else
	options.debugLevel = D2D1_DEBUG_LEVEL_NONE;
#endif
	ComPtr<ID2D1Factory2> factory = nullptr;
	result = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, __uuidof(ID2D1Factory2),
		&options, reinterpret_cast<void**>(factory.ReleaseAndGetAddressOf()));
	if (FAILED(result))
		return SDX_ERROR_DIRECT2D_DEVICE_CREATE_FAILED;

	// Create Direct2D device
	// Can fail if directX device not set to support D3D11_CREATE_DEVICE_BGRA_SUPPORT (SDXDirectX)
	result = factory->CreateDevice(dxgiDevice.Get(), m_device.ReleaseAndGetAddressOf());
	if (FAILED(result))
		return SDX_ERROR_DIRECT2D_DEVICE_CREATE_FAILED;

	// Create the context
	result = m_device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,
		&m_context);
	if (FAILED(result))
		return SDX_ERROR_DIRECT2D_DEVICE_CREATE_FAILED;

	return SDX_ERROR_NONE;
}

SDXErrorId SDXEngine::SDXDirect2D::CreateBitmapRenderTarget()
{
	if (m_directX == nullptr)
		return SDX_ERROR_DIRECT2D_NOT_SET_DIRECTX;

	if (m_context == nullptr)
		return SDX_ERROR_DIRECT2D_DEVICE_NOT_CREATED;

	ID2D1Image * pImage = nullptr;
	m_context->GetTarget(&pImage);

	if (pImage != nullptr)
	{
		// Release! We're deferencing the counter and not deleting the it.
		// Else check against m_targetBitmap and not bother with the GetTarget().
		pImage->Release();
		return SDX_ERROR_DIRECT2D_BITMAPRENDERTARGET_ALREADY_CREATED;
	}

	// Bitmap properties
	D2D1_BITMAP_PROPERTIES1 bProps;
	bProps.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bProps.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
	bProps.dpiX = 96.0f;
	bProps.dpiY = 96.0f;
	bProps.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	bProps.colorContext = nullptr;

	// Get DXGI version of the backbuffer for Direct2D
	ComPtr<IDXGISurface> dxgiBuffer = nullptr;
	ComPtr<IDXGISwapChain> swapChain = m_directX->GetSwapChain();
	if (swapChain == nullptr)
		return SDX_ERROR_SWAPCHAIN_NOT_CREATED;

	HRESULT result = swapChain->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(dxgiBuffer.ReleaseAndGetAddressOf()));

	if (SUCCEEDED(result))
	{
		// Create the bitmap
		result = m_context->CreateBitmapFromDxgiSurface(dxgiBuffer.Get(), &bProps, m_targetBitmap.ReleaseAndGetAddressOf());
		if (SUCCEEDED(result))
		{
			// Set newly created bitmap as render target
			m_context->SetTarget(m_targetBitmap.Get());
			return SDX_ERROR_NONE;
		}
	}

	return SDX_ERROR_DIRECT2D_BITMAPRENDERTARGET_CREATE_FAILED;
}

SDXErrorId SDXEngine::SDXDirect2D::InitialiseTextFormats()
{
	HRESULT result;
	result = m_context->CreateSolidColorBrush(D2D1::ColorF(0, 255, 0), m_colorBrush.ReleaseAndGetAddressOf());
	if (FAILED(result))
		return SDX_ERROR_DIRECT2D_CREATEBURSHES_FAILED;

	result = m_writeFactory->CreateTextFormat(L"Lucudia Console", nullptr, DWRITE_FONT_WEIGHT_LIGHT,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 14.0f, L"en-GB", m_textFormat.ReleaseAndGetAddressOf());
	if (FAILED(result))
		return SDX_ERROR_DIRECT2D_SETUPTEXTFORMAT_FAILED;

	result = m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	if (FAILED(result))
		return SDX_ERROR_DIRECT2D_SETUPTEXTFORMAT_FAILED;

	result = m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	if (FAILED(result))
		return SDX_ERROR_DIRECT2D_SETUPTEXTFORMAT_FAILED;

	return SDX_ERROR_NONE;
}
