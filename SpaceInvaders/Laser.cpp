#include "Laser.h"
#include "SpriteComponent.h"
#include "Assets.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Alien.h"
#include "Ship.h"

Laser::Laser() :
	Actor(),
	deathTimer(0.85f),
	collision(nullptr)
{	
	MoveComponent* mc = new MoveComponent(this);

	new SpriteComponent(this, Assets::getTexture("Laser"));
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
		vector<vector<Alien*>> aliens;
		aliens = getGame().getAliens();

		for (auto &alienVec : aliens)
		{
			for (auto alien : alienVec)
			{
				if (alien != nullptr && alien->getState() == Actor::ActorState::Active)
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
		if (getGame().getSaucer() != nullptr && getGame().getSaucer()->getState() == Actor::ActorState::Active)
		{
			if (Intersect(*collision, getGame().getSaucer()->getCollision()))
			{
				setState(ActorState::Dead);
				getGame().getSaucer()->setState(ActorState::Dead);
			}
		}
	}
}