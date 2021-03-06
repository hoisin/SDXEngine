//--------------------------------------------------------------------------
//
// Test app for Unit testing
//
//--------------------------------------------------------------------------
#pragma once

#include "..\SDXEngine\SDXBaseApp.h"

using namespace SDXEngine;

class SDXTestApp : public SDXBaseApp
{
public:
	SDXTestApp();
	~SDXTestApp();

	// Exposing for unit testing purposes only
	HWND GetHwndTest() const;

protected:
	SDXErrorId OnInitialise() override;
	SDXErrorId OnUpdate(double deltaT) override;
	SDXErrorId OnDraw(double deltaT) override;

	void OnShutDown() override;

	bool OnEvent(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
