#include "stdafx.h"
#include "KeyScanner.h"
#include <boost/foreach.hpp>

namespace FS { namespace FGame
{
	KeyScanner::KeyScanner()
	{
	}

	void KeyScanner::addCode(const char *code, IListener *listener, int id)
	{
		CodeEntry ce;
		ce.code = code;
		ce.listener = listener;
		ce.id = id;
		ce.currPos = 0;
		m_codeList.push_back(ce);
	}

	void KeyScanner::inputKey(char c)
	{
		BOOST_FOREACH (CodeEntry & ce, m_codeList)
		{
			if (ce.code[ce.currPos] == c)
			{
				ce.currPos++;

				if (ce.code[ce.currPos] == '\0') // done
				{
					ce.currPos = 0;
					ce.listener->onKeySequence(ce.code, ce.id);
				}
			}
			else
			{
				ce.currPos = 0;
			}
		}
	}
}}