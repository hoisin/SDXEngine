#include "stdafx.h"
#include "CTimer.h"

CTimer::CTimer(void) :
	m_secondsPerCount(0.0),
	m_deltaTime(-1.0),
	m_currentTime(0),
	m_prevTime(0),
	m_stopTime(0),
	m_pauseTime(0),
	m_baseTime(0),
	m_bStopped(false)
{
	__int64 countsPerSecond;

	// Grab number of clock counts per second
	QueryPerformanceFrequency((LARGE_INTEGER*) &countsPerSecond);

	// Flip so we don't need to "/" for values later on
	m_secondsPerCount = 1.0 / (double)countsPerSecond;

	// Initialise the timer 
	Reset();
}


double CTimer::Time(void) const
{
	if(m_bStopped)
		return (double)(((m_stopTime - m_pauseTime) - m_baseTime) * m_secondsPerCount);
	else
		return (double)(((m_currentTime - m_pauseTime) - m_baseTime) * m_secondsPerCount);
}


double CTimer::DeltaTime(void) const
{
	return m_deltaTime;
}


void CTimer::Reset(void)
{
	__int64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*) &currentTime);

	m_currentTime = currentTime;
	m_baseTime = currentTime;
	m_prevTime = currentTime;
	m_stopTime = currentTime;
	m_pauseTime = 0;
}


void CTimer::Start(void)
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if(m_bStopped)
	{
		m_pauseTime += (startTime - m_stopTime);

		m_prevTime = startTime;

		m_stopTime = startTime;
		m_bStopped = false;
	}
}


void CTimer::Stop(void)
{
	if(!m_bStopped)
	{
		__int64 currentTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

		m_stopTime = currentTime;
		m_bStopped = true;
	}
}


void CTimer::Tick(void)
{
	if(m_bStopped)
	{
		m_deltaTime = 0.0;
		return;
	}

	__int64 currentTime;

	QueryPerformanceCounter((LARGE_INTEGER*) &currentTime);
	m_currentTime = currentTime;

	m_deltaTime = (m_currentTime - m_prevTime) * m_secondsPerCount;

	m_prevTime = m_currentTime;

	if(m_deltaTime < 0.0)
		m_deltaTime = 0.0;

}