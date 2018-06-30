#include "stdafx.h"
#include "SDXInputHandler.h"

using namespace SDXEngine;

SDXInputHandler::SDXInputHandler()
{
}


SDXInputHandler::~SDXInputHandler()
{
}

SDXErrorId SDXEngine::SDXInputHandler::Initialise()
{
	m_keyboard.SetHeldThreshold(0.100);
	m_mouse.SetHeldThreshold(0.5);
	return SDX_ERROR_NONE;
}

void SDXEngine::SDXInputHandler::UpdateInput(double deltaT)
{
	m_keyboard.Update(deltaT);
	m_mouse.Update(deltaT);
}

bool SDXEngine::SDXInputHandler::HandleMsg(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	bool handled = false;

	// SHIFT INTO KEYBOARD!!!!
	switch (uiMsg)
	{
	case WM_KEYDOWN:
		if (!m_keyboard.IsKeyHeld(wParam))
		{
			m_keyboard.SetKeyDown(wParam);
			handled = true;
		}
		break;

	case WM_KEYUP:
		m_keyboard.SetKeyUp(wParam);
		handled = true;
		break;

	default:
		break;
	}

	handled = m_mouse.HandleEvent(hWnd, uiMsg, wParam, lParam);

	return handled;
}

SDXKeyboard * SDXInputHandler::GetKeyboard()
{
	return &m_keyboard;
}

SDXMouse * SDXEngine::SDXInputHandler::GetMouse()
{
	return &m_mouse;
}
