//--------------------------------------------------------------------------
//
//	Render state
//
//--------------------------------------------------------------------------
#pragma once

namespace SDXEngine
{

	enum SDXCullMode
	{
		SDX_CULL_FRONT,
		SDX_CULL_BACK,
		SDX_CULL_NONE
	};

	class SDXRasterState
	{
	public:
		SDXRasterState();
		~SDXRasterState();

		void SetWireFrame(bool bEnable);
		void SetFrontCounterClockwise(bool bEnable);	// Clockwise winding order if false
		void SetDepthClip(bool bEnable);
		void SetScissor(bool bEnable);
		void SetMultiSample(bool bEnable);
		void SetAntialisedLine(bool bEnable);
		void SetCullMode(SDXCullMode mode);
		void SetDepthBiasClamp(float value);
		void SetSlopeScaledDepthBias(float value);

		bool IsWireFrame() const;
		bool IsFrontCounterClockwise() const;
		bool IsDepthClip() const;
		bool IsScissor() const;
		bool IsMultiSample() const;
		bool IsAntialisedLine() const;
		SDXCullMode GetCullMode() const;
		float GetDepthBiasClamp() const;
		float GetSlopeScaledDepthBias() const;

	private:
		bool m_bWireFrame = false;
		bool m_bFrontCounterClockwise = false;
		bool m_bDepthClip = true;
		bool m_bScissor = false;
		bool m_bMultiSample = false;
		bool m_bAntialisedLine = false;

		SDXCullMode m_cullMode = SDX_CULL_BACK;
		float m_depthBiasClamp = 0.f;
		float m_slopeScaledDepthBias = 0.f;
	};

}