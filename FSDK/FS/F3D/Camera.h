#pragma once

#include <FS/FMath/Vec2.h>
#include <FS/FMath/Vec3.h>
#include <FS/F3D/GL.h>
#include <FS/FMath/Mat4x4.h>
#include <FS/FMath/Math.h>

namespace FS { namespace F3D
{
	using namespace FMath;

	class Camera
	{
		friend class CameraController;

	private:
		Mat4x4 m_transform;
		Real m_aspectRatio;

	public:
		Camera()
			: m_aspectRatio(4.0 / 3.0)
		{
		}

		void setAspectRatio(Real a) { m_aspectRatio = a; }

		void updateGLMatrix()
		{
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(60.0, m_aspectRatio, 1.0, 1000.0);

			glMatrixMode(GL_MODELVIEW);

			GLfloat data[16];
			m_transform.getFastInverse().getData(data);
			glLoadMatrixf(data);
		}

		void lookAt(Vec3 center)
		{
			Vec3 up(0, 0, 1);
			Vec3 pos = m_transform.getTranslation();
			Vec3 r = (center - pos).normal();

			Vec3 xaxis, yaxis, zaxis;

			zaxis = -r;
			Vec3 ydir = up.removeComponent(zaxis);
			Real ydirlen = 0;
			if ( FMath::equalsZero(ydirlen = ydir.length()) )
			{
				m_transform.setIdentity();
			}
			else
			{
				yaxis = ydir / ydirlen;
				xaxis = yaxis.cross(zaxis);
				m_transform.setAxes(xaxis, yaxis, zaxis);
			}
		}

		Vec3 getPosition() const { return m_transform.getTranslation(); }
		void setPosition(const Vec3 & pos) { m_transform.setTranslation(pos); }

		void moveForward(Real amt)
		{
			Vec3 zaxis = m_transform.getZAxis();
			Vec3 pos = getPosition();
			pos += zaxis * -amt;
			m_transform.setTranslation(pos);
		}

		void moveSideways(Real amt)
		{
			Vec3 xaxis = m_transform.getXAxis();
			Vec3 pos = getPosition();
			pos += xaxis * amt;
			m_transform.setTranslation(pos);
		}

		void setTransform(const Mat4x4 & m) { m_transform = m; }
	};

	class ICameraWindow
	{
	public:
		virtual void resetMousePos(int & x, int & y) = 0;
		virtual void showCursor(bool visible) = 0;
		
		void resetMousePos()
		{
			int x, y;
			resetMousePos(x, y);
		}

		static void utilResetMousePos(HWND hWnd, int & x, int & y)
		{
			POINT pt, center;
			RECT rect;

			::GetWindowRect(hWnd, &rect);
			::GetCursorPos(&pt);

			center.x = (rect.right + rect.left) / 2;
			center.y = (rect.bottom + rect.top) / 2;

			x = pt.x - center.x;
			y = pt.y - center.y;

			::SetCursorPos(center.x, center.y);
		}

		static void utilShowCursor(bool visible)
		{
			::ShowCursor(visible);
		}
	};

	class CameraController
	{
	private:
		Camera *m_camera;
		ICameraWindow *m_camWin;
		bool m_firstPersonMode;
		Real m_yawAngle, m_pitchAngle;
		bool m_enabled;

	public:
		CameraController()
			: m_firstPersonMode(false), m_yawAngle(0), m_pitchAngle(0), m_enabled(true)
		{
		}

		void init(Camera *cam, ICameraWindow *camWin)
		{
			m_camera = cam;
			m_camWin = camWin;
			updateMatrix();
		}

		void setCamera(Camera *cam) { m_camera = cam; }
		void setWindow(ICameraWindow *camWin) { m_camWin = camWin; }
		void setEnabled(bool enabled) { m_enabled = enabled; }

		void inputKeyboard(float timeInterval)
		{
			if (!m_enabled) return;

			if (m_firstPersonMode)
			{
				BYTE keyState[256];
				::GetKeyboardState(keyState);

				float speed = 5 * timeInterval;
				if (keyState['W'] & 0x80) m_camera->moveForward( speed);
				if (keyState['S'] & 0x80) m_camera->moveForward(-speed);
				if (keyState['D'] & 0x80) m_camera->moveSideways( speed);
				if (keyState['A'] & 0x80) m_camera->moveSideways(-speed);
			}
		}

		bool inputChar(char c)
		{
			if (!m_enabled) return false;

			if (c == 'z') 
			{
				if (m_firstPersonMode)
				{
					m_firstPersonMode = false;
					m_camWin->showCursor(true);
				}
				else
				{
					m_firstPersonMode = true;
					m_camWin->showCursor(false);
					m_camWin->resetMousePos();
				}
				return true;
			}
			return false;
		}

		void inputMouse(float timeInterval)
		{
			if (!m_enabled) return;

			if (m_firstPersonMode)
			{
				int x, y;
				m_camWin->resetMousePos(x, y);

				m_yawAngle   += x * -0.005;
				m_pitchAngle += y * -0.005;

				if (m_pitchAngle > FMath::PI / 2.0)
					m_pitchAngle = FMath::PI / 2.0;
				else if (m_pitchAngle < -FMath::PI / 2.0)
					m_pitchAngle = -FMath::PI / 2.0;

				updateMatrix();
			}
		}

	private:
		void updateMatrix()
		{
			Mat4x4 m;
			m.rotateX(FMath::PI / 2 + m_pitchAngle);
			m.rotateZ(m_yawAngle);
			m.setTranslation(m_camera->m_transform.getTranslation());

			m_camera->setTransform(m);
		}
	};
}}