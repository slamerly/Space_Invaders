#include "Shield.h"
#include "SpriteComponent.h"
#include "Assets.h"
#include "Game.h"

Shield::Shield() :
	Actor()
{
	SpriteComponent* sc = new SpriteComponent(this, Assets::getTexture("Shield"));

	collision = new CircleCollisionComponent(this);
	collision->setRadius(30.0f);
}

Shield::~Shield()
{
	getGame().removeShield(this);
}

void Shield::setLife(int lifeP)
{
	life = lifeP;
}
