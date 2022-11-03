#include "Alien.h"
#include "SpriteComponent.h"
#include "Assets.h"
#include "Game.h"
#include "LaserAlien.h"

Alien::Alien() : 
	Actor()
{
	SpriteComponent* sc = new SpriteComponent(this, Assets::getTexture("Alien"));

	collision = new CircleCollisionComponent(this);
	collision->setRadius(10.0f);

	move = new MoveComponent(this);
	move->setSideSpeed(-25.0f);
}

Alien::~Alien()
{
	getGame().removeAlien(this);
}

void Alien::Shot()
{
	LaserAlien* laser = new LaserAlien();
	laser->setPosition(getPosition());
	laser->setRotation(getRotation());
}
