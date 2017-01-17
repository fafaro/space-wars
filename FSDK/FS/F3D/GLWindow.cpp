#include "stdafx.h"
#include <FS/F3D/GLWindow.h>

namespace FS { namespace F3D
{
	const TCHAR *GLWindow::wcName = TEXT("FS::F3D_GLWindow");

	//-----------------------------------------------------------------
	// Public:
	//-----------------------------------------------------------------
	GLWindow::GLWindow(const char *title, int width, int height, bool fullScreen)
	{
		createWindow(width, height, fullScreen);
		setTitle(title);
		show(true);
		createGLContext();
		initGL();
	}

	GLWindow::~GLWindow()
	{
		close();
	}

	void GLWindow::setTitle(const char *title)
	{
		wchar_t buffer[128];
		::mbsrtowcs(buffer, &title, 128, NULL);
		::SetWindowText(m_hWnd, buffer);
	}

	void GLWindow::setIcon(WORD rsrc)
	{
		HINSTANCE hInstance = (HINSTANCE)::GetModuleHandle(NULL);
		HICON icon = ::LoadIcon(hInstance, MAKEINTRESOURCE(rsrc));
		::SetClassLong(m_hWnd, GCL_HICON, (LONG)icon);
	}

	int GLWindow::run()
	{
		MSG msg;

		for ( ;; )
		{
			while ( ::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
			{
				if ( msg.message == WM_QUIT) goto endOfMainLoop;
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}

			onIdle();
		}

	endOfMainLoop:
		return (int)msg.wParam;
	}

	void GLWindow::show(bool visible)
	{
		::ShowWindow(m_hWnd, visible ? SW_SHOW : SW_HIDE);
		::UpdateWindow(m_hWnd);
	}

	void GLWindow::close()
	{
		if ( m_glContext ) ::wglDeleteContext(m_glContext);
		if ( m_hDC       ) ::ReleaseDC(m_hWnd, m_hDC);
		if ( m_hWnd      ) ::DestroyWindow(m_hWnd);

		m_glContext = NULL;
		m_hDC       = NULL;
		m_hWnd      = NULL;
	}

	void GLWindow::drawText(Real x, Real y, const char *str)
	{
		int len = (int)::strlen(str);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix(); glLoadIdentity(); gluOrtho2D(0, 1, 0, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix(); glLoadIdentity();

		glDisable(GL_DEPTH_TEST);
		glRasterPos2f((float)x, (float)y);
		glListBase(m_fontListBase); 
		glCallLists(len, GL_UNSIGNED_BYTE, str);
		glEnable(GL_DEPTH_TEST);

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}

	void GLWindow::initGL()
	{
		::SelectObject(m_hDC, ::GetStockObject(SYSTEM_FONT)); 
		
		m_fontListBase = ::glGenLists(256);
		::wglUseFontBitmaps (m_hDC, 0, 255, m_fontListBase); 
	}

	void GLWindow::render()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		swapBuffers();
	}

	//-----------------------------------------------------------------
	// Protected:
	//-----------------------------------------------------------------
	void GLWindow::swapBuffers()
	{
		::SwapBuffers(m_hDC);
	}

	void GLWindow::onIdle()
	{
		render();
	}

	void GLWindow::onPaint()
	{
		render();
	}

	void GLWindow::onKey(char c)
	{
		switch (c)
		{
		case VK_ESCAPE:
			close();
			break;
		}
	}

	void GLWindow::onResize(int w, int h)
	{
		glViewport(0, 0, w, h);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60, (double)w / h, 0.1, 1000.0);
		glMatrixMode(GL_MODELVIEW);
	}

	//-----------------------------------------------------------------
	// Private:
	//-----------------------------------------------------------------
	void GLWindow::registerWC()
	{
		WNDCLASSEX wc;
		HINSTANCE hInstance = (HINSTANCE)::GetModuleHandle(NULL);
		if ( ::GetClassInfoEx(hInstance, wcName, &wc) ) return;

		::ZeroMemory(&wc, sizeof(wc));
		wc.cbSize		 = sizeof(wc);
		wc.style		 = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc	 = &GLWindow::WndProc;
		wc.cbClsExtra	 = 0;
		wc.cbWndExtra	 = 0;
		wc.hInstance	 = hInstance;
		wc.hIcon		 = ::LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor		 = ::LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName	 = NULL;
		wc.lpszClassName = wcName;
		wc.hIconSm		 = NULL;

		::RegisterClassEx(&wc);
	}

	void GLWindow::createWindow(int width, int height, bool fullScreen)
	{
		registerWC();

		DWORD winStyle = WS_OVERLAPPEDWINDOW;

		int sx = ::GetSystemMetrics(SM_CXSCREEN);
		int sy = ::GetSystemMetrics(SM_CYSCREEN);
		int x = (sx - width) / 2;
		int y = (sy - height) / 2;
		RECT rect = { x, y, x + width, y + height };
		::AdjustWindowRect(&rect, winStyle, NULL);

		m_hWnd = ::CreateWindowEx(/*WS_EX_TOOLWINDOW*/0, wcName, TEXT("GLWindow"), 
			winStyle, 
			rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 
			NULL, NULL, (HINSTANCE)::GetModuleHandle(NULL), this);
	}
	
	void GLWindow::createGLContext()
	{
		PIXELFORMATDESCRIPTOR pfd = { 
			sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd  
			1,                     // version number  
			PFD_DRAW_TO_WINDOW |   // support window  
			PFD_SUPPORT_OPENGL |   // support OpenGL  
			PFD_DOUBLEBUFFER,      // double buffered  
			PFD_TYPE_RGBA,         // RGBA type  
			24,                    // 24-bit color depth  
			0, 0, 0, 0, 0, 0,      // color bits ignored  
			8,                     // no alpha buffer  
			0,                     // shift bit ignored  
			0,                     // no accumulation buffer  
			0, 0, 0, 0,            // accum bits ignored  
			32,                    // 32-bit z-buffer      
			0,                     // no stencil buffer  
			0,                     // no auxiliary buffer  
			PFD_MAIN_PLANE,        // main layer  
			0,                     // reserved  
			0, 0, 0                // layer masks ignored  
			}; 

		m_hDC = ::GetDC(m_hWnd);
		int iFormat = ::ChoosePixelFormat(m_hDC, &pfd);
		::SetPixelFormat(m_hDC, iFormat, &pfd);

		m_glContext = ::wglCreateContext(m_hDC);
		::wglMakeCurrent(m_hDC, m_glContext);
	}

	LRESULT CALLBACK GLWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lResult = 0;
		GLWindow *This = reinterpret_cast<GLWindow *>(::GetWindowLongPtr(hWnd, GWL_USERDATA));

		switch (uMsg)
		{
		case WM_CREATE:
			This = (GLWindow *)((LPCREATESTRUCT)lParam)->lpCreateParams;
			::SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)This);
			break;
		case WM_CHAR:
			This->onKey((char)wParam);
			break;
		case WM_PAINT:
			This->onPaint();
			::ValidateRect(hWnd, NULL);
			break;
		case WM_SIZE:
			This->onResize(LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_DESTROY:
			::PostQuitMessage(0);
			break;
		default:
			lResult = ::DefWindowProc(hWnd, uMsg, wParam, lParam);
		}

		return lResult;
	}

}}