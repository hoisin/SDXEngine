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

using namespace SDXEngine;

class CMaze
{
public:
	CMaze();
	~CMaze();

	bool Initialise(SDXRenderer* pRenderer);

	void Update(double deltaT);

	void Draw(double deltaT);

	//CObject* GetPlayer();

private:
	void LoadCrap();

	void DrawEnvironment();

private:
	SDXSceneMGR m_pSceneMGR;

	CMazeMap m_map;
	
	// Probably change to array of objects if multiple dynamic objects
	//CObject m_player;

	int m_tilesSize;
	int m_tilesInX;
	int m_tilesInZ;



	double m_updateTimer = 0.0;
	double m_frameTimer = 0.0;
	double m_tick = 50.0;
};
