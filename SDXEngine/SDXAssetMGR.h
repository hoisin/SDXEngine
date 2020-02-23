#pragma once

#include "SDXErrors.h"
#include "SDXMeshMGR.h"
#include "SDXMaterialMGR.h"
#include "SDXMeshGenerator.h"
#include "SDXShaderMGR.h"

namespace SDXEngine
{
	class SDXAssetMGR
	{
	public:
		SDXAssetMGR();
		~SDXAssetMGR();

		SDXErrorId Initialise(SDXDirectX* pDx);

		void Close();
		static SDXAssetMGR* GetInstance();

	private:
		static SDXAssetMGR* m_instance;

		SDXDirectX* m_pDirectX = nullptr;

		SDXMeshGenerator	m_meshGenerator;
		SDXMeshMGR			m_meshMGR;
		SDXShaderMGR		m_shaderMGR;
		SDXMaterialMGR		m_materialMGR;
	};

#define ASSETMGR SDXAssetMGR::GetInstance()
}


