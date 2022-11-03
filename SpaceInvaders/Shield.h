#pragma once
#include "Actor.h"
#include "CircleCollisionComponent.h"
#include "MoveComponent.h"

class Shield :
    public Actor
{
public:
    Shield();
    ~Shield();

    CircleCollisionComponent& getCollision() { return *collision; }
    int getLife() { return life; }
    void setLife(int lifeP);

private:
    CircleCollisionComponent* collision;
    int life = 10;
};