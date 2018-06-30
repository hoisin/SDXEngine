#include "stdafx.h"
#include "SDXKeyboard.h"

using namespace SDXEngine;

SDXKeyboard::SDXKeyboard() : m_heldTimeThreshold(0)
{
	for (int i = 0; i < 256; i++)
	{
		m_keys[i] = 'u';
		m_keyHeldTime[i] = 0;
	}
}


SDXKeyboard::~SDXKeyboard()
{
}

void SDXEngine::SDXKeyboard::SetHeldThreshold(double time)
{
	m_heldTimeThreshold = time;
}

void SDXKeyboard::Update(double deltaT)
{
	// Update all the keys
	for(int i = 0; i < 256; i++)
	{
		//// If the key 
		//if (IsKeyUp(i))
		//	m_keyHeldTime[i] = 0;
		
		// If the key is down, check the elapsed time.
		// If exceed the threadhold, the key is being 'held'
		if (IsKeyDown(i))
		{
			m_keyHeldTime[i] += deltaT;
			if (m_keyHeldTime[i] >= m_heldTimeThreshold)
			{
				m_keys[i] = 'h';
			}
		}
	}
}

void SDXEngine::SDXKeyboard::SetKeyDown(int key)
{
	if (key >= 0 && key < 256)
	{
		m_keys[key] = 'd';
	}
}

void SDXEngine::SDXKeyboard::SetKeyUp(int key)
{
	if (key >= 0 && key < 256)
	{
		m_keys[key] = 'u';
		m_keyHeldTime[key] = 0;
	}
}

bool SDXKeyboard::IsKeyDown(int key) const
{
	if (key < 0 || key >= 256)
		return false;

	if (m_keys[key] == 'd')
		return true;

	return false;
}

bool SDXKeyboard::IsKeyUp(int key) const
{
	if (key < 0 || key >= 256)
		return false;

	if (m_keys[key] == 'u')
		return true;

	return false;
}

bool SDXKeyboard::IsKeyHeld(int key) const
{
	if (key < 0 || key >= 256)
		return false;

	if (m_keys[key] == 'h')
		return true;

	return false;
}
