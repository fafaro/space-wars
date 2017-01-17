#pragma once

#include <FS/Windows.h>

namespace FS
{
	class Keyboard
	{
	private:
		BYTE   m_keyState[256];
		double m_interval;

	public:
		Keyboard();

		void update(double interval);

		bool keyDown(int code) const { return (m_keyState[code] & 0x80) != 0; }
		double getInterval() const { return m_interval; }
	};
}