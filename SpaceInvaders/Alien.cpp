#include "Alien.h"
#include "SpriteComponent.h"
#include "Assets.h"
#include "Game.h"

Alien::Alien() : 
	Actor()
{
	SpriteComponent* sc = new SpriteComponent(this, Assets::getTexture("Alien"));

	collision = new CircleCollisionComponent(this);
	collision->setRadius(10.0f);
}

Alien::~Alien()
{
	getGame().removeAlien(this);
}
