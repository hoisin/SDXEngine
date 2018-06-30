#pragma once

#pragma comment(lib, "D2d1.lib")
#pragma comment(lib, "Dwrite.lib")

#include <d2d1_3.h>
#include <dwrite_3.h>
#include "SDXDirectX.h"
#include "SDXErrors.h"

namespace SDXEngine
{
	class SDXDirect2D
	{
	public:
		SDXDirect2D();
		~SDXDirect2D();

		SDXErrorId SetSDXDirectX(SDXDirectX* directX);

		SDXErrorId CreateDevice();
		SDXErrorId CreateBitmapRenderTarget();
		SDXErrorId InitialiseTextFormats();

		// Needs to be called within Begin/End draw
		SDXErrorId RenderText(UINT x, UINT y, const std::string& text);

	private:
		SDXErrorId CreateBrushes();
		SDXErrorId SetupTextFormat();

	private:
		// We don't handle this here
		SDXDirectX * m_directX;	

		IDWriteFactory2*		m_writeFactory;
		ID2D1Device1*			m_device;
		ID2D1DeviceContext1*	m_context;

		// Brushes
		ID2D1SolidColorBrush*	m_yellowBrush;
		ID2D1SolidColorBrush*   m_whiteBrush;
		ID2D1SolidColorBrush*	m_blackBrush;

		// Text formats
		IDWriteTextFormat*		m_textFormat;

		// Text layouts
		IDWriteTextLayout*		m_textLayout;

		// Bitmap target for drawing to
		ID2D1Bitmap1*			m_targetBitmap;
	};
}
