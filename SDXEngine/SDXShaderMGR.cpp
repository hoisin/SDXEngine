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

	m_shaders.push_back(SShader());
	SShader* newShader = &m_shaders.back();
	newShader->id = assignID;

	SDXErrorId error = m_loadHelper.LoadVertexShader(m_pDX, vertexShaderFile, desc, inputElements, "Main", "vs_5_0", newShader->vertexShader.ReleaseAndGetAddressOf(),
		newShader->inputLayout.ReleaseAndGetAddressOf());
	if (IsError(error))
	{
		m_shaders.pop_back();
		return error;
	}

	error = m_loadHelper.LoadPixelShader(m_pDX, pixelShaderFile, "Main", "ps_5_0", newShader->pixelShader.ReleaseAndGetAddressOf());
	if (IsError(error))
	{
		m_shaders.pop_back();
		return error;
	}

	return SDX_ERROR_NONE;
}

SDXErrorId SDXEngine::SDXShaderMGR::BindConstant(const std::string& id, CD3D11_BUFFER_DESC* desc)
{
	if (m_pDX == nullptr)
		return SDX_ERROR_SHADERMGR_DIRECTX_NOTSET;

	if (id.empty())
		return SDX_ERROR_SHADERMGR_NULL_ID;

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

SShader* SDXEngine::SDXShaderMGR::GetShader(UINT handle)
{
	if(handle > static_cast<UINT>(m_shaders.size()))
		return nullptr;

	return &m_shaders[handle];
}

SShader* SDXEngine::SDXShaderMGR::GetShader(const std::string& id)
{
	// Search for matching ID
	for (UINT i = 0; i < static_cast<UINT>(m_shaders.size()); i++)
	{
		// If match found, return the shader
		if (m_shaders[i].id == id)
			return &m_shaders[i];
	}

	// else failed to find
	return nullptr;
}

ComPtr<ID3D11Buffer> SDXEngine::SDXShaderMGR::GetCBuffer(UINT handle)
{
	if (handle > static_cast<UINT>(m_cBuffers.size()))
		return nullptr;

	return m_cBuffers[handle].cBuffer;
}

ComPtr<ID3D11Buffer> SDXEngine::SDXShaderMGR::GetCBuffer(const std::string& id)
{
	// Search for matching ID
	for (UINT i = 0; i < static_cast<UINT>(m_cBuffers.size()); i++)
	{
		if (m_cBuffers[i].id == id)
			return m_cBuffers[i].cBuffer;
	}

	// else failed to find
	return nullptr;
}
