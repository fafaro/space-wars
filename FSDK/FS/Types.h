#pragma once

namespace FS
{
	typedef double        Real;
	typedef unsigned char Byte;

	class Exception { };
	class FileNotFoundException : public Exception { };

	template <class T>
	void swap(T & a, T & b)
	{
		T c = a;
		a = b;
		b = c;
	}
}