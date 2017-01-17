#pragma once

#include <FS/Types.h>
#include <FS/FMath/Math.h>
#include <FS/FMath/Vec2.h>

namespace FS { namespace FMath 
{
	class Vec3
	{
	public:
		union
		{
			struct { Real x, y, z; };
			Real p[3];
		};

		Vec3() 
			: x(0), y(0), z(0)
		{ }
		Vec3(Real s)
			: x(s), y(s), z(s) { }
		Vec3(Real x, Real y, Real z)
			: x(x), y(y), z(z) { }
		Vec3(const Vec2 & v2)
			: x(v2.x), y(v2.y), z(0) { }

		Real length()    const { return ::sqrt(x * x + y * y + z * z); }
		Real lengthSqr() const { return       (x * x + y * y + z * z); }

		const Vec3 normal() const { return *this / length(); }

		const Vec3 component(const Vec3 & n) const
		{
			return n * this->dot(n);
		}

		const Vec3 removeComponent(const Vec3 & n) const
		{
			const Vec3 & r = *this;
			return r - r.component(n);
		}

		const Vec3 operator+ (const Vec3 & rhs) const 
		{ return Vec3(x + rhs.x, y + rhs.y, z + rhs.z); }
		const Vec3 operator- (const Vec3 & rhs) const 
		{ return Vec3(x - rhs.x, y - rhs.y, z - rhs.z); }
		const Vec3 operator* (const Real s) const
		{ return Vec3(x * s, y * s, z * s); }
		const Vec3 operator/ (const Real s) const
		{ 
			Real inv = (Real)1.0 / s;
			return Vec3(x * inv, y * inv, z * inv); 
		}

		const Vec3 operator- () const
		{
			return Vec3(-x, -y, -z);
		}

		const Vec3 & operator+= (const Vec3 & rhs) 
		{ 
			x += rhs.x; y += rhs.y; z += rhs.z; 
			return *this;
		}

		const Vec3 & operator-= (const Vec3 & rhs) 
		{ 
			x -= rhs.x; y -= rhs.y; z -= rhs.z; 
			return *this;
		}

		const Vec3 & operator*= (const Real s)
		{
			*this = *this * s;
			return *this;
		}

		const Vec3 & operator/= (const Real s)
		{
			*this = *this / s;
			return *this;
		}

		Real dot(const Vec3 & rhs) const
		{
			return x * rhs.x + y * rhs.y + z * rhs.z;
		}

		const Vec3 cross(const Vec3 & rhs) const
		{
			// |  i  j  k |
			// | ax ay az |
			// | bx by bz |
			const Vec3 & a = *this;
			const Vec3 & b = rhs;

			return Vec3(a.y * b.z - a.z * b.y,
				        a.z * b.x - a.x * b.z,
						a.x * b.y - a.y * b.x);
		}

		Vec3 inTermsOf(const Vec3 & xaxis, const Vec3 & yaxis, const Vec3 & zaxis) const
		{
			const Vec3 & p = *this;

			return Vec3(p.dot(xaxis), p.dot(yaxis), p.dot(zaxis));
		}

		Vec3 transform(const Vec3 & nx, const Vec3 & ny, const Vec3 & nz)
		{
			return nx * x + ny * y + nz * z;
		}

		const Vec3 capLength(Real cap) const
		{
			Real len = length();
			if ( len > cap )
			{
				return *this * cap / len;
			}
			return *this;
		}

		static Real distance(const Vec3 & a, const Vec3 & b)
		{
			return (a - b).length();
		}

		static Vec3 random(const Vec3 & bmin, const Vec3 & bmax)
		{
			Vec3 p;
			for (int i = 0; i < 3; i++)
				p.p[i] = FMath::random(bmin.p[i], bmax.p[i]);
			return p;
		}

		static Vec3 random(Real s)
		{
			return random(Vec3(-s, -s, -s), Vec3(s, s, s));
		}
	};

	inline const Vec3 operator* (Real s, const Vec3 & r)
	{
		return r * s;
	}

	//-------------------------------------------------------
	class Vec3i
	{
	public:
		union
		{
			struct { int x, y, z; };
		};
	};
}}