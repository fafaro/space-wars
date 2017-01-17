#pragma once

#include <vector>
#include <boost/foreach.hpp>

#include <FS/FMath/Vec3.h>
#include <FS/FMath/Quat.h>
#include <FS/FMath/Mat4x4.h>
#include <FS/Keyboard.h>

namespace FS { namespace FGame
{
	using namespace FMath;

	class Game;

	class Entity
	{
	private:
		mutable Mat4x4 m_transform;

	protected:
		Game  *m_game;
		Vec3   m_pos;
		Quat   m_orient;
		Vec3   m_vel;
		Quat   m_angVel;
		int    m_texId;
		Real   m_timeToLive;
		Real   m_life;

	public:
		Entity(Game *game);

		const Mat4x4 & getTransform() const;
		Vec3 getPosition() const { return m_pos; }
		void setPosition(const Vec3 & pos) { m_pos = pos; }
		void setVelocity(const Vec3 & vel) { m_vel = vel; }
		void addVelocity(const Vec3 & acc) { m_vel += acc; }
		void setTexture(int texId) { m_texId = texId; }
		Real getTimeToLive() const { return m_timeToLive; }
		void setTimeToLive(Real ttl) { m_timeToLive = ttl; }

		Real getLife() const { return m_life; }
		void setLife(Real life) { m_life = life; }
		void addDamage(Real amt);

		virtual void step(Real time);
		virtual void render(void *ren) { }
	};

	class Game
	{
	public:
		typedef std::vector<Entity *> EntityList;

	protected:
		EntityList m_entityList;

	public:
		Game();
		~Game();

		void addEntity(Entity *ent);
		void removeEntity(Entity *ent);
		void postRemoveEntity(Entity *ent);
		void step(Real time);
		virtual void render(void *pren);
		virtual void inputKeyboard(const Keyboard & keyboard);

		// Timer ---------------------------------
	private:
		struct Timer { Real phase, interval; int id; };
		typedef std::vector<Timer> TimerList;

		TimerList m_timerList;
	public:
		void addTimer(Real interval, int id);
		virtual void onTimer(int id);



	private:
		EntityList m_removeEntityList;
	};
}}