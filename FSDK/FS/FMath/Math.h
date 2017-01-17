#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <FS/Types.h>

namespace FS { namespace FMath
{
	const Real PI      = (Real)3.14159265358979323846;
	const Real EPSILON = (Real)0.0001;

	inline bool valueBetween(Real s, Real low, Real high)
	{
		return s > low && s < high;
	}

	inline void capValue(Real & s, Real low, Real high)
	{
		if ( s < low ) s = low;
		else if ( s > high ) s = high;
	}

	inline Real degtorad(Real d) { return d * PI / 180.0; }
	inline Real radtodeg(Real r) { return r * 180.0 / PI; }

	inline Real abs(Real x) { return ::abs(x); }
	//inline Real max(Real a, Real b) { return a > b ? a : b; }
	//inline Real min(Real a, Real b) { return a < b ? a : b; }
	inline Real cos(Real x) { return ::cos(x); }
	inline Real sin(Real x) { return ::sin(x); }
	inline Real sqrt(Real x) { return ::sqrt(x); }

	inline bool equalsZero(Real x) { return abs(x) < EPSILON; }

	inline Real random(Real rmin, Real rmax)
	{
		return rmin + ((Real)::rand() / RAND_MAX) * (rmax - rmin);
	}
}}