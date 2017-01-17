#include "stdafx.h"
#include <FS/FGame/Game.h>
#include <algorithm>

namespace FS { namespace FGame
{
	using namespace FMath;

	Entity::Entity(Game *game)
		: m_game(game), 
		  m_pos(0), 
		  m_timeToLive(10000),
		  m_life(100)
	{
	}

	const Mat4x4 & Entity::getTransform() const
	{
		// FIXME-OPTIMIZATION: Store old m_orient, and
		// update m_transform if m_orient hasn't changed
		m_transform.setOrient(m_orient);
		m_transform.setTranslation(m_pos);
		return m_transform;
	}

	void Entity::addDamage(Real amt) 
	{ 
		m_life -= amt; 
		if (m_life <= 0) m_game->postRemoveEntity(this); 
	}

	void Entity::step(Real time)
	{
		m_timeToLive -= time;
		m_pos += m_vel * time;
	}

	Game::Game()
	{
	}

	Game::~Game()
	{
		BOOST_FOREACH (Entity *e, m_entityList)
			delete e;
	}

	void Game::addEntity(Entity *ent)
	{
		m_entityList.push_back(ent);
	}

	void Game::removeEntity(Entity *ent)
	{
		EntityList & elist = m_entityList;

		EntityList::iterator newEnd = std::remove(elist.begin(), elist.end(), ent);
		if (newEnd == elist.end()) return; // the object din't exist
		elist.erase(newEnd);

		delete ent;
	}

	void Game::postRemoveEntity(Entity *ent)
	{
		m_removeEntityList.push_back(ent);
	}

	void Game::step(Real time)
	{
		EntityList eListCopy = m_entityList;
		BOOST_FOREACH (Entity *e, eListCopy)
		{
			e->step(time);
			if ( e->getTimeToLive() <= 0 ) postRemoveEntity(e);
		}

		// Call Timers
		BOOST_FOREACH (Timer & timer, m_timerList)
		{
			timer.phase += time;
			if (timer.phase > timer.interval)
			{
				timer.phase -= timer.interval;
				onTimer(timer.id);
			}
		}

		BOOST_FOREACH (Entity *e, m_removeEntityList)
			removeEntity(e);
		m_removeEntityList.clear();
	}

	void Game::render(void *pren)
	{
		BOOST_FOREACH (Entity *e, m_entityList)
			e->render(pren);
	}

	void Game::inputKeyboard(const Keyboard & keyboard)
	{
	}
	
	void Game::addTimer(Real interval, int id)
	{
		Timer t;
		t.id       = id;
		t.interval = interval;
		t.phase    = 0;
		m_timerList.push_back(t);
	}

	void Game::onTimer(int id)
	{
	}
}}