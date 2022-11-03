#include "UFO.h"
#include "SpriteComponent.h"
#include "Assets.h"
#include "Game.h"

UFO::UFO() :
	Actor()
{
	SpriteComponent* sc = new SpriteComponent(this, Assets::getTexture("UFO"));

	collision = new CircleCollisionComponent(this);
	collision->setRadius(20.0f);

	move = new MoveComponent(this);
}

UFO::~UFO()
{
}