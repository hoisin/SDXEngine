#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")

#include <d3d11.h>
#include <d3d11_1.h>
#include <wrl.h>
#include <string>
#include <vector>
#include <map>

#include "SDXDirectX.h"
#include "SDXErrors.h"

using namespace Microsoft::WRL;

namespace SDXEngine
{
	class SDXTextureManager
	{
	public:
		SDXTextureManager(SDXDirectX* dxHDL);
		~SDXTextureManager();

		SDXErrorId LoadTexture(const std::string& fileDir, const std::string& textureId);

		ComPtr<ID3D11ShaderResourceView> GetTexture(const std::string& textureId);

	public:
		SDXTextureManager(const SDXTextureManager& other) = delete;
		SDXTextureManager() = delete;

	private:
		ComPtr<ID3D11ShaderResourceView> CreateTexture(const std::string& fileDir);

	private:
		std::vector<ComPtr<ID3D11ShaderResourceView>> m_textures;
		std::map<std::string, int> m_mapToIdx;
		SDXDirectX* m_dxHDL = nullptr;
	};
}