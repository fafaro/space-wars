#pragma once

#include <FS/Windows.h>
#include <FS/F3D/GL.h>
#include <FS/Types.h>

namespace FS { namespace F3D
{
	using FMath::Vec3;

	class GLWindow
	{
	public:
		GLWindow(const char *title, int width, int height, bool fullScreen);
		~GLWindow();

		void setTitle(const char *title);
		void setIcon(WORD rsrc);
		int run();
		void show(bool visible);
		void close();
		void drawText(Real x, Real y, const char *str);

		virtual void initGL();
		virtual void render();

	protected:
		void swapBuffers();

		virtual void onIdle();
		virtual void onPaint();
		virtual void onKey(char c);
		virtual void onResize(int w, int h);

	protected:
		HWND  m_hWnd;
	private:
		HDC   m_hDC;
		HGLRC m_glContext;
		GLuint m_fontListBase;
		static const TCHAR *wcName;

		void registerWC();
		void createWindow(int width, int height, bool fullScreen);
		void createGLContext();
		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	};
}}