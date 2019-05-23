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

SDXErrorId SDXEngine::SDXShaderMGR::BindConstant(const std::string& id, CD3D11_BUFFER_DESC* desc)
{
	// Existing search
	for (int i = 0; i < static_cast<int>(m_cBuffers.size()); i++)
	{
		if (m_cBuffers[i].id == id)
			return SDX_ERROR_SHADERMGR_BIND_CONSTANT_ID_ALREADY_EXIST;
	}

	m_cBuffers.push_back(SCBuffer());
	SCBuffer* pBuffer = &m_cBuffers.back();
	pBuffer->id = id;

	HRESULT result = m_pDX->GetDevice()->CreateBuffer(desc, nullptr, pBuffer->cBuffer.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		m_cBuffers.pop_back();
		return SDX_ERROR_SHADERMGR_BIND_CONSTANT_FAILED;
	}

	return SDX_ERROR_NONE;
}
