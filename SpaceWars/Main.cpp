#include "StdAfx.h"
#include "Resource.h"
#include "FS/Timer.h"
#include "FS/FMath/Math.h"
#include <FS/F3D/GL.h>
#include <FS/F3D/GLWindow.h>
#include <FS/F3D/Camera.h>
#include <FS/FGame/Game.h>
#include <FS/FGame/KeyScanner.h>
#include "SpaceWars.h"

using FS::FMath::Vec3;

class MyGLWindow : public FS::F3D::GLWindow, public FS::F3D::ICameraWindow, public FS::FGame::KeyScanner::IListener
{
private:
	SpaceWars::Game          *m_game;
	FS::F3D::Camera           m_camera;
	FS::F3D::CameraController m_camCtrl;
	FS::Timer                 m_timer;
	float                     m_timeScale;
	FS::FGame::KeyScanner     m_keyScanner;

public:
	MyGLWindow(SpaceWars::Game *game)
		: GLWindow("Space Wars", 160, 120, false), 
		  m_game(game),
		  m_timeScale(1.0)
	{
		setIcon(IDI_ICON1);
		initGL();

		m_camCtrl.init(&m_camera, this);
		//m_camCtrl.setEnabled(false);
		m_camera.setPosition(Vec3(0, -1, 20));
		m_camera.lookAt(Vec3(0, 0, 0));

		m_keyScanner.addCode("ak47", this, 0);
		m_keyScanner.addCode("stardust", this, 1);
	}

	virtual void initGL()
	{
		GLWindow::initGL();

		glMatrixMode(GL_PROJECTION);
		gluPerspective(60, 4.0 / 3.0, 1.0, 1000.0);
		glMatrixMode(GL_MODELVIEW);
		gluLookAt(0, -3, 1.5, 0, 0, 0, 0, 0, 1);

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendFunc(GL_SRC_ALPHA, GL_ZERO);
		glAlphaFunc(GL_GEQUAL, 0.5);
		glEnable(GL_ALPHA_TEST);

		glEnable(GL_CULL_FACE);
	}


	virtual void render()
	{
		m_camera.updateGLMatrix();

		glClearColor(0.1f, 0.1f, 0.5f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glColor4f(1, 1, 1, 1);
		//glDisable(GL_TEXTURE_2D); glDisable(GL_BLEND);
		//FS::F3D::MyGL::drawGrid(10, 1.0);
		//glEnable(GL_TEXTURE_2D); glEnable(GL_BLEND);
		m_game->render(0);

		swapBuffers();
	}

protected:
	virtual void onResize(int w, int h)
	{
		m_camera.setAspectRatio((float)w / h);
		glViewport(0, 0, w, h);
	}

	virtual void onIdle()
	{
		m_timer.tick();

		FS::Keyboard keyboard;
		keyboard.update(m_timer.getInterval() * m_timeScale);

		// input
		m_camCtrl.inputKeyboard(m_timer.getInterval());
		m_camCtrl.inputMouse(m_timer.getInterval());
		m_game->inputKeyboard(keyboard);

		// follow cam
		//m_camera.setPosition(m_game->getSpaceShip(0)->getTransform().transform(Vec3(-5, 0, 3)));
		//m_camera.setPosition(m_game->getSpaceShip(0)->getPosition() + Vec3(0, -5, 3));
		//m_camera.lookAt(m_game->getSpaceShip(0)->getPosition());

		// step
		m_game->step(m_timer.getInterval() * m_timeScale);

		// display
		render();

		::Sleep(10);
	}

	virtual void onKey(char c)
	{
		m_keyScanner.inputKey(c);

		if (m_camCtrl.inputChar(c))
		{
		}
		else if (c == '+')
		{
			m_timeScale *= 2;
		}
		else if (c == '-')
		{
			m_timeScale /= 2;
		}
		else
		{
			GLWindow::onKey(c);
		}
	}
	
	// ICameraWindow Implementation
public:
	virtual void resetMousePos(int & x, int & y)
	{
		ICameraWindow::utilResetMousePos(m_hWnd, x, y);
	}

	virtual void showCursor(bool visible)
	{
		ICameraWindow::utilShowCursor(visible);
	}

	// KeyScanner::IL6istener Implementation
protected:
	virtual void onKeySequence(const char *code, int id)
	{
		switch (id)
		{
		case 0: // AK47
			m_game->getSpaceShip(0)->setBulletInterval(0.02);
			break;

		case 1: // Asteroid Shower
			{
				Vec3 bmin, bmax;
				m_game->getWorldBounds(bmin, bmax);

				for (int i = 0; i < 10; i++) 
				{
					Vec3 vel = Vec3::random(3);
					Vec3 pos = Vec3::random(bmin, bmax);
					vel.z = 0;
					pos.z = 0;
					m_game->createStone(pos, vel);
				}
			}
			break;
		}
	}
};

int main()
{
	SpaceWars::Game game;
	MyGLWindow win(&game);

	game.init();

	return win.run();
}