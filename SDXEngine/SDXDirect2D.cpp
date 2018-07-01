#include "stdafx.h"
#include "SDXDirect2D.h"

using namespace SDXEngine;

SDXDirect2D::SDXDirect2D() : m_directX(nullptr), m_writeFactory(nullptr), m_device(nullptr), m_context(nullptr), m_yellowBrush(nullptr),
	m_whiteBrush(nullptr), m_blackBrush(nullptr), m_textFormat(nullptr), m_textLayout(nullptr), m_targetBitmap(nullptr)
{
}


SDXDirect2D::~SDXDirect2D()
{
	if (m_targetBitmap)
	{
		m_targetBitmap->Release();
		m_targetBitmap = nullptr;
	}

	if (m_textLayout)
	{
		m_textLayout->Release();
		m_textLayout = nullptr;
	}

	if (m_textFormat)
	{
		m_textFormat->Release();
		m_textFormat = nullptr;
	}

	if (m_blackBrush)
	{
		m_blackBrush->Release();
		m_blackBrush = nullptr;
	}

	if (m_whiteBrush)
	{
		m_whiteBrush->Release();
		m_whiteBrush = nullptr;
	}

	if (m_yellowBrush)
	{
		m_yellowBrush->Release();
		m_yellowBrush = nullptr;
	}

	if (m_context)
	{
		m_context->Release();
		m_context = nullptr;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = nullptr;
	}

	if (m_writeFactory)
	{
		m_writeFactory->Release();
		m_writeFactory = nullptr;
	}
}

SDXErrorId SDXDirect2D::SetSDXDirectX(SDXDirectX * directX)
{
	if (directX)
	{
		m_directX = directX;
		return SDX_ERROR_NONE;
	}
	return SDX_ERROR_DIRECT2D_NULLPTR_SET;
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
		reinterpret_cast<IUnknown**>(&m_writeFactory));

	if (SUCCEEDED(result))
	{
		// Get the dxgi device
		IDXGIDevice* dxgiDevice = nullptr;
		result = m_directX->GetDevice()->QueryInterface(__uuidof(IDXGIDevice),
			reinterpret_cast<void**>(&dxgiDevice));

		if (SUCCEEDED(result))
		{
			// Create the Direct2D factory
			D2D1_FACTORY_OPTIONS options;
#if defined (DEBUG) | defined(_DEBUG)
			options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#else
			options.debugLevel = D2D1_DEBUG_LEVEL_NONE;
#endif
			ID2D1Factory2* factory = nullptr;
			result = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, __uuidof(ID2D1Factory2),
				&options, reinterpret_cast<void**>(&factory));

			if (SUCCEEDED(result))
			{
				// Create Direct2D device
				// Can fail if directX device not set to support D3D11_CREATE_DEVICE_BGRA_SUPPORT (SDXDirectX)
				result = factory->CreateDevice(dxgiDevice, &m_device);

				if (SUCCEEDED(result))
				{
					// Create the context
					result = m_device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,
						&m_context);
					
					if (SUCCEEDED(result))
					{
						// Relase un-needed factories/dxgiDevice
						dxgiDevice->Release();
						factory->Release();
						return SDX_ERROR_NONE;
					}
				}
				factory->Release();
			}
			dxgiDevice->Release();
		}
	}

	return SDX_ERROR_DIRECT2D_DEVICE_CREATE_FAILED;
}

