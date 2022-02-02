#include "stdafx.h"
#include "CMazeMap.h"

CMazeMap::CMazeMap()
{
}

CMazeMap::~CMazeMap()
{
}

void CMazeMap::CreateMap(int tileSize, int tilesX, int tilesZ)
{
	for (int i = 0; i < static_cast<int>(m_map.size()); i++)
	{
		m_map[i].clear();
	}
	m_map.clear();

	m_tileSize = tileSize;
	m_mapSizeXYZ.x = m_tileSize * tilesX;
	m_mapSizeXYZ.z = m_tileSize * tilesZ;

	// Get map max/min
	m_mapMin.x = -(m_mapSizeXYZ.x / 2.f);
	m_mapMin.z = -(m_mapSizeXYZ.z / 2.f);

	m_mapMax.x = (m_mapSizeXYZ.x / 2.f);
	m_mapMax.z = (m_mapSizeXYZ.z / 2.f);

	m_map.resize(tilesZ);
	for (int z = 0; z < tilesZ; z++)
	{
		m_map[z].resize(tilesX);
	}
}

CTile* CMazeMap::GetTileFromCoordinate(float x, float z)
{
	int pX = (x + (m_mapSizeXYZ.x / 2)) / m_tileSize;
	int pZ = (z + (m_mapSizeXYZ.z / 2)) / m_tileSize;

	CTile* tile = nullptr;

	if (pZ >= 0 && pZ < static_cast<int>(m_map.size()))
	{
		if (pX >= 0 && pX < static_cast<int>(m_map[pZ].size()))
		{
			return &m_map[pZ][pX];
		}
	}

	return nullptr;
}

void CMazeMap::CalculateWallsToRender()
{
	// Testing
	m_map[0][0].bAcess = false;
	m_map[0][49].bAcess = false;
	m_map[49][0].bAcess = false;
	m_map[49][49].bAcess = false;

	for (int z = 0; z < static_cast<int>(m_map.size()); z++)
	{
		for (int x = 0; x < static_cast<int>(m_map[z].size()); x++)
		{
			// If blocking tile
			if (m_map[z][x].bAcess == false)
			{
				// Check 'left' of current tile
				if (x > 0)
				{
					if (m_map[z][x - 1].bAcess == true)
					{
						m_renderWalls.push_back(CDrawWall{
							XMFLOAT3((m_tileSize * x) - (m_mapSizeXYZ.x / 2), 0, (m_tileSize * z) + (m_tileSize / 2) - (m_mapSizeXYZ.x / 2)),
							XMFLOAT3(0, 90, 0) });
					}
				}

				// Check 'right' of current tile
				if (x < (static_cast<int>(m_map[z].size() - 1)))
				{
					if (m_map[z][x + 1].bAcess == true)
					{
						m_renderWalls.push_back(CDrawWall{
							XMFLOAT3((m_tileSize * x) + m_tileSize - (m_mapSizeXYZ.x / 2), 0, (m_tileSize * z) + (m_tileSize / 2) - (m_mapSizeXYZ.x / 2)),
							XMFLOAT3(0, -90, 0) });
					}
				}

				// Check 'top' of current tile
				if (z > 0)
				{
					if (m_map[z - 1][x].bAcess == true)
					{
						m_renderWalls.push_back(CDrawWall{
							XMFLOAT3((m_tileSize * x) + (m_tileSize / 2) - (m_mapSizeXYZ.x / 2), 0, (m_tileSize * z) - (m_mapSizeXYZ.x / 2)),
							XMFLOAT3(0, 0, 0) });
					}
				}

				// Check 'bottom' of current tile
				if (z < static_cast<int>(m_map.size()) - 1)
				{
					if (m_map[z + 1][x].bAcess == true)
					{
						m_renderWalls.push_back(CDrawWall{
							XMFLOAT3((m_tileSize * x) + (m_tileSize / 2)  - (m_mapSizeXYZ.x / 2), 0, (m_tileSize * z) + m_tileSize - (m_mapSizeXYZ.x / 2)),
							XMFLOAT3(0, 180, 0) });
					}
				}
			}
		}
	}
}