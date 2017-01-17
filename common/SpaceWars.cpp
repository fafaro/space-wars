#include "StdAfx.h"
#include "SpaceWars.h"
#include <FS/FMath/Vec3.h>

namespace SpaceWars
{
	using namespace FS;
	using FMath::Vec3;

	void Entity::wrapPosBounds()
	{
		Vec3 bmin, bmax;
		getGame()->getWorldBounds(bmin, bmax);

		for (int i = 0; i < 3; i++)
		{
			if      (m_pos.p[i] < bmin.p[i]) m_pos.p[i] = bmax.p[i];
			else if (m_pos.p[i] > bmax.p[i]) m_pos.p[i] = bmin.p[i];
		}
	}

	bool Entity::insideBounds()
	{
		Vec3 bmin, bmax;
		getGame()->getWorldBounds(bmin, bmax);

		for (int i = 0; i < 3; i++)
		{
			if (m_pos.p[i] < bmin.p[i]) return false;
			if (m_pos.p[i] > bmax.p[i]) return false;
		}
		return true;
	}

	void Entity::pushTransform()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		GLfloat data[16];
		getTransform().getData(data);
		glMultMatrixf(data);
	}

	//===================================================================
	// StaticPlaneEntity
	//===================================================================
	StaticPlaneEntity::StaticPlaneEntity(SpaceWars::Game *game)
		: Entity(game)
	{
	}

	void StaticPlaneEntity::render(void *ren)
	{
		float size = 10;

		FS::F3D::TexMgr::instance()->bind(m_texId);

		this->pushTransform();

		glDisable(GL_BLEND);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(-size, -size, 0);
		glTexCoord2f(1, 0); glVertex3f( size, -size, 0);
		glTexCoord2f(1, 1); glVertex3f( size,  size, 0);
		glTexCoord2f(0, 1); glVertex3f(-size,  size, 0);
		glEnd();
		glEnable(GL_BLEND);

		glPopMatrix();
	}

	//===================================================================
	// Bullet
	//===================================================================
	Bullet::Bullet(Game *game)
		: Entity(game)
	{
		setTimeToLive(2);
		setCollRadius(0.2 / 2);
	}

	void Bullet::step(FS::Real interval)
	{
		Entity::step(interval);

		//wrapPosBounds();
		if (!insideBounds())
			getGame()->postRemoveEntity(this);
	}

	void Bullet::render(void *pren)
	{
		pushTransform();
		FS::F3D::TexMgr::instance()->bind(m_texId);

		float size = getCollRadius();

		glBegin(GL_QUADS);
		glTexCoord2d(0, 0); glVertex3f(-size, -size, 0);
		glTexCoord2d(1, 0); glVertex3f( size, -size, 0);
		glTexCoord2d(1, 1); glVertex3f( size,  size, 0);
		glTexCoord2d(0, 1); glVertex3f(-size,  size, 0);
		glEnd();

		glPopMatrix();
	}

	//===================================================================
	// Stone
	//===================================================================
	Stone::Stone(Game *game)
		: Entity(game)
	{
		setLife(30);
		m_angVel = FS::FMath::Quat::quatFromRotation(90, 3, 1, 0.5);
	}

	void Stone::step(FS::Real interval)
	{
		Entity::step(interval);

		m_orient *= m_angVel.exponent(interval);

		wrapPosBounds();
	}

	void Stone::render(void *pren)
	{
		pushTransform();
		FS::F3D::TexMgr::instance()->bind(m_texId);

		FS::F3D::MeshGL::render(m_mesh);

		glPopMatrix();
	}

	//===================================================================
	// SpaceShip
	//===================================================================
	SpaceShip::SpaceShip(Game *game)
		: Entity(game),
		  m_shootPhase(0),
		  m_shootGun(0),
		  m_bulletInterval(0.2)
	{
		//m_angVel.rotate(FS::FMath::degtorad(45), 0,0, 1);
	}

	void SpaceShip::step(FS::Real time)
	{
		FS::Real v = m_vel.length();
		FS::Real newv = v - 1 * time;
		if (newv <= 0) m_vel = Vec3(0);
		else m_vel = m_vel * newv / v;

		m_pos += m_vel * time;
		m_shootPhase += time;
		//m_orient *= m_angVel.exponent(time);
		wrapPosBounds();
	}

	void SpaceShip::render(void *pren)
	{
		FS::F3D::TexMgr *texMgr = FS::F3D::TexMgr::instance();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		GLfloat data[16];
		getTransform().getData(data);
		glMultMatrixf(data);

		if (0)
		{
			texMgr->bind(m_texId);

			glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex3f(-0.5, -0.5, 0);
			glTexCoord2d(1, 0); glVertex3f( 0.5, -0.5, 0);
			glTexCoord2d(1, 1); glVertex3f( 0.5,  0.5, 0);
			glTexCoord2d(0, 1); glVertex3f(-0.5,  0.5, 0);
			glEnd();
		}

		//if (0)
		if (m_mesh) 
		{
			//glEnable(GL_LIGHTING);
			//glEnable(GL_LIGHT0);
			texMgr->bind(m_texId);
			FS::F3D::MeshGL::render(m_mesh);
			//glDisable(GL_LIGHTING);
		}

		glPopMatrix();
	}

	void SpaceShip::accelForward(FS::Real amt)
	{
		m_vel += this->getTransform().getXAxis() * amt;
		m_vel = m_vel.capLength(6.0);
	}

	void SpaceShip::turnLeft(FS::Real degs)
	{
		m_orient *= FS::FMath::Quat::quatFromRotation(FS::FMath::degtorad(degs), 0, 0, 1);
	}

	void SpaceShip::shoot()
	{
		if ( m_shootPhase < 0 ) return;

		Vec3 p;
		switch (m_shootGun)
		{
		case 0:
			p = Vec3(0.2, -0.3, 0);
			break;
		case 1:
			p = Vec3(0.2,  0.3, 0);
			break;
		}
		getGame()->createBullet(getTransform().transform(p), getTransform().getXAxis() * 12 /*+ m_vel*/);
		m_shootPhase = -m_bulletInterval; // 0.2 second/bullet
		m_shootGun = (m_shootGun + 1) % 2;
	}

	//===================================================================
	// Game
	//===================================================================
	Game::Game()
	{
		addTimer(2, 0);
	}

	void Game::init()
	{
		FS::F3D::TexMgr *texMgr = FS::F3D::TexMgr::instance();

		texMgr->load(TEXTURE_PATH "spaceship01.bmp", "SPACESHIP01");
		texMgr->load(TEXTURE_PATH "spaceship02.bmp", "SPACESHIP02");
		texMgr->load(TEXTURE_PATH "stars2.bmp"     , "BACKGROUND");
		texMgr->load(TEXTURE_PATH "bullet.bmp"    ,  "BULLET");
		texMgr->load(TEXTURE_PATH "spaceship-texture-red.bmp", "SPACESHIP-TEXTURE-RED");
		texMgr->load(TEXTURE_PATH "spaceship-texture-blue.bmp", "SPACESHIP-TEXTURE-BLUE");
		texMgr->load(TEXTURE_PATH "stone.bmp", "STONE-TEXTURE");
		//texMgr->load(TEXTURE_PATH "test.bmp", "SPACESHIP-TEXTURE");

		FS::F3D::MeshMgr *meshMgr = FS::F3D::MeshMgr::instance();
		meshMgr->load(MESH_PATH "SpaceShip.fmdl", "SPACESHIP");
		meshMgr->load(MESH_PATH "Stone.fmdl", "STONE");

		FS::F3D::Mesh *mesh;
		
		mesh = meshMgr->getMesh("SPACESHIP");
		mesh->scale(0.06);
		mesh->rotate(90, 0, 0, 1);

		mesh= meshMgr->getMesh("STONE");
		mesh->scale(0.5);
		mesh->translateToCentroid();

		StaticPlaneEntity *p = new StaticPlaneEntity(this);
		p->setTexture(texMgr->getTexId("BACKGROUND"));
		p->setPosition(Vec3(0, 0, -1));
		this->addEntity(p);

		m_spaceShip[0] = new SpaceShip(this);
		m_spaceShip[1] = new SpaceShip(this);
		this->addEntity(m_spaceShip[0]);
		this->addEntity(m_spaceShip[1]);

		F3D::Mesh *ssMesh = meshMgr->getMesh("SPACESHIP");

		m_spaceShip[0]->setPosition(Vec3(-2, 0.1, 0.15));
		m_spaceShip[0]->setTexture(texMgr->getTexId("SPACESHIP-TEXTURE-RED"));
		m_spaceShip[0]->setMesh(ssMesh);
		m_spaceShip[0]->setCollRadius(ssMesh->boundingRadius());

		m_spaceShip[1]->setPosition(Vec3( 2, 0.1, 0.1));
		m_spaceShip[1]->setTexture(texMgr->getTexId("SPACESHIP-TEXTURE-BLUE"));
		m_spaceShip[1]->setMesh(ssMesh);
		m_spaceShip[1]->setCollRadius(ssMesh->boundingRadius());

		createStone(Vec3(0, 0, 0), Vec3(1, 2, 0));
	}

	void Game::inputKeyboard(const FS::Keyboard & kb)
	{
		if (kb.keyDown(VK_UP)   ) m_spaceShip[0]->accelForward( 15 * kb.getInterval());
		if (kb.keyDown(VK_DOWN) ) m_spaceShip[0]->accelForward(-15 * kb.getInterval());
		if (kb.keyDown(VK_LEFT) ) m_spaceShip[0]->turnLeft( 250 * kb.getInterval());
		if (kb.keyDown(VK_RIGHT)) m_spaceShip[0]->turnLeft(-250 * kb.getInterval());
		if (kb.keyDown(VK_LCONTROL)) m_spaceShip[0]->shoot();
	}

	void Game::step(FS::Real time)
	{
		FS::FGame::Game::step(time);

		typedef EntityList::iterator EntIter;
		EntityList elist = m_entityList; // Make a copy, becuz m_entityList...
		                                 // can change during processing, hence work on copy

		for (EntIter i1 = elist.begin(); i1 < elist.end() - 1; i1++)
		{
			for (EntIter i2 = i1 + 1; i2 < elist.end(); i2++)
			{
				checkCollision((Entity*)*i1, (Entity*)*i2);
			}
		}

		if (0)
		for (int i = 0; i < 1; i++)
		{
			SpaceShip & ss = *m_spaceShip[i];

			Vec3 pos = ss.getPosition() - Vec3(0, 0, ss.getPosition().z);
			FS::Real dist = pos.length();
			//if (FS::FMath::equalsZero(dist)) continue;
			if (dist < 1) continue;
			Vec3 f = pos * (20.0 * -time / (dist * dist));
			ss.addVelocity(f);
		}
	}

	void Game::render(void *pren)
	{
		glEnable(GL_TEXTURE_2D);
		FGame::Game::render(pren);

		namespace GL = FS::F3D::GL;

		glPushAttrib(GL_LIGHTING_BIT | GL_TEXTURE_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_LINES);
		GL::color(1, 0, 0); GL::vertex(0, 0, 0.5); GL::vertex(1, 0, 0.5);
		GL::color(0, 1, 0); GL::vertex(0, 0, 0.5); GL::vertex(0, 1, 0.5);
		GL::color(0, 0, 1); GL::vertex(0, 0, 0.5); GL::vertex(0, 0, 1.5);
		GL::color(1, 1, 1);
		glEnd();

		glPopAttrib();
	}
	
	void Game::onTimer(int id)
	{
		Vec3 bmin, bmax;
		this->getWorldBounds(bmin, bmax);
		Vec3 pos = Vec3::random(bmin, bmax);
		Vec3 vel = Vec3::random(3);
		pos.z = vel.z = 0;
		createStone(pos, vel);
	}

	void Game::createBullet(const Vec3 & pos, const Vec3 & vel)
	{
		Bullet *b = new Bullet(this);
		addEntity(b);
		b->setPosition(pos);
		b->setVelocity(vel);
		b->setTexture(FS::F3D::TexMgr::instance()->getTexId("BULLET"));
	}

	void Game::createStone(const FS::FMath::Vec3 & pos, const FS::FMath::Vec3 & vel)
	{
		Stone *s = new Stone(this);
		F3D::Mesh *mesh = F3D::MeshMgr::instance()->getMesh("STONE");
		addEntity(s);
		s->setPosition(pos);
		s->setVelocity(vel);
		s->setTexture(FS::F3D::TexMgr::instance()->getTexId("STONE-TEXTURE"));
		s->setMesh(mesh);
		s->setCollRadius(mesh->boundingRadius());
	}

	void Game::getWorldBounds(Vec3 & bmin, Vec3 & bmax)
	{
		float size = 10;
		bmin = Vec3(-size, -size, -size);
		bmax = Vec3( size,  size,  size);
	}

	void Game::checkCollision(Entity *e1, Entity *e2)
	{
		EntityType::Type etype1 = e1->getEntityType();
		EntityType::Type etype2 = e2->getEntityType();

		bool doSwap = false;

		Real dist = Vec3::distance(e1->getPosition(), e2->getPosition());
		if (dist > e1->getCollRadius() + e2->getCollRadius()) return;

		if ( (etype1 == EntityType::Bullet && etype2 == EntityType::Stone) ||
			 (doSwap = true, etype2 == EntityType::Bullet && etype1 == EntityType::Stone) )
		{
			if (doSwap) FS::swap(e1, e2);

			Bullet *bullet = (Bullet *)e1;
			Stone  *stone  = (Stone *)e2;

			stone->addDamage(10);
			postRemoveEntity(bullet);
		}
	}
}