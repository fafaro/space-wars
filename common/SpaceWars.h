#pragma once

#include <FS/FMath/Math.h>
#include <FS/FGame/Game.h>
#include <FS/F3D/TexMgr.h>
#include <FS/F3D/MeshMgr.h>
#include <FS/F3D/Mesh.h>

//#define USE_PLAIN_TEXTURES

#ifdef USE_PLAIN_TEXTURES
#define TEXTURE_PATH "textures_plain/"
#else
#define TEXTURE_PATH "textures/"
#endif

#define MESH_PATH "meshes/"

namespace SpaceWars
{
	using namespace FS;
	using FMath::Vec3;

	class Game;

	class EntityType
	{
	public:
		typedef int Type;
		
		static const Type Generic     = 1;
		static const Type StaticPlane = 2;
		static const Type Bullet      = 3;
		static const Type Stone       = 4;
		static const Type SpaceShip   = 5;

	private:
		EntityType() { }
	};

	class Entity : public FGame::Entity
	{
	private:
		Real m_collRadius;

	protected:
		F3D::Mesh *m_mesh;

	public:
		Entity(Game *game)
			: FGame::Entity((FGame::Game*)game),
			  m_mesh(0),
			  m_collRadius(0.2)
		{
		}

		virtual EntityType::Type getEntityType() const { return EntityType::Generic; }

		Game *getGame() const { return (Game *)m_game; }
		void wrapPosBounds();
		bool insideBounds();
		void setMesh(F3D::Mesh *mesh) { m_mesh = mesh; }
		void pushTransform();

		Real getCollRadius() const { return m_collRadius; }
		void setCollRadius(Real rad) { m_collRadius = rad; }
	};

	//===================================================================
	// StaticPlaneEntity
	//===================================================================
	class StaticPlaneEntity : public Entity
	{
	public:
		StaticPlaneEntity(Game *game);

		virtual EntityType::Type getEntityType() const { return EntityType::StaticPlane; }
		//virtual void step(FS::Real time) { }
		virtual void render(void *ren);
	};

	//===================================================================
	// Bullet
	//===================================================================
	class Bullet : public Entity
	{
	public:
		Bullet(Game *game);

		virtual EntityType::Type getEntityType() const { return EntityType::Bullet; }

		virtual void step(Real interval);
		virtual void render(void *pren);
	};

	//===================================================================
	// Stone
	//===================================================================
	class Stone : public Entity
	{
	public:
		Stone(Game *game);

		virtual EntityType::Type getEntityType() const { return EntityType::Stone; }

		virtual void step(Real interval);
		virtual void render(void *pren);
	};

	//===================================================================
	// Spaceship
	//===================================================================
	class SpaceShip : public Entity
	{
	private:
		Real m_shootPhase;
		int  m_shootGun;
		Real m_bulletInterval;

	public:
		SpaceShip(Game *game);

		virtual EntityType::Type getEntityType() const { return EntityType::SpaceShip; }

		virtual void step(Real time);
		virtual void render(void *pren);
		void accelForward(Real amt);
		void turnLeft(Real degs);
		void shoot();
		void setBulletInterval(Real invl) { m_bulletInterval = invl; }
	};

	//===================================================================
	// SpaceWarsGame
	//===================================================================
	class Game : public FGame::Game
	{
	private:
		SpaceShip *m_spaceShip[2];

	public:
		Game();

		void init();
		SpaceShip *getSpaceShip(int index) { return m_spaceShip[index]; }
		virtual void inputKeyboard(const Keyboard & kb);
		virtual void step(Real time);
		virtual void render(void *pren);
		virtual void onTimer(int id);
		void createBullet(const Vec3 & pos, const Vec3 & vel);
		void createStone(const Vec3 & pos, const Vec3 & vel);
		void getWorldBounds(Vec3 & bmin, Vec3 & bmax);
		void checkCollision(Entity *e1, Entity *e2);
	};
}