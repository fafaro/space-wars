#pragma once

#include <vector>
#include <map>
#include <string>
#include <boost/foreach.hpp>
#include <FS/FMath/Vec3.h>
#include <FS/F3D/Mesh.h>

namespace FS { namespace F3D
{
	using FS::FMath::Vec3;

	class MeshMgr
	{
		typedef std::vector<Mesh *> MeshList;
		typedef std::map<std::string, Mesh *> NameMap;

		MeshList m_meshList;
		NameMap m_names;

	private:
		MeshMgr() { }
		~MeshMgr();

	public:
		void load(const char *fileName, const char *meshName);
		Mesh *getMesh(const char *meshName) 
		{ 
			NameMap::iterator index = m_names.find(meshName);
			if (index == m_names.end()) return 0;
			return index->second; 
		}

	private: static MeshMgr *s_singleton;
	public: static MeshMgr *instance();
	};
}}