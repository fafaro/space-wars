#include "stdafx.h"
#include <FS/F3D/TexMgr.h>
#include <FS/Types.h>
#include <stdio.h>

namespace FS { namespace F3D
{
	TexMgr *TexMgr::s_singleton = 0;

	TexMgr *TexMgr::instance()
	{
		if ( s_singleton == 0 )
			s_singleton = new TexMgr;
		return s_singleton;
	}

	int TexMgr::load(const char *fileName, const char *texName)
	{
		BITMAPFILEHEADER bfh;
		BITMAPINFOHEADER bih;

		DWORD *data = 0;

		// Load the file into data
		FILE *f = fopen(fileName, "rb");
		if (f == NULL) throw FileNotFoundException();

		fread(&bfh, sizeof(bfh), 1, f);
		fread(&bih, sizeof(bih), 1, f);

		fseek(f, bfh.bfOffBits, SEEK_SET);
		int nzeroes = (4 - (bih.biWidth % 4)) % 4;
		data = (DWORD *)malloc(sizeof(DWORD) * bih.biWidth * bih.biHeight);
		//for (int y = bih.biHeight - 1; y >= 0; y--)
		for (int y = 0; y < bih.biHeight; y++)
		{
			for (int x = 0; x < bih.biWidth; x++)
			{
				DWORD rgba = 0;
				fread(&rgba, 3, 1, f);
				if (rgba == 0x000000FF) rgba = 0; //rgba &= ~0xFF000000;
				else rgba |= 0xFF000000;
				data[x + y * bih.biWidth] = rgba;
			}
			fseek(f, nzeroes, SEEK_CUR);
		}
		fclose(f);

		// Load bitmap into texture
		Texture tex;
		glGenTextures(1, &tex.m_glName);
		glBindTexture(GL_TEXTURE_2D, tex.m_glName);
		GLenum err = glGetError();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bih.biWidth, bih.biHeight, 
			0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data);
		assert(glGetError()==GL_NO_ERROR);
		m_texList.push_back(tex);
		m_names[texName] = tex.m_glName;

		free(data);

		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	

		return 0;
	}

	void TexMgr::bind(int texId)
	{
		if (texId == -1)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, (GLuint)texId);
			assert(glGetError()==GL_NO_ERROR);
		}
	}

	int TexMgr::getTexId(const char *texName)
	{
		return m_names[texName];
		//Texture *texPtr = m_names[texName];
		//if (texPtr == NULL) return -1;
		//return texPtr->m_glName;
	}
}}