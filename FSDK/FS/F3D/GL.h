#pragma once

#include <FS/Windows.h>

#include <gl/gl.h>
#include <gl/glu.h>

#include <FS/FMath/Math.h>
#include <FS/FMath/Vec3.h>

namespace FS { namespace F3D { namespace GL
{
	using FMath::Vec2;
	using FMath::Vec3;

	float rtOtherSide(float hyp, float side);
	void vertex(Real x, Real y, Real z);
	void vertex(const Vec3 & p);
	void normal(const Vec3 & n);
	void color(float r, float g, float b);
	void texCoord(const Vec2 & t);
	void translate(const Vec3 & p);
	void setMtlSpecular(float r, float g, float b, float a);
	void setMtlShininess(float amt);
	void drawSphere(float radius, int nSlices, int nPlates, bool solid = true);
	void drawGrid(int grid, float cellSize);
	void drawBox(const Vec3 & min, const Vec3 & max);
}}}