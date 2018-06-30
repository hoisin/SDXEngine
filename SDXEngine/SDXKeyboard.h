//--------------------------------------------------------------------------
//
//	Keyboard handling class
//
//  Keys state:
//	u - up
//  d - down
//  h - held
//
//--------------------------------------------------------------------------

#pragma once#

namespace SDXEngine
{
	class SDXKeyboard
	{
	public:
		SDXKeyboard();
		~SDXKeyboard();

		void SetHeldThreshold(double time);
		void Update(double deltaT);

		void SetKeyDown(int key);
		void SetKeyUp(int key);

		bool IsKeyDown(int key) const;
		bool IsKeyUp(int key) const;
		bool IsKeyHeld(int key) const;

	private:
		char m_keys[256];
		double m_keyHeldTime[256];
		double m_heldTimeThreshold;
	};
}