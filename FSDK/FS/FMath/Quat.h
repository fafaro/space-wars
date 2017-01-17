#pragma once

#include <FS/Types.h>
#include <FS/FMath/Math.h>
#include <FS/FMath/Vec3.h>

namespace FS { namespace FMath
{
	class Quat
	{
	public:
		Real w, x, y, z;

	public:
		Quat()
		{
			x = y = z = 0;
			w = 1;
		}

		Quat(Real w, Real x, Real y, Real z)
			: w(w), x(x), y(y), z(z)
		{
		}

		void getAngleDirection(Real & angle, Real & ux, Real & uy, Real & uz)
		{
			// w = cos(a / 2)
			// (x,y,z) = u sin(a / 2)
			angle = (Real)2.0 * acos(w);
			Vec3 u = Vec3(x, y, z) / sin(angle * 0.5);
			ux = u.x; uy = u.y; uz = u.z;
		}

		void rotate(const Real angle, const Real ux, const Real uy, const Real uz)
		{
			Quat q = quatFromRotation(angle, ux, uy, uz);
			*this *= q;
		}

		static Quat quatFromRotation(const Real angle, const Real ux, const Real uy, const Real uz)
		{
			Quat q;

			// FIXME: Should this really be -cos?
			q.w = -cos(angle / 2);

			Vec3 u = Vec3(ux, uy, uz).normal();
			u *= sin(angle / 2);
			q.x = u.x; q.y = u.y; q.z = u.z;

			return q;
		}

		const Quat operator* (const Real & s) const
		{
			return Quat(w * s, x * s, y * s, z * s);
		}

		const Quat  operator* (const Quat & rhs) const
		{
			// (aw + ax i + ay j + az k)(bw + bx i + by j + bz k)
			// = aw.bw   + aw.bx i + aw.by j + aw.bz k +
			//   ax.bw i - ax.bx   + ax.by k - ax.bz j +
			//   ay.bw j - ay.bx k - ay.by   + ay.bz i +
			//   az.bw k + az.bx j - az.by i - az.bz
			const Quat & a = *this;
			const Quat & b = rhs;

			Quat q;

			q.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
			q.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
			q.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
			q.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;

			return q;
		}

		const Quat & operator*= (const Quat & rhs)
		{
			*this = *this * rhs;
			return *this;
		}

		const Quat exponent(Real exp) const
		{
			const Quat & q = *this;

			// Check for the case of an identity quaternion.   
			// This will protect against divide by zero   
			if (FMath::abs(q.w) > (Real)0.9999999) {   
				return *this;   
			}   
    
			// Extract the half angle alpha (alpha = theta/2)   
			Real alpha = acos(q.w);   

			// Compute new alpha value   
			Real newAlpha = alpha * exp;   

			// Compute new w value   
			Quat result;   
			result.w = cos(newAlpha);   

			// Compute new xyz values   
			Real mult = sin(newAlpha) / sin(alpha);   
			result.x = q.x * mult;   
			result.y = q.y * mult;   
			result.z = q.z * mult;   

			// Return it   
			return result;   
		}
	};
}}