#include "stdafx.h"
#include "SDXAssetMGR.h"

using namespace SDXEngine;

SDXAssetMGR* SDXAssetMGR::m_instance = nullptr;

SDXEngine::SDXAssetMGR::SDXAssetMGR()
{
}

SDXEngine::SDXAssetMGR::~SDXAssetMGR()
{
}

SDXErrorId SDXEngine::SDXAssetMGR::Initialise(SDXDirectX* pDx)
{
	SDXErrorId error = SDX_ERROR_NONE;

	m_pDirectX = pDx;

	error = m_shaderMGR.Initialise(m_pDirectX);

	return error;
}

SDXErrorId SDXEngine::SDXAssetMGR::LoadShader(const std::string& vertexShaderFile, const std::string& pixelShaderFile, D3D11_INPUT_ELEMENT_DESC* desc, int inputElements, const std::string& assignID)
{
	SDXErrorId error = SDX_ERROR_ASSETMGR_NOT_INITIALISED;

	if (m_pDirectX)
	{
		error = m_shaderMGR.LoadShader(vertexShaderFile, pixelShaderFile, desc, inputElements, assignID);
	}

	return error;
}

SDXErrorId SDXEngine::SDXAssetMGR::BindShaderConstant(const std::string& id, CD3D11_BUFFER_DESC* desc)
{
	return m_shaderMGR.BindConstant(id, desc);
}

SShader* SDXEngine::SDXAssetMGR::GetShader(const std::string& shaderID)
{
	return m_shaderMGR.GetShader(shaderID);
}

ComPtr<ID3D11Buffer> SDXEngine::SDXAssetMGR::GetCBuffer(const std::string& id)
{
	return m_shaderMGR.GetCBuffer(id);
}

void SDXEngine::SDXAssetMGR::Close()
{
	if (m_instance)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}

SDXAssetMGR* SDXEngine::SDXAssetMGR::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new SDXAssetMGR();
	}

	return m_instance;
}
