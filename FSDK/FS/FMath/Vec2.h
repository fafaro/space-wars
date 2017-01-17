#pragma once

#include <FS/Types.h>
#include <FS/FMath/Math.h>

namespace FS { namespace FMath
{
	class Vec2
	{
	public:
		union
		{
			struct { Real x, y; };
			Real p[2];
		};

		Vec2() { }
		Vec2(Real x, Real y)
			: x(x), y(y) { }

		const Vec2 operator* (const Real s) const { return Vec2(x * s, y * s); }
		const Vec2 operator/ (const Real s) const { return *this * ((Real)1.0 / s); }
	};
}}