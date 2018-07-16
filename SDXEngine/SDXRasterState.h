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
		bool IsFronCounterClockwise() const;
		bool IsDepthClip() const;
		bool IsScissor() const;
		bool IsMultiSample() const;
		bool IsAntialisedLine() const;
		SDXCullMode GetCullMode() const;
		float GetDepthBiasClamp() const;
		float GetSlopeScaledDepthBias() const;

	private:
		bool m_bWireFrame;
		bool m_bFrontCounterClockwise;
		bool m_bDepthClip;
		bool m_bScissor;
		bool m_bMultiSample;
		bool m_bAntialisedLine;

		SDXCullMode m_cullMode;
		float m_depthBiasClamp;
		float m_slopeScaledDepthBias;
	};

}