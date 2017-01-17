#include "stdafx.h"
#include <FS/IntervalTimer.h>

namespace FS
{
	IntervalTimer::IntervalTimer()
	{
		::QueryPerformanceFrequency(&m_freq);
		m_prevTick.QuadPart = 0;
		m_interval = 0.0;
		m_intervalCap = 1.0;
	}

	void IntervalTimer::step()
	{
		LARGE_INTEGER currTick;
		::QueryPerformanceCounter(&currTick);
		if ( m_prevTick.QuadPart == 0 ) m_interval = 0.0;
		else m_interval = (double)(currTick.QuadPart - m_prevTick.QuadPart) / m_freq.QuadPart;
		m_prevTick = currTick;
	}
}