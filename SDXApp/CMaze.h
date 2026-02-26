/*
	Simple maze test app.

	Wall collision.
	No jumping or Y vertical movement, only XZ so tied to the 'ground'
	Only the player currently
*/
#pragma once

#include "CMazeMap.h"
#include "CObject.h"

#include "..\SDXEngine\SDXSceneMGR.h"
#include "..\SDXEngine\SDXRenderer.h"
#include "..\SDXEngine\SDXCameraFP.h"

using namespace SDXEngine;

class CMaze
{
public:
	CMaze();
	~CMaze();

	bool Initialise(SDXRenderer* pRenderer);

	void Update(double deltaT);

	void Draw(double deltaT);

	const CObject* const GetPlayer();

	const CMazeMap& GetMap() const { return m_map; }

	SDXCameraFP& GetPlayerCamera() { return m_playerCamera; }

	void UpdatePlayerPosition(const XMFLOAT3& newPos);

private:
	void DrawEnvironment();

private:
	SDXSceneMGR m_pSceneMGR;

	CMazeMap m_map;
	
	SDXCameraFP m_playerCamera;
	CObject m_player;

	double m_updateTimer = 0.0;
	double m_frameTimer = 0.0;
	double m_tick = 50.0;
};
