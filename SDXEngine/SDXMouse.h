//--------------------------------------------------------------------------
//
//	Mouse handler
//
//--------------------------------------------------------------------------
#pragma once

namespace SDXEngine
{
	enum SDXMouseButton
	{
		SDXMOUSE_LEFT,
		SDXMOUSE_RIGHT,
		SDXMOUSE_MIDDLE,
		SDXMOUSE_TOTALBUTTONS
	};

	class SDXMouse
	{
	public:
		SDXMouse();
		~SDXMouse();

		void SetHeldThreshold(double threshold);

		POINT GetPos() const;
		POINT GetPrevPos() const;
		POINT GetButtonDownPos(SDXMouseButton button) const;
		POINT GetButtonUpPos(SDXMouseButton button) const;

		bool HandleEvent(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);

		void Update(double deltaT);

		bool IsButtonDown(SDXMouseButton button) const;
		bool IsButtonUp(SDXMouseButton button) const;
		bool IsButtonHeld(SDXMouseButton button) const;

		bool IsCursorVisible() const;
		void ShowCursor(bool bShow);
		void UpdatePrevPos();
		void SetCursorPos(POINT pos);

	private:
		POINT m_pos;
		POINT m_prevPos;
		POINT m_buttonDownPos[SDXMOUSE_TOTALBUTTONS];
		POINT m_buttonUpPos[SDXMOUSE_TOTALBUTTONS];

		char m_buttons[SDXMOUSE_TOTALBUTTONS];			// Same as keyboard with 'd', 'u' and 'h' states
		double m_heldTime[SDXMOUSE_TOTALBUTTONS];		// Held time for each button

		double m_heldThreshold;
		bool m_bShowCursor;
	};
}

