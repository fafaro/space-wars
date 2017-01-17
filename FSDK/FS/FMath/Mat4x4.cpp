#include "stdafx.h"
#include <FS/FMath/Mat4x4.h>
#include <FS/FMath/Quat.h>

namespace FS { namespace FMath
{
	void Mat4x4::setOrient(const Quat & q)
	{
		// http://www.j3d.org/matrix_faq/matrfaq_latest.html#Q54
		Real xx      = q.x * q.x;
		Real xy      = q.x * q.y;
		Real xz      = q.x * q.z;
		Real xw      = q.x * q.w;
		Real yy      = q.y * q.y;
		Real yz      = q.y * q.z;
		Real yw      = q.y * q.w;
		Real zz      = q.z * q.z;
		Real zw      = q.z * q.w;
		m[0]  = 1 - 2 * ( yy + zz );
		m[1]  =     2 * ( xy - zw );
		m[2]  =     2 * ( xz + yw );
		m[4]  =     2 * ( xy + zw );
		m[5]  = 1 - 2 * ( xx + zz );
		m[6]  =     2 * ( yz - xw );
		m[8]  =     2 * ( xz - yw );
		m[9]  =     2 * ( yz + xw );
		m[10] = 1 - 2 * ( xx + yy );
		m[3]  = m[7] = m[11] = m[12] = m[13] = m[14] = 0;
		m[15] = 1;
	}
}}
