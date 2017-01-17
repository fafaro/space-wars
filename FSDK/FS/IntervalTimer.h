#pragma once

#include <FS/Windows.h>

namespace FS
{
	class IntervalTimer
	{
	private:
		LARGE_INTEGER m_freq;
		LARGE_INTEGER m_prevTick;
		double        m_interval;
		double        m_intervalCap;

	public:
		IntervalTimer();

		double getInterval() const 
		{ 
			return m_interval < m_intervalCap ? m_interval : m_intervalCap; 
		}

		void setIntervalCap(double cap) { m_intervalCap = cap; }

		void step();
	};
}