#include "stdafx.h"
#include "SDXBaseApp.h"
#include <timeapi.h>
#include <sstream>

using namespace SDXEngine;

bool SDXBaseApp::m_bAppActive = false;
CTimer SDXBaseApp::m_timer = CTimer();

SDXBaseApp::SDXBaseApp() : 
	m_hWnd(nullptr), 
	m_hInstance(nullptr), 
	m_hMutex(nullptr), 
	m_windowName(""),
	m_appClassName(""),
	m_windowWidth(0), 
	m_windowHeight(0), 
	m_bRun(false),
	m_errorHelperInst(nullptr)
{
}

SDXBaseApp::~SDXBaseApp()
{
	m_errorHelperInst->Release();
	m_errorHelperInst = nullptr;
}

/// <summary>
/// Initiailises the window.
/// Registers the window, creates the window and runs code in OnInitialise() last.
/// </summary>
/// <param name="windowTitle">The title of the window</param>
/// <param name="windowWidth">Width of the window</param>
/// <param name="windowHeight">Height of the window</param>
/// <returns>True if successful</returns>
bool SDXBaseApp::Initialise(
	const std::string& windowTitle, 
	UINT windowWidth,
	UINT windowHeight)
{
	m_errorHelperInst = ERRORHELPER->GetInstance();

	m_windowName = windowTitle;
	if (m_windowName == "")
		m_windowName = "undefined_name";

	m_appClassName = m_windowName + "App";
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;

	// Prevent multiple instances running
	m_hMutex = CreateMutex(NULL, true, m_appClassName.c_str());
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL, "Application already running!", "Multiple instances found",
			MB_ICONINFORMATION | MB_OK);
		return false;
	}

	if (!RegisterAppClass())
	{
		MessageBox(NULL, "Register window class failed!", "Initialisation Error",
			MB_ICONINFORMATION | MB_OK);
		return false;
	}

	if (!CreateAppWindow())
	{
		MessageBox(NULL, "Create window failed!", "Initialisation Error",
			MB_ICONINFORMATION | MB_OK);
		return false;
	}

	// Run OnInitialise for derived behavior
	SDXErrorId error = OnInitialise();
	if (error != SDXErrorId::SDX_ERROR_NONE)
	{
		std::string errorMsg = "Initialise failed with " + ERRORHELPER->ErrorToString(error);
		MessageBox(NULL, errorMsg.c_str(), "Initialisation Error",
			MB_ICONINFORMATION | MB_OK);
		return false;
	}

	std::vector<int*> testVec = std::vector<int*>();

	int maxVal = 1000000;
	CTimer timer = CTimer();
	timer.Reset();
	timer.Start();
	timer.Tick();

	for (int i = 0; i < maxVal; i++)
	{
		testVec.push_back(new int(i));
	}

	for (int i = 0; i < maxVal; i++)
	{
		delete testVec[i];
	}

	timer.Tick();
	std::string output = std::format("Time elapsed ms: {}", timer.DeltaTimeMilli());
	MessageBox(nullptr, output.c_str(), "Test", 0);

	

	// Flag to run
	m_bRun = true;
	return true;
}

/// <summary>
/// Main loop
/// </summary>
void SDXBaseApp::Run()
{
	MSG msg;
	SDXErrorId error = SDXErrorId::SDX_ERROR_NONE;

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
				OnUpdate(m_timer.DeltaTimeMilli());
				OnDraw(m_timer.DeltaTimeMilli());
				CalculateFrameStats(m_timer.DeltaTimeMilli());
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

LRESULT SDXBaseApp::MsgHandlerMain(
	HWND hWnd, 
	UINT uiMsg, 
	WPARAM wParam, 
	LPARAM lParam)
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

bool SDXBaseApp::RegisterAppClass()
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
		return false;

	return true;
}

bool SDXBaseApp::CreateAppWindow()
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

	if (m_hWnd == 0)
	{
		return false;
	}

	// Recalculate window to get client area to correct size
	RECT rClient, rWindow;
	POINT ptDiff;
	GetClientRect(m_hWnd, &rClient);
	GetWindowRect(m_hWnd, &rWindow);
	ptDiff.x = (rWindow.right - rWindow.left) - rClient.right;
	ptDiff.y = (rWindow.bottom - rWindow.top) - rClient.bottom;
	MoveWindow(m_hWnd, rWindow.left, rWindow.top, m_windowWidth + ptDiff.x, m_windowHeight + ptDiff.y, TRUE);

	// Store a pointer to this object in the window, otherwise we can't grab it using
	// GetWindowLongPtr(..) in the callback for messages
	SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (INT_PTR)this);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return true;
}

void SDXBaseApp::CalculateFrameStats(double deltaT)
{
	static int frameCount = 0;
	static double timeElapsed = 0.0f;

	// Increment counter and elapsed time
	frameCount++;
	timeElapsed += deltaT;

	// If exceed over a second
	if ((timeElapsed) >= 1000.0f) {
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
