#include "stdafx.h"
#include <FS/Keyboard.h>

namespace FS
{
	Keyboard::Keyboard()
	{
		::memset(m_keyState, 0, sizeof(m_keyState));
	}

	void Keyboard::update(double interval)
	{
		m_interval = interval;
		::GetKeyboardState(m_keyState);
	}
}