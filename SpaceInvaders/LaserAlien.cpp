#include "LaserAlien.h"
#include "SpriteComponent.h"
#include "Assets.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Alien.h"
#include "Ship.h"

LaserAlien::LaserAlien() :
	Actor(),
	deathTimer(1.2f),
	collision(nullptr)
{
	MoveComponent* mc = new MoveComponent(this);

	new SpriteComponent(this, Assets::getTexture("LaserAlien"));
	mc->setForwardSpeed(-400.0f);

	collision = new CircleCollisionComponent(this);
	collision->setRadius(11.0f);
}

void LaserAlien::updateActor(float dt)
{
	deathTimer -= dt;
	if (deathTimer <= 0.0f || getPosition().y >= WINDOW_HEIGHT-20)
	{
		setState(Actor::ActorState::Dead);
	}
	else
	{
		Ship* ship;
		ship = getGame().getShipActive();
		if (Intersect(*collision, ship->getCollision()))
		{
			setState(ActorState::Dead);
			ship->setState(ActorState::Dead);
			getGame().shipDestroy();
		}
	}
}