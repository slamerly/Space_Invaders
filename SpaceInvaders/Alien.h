#pragma once
#include "Actor.h"
#include "CircleCollisionComponent.h"

class Alien :
    public Actor
{
public:
    Alien();
    ~Alien();

    CircleCollisionComponent& getCollision() { return *collision; }
    void Shoot();

private:
    CircleCollisionComponent* collision;
};

