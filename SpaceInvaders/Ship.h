#pragma once
#include "Actor.h"
#include "Laser.h"
#include "CircleCollisionComponent.h"
#include <SDL_stdinc.h>

class Ship :
    public Actor
{
public:
    Ship();

    void actorInput(const Uint8* keyState) override;
    void updateActor(float dt) override;

    //CircleCollisionComponent& getCollision() { return *collision; }

private:
    //float laserCooldown;
    Laser* laser = nullptr;
    //CircleCollisionComponent* collision;
};

