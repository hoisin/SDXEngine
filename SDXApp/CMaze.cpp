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
	m_tilesInX = 100;
	m_tilesInZ = 100;

	m_map.CreateMap(m_tilesSize, m_tilesInX, m_tilesInZ);

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
	//m_pSceneMGR.AddRenderItem(XMFLOAT3(20, 0, 0), XMFLOAT3(0, 0, 0), ASSETMGR->GetMesh("cube_1"));

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
	SDXMeshData* pFloorPlane = ASSETMGR->GetMeshGenerator()->GeneratePlane(m_map.m_mapSizeXYZ.x, SDXVERTEX_TYPE_PC, 20, XMFLOAT3(0, 1, 0));
	pFloorPlane->SetMaterialID("basicMaterial");
	SDXMeshData* pCeilingPlane = ASSETMGR->GetMeshGenerator()->GeneratePlane(m_map.m_mapSizeXYZ.x, SDXVERTEX_TYPE_PC, 20, XMFLOAT3(0.3, 0.3, 0.1));
	pCeilingPlane->SetMaterialID("basicMaterial");
	SDXMeshData* pWallQuad = ASSETMGR->GetMeshGenerator()->GenerateQuad(m_map.m_mapSizeXYZ.x, 128.f, SDXVERTEX_TYPE_PC, 20, 2, XMFLOAT3(0.1, 0.3, 0.5));
	pWallQuad->SetMaterialID("basicMaterial");

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

	delete pFloorPlane;
	delete pCeilingPlane;
	delete pWallQuad;
}

void CMaze::DrawEnvironment()
{
	m_pSceneMGR.AddRenderItem(XMFLOAT3(0, 64, 0), XMFLOAT3(180, 0, 0), ASSETMGR->GetMesh("ceiling_plane"));
	m_pSceneMGR.AddRenderItem(XMFLOAT3(0, -64, 0), XMFLOAT3(0, 0, 0), ASSETMGR->GetMesh("floor_plane"));

	m_pSceneMGR.AddRenderItem(XMFLOAT3(0, 0, m_map.m_mapSizeXYZ.x/2), XMFLOAT3(0, 0, 0), ASSETMGR->GetMesh("wall_quad"));
	m_pSceneMGR.AddRenderItem(XMFLOAT3(0, 0, -m_map.m_mapSizeXYZ.x / 2), XMFLOAT3(0, 180, 0), ASSETMGR->GetMesh("wall_quad"));
	m_pSceneMGR.AddRenderItem(XMFLOAT3(m_map.m_mapSizeXYZ.x / 2, 0, 0), XMFLOAT3(0, 90, 0), ASSETMGR->GetMesh("wall_quad"));
	m_pSceneMGR.AddRenderItem(XMFLOAT3(-m_map.m_mapSizeXYZ.x / 2, 0, 0), XMFLOAT3(0, -90, 0), ASSETMGR->GetMesh("wall_quad"));
}
	