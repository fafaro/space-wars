#include "stdafx.h"
#include <FS/F3D/GL.h>

namespace FS { namespace F3D { namespace GL
{
	float rtOtherSide(float hyp, float side)
	{
		return sqrt(hyp * hyp - side * side);
	}

	void normal(const Vec3 & n)
	{
		glNormal3f((float)n.x, (float)n.y, (float)n.z);
	}

	void vertex(Real x, Real y, Real z)
	{
		glVertex3f((float)x, (float)y, (float)z);
	}

	void vertex(const Vec3 & p)
	{
		vertex(p.x, p.y, p.z);
	}

	void color(float r, float g, float b)
	{
		glColor3f(r, g, b);
	}

	void texCoord(const Vec2 & t)
	{
		glTexCoord2f((float)t.x, (float)t.y);
	}

	void translate(const Vec3 & p)
	{
		glTranslatef((float)p.x, (float)p.y, (float)p.z);
	}

	void setMtlSpecular(float r, float g, float b, float a)
	{
		float params[] =  { r, g, b, a };
		glMaterialfv(GL_FRONT, GL_SPECULAR, params);
	}

	void setMtlShininess(float amt)
	{
		glMaterialf(GL_FRONT, GL_SHININESS, amt);
	}

	void drawSphere(float radius, int nSlices, int nPlates, bool solid)
	{
		float plateAngle = (float)M_PI / nPlates;
		float sliceAngle = (float)M_PI * 2.0f / nSlices;

		glPolygonMode(GL_FRONT_AND_BACK, solid ? GL_FILL : GL_LINE);

		// i = 0
		glBegin(GL_TRIANGLES);
			Vec3 v1 = Vec3(0, 0, radius);
			Vec3 n1 = Vec3(0, 0, 1);
			float z = radius * cos(plateAngle);
			float r = radius * sin(plateAngle);
			for (int j = 0; j < nSlices; j++)
			{
				float currAngle = j * sliceAngle;
				float nextAngle;
				if ( j == nSlices - 1 ) nextAngle = 0;
				else nextAngle = (j + 1) * sliceAngle;
				Vec3 v2 = Vec3(r * cos(currAngle), r * sin(currAngle), z);
				Vec3 v3 = Vec3(r * cos(nextAngle), r * sin(nextAngle), z);
				normal(n1); vertex(v1);
				normal(v2.normal()); vertex(v2);
				normal(v3.normal()); vertex(v3);
			}
		glEnd();

		glBegin(GL_QUADS);
		for (int i = 1; i < nPlates - 1; i++)
		{
			float ang1 = i * plateAngle;
			float ang2 = ang1 + plateAngle;
			float z1 = radius * cos(ang1);
			float r1 = radius * sin(ang1);
			float z2 = radius * cos(ang2);
			float r2 = radius * sin(ang2);
			for (int j = 0; j < nSlices; j++)
			{
				float currAngle = j * sliceAngle;
				float nextAngle;
				if ( j == nSlices - 1 ) nextAngle = 0;
				else nextAngle = (j + 1) * sliceAngle;

				Vec3 v1(r2 * cos(currAngle), r2 * sin(currAngle), z2);
				Vec3 v2(r2 * cos(nextAngle), r2 * sin(nextAngle), z2);
				Vec3 v3(r1 * cos(nextAngle), r1 * sin(nextAngle), z1);
				Vec3 v4(r1 * cos(currAngle), r1 * sin(currAngle), z1);

				normal(v1.normal()); vertex(v1);
				normal(v2.normal()); vertex(v2);
				normal(v3.normal()); vertex(v3);
				normal(v4.normal()); vertex(v4);
			}
		}
		glEnd();

		/*float plateHeight = radius / nPlates;
		float subAngle = (float)M_PI * 2.0f / nSlices;
		float plateAngle = (float)M_PI_2 / nPlates;

		glBegin(solid ? GL_QUADS : GL_LINE_LOOP);
		for (int i = 0; i < nPlates; i++)
		{
			float loZ = radius * sin(i * plateAngle);
			float hiZ = radius * sin((i+1) * plateAngle);
			float loRad = rtOtherSide(radius, loZ);
			float hiRad = rtOtherSide(radius, hiZ);

			for (int j = 0; j < nSlices; j++)
			{
				float angle1 = j * subAngle;
				float angle2 = angle1 + subAngle;
				float cos1 = cos(angle1);
				float sin1 = sin(angle1);
				float cos2 = cos(angle2);
				float sin2 = sin(angle2);

				Vec3 n1, n2, n3, n4;

				n1 = Vec3(loRad * cos1, loRad * sin1, loZ).getNormal();
				n2 = Vec3(loRad * cos2, loRad * sin2, loZ).getNormal();
				n3 = Vec3(hiRad * cos2, hiRad * sin2, hiZ).getNormal();
				n4 = Vec3(hiRad * cos1, hiRad * sin1, hiZ).getNormal();

				normal(n1);
				vertex(loRad * cos1, loRad * sin1, loZ);
				normal(n2);
				vertex(loRad * cos2, loRad * sin2, loZ);
				normal(n3);
				vertex(hiRad * cos2, hiRad * sin2, hiZ);
				normal(n4);
				vertex(hiRad * cos1, hiRad * sin1, hiZ);

				n1 = Vec3(loRad * cos1, loRad * sin1, -loZ).getNormal();
				n2 = Vec3(hiRad * cos1, hiRad * sin1, -hiZ).getNormal();
				n3 = Vec3(hiRad * cos2, hiRad * sin2, -hiZ).getNormal();
				n4 = Vec3(loRad * cos2, loRad * sin2, -loZ).getNormal();

				normal(n1);
				vertex(loRad * cos1, loRad * sin1, -loZ);
				normal(n2);
				vertex(hiRad * cos1, hiRad * sin1, -hiZ);
				normal(n3);
				vertex(hiRad * cos2, hiRad * sin2, -hiZ);
				normal(n4);
				vertex(loRad * cos2, loRad * sin2, -loZ);
			}
		}
		glEnd();*/
	}

	void drawGrid(int grid, float cellSize)
	{
		int   numCells = grid;
		float gridSize = cellSize * numCells;

		glBegin(GL_LINES);
		for (int i = -numCells; i <= numCells; i++)
		{
			glVertex3f(-gridSize, i * cellSize, 0);
			glVertex3f( gridSize, i * cellSize, 0);

			glVertex3f(i * cellSize, -gridSize, 0);
			glVertex3f(i * cellSize,  gridSize, 0);
		}
		glEnd();
	}

	void drawBox(const Vec3 & min, const Vec3 & max)
	{
		glBegin(GL_QUADS);
		vertex(min.x, min.y, min.z);
		vertex(max.x, min.y, min.z);
		vertex(max.x, max.y, min.z);
		vertex(min.x, max.y, min.z);
		glEnd();
	}
}}}