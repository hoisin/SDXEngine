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
	return SDXErrorId::SDX_ERROR_NONE;
}

void SDXTestApp::OnUpdate(double deltaT)
{
	// Set to quit for testing
	StopRun();
}

void SDXTestApp::OnDraw(double deltaT)
{
}

void SDXTestApp::OnShutDown()
{
}

bool SDXTestApp::OnEvent(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return false;
}
