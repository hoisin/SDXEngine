//--------------------------------------------------------------------------
//
//	Input interface handler
//
//--------------------------------------------------------------------------

#pragma once

#include "SDXKeyboard.h"
#include "SDXMouse.h"
#include "SDXErrors.h"

namespace SDXEngine
{
	class SDXInputHandler
	{
	public:
		SDXInputHandler();
		~SDXInputHandler();

		SDXErrorId Initialise();

		void UpdateInput(double deltaT);

		bool HandleMsg(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);

		SDXKeyboard* GetKeyboard();
		SDXMouse* GetMouse();

	private:
		SDXKeyboard m_keyboard;
		SDXMouse m_mouse;
	};
}