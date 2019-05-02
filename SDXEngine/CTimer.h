/*
	Class which provides timer functionality.
*/

#pragma once

#include <chrono>

class CTimer
{
public:
	CTimer();
	~CTimer();

	// Returns the current time in seconds.
	// Note, if reset is not called before starting, then current time starts at the current duration of the app.
	double Time() const;

	// Returns the elapsed time since last Tick().
	double DeltaTimeMicro() const;
	double DeltaTimeMilli() const;
	double DeltaTimeSecs() const;

	// Resets timer. Returned value from Time will start from 0
	void Reset();

	// Tells timer to start
	void Start();

	// Stops timer
	void Stop();

	// Update timer (call in update loop).
	void Tick();

protected:
	std::chrono::high_resolution_clock m_timer;

	std::chrono::time_point<std::chrono::steady_clock> m_current = {};
	std::chrono::time_point<std::chrono::steady_clock> m_previous = {};
	std::chrono::time_point<std::chrono::steady_clock> m_stopPoint = {};
	std::chrono::time_point<std::chrono::steady_clock> m_base = {};
	std::chrono::time_point<std::chrono::steady_clock> m_pauseDuration = {};

	double m_deltaTime = 0;
	bool m_bStopped = true;
};



