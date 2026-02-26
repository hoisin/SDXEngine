#include "stdafx.h"
#include "CMaze.h"

#include "..\SDXEngine\SDXAssetMGR.h"

using namespace SDXEngine;

CMaze::CMaze()
{
}

CMaze::~CMaze()
{
}

bool CMaze::Initialise(SDXRenderer* pRenderer)
{
	m_pSceneMGR.Initialise(pRenderer);

	m_map.LoadMap("C:/Dev/SDXEngine/Assets/Map/mapdata.txt");
	m_map.CalculateWallsToRender();

	m_player.SetModelID("player_avatar");
	m_player.SetPosition(XMFLOAT3(0.f, -56.f, 0.f));

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

	auto camPos = m_playerCamera.GetPosition();
	UpdatePlayerPosition(XMFLOAT3(camPos.m128_f32[0], camPos.m128_f32[1], camPos.m128_f32[2]));
}

void CMaze::Draw(double deltaT)
{
	m_frameTimer += deltaT;
	double interp = m_frameTimer / m_tick;

	m_pSceneMGR.ClearRenderList();

	// Draw environment
	DrawEnvironment();

	// Draw Dynamic objects
	m_pSceneMGR.AddRenderItem(m_player.GetPosition(), XMFLOAT3(0, 0, 0), ASSETMGR->GetMesh("player_avatar"));

	m_pSceneMGR.Render();
}

const CObject* const CMaze::GetPlayer()
{
	return &m_player;
}

void CMaze::UpdatePlayerPosition(const XMFLOAT3& newPos)
{
	// previous valid player position
	XMFLOAT3 prevPos = m_player.GetPrevPosition();

	// desired from camera
	float desiredX = newPos.x;
	float desiredZ = newPos.z;

	// Determine collision radius: prefer player's collision sphere if set
	float radius = 4.0f; // default
	BoundingSphere bs = m_player.GetCollisionSphere();
	if (bs.Radius > 0.0f)
		radius = bs.Radius;

	// Resolve collisions using map (pass previous valid position to avoid snapping)
	bool restricted = m_map.RestrictCoordinate(prevPos.x, prevPos.z, desiredX, desiredZ, radius);

	XMFLOAT3 clampedPos = XMFLOAT3(desiredX, newPos.y, desiredZ);

	// store previous position and update
	if (!restricted)
	{
		m_player.SetPrevPosition(prevPos);
		m_player.SetPosition(clampedPos);
	}
	else
	{
		m_playerCamera.SetLookAt(m_playerCamera.GetPrevLook());
	}

	m_player.SetPrevPosition(clampedPos);
	m_playerCamera.SetPosition(XMVectorSet(clampedPos.x, clampedPos.y, clampedPos.z, 0.f));
	m_playerCamera.UpdatePreviousFrameData();
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