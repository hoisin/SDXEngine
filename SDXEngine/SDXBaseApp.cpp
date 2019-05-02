#include "stdafx.h"
#include "SDXBaseApp.h"
#include <timeapi.h>
#include <sstream>

using namespace SDXEngine;

bool SDXBaseApp::m_bAppActive = false;
CTimer SDXBaseApp::m_timer = CTimer();

SDXBaseApp::SDXBaseApp() : m_hInstance(nullptr), m_hMutex(nullptr), m_windowName(""), m_appClassName(""),
	m_windowWidth(0), m_windowHeight(0), m_bRun(false)
{
}


SDXBaseApp::~SDXBaseApp()
{
}

bool SDXBaseApp::Initialise(const std::string& windowTitle, UINT windowWidth,
	UINT windowHeight, HINSTANCE hInstance)
{
	m_windowName = windowTitle;
	if (m_windowName == "")
		m_windowName = "undefined_name";

	m_appClassName = m_windowName + "App";
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;

	m_hMutex = CreateMutex(NULL, true, m_appClassName.c_str());
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL, "Application already running!", "Multiple instances found",
			MB_ICONINFORMATION | MB_OK);
		return false;
	}

	SDXErrorId error = SDX_ERROR_NONE;

	// Attempt to register window class
	error = RegisterAppClass();
	if (error != SDX_ERROR_NONE)
	{
		// Error log
		return false;
	}

	// Attempt to create the window
	error = CreateAppWindow();
	if (error != SDX_ERROR_NONE)
	{
		// Error log
		return false;
	}

	// Run OnInitialise for derived behavior
	if (OnInitialise() != SDX_ERROR_NONE)
	{
		// Do some error logging
		return false;
	}

	// Flag to run
	m_bRun = true;;
	return true;
}

void SDXBaseApp::Run()
{
	MSG msg;
	SDXErrorId error = SDX_ERROR_NONE;

	m_timer.Reset();
	m_timer.Start();

	while (m_bRun)
	{
		// Check for event updates
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// Otherwise send message to appropriate window
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			m_timer.Tick();

			// Run loop only when the app is active
			if (m_bAppActive)
			{
				OnUpdate(m_timer.DeltaTimeSecs());
				OnDraw(m_timer.DeltaTimeSecs());
				CalculateFrameStats(m_timer.DeltaTimeSecs());
			}
			else
			{
				// Avoid hogging resources if not active
				Sleep(200);
			}
		}
	}

	ShutDown();
}

SDXErrorId SDXBaseApp::OnUpdate(double deltaT)
{
	// Implement in derived
	return SDX_ERROR_NONE;
}

SDXErrorId SDXBaseApp::OnDraw(double deltaT)
{
	// Implement in derived
	return SDX_ERROR_NONE;
}

bool SDXBaseApp::OnEvent(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Implement in derived
	return false;
}

HWND SDXEngine::SDXBaseApp::GetHwnd() const
{
	return m_hWnd;
}

UINT SDXEngine::SDXBaseApp::GetWindowWidth() const
{
	return m_windowWidth;
}

UINT SDXEngine::SDXBaseApp::GetWindowHeight() const
{
	return m_windowHeight;
}

void SDXEngine::SDXBaseApp::StopRun()
{
	m_bRun = false;
}

