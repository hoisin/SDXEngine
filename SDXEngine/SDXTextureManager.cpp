#include "stdafx.h"
#include "SDXTextureManager.h"
#include "WICTextureLoader.h"

using namespace SDXEngine;

SDXEngine::SDXTextureManager::SDXTextureManager(SDXDirectX* dxHDL) : m_dxHDL(dxHDL)
{
}

SDXEngine::SDXTextureManager::~SDXTextureManager()
{
}

SDXErrorId SDXEngine::SDXTextureManager::LoadTexture(const std::string& fileDir, const std::string& textureId)
{
	// Check for existing entry
	if (m_mapToIdx.find(textureId) != m_mapToIdx.end())
	{
		return SDXErrorId::SDX_ERROR_TEXTUREMGR_ID_ALREADY_EXIST;
	}

	// Load texture and create shader resource
	m_textures.push_back(ComPtr<ID3D11ShaderResourceView>(CreateTexture(fileDir)));
	if (m_textures.back() == nullptr)
	{
		return SDXErrorId::SDX_ERROR_TEXTUREMGR_FAILED_CREATE;
	}

	int idx = static_cast<int>(m_textures.size()) - 1;
	m_mapToIdx.insert(std::pair<std::string, int>(textureId, idx));

	return SDXErrorId::SDX_ERROR_NONE;
}

ComPtr<ID3D11ShaderResourceView> SDXEngine::SDXTextureManager::GetTexture(const std::string& textureId)
{
	auto find = m_mapToIdx.find(textureId);
	if (find != m_mapToIdx.end())
	{
		return m_textures[find->second];
	}

	return nullptr;
}

ComPtr<ID3D11ShaderResourceView> SDXEngine::SDXTextureManager::CreateTexture(const std::string& fileDir)
{
	if (m_dxHDL == nullptr)
	{
		return nullptr;
	}

	ComPtr<ID3D11ShaderResourceView> pRes = nullptr;
	std::wstring wFileTextureName = std::wstring(fileDir.begin(), fileDir.end());
	HRESULT result = CreateWICTextureFromFile(m_dxHDL->GetDevice().Get(), m_dxHDL->GetContext().Get(),
		wFileTextureName.c_str(), NULL, pRes.ReleaseAndGetAddressOf());

	if (FAILED(result))
	{
		if (pRes)
		{
			pRes->Release();
			pRes = nullptr;
		}
		return nullptr;
	}

	return pRes;
}
