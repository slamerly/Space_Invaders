#include "Ship.h"
#include "SpriteComponent.h"
#include "Assets.h"
#include "InputComponent.h"
#include "Maths.h"
#include <iostream>

Ship::Ship() : 
	Actor()
{
	SpriteComponent* sc = new SpriteComponent(this, Assets::getTexture("Ship"));
	InputComponent* ic = new InputComponent(this);
	//ic->setMaxForwardSpeed(300.0f);
	//ic->setMaxAngularSpeed(Maths::twoPi);
	ic->setMaxSideSpeed(300.0f);

	collision = new CircleCollisionComponent(this);
	collision->setRadius(40.0f);
}

void Ship::actorInput(const Uint8* keyState)
{
	if (keyState[SDL_SCANCODE_SPACE] && ((laser == nullptr) || (laser->getState() != ActorState::Active)))
	{
		laser = new Laser();
		laser->setPosition(getPosition());
		laser->setRotation(getRotation());
		//laserCooldown = 0.0f;
	}
}

void Ship::updateActor(float dt)
{
	//laserCooldown -= dt;
}
