#include "StdAfx.h"
#include <FS/F3D/MeshMgr.h>

namespace FS { namespace F3D
{
	using FS::FMath::Vec3;
	
	MeshMgr::~MeshMgr()
	{
		BOOST_FOREACH (Mesh *mesh, m_meshList) delete mesh;
		m_meshList.clear();
	}

	void MeshMgr::load(const char *fileName, const char *meshName)
	{
		Mesh *mesh = new Mesh;
		mesh->load(fileName);
		m_meshList.push_back(mesh);
		m_names[meshName] = mesh;
	}

	MeshMgr *MeshMgr::s_singleton = 0;
	MeshMgr *MeshMgr::instance()
	{
		if (s_singleton == 0)
			s_singleton = new MeshMgr;
		return s_singleton;
	}
}}