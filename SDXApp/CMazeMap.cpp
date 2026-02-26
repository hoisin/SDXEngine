#include "stdafx.h"
#include "CMazeMap.h"
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cmath>
#include <windows.h>

CMazeMap::CMazeMap()
    : m_mapSizeXYZ(0.0f, 0.0f, 0.0f),
      m_mapMin(0.0f, 0.0f, 0.0f),
      m_mapMax(0.0f, 0.0f, 0.0f),
      m_tileSize(0.0f)
{
}

CMazeMap::~CMazeMap()
{
}

void CMazeMap::CreateMap(int tileSize, int intTilesX, int intTilesZ)
{
	for (int i = 0; i < static_cast<int>(m_map.size()); i++)
	{
		m_map[i].clear();
	}
	m_map.clear();

	m_tileSize = tileSize;
	m_mapSizeXYZ.x = m_tileSize * intTilesX;
	m_mapSizeXYZ.z = m_tileSize * intTilesZ;

	// Get map max/min
	m_mapMin.x = -(m_mapSizeXYZ.x / 2.f);
	m_mapMin.z = -(m_mapSizeXYZ.z / 2.f);

	m_mapMax.x = (m_mapSizeXYZ.x / 2.f);
	m_mapMax.z = (m_mapSizeXYZ.z / 2.f);

	m_map.resize(intTilesZ);
	for (int z = 0; z < intTilesZ; z++)
	{
		m_map[z].resize(intTilesX);
	}

	// Initialize default accessibility to true
	for (int z = 0; z < static_cast<int>(m_map.size()); ++z)
	{
		for (int x = 0; x < static_cast<int>(m_map[z].size()); ++x)
		{
			m_map[z][x].bAccess = true;
		}
	}
}

CTile* CMazeMap::GetTileFromCoordinate(float x, float z)
{
	int pX = static_cast<int>(std::floor((x + (m_mapSizeXYZ.x / 2.0f)) / m_tileSize));
	int pZ = static_cast<int>(std::floor((z + (m_mapSizeXYZ.z / 2.0f)) / m_tileSize));

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
	m_renderWalls.clear();

	for (int z = 0; z < static_cast<int>(m_map.size()); z++)
	{
		for (int x = 0; x < static_cast<int>(m_map[z].size()); x++)
		{
			// If blocking tile
			if (m_map[z][x].bAccess == false)
			{
				// Check 'left' of current tile
				if (x > 0)
				{
					if (m_map[z][x - 1].bAccess == true)
					{
						m_renderWalls.push_back(CDrawWall{
						XMFLOAT3((m_tileSize * x) - (m_mapSizeXYZ.x / 2), 0, (m_tileSize * z) + (m_tileSize / 2) - (m_mapSizeXYZ.x / 2)),
						XMFLOAT3(0, 90, 0) });
					}
				}

				// Check 'right' of current tile
				if (x < (static_cast<int>(m_map[z].size() - 1)))
				{
					if (m_map[z][x + 1].bAccess == true)
					{
						m_renderWalls.push_back(CDrawWall{
						XMFLOAT3((m_tileSize * x) + m_tileSize - (m_mapSizeXYZ.x / 2), 0, (m_tileSize * z) + (m_tileSize / 2) - (m_mapSizeXYZ.x / 2)),
						XMFLOAT3(0, -90, 0) });
					}
				}

				// Check 'top' of current tile
				if (z > 0)
				{
					if (m_map[z - 1][x].bAccess == true)
					{
						m_renderWalls.push_back(CDrawWall{
						XMFLOAT3((m_tileSize * x) + (m_tileSize / 2) - (m_mapSizeXYZ.x / 2), 0, (m_tileSize * z) - (m_mapSizeXYZ.x / 2)),
						XMFLOAT3(0, 0, 0) });
					}
				}

				// Check 'bottom' of current tile
				if (z < static_cast<int>(m_map.size()) - 1)
				{
					if (m_map[z + 1][x].bAccess == true)
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

bool CMazeMap::SaveMap(const std::string& path) const
{
	std::ofstream ofs(path, std::ios::out);
	if (!ofs.is_open())
		return false;

	int tilesZ = static_cast<int>(m_map.size());
	int tilesX = (tilesZ > 0) ? static_cast<int>(m_map[0].size()) : 0;

	// Header: tileSize tilesX tilesZ
	ofs << m_tileSize << ' ' << tilesX << ' ' << tilesZ << '\n';

	for (int z = 0; z < tilesZ; ++z)
	{
		for (int x = 0; x < tilesX; ++x)
		{
			ofs << (m_map[z][x].bAccess ? '1' : '0');
		}
		ofs << '\n';
	}

	return ofs.good();
}

bool CMazeMap::LoadMap(const std::string& path)
{
	std::ifstream ifs(path, std::ios::in);
	if (!ifs.is_open())
		return false;

	int tileSize = 0;
	int tilesX = 0;
	int tilesZ = 0;

	// Read header line
	ifs >> tileSize >> tilesX >> tilesZ;
	if (!ifs.good())
		return false;

	// consume end of header line
	ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	// Recreate map with given sizes
	CreateMap(tileSize, tilesX, tilesZ);

	std::string line;
	for (int z = 0; z < tilesZ; ++z)
	{
		if (!std::getline(ifs, line))
			return false;

		// If line contains separators or spaces, remove them
		if ((int)line.size() < tilesX)
		{
			// Try to parse tokens separated by spaces
			std::istringstream iss(line);
			std::string token;
			int x = 0;
			while (x < tilesX && iss >> token)
			{
				char c = token.empty() ? '0' : token[0];
				m_map[z][x].bAccess = (c == '1');
				++x;
			}
			// If tokens were insufficient, continue reading from next input tokens
			while ((int)std::count(line.begin(), line.end(), ' ') + 1 < tilesX && std::getline(ifs, line))
			{
				std::istringstream iss2(line);
				std::string token2;
				int x2 = 0;
				while (x2 < tilesX && iss2 >> token2)
				{
					// find first uninitialized column
					int idx = 0;
					while (idx < tilesX && (m_map[z][idx].bAccess || !m_map[z][idx].bAccess)) // this always true but kept to be safe; we simply fill left to right
					{
						++idx;
						if (idx >= tilesX) break;
					}
					if (idx >= tilesX) break;
					char c = token2.empty() ? '0' : token2[0];
					m_map[z][idx].bAccess = (c == '1');
					++x2;
				}
			}
		}
		else
		{
			// Interpret first tilesX characters
			for (int x = 0; x < tilesX; ++x)
			{
				char c = line[x];
				m_map[z][x].bAccess = (c == '1');
			}
		}
	}

	// Clear walls to force recalculation if needed
	m_renderWalls.clear();

	return true;
}

bool CMazeMap::RestrictCoordinate(float prevX, float prevZ, float& x, float& z, float radius) const
{
	// Simplified: only restrict to map bounds (account for radius)
	const float margin = 0.01f;

	// If invalid map, leave coordinates unchanged
	if (m_tileSize <= 0.0f || m_map.empty() || m_map[0].empty())
	{
		return false;
	}

	if (m_mapMin.x > (x - radius) ||
		m_mapMin.z > (z - radius) ||
		m_mapMax.x < (x + radius) ||
		m_mapMax.z < (z + radius))
	{
		x = prevX;
		z = prevZ;

		return true;
	}

	return false;
}