LRESULT SDXBaseApp::MsgHandlerMain(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	if (hWnd == NULL)
		throw std::runtime_error("Invalid window handle");

	// Retrieve a pointer to the Application object
	SDXBaseApp* app = (SDXBaseApp*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	if (app == NULL)
		return DefWindowProc(hWnd, uiMsg, wParam, lParam);

	PAINTSTRUCT ps;
	// Handle required messages
	switch (uiMsg)
	{
	case WM_DESTROY:
		ShowWindow(hWnd, SW_HIDE);
		app->m_bRun = false;
		PostQuitMessage(0);
		break;

	case WM_PAINT:
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_ACTIVATE:
	{
		switch (LOWORD(wParam))
		{
		case WA_ACTIVE:
		case WA_CLICKACTIVE:
			m_bAppActive = true;
			m_timer.Start();
			break;
		case WA_INACTIVE:
			// Set this to false if you want the app to 'pause' when not focused
			m_bAppActive = true;
			m_timer.Stop();
			break;
		}
		break;
	}
	case WM_SIZE:
		break;
	}

	// Run the OnEvent() function to handle our events
	if (app->OnEvent(hWnd, uiMsg, wParam, lParam) == false)
		return DefWindowProc(hWnd, uiMsg, wParam, lParam);

	return 0;
}

SDXErrorId SDXBaseApp::RegisterAppClass()
{
	WNDCLASSEX wcex;
	memset(&wcex, 0, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.hInstance = m_hInstance;
	wcex.lpfnWndProc = MsgHandlerMain;
	wcex.lpszClassName = m_appClassName.c_str();
	wcex.lpszMenuName = NULL;

	// Register Class
	if (!RegisterClassEx(&wcex))
		return SDX_ERROR_APPCLASSREG_FAIL;

	return SDX_ERROR_NONE;
}

SDXErrorId SDXBaseApp::CreateAppWindow()
{
	// Create and determine window size
	RECT windowSize;
	windowSize.left = 0;
	windowSize.right = m_windowWidth;
	windowSize.top = 0;
	windowSize.bottom = m_windowHeight;

	AdjustWindowRectEx(&windowSize, WS_VISIBLE | WS_SYSMENU | WS_MINIMIZEBOX |
		WS_CLIPCHILDREN | WS_CLIPSIBLINGS, false, WS_EX_OVERLAPPEDWINDOW);

	m_hWnd = CreateWindowEx(0, m_appClassName.c_str(), m_windowName.c_str(), WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN,
		0, 0, (windowSize.right - windowSize.left), (windowSize.bottom - windowSize.top), NULL,
		NULL, m_hInstance, NULL);

	// Recalculate window to get client area to correct size
	RECT rClient, rWindow;
	POINT ptDiff;
	GetClientRect(m_hWnd, &rClient);
	GetWindowRect(m_hWnd, &rWindow);
	ptDiff.x = (rWindow.right - rWindow.left) - rClient.right;
	ptDiff.y = (rWindow.bottom - rWindow.top) - rClient.bottom;
	MoveWindow(m_hWnd, rWindow.left, rWindow.top, m_windowWidth + ptDiff.x, m_windowHeight + ptDiff.y, TRUE);

	if (m_hWnd == 0)
		return SDX_ERROR_CREATEWINDOW_FAIL;

	// Store a pointer to this object in the window, otherwise we can't grab it using
	// GetWindowLongPtr(..) in the callback for messages
	SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (INT_PTR)this);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return SDX_ERROR_NONE;
}

void SDXBaseApp::CalculateFrameStats(double deltaT)
{
	static int frameCount = 0;
	static double timeElapsed = 0.0f;

	// Increment counter and elapsed time
	frameCount++;
	timeElapsed += deltaT;

	// If exceed over a second
	if ((timeElapsed) >= 1.0f) {
		float fps = (float)frameCount;
		float mspf = 1000.0f / fps;

		// Write stats on the title of the window
		std::ostringstream outs;
		outs.precision(6);
		outs << m_windowName << "    " << "FPS: " << fps << "    " << "Frame Time: " << mspf << " (ms)";

		SetWindowText(m_hWnd, outs.str().c_str());

		// Reset vars
		frameCount = 0;
		timeElapsed = 0;
	}
}

void SDXBaseApp::ShutDown()
{
	OnShutDown();

	if (m_hWnd)
	{
		DestroyWindow(m_hWnd);
		m_hWnd = nullptr;
	}

	// Release and CloseHandle to fully close the mutex (for unit tests)
	if (m_hMutex)
	{
		ReleaseMutex(m_hMutex);
		CloseHandle(m_hMutex);
		m_hMutex = nullptr;
	}

	UnregisterClass(m_appClassName.c_str(), m_hInstance);

	m_bAppActive = false;
	m_windowWidth = 0;
	m_windowHeight = 0;
}
