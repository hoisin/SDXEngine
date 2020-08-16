#include "stdafx.h"
#include "SDXSceneMGR.h"

using namespace SDXEngine;

SDXEngine::SDXSceneMGR::SDXSceneMGR() :m_pRenderer(nullptr), m_nextFreeDrawItem(0)
{
}

SDXEngine::SDXSceneMGR::~SDXSceneMGR()
{
}

SDXErrorId SDXEngine::SDXSceneMGR::Initialise(SDXRenderer* pRenderer)
{
	// 10k item draw limit
	int maxCap = 10000;
	m_drawItemCache.resize(maxCap);

	m_pRenderer = pRenderer;
	return SDXErrorId::SDX_ERROR_NONE;
}

void SDXEngine::SDXSceneMGR::AddRenderItem(const XMFLOAT3 position, const XMFLOAT3 orientation, SDXMesh* pMesh, XMFLOAT3 scale)
{
	SDXDrawItem* pItem = GetNextFreeDrawItem();

	if (pItem)
	{
		pItem->worldPos = position;
		pItem->rotation = orientation;
		pItem->mesh = pMesh;

		m_drawList.push_front(pItem);
	}
}

void SDXEngine::SDXSceneMGR::ClearRenderList()
{
	m_drawList.clear();
	m_nextFreeDrawItem = 0;
}

void SDXEngine::SDXSceneMGR::Render()
{
	//m_pRenderer->BeginDraw();

	m_pRenderer->Render(m_drawList);

	//m_pRenderer->EndDraw();
}

SDXDrawItem* SDXEngine::SDXSceneMGR::GetNextFreeDrawItem()
{
	if (m_nextFreeDrawItem < static_cast<int>(m_drawItemCache.size()))
	{
		SDXDrawItem* pNext = &m_drawItemCache[m_nextFreeDrawItem];
		m_nextFreeDrawItem++;

		return pNext;
	}

	return nullptr;
}
