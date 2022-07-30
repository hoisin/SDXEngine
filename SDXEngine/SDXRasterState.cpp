#include "stdafx.h"
#include "SDXRasterState.h"

using namespace SDXEngine;

SDXRasterState::SDXRasterState() :
	m_bWireFrame(false), m_bFrontCounterClockwise(false), m_bDepthClip(true),
	m_bScissor(false), m_bMultiSample(false), m_bAntialisedLine(false),
	m_cullMode(SDX_CULL_BACK), m_depthBiasClamp(0.f), m_slopeScaledDepthBias(0.f)
{
}


SDXRasterState::~SDXRasterState()
{
}

void SDXEngine::SDXRasterState::SetWireFrame(bool bEnable)
{
	m_bWireFrame = bEnable;
}

void SDXEngine::SDXRasterState::SetFrontCounterClockwise(bool bEnable)
{
	m_bFrontCounterClockwise = bEnable;
}

void SDXEngine::SDXRasterState::SetDepthClip(bool bEnable)
{
	m_bDepthClip = bEnable;
}

void SDXEngine::SDXRasterState::SetScissor(bool bEnable)
{
	m_bScissor = bEnable;
}

void SDXEngine::SDXRasterState::SetMultiSample(bool bEnable)
{
	m_bMultiSample = bEnable;
}

void SDXEngine::SDXRasterState::SetAntialisedLine(bool bEnable)
{
	m_bAntialisedLine = bEnable;
}

void SDXEngine::SDXRasterState::SetCullMode(SDXCullMode mode)
{
	m_cullMode = mode;
}

void SDXEngine::SDXRasterState::SetDepthBiasClamp(float value)
{
	m_depthBiasClamp = value;
}

void SDXEngine::SDXRasterState::SetSlopeScaledDepthBias(float value)
{
	m_slopeScaledDepthBias = value;
}

bool SDXEngine::SDXRasterState::IsWireFrame() const
{
	return m_bWireFrame;
}

bool SDXEngine::SDXRasterState::IsFrontCounterClockwise() const
{
	return m_bFrontCounterClockwise;;
}

bool SDXEngine::SDXRasterState::IsDepthClip() const
{
	return m_bDepthClip;
}

bool SDXEngine::SDXRasterState::IsScissor() const
{
	return m_bScissor;
}

bool SDXEngine::SDXRasterState::IsMultiSample() const
{
	return m_bMultiSample;
}

bool SDXEngine::SDXRasterState::IsAntialisedLine() const
{
	return m_bAntialisedLine;
}

SDXCullMode SDXEngine::SDXRasterState::GetCullMode() const
{
	return m_cullMode;
}

float SDXEngine::SDXRasterState::GetDepthBiasClamp() const
{
	return m_depthBiasClamp;
}

float SDXEngine::SDXRasterState::GetSlopeScaledDepthBias() const
{
	return m_slopeScaledDepthBias;
}
