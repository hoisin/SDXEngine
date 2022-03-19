#include "stdafx.h"
#include "CMaze.h"

#include "..\SDXEngine\SDXAssetMGR.h"

using namespace SDXEngine;

CMaze::CMaze() : m_tilesInX(0), m_tilesInZ(0), m_tilesSize(0)
{
}

CMaze::~CMaze()
{
}

bool CMaze::Initialise(SDXRenderer* pRenderer)
{
	m_pSceneMGR.Initialise(pRenderer);

	// Define the maze
	m_tilesSize = 32;
	m_tilesInX = 50;
	m_tilesInZ = 50;

	m_map.CreateMap(m_tilesSize, m_tilesInX, m_tilesInZ);
	m_map.CalculateWallsToRender();

	LoadCrap();

	return false;
}

void CMaze::Update(double deltaT)
{
	m_updateTimer += deltaT;
	if (m_updateTimer >= m_tick)
	{
		m_updateTimer = 0.0;
		m_frameTimer = 0.0;

		
	}
}

void CMaze::Draw(double deltaT)
{
	m_frameTimer += deltaT;
	double interp = m_frameTimer / m_tick;

	m_pSceneMGR.ClearRenderList();

	// Draw environment
	DrawEnvironment();

	// Draw Dynamic objects
	// Should be nothing though since only the player and can't see its avatar.
	m_pSceneMGR.Render();
}

//CObject* CMaze::GetPlayer()
//{
//	return nullptr;
//}

/*
	Hard coded loading of assets required.
	Graphics, etc....
*/
void CMaze::LoadCrap()
{
	// Create floor plane (quad)
	SDXMeshData* pFloorPlane = ASSETMGR->GetMeshGenerator()->GeneratePlane(m_map.m_mapSizeXYZ.x, SDXVertexType::SDXVERTEX_TYPE_PNT, m_map.m_mapSizeXYZ.x / m_tilesSize, XMFLOAT3(0, 1, 0), 32, 32);
	pFloorPlane->SetMaterialID("basicFloorMaterial");
	SDXMeshData* pCeilingPlane = ASSETMGR->GetMeshGenerator()->GeneratePlane(m_map.m_mapSizeXYZ.x, SDXVertexType::SDXVERTEX_TYPE_PNT, m_map.m_mapSizeXYZ.x / m_tilesSize, XMFLOAT3(0.3, 0.3, 0.1), 4, 4);
	pCeilingPlane->SetMaterialID("basicCeilingMaterial");
	SDXMeshData* pWallQuad = ASSETMGR->GetMeshGenerator()->GenerateQuad(m_map.m_mapSizeXYZ.x, 128.f, SDXVertexType::SDXVERTEX_TYPE_PNT, m_map.m_mapSizeXYZ.x / m_tilesSize, 1, XMFLOAT3(0.1, 0.3, 0.5), 4);
	pWallQuad->SetMaterialID("basicWallMaterial");
	SDXMeshData* pInteriorWall = ASSETMGR->GetMeshGenerator()->GenerateQuad(m_map.m_tileSize, 128.f, SDXVertexType::SDXVERTEX_TYPE_PNT, 1, 1, XMFLOAT3(0.1, 0.3, 0.5), 0.5);
	pInteriorWall->SetMaterialID("basicWallInteriorMaterial");

	std::string floorPlaneID = "floor_plane";
	SDXMesh* pFloorMesh;
	ASSETMGR->CreateMesh(floorPlaneID, &pFloorMesh);
	pFloorMesh->AddSubMesh(pFloorPlane, ASSETMGR->GetDirectX());

	std::string ceilingPlaneID = "ceiling_plane";
	SDXMesh* pCeilingMesh;
	ASSETMGR->CreateMesh(ceilingPlaneID, &pCeilingMesh);
	pCeilingMesh->AddSubMesh(pCeilingPlane, ASSETMGR->GetDirectX());

	std::string wallQuadID = "wall_quad";
	SDXMesh* pWallMesh;
	ASSETMGR->CreateMesh(wallQuadID, &pWallMesh);
	pWallMesh->AddSubMesh(pWallQuad, ASSETMGR->GetDirectX());

	std::string wallInteriorId = "wall_interior";
	SDXMesh* pWallInteriorMesh;
	ASSETMGR->CreateMesh(wallInteriorId, &pWallInteriorMesh);
	pWallInteriorMesh->AddSubMesh(pInteriorWall, ASSETMGR->GetDirectX());

	delete pFloorPlane;
	delete pCeilingPlane;
	delete pWallQuad;
	delete pInteriorWall;
}

void CMaze::DrawEnvironment()
{
	// Render the floor/ceiling
	m_pSceneMGR.AddRenderItem(XMFLOAT3(0, 64, 0), XMFLOAT3(180, 0, 0), ASSETMGR->GetMesh("ceiling_plane"));
	m_pSceneMGR.AddRenderItem(XMFLOAT3(0, -64, 0), XMFLOAT3(0, 0, 0), ASSETMGR->GetMesh("floor_plane"));

	// Render the outer walls
	m_pSceneMGR.AddRenderItem(XMFLOAT3(0, 0, m_map.m_mapSizeXYZ.x/2), XMFLOAT3(0, 0, 0), ASSETMGR->GetMesh("wall_quad"));
	m_pSceneMGR.AddRenderItem(XMFLOAT3(0, 0, -m_map.m_mapSizeXYZ.x / 2), XMFLOAT3(0, 180, 0), ASSETMGR->GetMesh("wall_quad"));
	m_pSceneMGR.AddRenderItem(XMFLOAT3(m_map.m_mapSizeXYZ.x / 2, 0, 0), XMFLOAT3(0, 90, 0), ASSETMGR->GetMesh("wall_quad"));
	m_pSceneMGR.AddRenderItem(XMFLOAT3(-m_map.m_mapSizeXYZ.x / 2, 0, 0), XMFLOAT3(0, -90, 0), ASSETMGR->GetMesh("wall_quad"));

	for (int i = 0; i < static_cast<int>(m_map.m_renderWalls.size()); i++)
	{
		m_pSceneMGR.AddRenderItem(m_map.m_renderWalls[i].pos, m_map.m_renderWalls[i].rot, ASSETMGR->GetMesh("wall_interior"));
	}
}
	