#pragma once

#include <FS/Windows.h>

namespace FS
{
	class Timer
	{
	private:
		LARGE_INTEGER m_lastTick;
		LARGE_INTEGER m_freq;
		float         m_lastInterval;

	public:
		Timer();

		void tick();
		float getInterval() { return m_lastInterval; }
	};
}