SDXErrorId SDXEngine::SDXDirect2D::CreateBitmapRenderTarget()
{
	ID2D1Image* pImage = nullptr;

	if (m_directX == nullptr)
		return SDX_ERROR_DIRECT2D_NOT_SET_DIRECTX;

	if (m_context == nullptr)
		return SDX_ERROR_DIRECT2D_DEVICE_NOT_CREATED;

	m_context->GetTarget(&pImage);

	if (pImage != nullptr)
	{
		// Relase! We're deferencing the counter and not deleting the it.
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
	IDXGISurface* dxgiBuffer = nullptr;
	IDXGISwapChain* swapChain = m_directX->GetSwapChain();
	if (swapChain == nullptr)
		return SDX_ERROR_SWAPCHAIN_NOT_CREATED;

	HRESULT result = swapChain->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(&dxgiBuffer));

	if (SUCCEEDED(result))
	{
		// Create the bitmap
		result = m_context->CreateBitmapFromDxgiSurface(dxgiBuffer, &bProps, &m_targetBitmap);
		if (SUCCEEDED(result))
		{
			// Set newly created bitmap as render target
			m_context->SetTarget(m_targetBitmap);
			dxgiBuffer->Release();
			return SDX_ERROR_NONE;
		}
		dxgiBuffer->Release();
	}
	
	return SDX_ERROR_DIRECT2D_BITMAPRENDERTARGET_CREATE_FAILED;
}

SDXErrorId SDXEngine::SDXDirect2D::InitialiseTextFormats()
{
	SDXErrorId error = CreateBrushes();

	if (error == SDX_ERROR_NONE)
	{
		error = SetupTextFormat();
	}

	return error;
}

SDXErrorId SDXEngine::SDXDirect2D::RenderText(UINT x, UINT y, const std::string & text)
{
	if (m_context == nullptr)
		return SDX_ERROR_DIRECT2D_DEVICE_NOT_CREATED;

	// Default brush colour yellow, expand for other colours later
	if (m_textFormat == nullptr || m_yellowBrush == nullptr)
		return SDX_ERROR_DIRECT2D_TEXTFORMAT_NOT_SETUP;

	// Create text
	std::wstring drawText = StringToWideString(text);
	HRESULT result = m_writeFactory->CreateTextLayout(drawText.c_str(), static_cast<UINT32>(drawText.size()), m_textFormat,
		static_cast<float>(m_directX->GetClientAreaWidth()), static_cast<float>(m_directX->GetClientAreaHeight()),
			&m_textLayout);

	if (SUCCEEDED(result))
	{
		// Draw the text
		m_context->BeginDraw();
		m_context->DrawTextLayout(D2D1::Point2F(static_cast<float>(x), static_cast<float>(y)), m_textLayout, m_yellowBrush);
		result = m_context->EndDraw();
		if (SUCCEEDED(result))
			return SDX_ERROR_NONE;
	}

	return SDX_ERROR_DIRECT2D_DRAWTEXT_FAILED;
}

SDXErrorId SDXEngine::SDXDirect2D::CreateBrushes()
{
	if (m_context == nullptr)
		return SDX_ERROR_DIRECT2D_DEVICE_NOT_CREATED;

	// Create standard brushes
	HRESULT result = m_context->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &m_yellowBrush);
	if (SUCCEEDED(result))
	{
		result = m_context->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_blackBrush);
		if (SUCCEEDED(result))
		{
			result = m_context->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_whiteBrush);
			if(SUCCEEDED(result))
			{
				return SDX_ERROR_NONE;
			}
		}
	}

	return SDX_ERROR_DIRECT2D_CREATEBURSHES_FAILED;
}

SDXErrorId SDXEngine::SDXDirect2D::SetupTextFormat()
{
	if (m_writeFactory == nullptr)
		return SDX_ERROR_DIRECT2D_DEVICE_NOT_CREATED;
		
	HRESULT result = m_writeFactory->CreateTextFormat(L"Lucudia Console", nullptr, DWRITE_FONT_WEIGHT_LIGHT,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 12.0f, L"en-GB", &m_textFormat);

	if (SUCCEEDED(result))
	{
		result = m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		if(SUCCEEDED(result))
		{
			result = m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
			if (SUCCEEDED(result))
			{
				return SDX_ERROR_NONE;
			}
		}
	}

	return SDX_ERROR_DIRECT2D_SETUPTEXTFORMAT_FAILED;
}


