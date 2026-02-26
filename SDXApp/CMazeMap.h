#pragma once

#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

struct CTile
{
	bool bAccess = true;
};

struct CDrawWall
{
	XMFLOAT3 pos;
	XMFLOAT3 rot;
};

class CMazeMap
{
public:
	CMazeMap();
	~CMazeMap();

	void CreateMap(int tileSize, int tilesX, int tilesZ);

	CTile* GetTileFromCoordinate(float x, float z);

	void CalculateWallsToRender();

	bool SaveMap(const std::string& path) const;

	bool LoadMap(const std::string& path);

	bool RestrictCoordinate(float prevX, float prevZ, float& x, float& z, float radius) const;

public:
	std::vector<std::vector<CTile>> m_map;
	std::vector<CDrawWall> m_renderWalls;

	XMFLOAT3 m_mapSizeXYZ;
	XMFLOAT3 m_mapMin;
	XMFLOAT3 m_mapMax;

	float m_tileSize;
};