#include "stdafx.h"
#include "SDXMouse.h"

using namespace SDXEngine;

SDXMouse::SDXMouse() : m_heldThreshold(0), m_bShowCursor(true)
{
	memset(&m_pos, 0, sizeof(m_pos));
	memset(&m_prevPos, 0, sizeof(m_prevPos));
	memset(m_buttonDownPos, 0, sizeof(POINT) * SDXMOUSE_TOTALBUTTONS);
	memset(m_buttonUpPos, 0, sizeof(POINT) * SDXMOUSE_TOTALBUTTONS);

	for (int i = 0; i < SDXMOUSE_TOTALBUTTONS; i++)
	{
		m_buttons[i] = 'u';
		m_heldTime[i] = 0.0;
	}
}


SDXMouse::~SDXMouse()
{
}

void SDXEngine::SDXMouse::SetHeldThreshold(double threshold)
{
	m_heldThreshold = threshold;
}

POINT SDXEngine::SDXMouse::GetPos() const
{
	return m_pos;
}

POINT SDXEngine::SDXMouse::GetPrevPos() const
{
	return m_prevPos;
}

POINT SDXEngine::SDXMouse::GetButtonDownPos(SDXMouseButton button) const
{
	return m_buttonDownPos[button];
}

POINT SDXEngine::SDXMouse::GetButtonUpPos(SDXMouseButton button) const
{
	return m_buttonUpPos[button];
}

bool SDXEngine::SDXMouse::HandleEvent(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uiMsg)
	{
	case WM_MOUSEMOVE:
		POINT temp;
		GetCursorPos(&temp);
		ScreenToClient(GetActiveWindow(), &temp);
		m_prevPos = m_pos;
		m_pos = temp;
		return true;

	case WM_LBUTTONDOWN:
		m_buttons[SDXMOUSE_LEFT] = 'd';
		GetCursorPos(&m_buttonDownPos[SDXMOUSE_LEFT]);
		ScreenToClient(GetActiveWindow(), &m_buttonDownPos[SDXMOUSE_LEFT]);
		return true;

	case WM_LBUTTONUP:
		m_buttons[SDXMOUSE_LEFT] = 'u';
		GetCursorPos(&m_buttonUpPos[SDXMOUSE_LEFT]);
		ScreenToClient(GetActiveWindow(), &m_buttonUpPos[SDXMOUSE_LEFT]);
		m_heldTime[SDXMOUSE_LEFT] = 0.0;
		return true;

	case WM_RBUTTONDOWN:
		m_buttons[SDXMOUSE_RIGHT] = 'd';
		GetCursorPos(&m_buttonDownPos[SDXMOUSE_RIGHT]);
		ScreenToClient(GetActiveWindow(), &m_buttonDownPos[SDXMOUSE_RIGHT]);
		return true;

	case WM_RBUTTONUP:
		m_buttons[SDXMOUSE_RIGHT] = 'u';
		GetCursorPos(&m_buttonUpPos[SDXMOUSE_RIGHT]);
		ScreenToClient(GetActiveWindow(), &m_buttonUpPos[SDXMOUSE_RIGHT]);
		m_heldTime[SDXMOUSE_RIGHT] = 0.0;
		return true;

	case WM_MBUTTONDOWN:
		m_buttons[SDXMOUSE_MIDDLE] = 'd';
		GetCursorPos(&m_buttonDownPos[SDXMOUSE_MIDDLE]);
		ScreenToClient(GetActiveWindow(), &m_buttonDownPos[SDXMOUSE_MIDDLE]);
		return true;

	case WM_MBUTTONUP:
		m_buttons[SDXMOUSE_MIDDLE] = 'u';
		GetCursorPos(&m_buttonUpPos[SDXMOUSE_MIDDLE]);
		ScreenToClient(GetActiveWindow(), &m_buttonUpPos[SDXMOUSE_MIDDLE]);
		m_heldTime[SDXMOUSE_MIDDLE] = 0.0;
		return true;

	case WM_MOUSEHWHEEL:
		// To do later
		return true;
	}

	return false;
}

void SDXEngine::SDXMouse::Update(double deltaT)
{
	for (int i = 0; i < SDXMOUSE_TOTALBUTTONS; i++)
	{
		if (IsButtonDown((SDXMouseButton)i))
		{
			m_heldTime[i] += deltaT;
			if (m_heldTime[i] >= m_heldThreshold)
			{
				m_buttons[i] = 'h';
			}
		}
	}

	m_prevPos = m_pos;
}

bool SDXEngine::SDXMouse::IsButtonDown(SDXMouseButton button) const
{
	if (m_buttons[button] == 'd')
		return true;

	return false;
}

bool SDXEngine::SDXMouse::IsButtonUp(SDXMouseButton button) const
{
	if (m_buttons[button] == 'u')
		return true;

	return false;
}

bool SDXEngine::SDXMouse::IsButtonHeld(SDXMouseButton button) const
{
	if (m_buttons[button] == 'h')
		return true;

	return false;
}

bool SDXEngine::SDXMouse::IsCursorVisible() const
{
	return m_bShowCursor;
}

void SDXEngine::SDXMouse::ShowCursor(bool bShow)
{
	if (bShow)
	{
		::ShowCursor(TRUE);
		m_bShowCursor = true;
	}
	else
	{
		::ShowCursor(FALSE);
		m_bShowCursor = false;
	}
}

void SDXEngine::SDXMouse::UpdatePrevPos()
{
	m_prevPos = m_pos;
}

void SDXEngine::SDXMouse::SetCursorPos(POINT pos)
{
	POINT setPos(pos);
	m_pos = setPos;
	m_prevPos = setPos;
	ClientToScreen(GetActiveWindow(), &setPos);
	::SetCursorPos(setPos.x, setPos.y);
}


