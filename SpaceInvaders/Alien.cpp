#include "Alien.h"
#include "SpriteComponent.h"
#include "Assets.h"

Alien::Alien() : 
	Actor()
{
	SpriteComponent* sc = new SpriteComponent(this, Assets::getTexture("Alien"));

	collision = new CircleCollisionComponent(this);
	collision->setRadius(40.0f);
}

Alien::~Alien()
{
}
