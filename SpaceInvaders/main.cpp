#include <iostream>
#include <SDL.h>
#include <SDL_rect.h>
#include "Game.h"

int main(int argc, char** argv)
{
    // auto : compilateur deduit tout seul le type de variable
    auto& instance = Game::instance();
    bool isGameInit = instance.initialize();
    if (isGameInit)
    {
        instance.load();
        instance.loop();
        instance.unload();
    }
    instance.close();

    return 0;
}
