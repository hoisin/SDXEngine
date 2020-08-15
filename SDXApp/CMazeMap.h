#pragma once

#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

struct CTile
{
	bool bAcess = true;
};

class CMazeMap
{
public:
	CMazeMap();
	~CMazeMap();

	void CreateMap(int tileSize, int tilesX, int tilesZ);

	CTile* GetTileFromCoordinate(float x, float z);

public:
	std::vector<std::vector<CTile>> m_map;

	XMFLOAT3 m_mapSizeXYZ;
	XMFLOAT3 m_mapMin;
	XMFLOAT3 m_mapMax;

	float m_tileSize;
};