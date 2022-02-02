#include "stdafx.h"
#include "SDXAssetMGR.h"

using namespace SDXEngine;

SDXAssetMGR* SDXAssetMGR::m_instance = nullptr;

SDXAssetMGR::SDXAssetMGR()
{
}

SDXAssetMGR::~SDXAssetMGR()
{
}

SDXErrorId SDXAssetMGR::Initialise(SDXDirectX* pDx)
{
	SDXErrorId error = SDXErrorId::SDX_ERROR_NONE;

	m_pDirectX = pDx;

	m_textureMGR.reset(new SDXTextureManager(m_pDirectX));

	error = m_shaderMGR.Initialise(m_pDirectX);
	if (error != SDXErrorId::SDX_ERROR_NONE)
	{
		return error;
	}

	return error;
}

SDXMeshGenerator* SDXAssetMGR::GetMeshGenerator()
{
	return &m_meshGenerator;
}

SDXErrorId SDXAssetMGR::CreateMesh(const std::string& id, SDXMesh** outMesh)
{
	if (m_pDirectX)
	{
		*outMesh = m_meshMGR.CreateMesh(id);
		if (outMesh == nullptr)
		{
			return SDXErrorId::SDX_ERROR_ASSETMGR_DUPLICATE_MESHID;
		}
		else
		{
			return SDXErrorId::SDX_ERROR_NONE;
		}
	}

	return SDXErrorId::SDX_ERROR_ASSETMGR_NOT_INITIALISED;
}

SDXMesh* SDXAssetMGR::GetMesh(const std::string& id)
{
	return m_meshMGR.GetMesh(id);
}

int SDXAssetMGR::GetMeshCount()
{
	return m_meshMGR.GetMeshCount();
}

SDXErrorId SDXAssetMGR::LoadShader(const std::string& vertexShaderFile, const std::string& pixelShaderFile, D3D11_INPUT_ELEMENT_DESC* desc, int inputElements, const std::string& assignID)
{
	SDXErrorId error = SDXErrorId::SDX_ERROR_ASSETMGR_NOT_INITIALISED;

	if (m_pDirectX)
	{
		error = m_shaderMGR.LoadShader(vertexShaderFile, pixelShaderFile, desc, inputElements, assignID);
	}

	return error;
}

SDXErrorId SDXAssetMGR::BindShaderConstant(const std::string& id, CD3D11_BUFFER_DESC* desc)
{
	return m_shaderMGR.BindConstant(id, desc);
}

SShader* SDXAssetMGR::GetShader(const std::string& shaderID)
{
	return m_shaderMGR.GetShader(shaderID);
}

ComPtr<ID3D11Buffer> SDXAssetMGR::GetCBuffer(const std::string& id)
{
	return m_shaderMGR.GetCBuffer(id);
}

SDXErrorId SDXAssetMGR::AddMaterial(const std::string& id, const SMaterial& material)
{
	return m_materialMGR.AddMaterial(id, material);
}

SMaterial SDXAssetMGR::GetMaterial(const std::string& id)
{
	return m_materialMGR.GetMaterial(id);
}

SDXErrorId SDXAssetMGR::LoadTexture(const std::string& fileDir, const std::string& assignId)
{
	return m_textureMGR->LoadTexture(fileDir, assignId);
}

ComPtr<ID3D11ShaderResourceView> SDXAssetMGR::GetTexture(const std::string& textureId)
{
	return m_textureMGR->GetTexture(textureId);
}

SDXDirectX* SDXAssetMGR::GetDirectX()
{
	return m_pDirectX;
}

void SDXAssetMGR::Close()
{
	if (m_instance)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}

SDXAssetMGR* SDXAssetMGR::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new SDXAssetMGR();
	}

	return m_instance;
}
