#include "LaserAlien.h"
#include "SpriteComponent.h"
#include "Assets.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Ship.h"
#include "Shield.h"

LaserAlien::LaserAlien() :
	Actor(),
	deathTimer(2.0f),
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
		if (ship != nullptr)
		{
			if (Intersect(*collision, ship->getCollision()))
			{
				setState(ActorState::Dead);
				ship->setState(ActorState::Dead);
				getGame().shipDestroy();
			}
		}
	}
	vector<Shield*> shields;
	shields = getGame().getShields();

	for (auto shield : shields)
	{
		if (shield != nullptr && shield->getState() == Actor::ActorState::Active)
		{
			if (Intersect(*collision, shield->getCollision()))
			{
				setState(ActorState::Dead);
				shield->setLife(shield->getLife() - 1);
				if (shield->getLife() <= 0)
				{
					shield->setState(ActorState::Dead);
				}
				break;
			}
		}
	}
}