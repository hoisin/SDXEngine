//--------------------------------------------------------------------------
//
//	Application maze
//
//--------------------------------------------------------------------------

#pragma once

#include "..\SDXEngine\SDXBaseApp.h"
#include "..\SDXEngine\SDXInputHandler.h"

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
	SDXErrorId OnInitialise() override;
	SDXErrorId OnUpdate(double deltaT) override;
	SDXErrorId OnDraw(double deltaT) override;

	void OnShutDown() override;
	bool OnEvent(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

	void HandleCamera(double deltaT);
	void DrawDebugText();

private:
	SDXRenderer m_renderer;
	SDXCameraFP m_camera;

	SDXInputHandler m_inputHDLR;

	bool m_bDebugText;
};

