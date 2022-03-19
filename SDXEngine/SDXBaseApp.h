//--------------------------------------------------------------------------
//
//	Abstract base application frame work
//
//--------------------------------------------------------------------------

#pragma once

#include <Windows.h>
#include "SDXErrors.h"
#include "CTimer.h"

namespace SDXEngine
{
	class SDXBaseApp
	{
	public:
		SDXBaseApp();
		virtual ~SDXBaseApp();

		bool Initialise(const std::string& windowTitle, UINT windowWidth,
			UINT windowHeight);

		void Run();

	protected:
		// Virtual methods
		virtual SDXErrorId OnInitialise() = 0;
		virtual void OnUpdate(double deltaT) = 0;
		virtual void OnDraw(double deltaT) = 0;

		virtual void OnShutDown() = 0;

		virtual bool OnEvent(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

		// Methods for derived classes
		HWND GetHwnd() const;
		UINT GetWindowWidth() const;
		UINT GetWindowHeight() const;
		void StopRun();

	private:
		static LRESULT CALLBACK MsgHandlerMain(HWND hWnd, UINT uiMsg,
			WPARAM wParam, LPARAM lParam);

		bool RegisterAppClass();
		bool CreateAppWindow();

		void CalculateFrameStats(double deltaT);
		void ShutDown();

	private:
		HWND			m_hWnd;
		HINSTANCE		m_hInstance;
		HANDLE			m_hMutex;

		std::string		m_windowName;
		std::string		m_appClassName;

		UINT			m_windowWidth;
		UINT			m_windowHeight;

		static bool		m_bAppActive;
		bool			m_bRun;

		static CTimer   m_timer;

		SDXErrorHelper* m_errorHelperInst;
	};

}