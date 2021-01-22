#pragma once

#include "SDXErrors.h"
#include "SDXMeshMGR.h"
#include "SDXMaterialMGR.h"
#include "SDXMeshGenerator.h"
#include "SDXShaderMGR.h"
#include "SDXTextureManager.h"

#include <memory>

namespace SDXEngine
{
	class SDXAssetMGR
	{
	public:
		SDXAssetMGR();
		~SDXAssetMGR();

		SDXErrorId Initialise(SDXDirectX* pDx);

		// Mesh generator
		SDXMeshGenerator* GetMeshGenerator();

		// Mesh mgr
		SDXErrorId CreateMesh(const std::string& id, SDXMesh** outMesh = nullptr);

		SDXMesh* GetMesh(const std::string& id);
		int GetMeshCount();

		// Shader stuff
		SDXErrorId LoadShader(const std::string& vertexShaderFile, const std::string& pixelShaderFile,
			D3D11_INPUT_ELEMENT_DESC* desc, int inputElements, const std::string& assignID);

		SDXErrorId BindShaderConstant(const std::string& id, CD3D11_BUFFER_DESC* desc);

		SShader* GetShader(const std::string& shaderID);
		ComPtr<ID3D11Buffer> GetCBuffer(const std::string& id);

		// Material
		SDXErrorId AddMaterial(const std::string& id, const SMaterial& material);
		SMaterial GetMaterial(const std::string& id);

		// Texture
		SDXErrorId LoadTexture(const std::string& fileDir, const std::string& assignId);
		ComPtr<ID3D11ShaderResourceView> GetTexture(const std::string& textureId);

		SDXDirectX* GetDirectX();

		void Close();
		static SDXAssetMGR* GetInstance();

	private:
		static SDXAssetMGR* m_instance;

		SDXDirectX* m_pDirectX = nullptr;

		SDXMeshGenerator	m_meshGenerator;
		SDXMeshMGR			m_meshMGR;
		SDXShaderMGR		m_shaderMGR;
		SDXMaterialMGR		m_materialMGR;

		std::unique_ptr<SDXTextureManager> m_textureMGR;
	};

#define ASSETMGR SDXAssetMGR::GetInstance()
}


