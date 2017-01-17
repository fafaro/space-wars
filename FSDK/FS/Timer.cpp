#include "stdafx.h"
#include <FS/Timer.h>

namespace FS
{
	Timer::Timer()
	{
		::QueryPerformanceFrequency(&m_freq);
		m_lastTick.QuadPart = 0;
		m_lastInterval = 0;
	}

	void Timer::tick()
	{
		LARGE_INTEGER newTick;
		::QueryPerformanceCounter(&newTick);

		if (m_lastTick.QuadPart != 0)
		{
			m_lastInterval = float(newTick.QuadPart - m_lastTick.QuadPart) / m_freq.QuadPart;
		}
		else
		{
			m_lastInterval = 0;
		}

		m_lastTick = newTick;
	}
}