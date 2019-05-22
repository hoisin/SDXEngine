#include "stdafx.h"
#include "SDXShaderMGR.h"

using namespace SDXEngine;

SDXShaderMGR::SDXShaderMGR()
{
}


SDXShaderMGR::~SDXShaderMGR()
{
}

SDXErrorId SDXEngine::SDXShaderMGR::Initialise(SDXDirectX* pDX)
{
	if (pDX == nullptr)
		return SDX_ERROR_PARMETER_DIRECTX_NULLPTR;

	m_pDX = pDX;
	return SDX_ERROR_NONE;
}

SDXErrorId SDXEngine::SDXShaderMGR::LoadShader(const std::string& vertexShaderFile, const std::string& pixelShaderFile,
	D3D11_INPUT_ELEMENT_DESC* desc, int inputElements, const std::string& assignID)
{
	if (m_pDX == nullptr)
		return SDX_ERROR_SHADERMGR_DIRECTX_NOTSET;

	return SDX_ERROR_NONE;
}
