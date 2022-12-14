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

    CircleCollisionComponent& getCollision() { return *collision; }

private:
    Laser* laser = nullptr;
    CircleCollisionComponent* collision;
};

