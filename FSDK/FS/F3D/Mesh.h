#pragma once

#include <FS/Types.h>
#include <FS/FMath/Vec3.h>
#include <FS/F3D/GL.h>

namespace FS { namespace F3D
{
	using FS::Real;
	using FS::FMath::Vec2;
	using FS::FMath::Vec3;
	using FS::FMath::Vec3i;

	class MeshLoaderException { };

	class Mesh
	{
	public:
		typedef std::vector<Vec3>  VertexArray;
		typedef std::vector<Vec3i> FaceArray;
		typedef std::vector<Vec2>  TVertArray;
		typedef std::vector<Vec3i> TFaceArray;

	private:
		VertexArray m_vertexArray;
		FaceArray   m_faceArray;
		TVertArray  m_tvertArray;
		TFaceArray  m_tfaceArray;

	public:
		int  getNumVerts() const { return (int)m_vertexArray.size(); }
		void setNumVerts(int nverts) { m_vertexArray.resize(nverts); }
		Vec3 getVertex(int index) const { return m_vertexArray[index]; }
		void setVertex(int index, const Vec3 & v) { m_vertexArray[index] = v; }

		int  getNumFaces() const { return (int)m_faceArray.size(); }
		void setNumFaces(int nfaces) 
		{ 
			m_faceArray.resize(nfaces); 
			m_tfaceArray.resize(nfaces);
		}
		Vec3i getFace(int index) const { return m_faceArray[index]; }
		void  setFace(int index, const Vec3i & f) { m_faceArray[index] = f; }

		int  getNumTVerts() const { return (int)m_tvertArray.size(); }
		void setNumTVerts(int size) { m_tvertArray.resize(size); }
		Vec2 getTVert(int index) const { return m_tvertArray[index]; }
		void setTVert(int index, const Vec2 & v) { m_tvertArray[index] = v; }

		Vec3i getTFace(int index) const { return m_tfaceArray[index]; }
		void setTFace(int index, const Vec3i & v) { m_tfaceArray[index] = v; }

		Vec3 centroid() const;
		Real boundingRadius() const;

		void load(const char *fileName);
		void scale(Real s);
		void rotate(Real deg, Real x, Real y, Real z);
		void translate(Real x, Real y, Real z);
		void translate(const Vec3 & p);
		void translateToCentroid();
	};

	class MeshGL
	{
	public:
		static void render(Mesh *mesh)
		{
			namespace GL = FS::F3D::GL;

			glBegin(GL_TRIANGLES);
			for (int i = 0; i < mesh->getNumFaces(); i++)
			{
				Vec3i f = mesh->getFace(i);
				Vec3 a, b, c;
				a = mesh->getVertex(f.x);
				b = mesh->getVertex(f.y);
				c = mesh->getVertex(f.z);

				Vec3i tf = mesh->getTFace(i);
				Vec2 ta, tb, tc;
				ta = mesh->getTVert(tf.x);
				tb = mesh->getTVert(tf.y);
				tc = mesh->getTVert(tf.z);

				GL::normal((b-a).cross(c-a).normal());

				GL::texCoord(ta); GL::vertex(a);
				GL::texCoord(tb); GL::vertex(b);
				GL::texCoord(tc); GL::vertex(c);
			}
			glEnd();
		}
	};
}}