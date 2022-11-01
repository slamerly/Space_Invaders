#pragma once
#include "Actor.h"
#include "Alien.h"

class AlienTab :
    public Actor
{
public:
    AlienTab();
    ~AlienTab();

private:
    Alien* aliens[5][11];
};

