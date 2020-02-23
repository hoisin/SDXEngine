#include "stdafx.h"
#include "SDXMaterialMGR.h"

using namespace SDXEngine;

SDXEngine::SDXMaterialMGR::SDXMaterialMGR()
{
}

SDXEngine::SDXMaterialMGR::~SDXMaterialMGR()
{
}

SDXErrorId SDXEngine::SDXMaterialMGR::AddMaterial(const std::string& id, SMaterial material)
{
	auto find = m_materialMap.find(id);
	if (find == m_materialMap.end())
	{
		m_materialMap.insert(std::pair<std::string, SMaterial>(id, material));
		return SDX_ERROR_NONE;
	}

	return SDX_ERROR_MATERIALMGR_ID_ALREADY_EXIST;
}

SMaterial SDXEngine::SDXMaterialMGR::GetMaterial(const std::string& id)
{
	SMaterial returnMat = {};

	auto find = m_materialMap.find(id);

	if (find != m_materialMap.end())
	{
		returnMat = find->second;
	}

	return returnMat;
}
