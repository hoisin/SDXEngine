/*
	$ CTimer.h, Revision 1.1$
	Author: Matthew Tsang

	Class which provides timer functionality.

	Notes:

	QueryPerformanceFrequency(..)	-	Returns the number of clock cycles per second
	QueryPerformanceCounter(..)		-	Returns the number of clock cycles done so far
*/

#pragma once
#ifndef __CTIMER_H__
#define __CTIMER_H__

#include <Windows.h>

class CTimer
{
protected:
	double m_secondsPerCount;
	double m_deltaTime;

	// Should probably use LARGE_INTEGER since  __int64 is M$ specific.....
	__int64 m_currentTime;
	__int64 m_prevTime;
	__int64 m_stopTime;
	__int64 m_pauseTime;
	__int64 m_baseTime;
	
	bool m_bStopped;	

public:
	CTimer(void);

	// Returns the current time in seconds.
	// Note, if reset is not called before starting, then current time starts at the current duration of the app.
	double Time(void) const;

	// Returns the elapsed time since last Tick().
	double DeltaTime(void) const;

	// Resets timer. Returned value from Time will start from 0
	void Reset(void);

	// Tells timer to start
	void Start(void);

	// Stops timer
	void Stop(void);

	// Update timer (call in update loop).
	void Tick(void);
};

#endif



