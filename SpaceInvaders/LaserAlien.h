#pragma once
#include "Actor.h"
#include "CircleCollisionComponent.h"

class LaserAlien :
	public Actor
{
public:
	LaserAlien();

	void updateActor(float dt) override;

private:
	CircleCollisionComponent* collision;
	float deathTimer;
};