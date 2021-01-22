//--------------------------------------------------------------------------
//
//	Application maze
//
//--------------------------------------------------------------------------

#pragma once

#include "..\SDXEngine\SDXBaseApp.h"
#include "..\SDXEngine\SDXInputHandler.h"

#include "..\SDXEngine\SDXAssetMGR.h"

#include "..\SDXEngine\SDXRenderer.h"
#include "..\SDXEngine\SDXCameraFP.h"
#include "..\SDXEngine\SDXSceneMGR.h"

using namespace SDXEngine;

class SDXTestApp : public SDXEngine::SDXBaseApp
{
public:
	SDXTestApp();
	~SDXTestApp();

protected:
	// Overrides
	SDXErrorId OnInitialise() override;
	SDXErrorId OnUpdate(double deltaT) override;
	SDXErrorId OnDraw(double deltaT) override;

	void OnShutDown() override;
	bool OnEvent(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

	// Class methods
	void HandleCamera(double deltaT);
	void DrawDebugText();

	SDXErrorId InitialiseResources();

private:
	// Helper load methods
	SDXErrorId LoadShaders();
	SDXErrorId LoadTextures();
	SDXErrorId LoadMaterials();
	SDXErrorId LoadMeshes();

private:
	void DrawMyStuff();

private:
	SDXAssetMGR* m_pAssetMgr;

	SDXRenderer m_renderer;
	SDXSceneMGR m_sceneMGR;
	SDXCameraFP m_camera;

	SDXInputHandler m_inputHDLR;

	bool m_bDebugText;
};

