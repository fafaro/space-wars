#include "StdAfx.h"
#include <FS/F3D/Mesh.h>
#include <tinyxml.h>
#include <sstream>
#include <FS/FMath/Mat4x4.h>
#include <boost/foreach.hpp>

namespace FS { namespace F3D
{
	using FS::FMath::Mat4x4;

	void Mesh::load(const char *fileName)
	{
		TiXmlDocument xmlDoc;
		if ( false == xmlDoc.LoadFile(fileName) ) throw MeshLoaderException();
		TiXmlElement *eMesh  = xmlDoc.FirstChildElement("mesh");
		if (!eMesh) throw MeshLoaderException();

		// Declarations
		int numVerts;
		int numFaces;
		int numTVerts;

		//-----------------------------------------------------------------
		// Vertices
		TiXmlElement *eVerts = eMesh->FirstChildElement("vertices");
		if (!eVerts) throw MeshLoaderException();

		if (TIXML_SUCCESS != eVerts->QueryIntAttribute("count", &numVerts)) throw MeshLoaderException();
		this->setNumVerts(numVerts);

		std::istringstream vertStm(eVerts->GetText());
		for (int i = 0; i < numVerts; i++)
		{
			Vec3 v;
			vertStm >> v.x >> v.y >> v.z;
			// good() returns false at the end of stream
			if (vertStm.fail() || vertStm.bad()) throw MeshLoaderException();
			this->setVertex(i, v);
		}

		//-----------------------------------------------------------------
		// Faces
		TiXmlElement *eFaces = eMesh->FirstChildElement("faces");
		if (!eFaces) throw MeshLoaderException();

		if (TIXML_SUCCESS != eFaces->QueryIntAttribute("count", &numFaces)) throw MeshLoaderException();
		this->setNumFaces(numFaces);

		std::istringstream faceStm(eFaces->GetText());
		for (int i = 0; i < numFaces; i++)
		{
			Vec3i f;
			faceStm >> f.x >> f.y >> f.z;
			// good() returns false at the end of stream
			if (faceStm.fail() || faceStm.bad()) throw MeshLoaderException();
			this->setFace(i, f);
		}

		//-----------------------------------------------------------------
		// TVerts
		TiXmlElement *eTVerts = eMesh->FirstChildElement("tverts");
		if (!eTVerts) throw MeshLoaderException();

		if (TIXML_SUCCESS != eTVerts->QueryIntAttribute("count", &numTVerts)) throw MeshLoaderException();
		this->setNumTVerts(numTVerts);

		std::istringstream tvertStm(eTVerts->GetText());
		for (int i = 0; i < numTVerts; i++)
		{
			Vec2 v;
			tvertStm >> v.x >> v.y;
			// good() returns false at the end of stream
			if (tvertStm.fail() || tvertStm.bad()) throw MeshLoaderException();
			this->setTVert(i, v);
		}

		//-----------------------------------------------------------------
		// TFaces
		TiXmlElement *eTFaces = eMesh->FirstChildElement("tfaces");
		if (!eTFaces) throw MeshLoaderException();

		std::istringstream tfaceStm(eTFaces->GetText());
		for (int i = 0; i < numFaces; i++)
		{
			Vec3i f;
			tfaceStm >> f.x >> f.y >> f.z;
			// good() returns false at the end of stream
			if (tfaceStm.fail() || tfaceStm.bad()) throw MeshLoaderException();
			this->setTFace(i, f);
		}

	}

	void Mesh::scale(Real s)
	{
		for (int i = 0; i < getNumVerts(); i++)
			m_vertexArray[i] *= s;
	}

	void Mesh::rotate(Real deg, Real x, Real y, Real z)
	{
		Vec3 axis = Vec3(x, y, z).normal();
		Real angle = FMath::degtorad(deg);
		Mat4x4 mat = Mat4x4::rotation(angle, axis);

		for (int i = 0; i < getNumVerts(); i++)
			m_vertexArray[i] = mat * m_vertexArray[i];
	}
	
	void Mesh::translate(Real x, Real y, Real z)
	{
		translate(Vec3(x, y, z));
	}

	void Mesh::translate(const Vec3 & p)
	{
		for (int i = 0; i < getNumVerts(); i++)
			m_vertexArray[i] += p;
	}

	void Mesh::translateToCentroid()
	{
		translate(-centroid());
	}

	Vec3 Mesh::centroid() const
	{
		Vec3 c(0);
		for (int i = 0; i < getNumVerts(); i++)
			c += m_vertexArray[i];
		c /= getNumVerts();
		return c;
	}

	Real Mesh::boundingRadius() const
	{
		Real maxDist = 0;
		BOOST_FOREACH (Vec3 p, m_vertexArray)
		{
			maxDist = max(p.lengthSqr(), maxDist);
		}

		return FMath::sqrt(maxDist);
	}
}}
