#include "Laser.h"
#include "SpriteComponent.h"
#include "Assets.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Astroid.h"
#include "Alien.h"

Laser::Laser() :
	Actor(),
	deathTimer(0.85f),
	collision(nullptr)
{
	new SpriteComponent(this, Assets::getTexture("Laser"));
	MoveComponent* mc = new MoveComponent(this);
	mc->setForwardSpeed(800.0f);
	collision = new CircleCollisionComponent(this);
	collision->setRadius(11.0f);
}

void Laser::updateActor(float dt)
{
	deathTimer -= dt;
	if (deathTimer <= 0.0f)
	{
		setState(Actor::ActorState::Dead);
	}
	else
	{
		/*
		auto astroids = getGame().getAstroids();
		for (auto astroid : astroids)
		{
			if (Intersect(*collision, astroid->getCollision()))
			{
				setState(ActorState::Dead);
				astroid->setState(ActorState::Dead);
				break;
			}
		}
		*/
		vector<vector<Alien*>> aliens; 
		aliens = getGame().getAliens();
		for (auto alienVec : aliens)
		{
			for (auto alien : alienVec)
			{
				if (alien->getState() == Actor::ActorState::Active)
				{
					if (Intersect(*collision, alien->getCollision()))
					{
						setState(ActorState::Dead);
						alien->setState(ActorState::Dead);
						break;
					}
				}
			}
		}
		/*
		auto alien = getGame().getAlien();
		if (alien->getState() == Actor::ActorState::Active)
		{
			if (Intersect(*collision, alien->getCollision()))
			{
				setState(ActorState::Dead);
				alien->setState(ActorState::Dead);
			}
		}
		*/
	}
}