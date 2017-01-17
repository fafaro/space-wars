#pragma once

#include <FS/Types.h>
#include <FS/FMath/Math.h>
#include <FS/FMath/Vec3.h>

namespace FS { namespace FMath
{
	class Quat;

	class Mat4x4
	{
	public:
		union
		{
			// m11 m12 m13 m14
			// m21 m22 m23 m24
			// m31 m32 m33 m34
			// m41 m42 m43 m44

			Real m[16];

			struct
			{
				Real _11, _21, _31, _41;
				Real _12, _22, _32, _42;
				Real _13, _23, _33, _43;
				Real _14, _24, _34, _44;
			};
		};

		Mat4x4()
		{
			setIdentity();
		}

		Real *getPtr() { return m; }

		void setIdentity()
		{
			Real data[] = 
			{ 
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
			::memcpy((void *)m, (void *)data, sizeof(m));
		}

		Vec3 getXAxis()       const { return Vec3(_11, _21, _31); }
		Vec3 getYAxis()       const { return Vec3(_12, _22, _32); }
		Vec3 getZAxis()       const { return Vec3(_13, _23, _33); }
		Vec3 getTranslation() const { return Vec3(_14, _24, _34); }

		void setXAxis(const Vec3 & p)     
		{ _11 = (Real)p.x; _21 = (Real)p.y; _31 = (Real)p.z; }

		void setYAxis(const Vec3 & p)     
		{ _12 = (Real)p.x; _22 = (Real)p.y; _32 = (Real)p.z; }

		void setZAxis(const Vec3 & p)     
		{ _13 = (Real)p.x; _23 = (Real)p.y; _33 = (Real)p.z; }

		void setAxes(const Vec3 & x, const Vec3 & y, const Vec3 & z)
		{
			setXAxis(x); setYAxis(y); setZAxis(z);
		}

		void setTranslation(const Vec3 & trans) 
		{ _14 = (Real)trans.x; _24 = (Real)trans.y; _34 = (Real)trans.z; }

		Mat4x4 getFastInverse() const
		{
			Mat4x4 inv;

			Vec3 cxaxis = getXAxis();
			Vec3 cyaxis = getYAxis();
			Vec3 czaxis = getZAxis();
			Vec3 cpos   = getTranslation();

			inv.setXAxis(Vec3(1, 0, 0).inTermsOf(cxaxis, cyaxis, czaxis));
			inv.setYAxis(Vec3(0, 1, 0).inTermsOf(cxaxis, cyaxis, czaxis));
			inv.setZAxis(Vec3(0, 0, 1).inTermsOf(cxaxis, cyaxis, czaxis));
			inv.setTranslation((-cpos).inTermsOf(cxaxis, cyaxis, czaxis));

			return inv;
		}

		void getData(float *data) const
		{
			for (int i = 0; i < 16; i++)
			{
				data[i] = (float)m[i];
			}
		}

		void rotateX(Real rads)
		{
			Real cosx = cos(rads);
			Real sinx = sin(rads);

			Vec3 nx(1, 0, 0);
			Vec3 ny(0, cosx, sinx);
			Vec3 nz(0, -sinx, cosx);

			transform(nx, ny, nz);
		}

		void rotateY(Real rads)
		{
			Real cosx = cos(rads);
			Real sinx = sin(rads);

			Vec3 nx(cosx,   0, -sinx);
			Vec3 ny(   0,   1,     0);
			Vec3 nz(sinx,   0,  cosx);

			transform(nx, ny, nz);
		}

		void rotateZ(Real rads)
		{
			Real cosx = cos(rads);
			Real sinx = sin(rads);

			Vec3 nx( cosx, sinx,    0);
			Vec3 ny(-sinx, cosx,    0);
			Vec3 nz(    0,    0,    1);

			transform(nx, ny, nz);
		}

		static Mat4x4 rotation(Real radians, const Vec3 & n)
		{
			Mat4x4 mat;
			mat.setIdentity();

			Real xs, ys, zs;
			Real xC, yC, zC;
			Real xyC, yzC, zxC;

			Real c = FMath::cos(radians);
			Real s = FMath::sin(radians);
			Real C = 1 - c;

			xs  = n.x * s;  ys  = n.y * s;  zs  = n.z * s;
			xC  = n.x * C;  yC  = n.y * C;  zC  = n.z * C;
			xyC = n.x * yC; yzC = n.y * zC; zxC = n.z * xC;

			Real data[] = 
			{ 
				n.x*xC+c, xyC+zs, zxC-ys, 0,
				xyC-zs, n.y*yC+c, yzC+xs, 0,
				zxC+ys, yzC-xs, n.z*zC+c, 0,
				0, 0, 0, 1
			};

			::memcpy(mat.m, data, sizeof(data));

			return mat;
		}

		static Mat4x4 rotationDeg(Real deg, Real x, Real y, Real z)
		{
			return rotation(FS::FMath::degtorad(deg), Vec3(x,y,z));
		}

		const Vec3 transform(const Vec3 & p) const
		{
			return p.x * getXAxis() + p.y * getYAxis() + p.z * getZAxis() + getTranslation();
		}

		void transform(const Vec3 & nx, const Vec3 & ny, const Vec3 & nz)
		{
			setXAxis(getXAxis().transform(nx, ny, nz));
			setYAxis(getYAxis().transform(nx, ny, nz));
			setZAxis(getZAxis().transform(nx, ny, nz));
		}
		void setOrient(const Quat & q);

		const Vec3 operator* (const Vec3 & r) const
		{
			return this->transform(r);
		}
	};
}}