#include "stdafx.h"
#include "CTimer.h"

CTimer::CTimer()
{
	// Initialise the timer 
	Reset();
}


CTimer::~CTimer()
{
}

double CTimer::Time(void) const
{
	if (m_bStopped)
	{
		std::chrono::time_point<std::chrono::steady_clock> t1(m_stopPoint - m_pauseDuration);
		auto getTime = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - m_base);
		return getTime.count() * 0.001;
	}
	else
	{
		std::chrono::time_point<std::chrono::steady_clock> t1(m_current - m_pauseDuration);
		auto getTime = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - m_base);
		return getTime.count() * 0.001;
	}
}


double CTimer::DeltaTimeMicro() const
{
	return m_deltaTime;
}

double CTimer::DeltaTimeMilli() const
{
	return m_deltaTime * 0.001;
}

double CTimer::DeltaTimeSecs() const
{
	return m_deltaTime * 0.000001;
}

void CTimer::Reset(void)
{
	m_current = m_timer.now();
	m_previous = m_current;
	m_stopPoint = m_current;
	m_base = m_current;
	m_pauseDuration = {};
}


void CTimer::Start(void)
{
	auto start = m_timer.now();
	if (m_bStopped)
	{
		m_pauseDuration += (start - m_stopPoint);

		m_previous = start;

		m_stopPoint = start;
		m_bStopped = false;
	}
}


void CTimer::Stop(void)
{
	if (!m_bStopped)
	{
		m_stopPoint = m_current;
		m_bStopped = true;
	}
}


void CTimer::Tick(void)
{
	// Delta time 0 when stopped
	if (m_bStopped)
	{
		m_deltaTime = 0.0;
		return;
	}

	m_current = m_timer.now();
	auto delta = std::chrono::duration_cast<std::chrono::microseconds>(
		m_current - m_previous);

	m_deltaTime = static_cast<double>(delta.count());

	m_previous = m_current;

	if (m_deltaTime < 0.0)
		m_deltaTime = 0.0;
}