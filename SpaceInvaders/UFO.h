#pragma once
#include "Actor.h"
#include "CircleCollisionComponent.h"
#include "MoveComponent.h"

class UFO :
    public Actor
{
public:
    UFO();

    CircleCollisionComponent& getCollision() { return *collision; }
    MoveComponent& getMove() { return *move; }

private:
    CircleCollisionComponent* collision;
    MoveComponent* move;
};