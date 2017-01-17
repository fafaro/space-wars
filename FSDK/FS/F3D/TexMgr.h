#pragma once

#include <FS/F3D/GL.h>
#include <vector>
#include <map>
#include <string>

namespace FS { namespace F3D
{
	class Texture
	{
		friend class TexMgr;

	private:
		GLuint m_glName;
	};

	class TexMgr
	{
	private:
		std::vector<Texture> m_texList;

		// FIXME: Use a string class
		std::map<std::string, GLuint> m_names;
		
	public:
		static TexMgr *s_singleton;
		static TexMgr *instance();

		int load(const char *fileName, const char *texName);
		void bind(int texId);
		int getTexId(const char *texName);

	private:
		TexMgr() { }
	};
}}