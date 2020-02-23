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
	m_pDirectX = pDx;
	return SDX_ERROR_NONE;
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
