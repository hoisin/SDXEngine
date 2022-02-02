//--------------------------------------------------------------------------
//
//	Application maze
//
//--------------------------------------------------------------------------

#pragma once

#include "..\SDXEngine\SDXBaseApp.h"
#include "..\SDXEngine\SDXInputHandler.h"

#include "..\SDXEngine\SDXAssetMGR.h"

#include "CMaze.h"

// Testing
#include "..\SDXEngine\SDXRenderer.h"
#include "..\SDXEngine\SDXCameraFP.h"

using namespace SDXEngine;

class SDXApp : public SDXEngine::SDXBaseApp
{
public:
	SDXApp();
	~SDXApp();

protected:
	// Overrides
	SDXErrorId OnInitialise() override;
	void OnUpdate(double deltaT) override;
	void OnDraw(double deltaT) override;

	void OnShutDown() override;
	bool OnEvent(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

	// Class methods
	void HandleCamera(double deltaT);
	void DrawDebugText();

	SDXErrorId InitialiseResources();

	SDXErrorId TestInit();

private:
	// Helper load methods
	SDXErrorId LoadShaders();
	SDXErrorId LoadTextures();
	SDXErrorId LoadMaterials();

private:
	SDXAssetMGR* m_pAssetMgr;

	SDXRenderer m_renderer;
	SDXCameraFP m_camera;

	SDXInputHandler m_inputHDLR;

	CMaze m_maze;

	bool m_bDebugText;

	float testRot = 0;
	float testTrans = 0;
};

