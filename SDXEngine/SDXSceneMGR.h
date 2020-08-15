#pragma once

#include <list>
#include <vector>
#include "SDXRenderer.h"

namespace SDXEngine
{
	class SDXSceneMGR
	{
	public:
		SDXSceneMGR();
		~SDXSceneMGR();

		SDXErrorId Initialise(SDXRenderer* pRenderer);

		void AddRenderItem(const XMFLOAT3 position, const XMFLOAT3 orientation, SDXMesh* pMesh, XMFLOAT3 scale = XMFLOAT3(1, 1, 1));

		void ClearRenderList();

		void Render();

	private:
		SDXDrawItem* GetNextFreeDrawItem();

	private:
		std::list<SDXDrawItem*> m_drawList;

		std::vector<SDXDrawItem> m_drawItemCache;

		SDXRenderer* m_pRenderer;

		int m_nextFreeDrawItem;
	};
};
