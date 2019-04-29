#pragma once

#pragma comment(lib, "D2d1.lib")
#pragma comment(lib, "Dwrite.lib")

#include <d2d1_3.h>
#include <dwrite_3.h>
#include <wrl.h>
#include "SDXDirectX.h"
#include "SDXErrors.h"

using namespace Microsoft::WRL;

namespace SDXEngine
{
	class SDXDirect2D
	{
	public:
		SDXDirect2D();
		~SDXDirect2D();

		SDXErrorId Initialise(SDXDirectX* pDX);

		// Needs to be called within Begin/End draw
		// This draws specified text.
		// Will generate new render surface for new text.
		// Will also check if text was the same as last and skip the generation of surface
		SDXErrorId RenderText(UINT x, UINT y, const std::string& text);

		// Set text to render
		SDXErrorId SetRenderText(const std::string text);
		// This renders whatever text was last set
		SDXErrorId RenderText(UINT x, UINT y);

		// Must have called RenderText(UINT, UINT, STRING) or SetRenderText() which will return
		// the string text dimensions
		float GetRenderTextWidth() const;
		float GetRenderTextHeight() const;

		void ShutDown();

	private:
		SDXErrorId CreateDevice();
		SDXErrorId CreateBitmapRenderTarget();
		SDXErrorId InitialiseTextFormats();

	private:
		// We don't handle this here
		SDXDirectX * m_directX = nullptr;	

		ComPtr<IDWriteFactory2>			m_writeFactory = nullptr;
		ComPtr<ID2D1Device1>			m_device = nullptr;
		ComPtr<ID2D1DeviceContext1>		m_context = nullptr;

		// Brushes
		ComPtr<ID2D1SolidColorBrush>	m_colorBrush = nullptr;

		// Text formats
		ComPtr<IDWriteTextFormat>		m_textFormat = nullptr;

		// Text layouts
		ComPtr<IDWriteTextLayout>		m_textLayout = nullptr;

		// Bitmap target for drawing to
		ComPtr<ID2D1Bitmap1>			m_targetBitmap = nullptr;

		DWRITE_TEXT_METRICS				m_textMetrics = DWRITE_TEXT_METRICS();
		std::string						m_textToDraw = "";

		bool							m_bInit = false;
	};
}
