#include "pch.h"
#include "SDXTestApp.h"


SDXTestApp::SDXTestApp()
{
}


SDXTestApp::~SDXTestApp()
{
}

HWND SDXTestApp::GetHwndTest() const
{
	return GetHwnd();
}

SDXErrorId SDXTestApp::OnInitialise()
{
	return SDX_ERROR_NONE;
}

SDXErrorId SDXTestApp::OnUpdate(double deltaT)
{
	// Set to quit for testing
	StopRun();
	return SDX_ERROR_NONE;
}

SDXErrorId SDXTestApp::OnDraw(double deltaT)
{
	return SDX_ERROR_NONE;
}

void SDXTestApp::OnShutDown()
{
}

bool SDXTestApp::OnEvent(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return false;
}
