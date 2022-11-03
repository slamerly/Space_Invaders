#pragma once
#include "Actor.h"
#include "CircleCollisionComponent.h"
#include "MoveComponent.h"

class Alien :
    public Actor
{
public:
    Alien();
    ~Alien();

    CircleCollisionComponent& getCollision() { return *collision; }
    MoveComponent& getMove() { return *move; }
    void Shot();

private:
    CircleCollisionComponent* collision;
    MoveComponent* move;
};

