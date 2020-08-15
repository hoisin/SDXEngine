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
