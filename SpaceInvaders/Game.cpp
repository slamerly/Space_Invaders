#include "Game.h"
#include "Timer.h"
#include "Assets.h"
#include "AnimSpriteComponent.h"
#include "BackgroundSpriteComponent.h"
#include "Astroid.h"
#include "Ship.h"
#include <iostream>

bool Game::initialize()
{
    bool isWindowInit = window.initialize();
    bool isRenderInit = renderer.initialize(window);
    return isWindowInit && isRenderInit;
}

void Game::load()
{
    // Load textures
    Assets::loadTexture(renderer, "Res\\Ship01.png", "Ship01");
    Assets::loadTexture(renderer, "Res\\Ship02.png", "Ship02");
    Assets::loadTexture(renderer, "Res\\Ship03.png", "Ship03");
    Assets::loadTexture(renderer, "Res\\Ship04.png", "Ship04");
    Assets::loadTexture(renderer, "Res\\Farback01.png", "Farback01");
    Assets::loadTexture(renderer, "Res\\Farback02.png", "Farback02");
    Assets::loadTexture(renderer, "Res\\Stars.png", "Stars");
    Assets::loadTexture(renderer, "Res\\Astroid.png", "Astroid");
    Assets::loadTexture(renderer, "Res\\Ship01-SpaceInvaders.png", "Ship");
    Assets::loadTexture(renderer, "Res\\Laser.png", "Laser");
    Assets::loadTexture(renderer, "Res\\Alien.png", "Alien");

    // Single sprite
    /*
    Actor* actor = new Actor();
    SpriteComponent* sprite = new SpriteComponent(actor, Assets::getTexture("Ship01"));
    actor->setPosition(Vector2{ 100, 100 });
    */

    // Animated sprite
    /*
    vector<Texture*> animTextures{
        &Assets::getTexture("Ship01"),
        &Assets::getTexture("Ship02"),
        &Assets::getTexture("Ship03"),
        &Assets::getTexture("Ship04"),
    };
    Actor* ship = new Actor();
    AnimSpriteComponent* animatedSprite = new AnimSpriteComponent(ship, animTextures);
    ship->setPosition(Vector2{ 100, 300 });
    */

    // Controlled ship
    Ship* ship = new Ship();
    ship->setPosition(Vector2{ 512, 700 });

    // Background
    // Create the "far back" background
    vector<Texture*> bgTexsFar{
        &Assets::getTexture("Farback01"),
        &Assets::getTexture("Farback02")
    };
    Actor* bgFar = new Actor();
    BackgroundSpriteComponent* bgSpritesFar = new BackgroundSpriteComponent(bgFar, bgTexsFar);
    //bgSpritesFar->setScrollSpeed(-100.0f);

    // Create the closer background
    Actor* bgClose = new Actor();
    std::vector<Texture*> bgTexsClose{
        &Assets::getTexture("Stars"),
        &Assets::getTexture("Stars")
    };
    BackgroundSpriteComponent* bgSpritesClose = new BackgroundSpriteComponent(bgClose, bgTexsClose, 50);
    //bgSpritesClose->setScrollSpeed(-200.0f);

    /*
    const int astroidNumber = 20;
    for (int i = 0; i < astroidNumber; ++i)
    {
        new Astroid();
    }
    */


    // ALIEN TAB
    float initY = 100;
    for (int li = 0; li < 5; li++)
    {
        float initX = 192;
        vector<Alien*> vectorTempAlien;
        for (int co = 0; co < 11; co++)
        {
            Alien* tempAlien = new Alien();
            tempAlien->setPosition({ initX, initY });
            //aliens[li][co] = new Alien();
            //aliens[li][co]->setPosition({ initX, initY });
            vectorTempAlien.emplace_back(tempAlien);
            initX += 64;
            /*if (li == 4)
            {
                aliensShooters.emplace_back(tempAlien);
            }
            */
        }
        aliens.emplace_back(vectorTempAlien);
        initY += 48;
    }
    aliensShooters = aliens[4];
    /*
    Alien* alienTemp = new Alien();
    alienTemp->setPosition({ 192, 100 });
    alien = alienTemp;
    */
}

void Game::loop()
{
    Timer timer;
    float dt = 0;
    while (isRunning)
    {
        float dt = timer.computeDeltaTime() / 1000.0f;
        processInput();
        update(dt);
        render();
        timer.delayTime();
    }
}

void Game::unload()
{
    // Delete actors
    // Because ~Actor calls RemoveActor, have to use a different style loop
    while (!actors.empty())
    {
        delete actors.back();
    }

    // Resources
    Assets::clear();
}

void Game::close()
{
    renderer.close();
    window.close();
    SDL_Quit();
}

void Game::addActor(Actor* actor)
{
    if (isUpdatingActors)
    {
        pendingActors.emplace_back(actor);
    }
    else
    {
        actors.emplace_back(actor);
    }
}

void Game::removeActor(Actor* actor)
{
    // Erase actor from the two vectors
    auto iter = std::find(begin(pendingActors), end(pendingActors), actor);
    if (iter != end(pendingActors))
    {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, end(pendingActors) - 1);
        pendingActors.pop_back();
    }
    iter = std::find(begin(actors), end(actors), actor);
    if (iter != end(actors))
    {
        std::iter_swap(iter, end(actors) - 1);
        actors.pop_back();
    }
}

vector<Astroid*>& Game::getAstroids()
{
    return astroids;
}

void Game::addAstroid(Astroid* astroid)
{
    astroids.emplace_back(astroid);
}

void Game::removeAstroid(Astroid* astroid)
{
    auto iter = std::find(begin(astroids), end(astroids), astroid);
    if (iter != astroids.end())
    {
        astroids.erase(iter);
    }
}

void Game::removeAlien(Alien* alienTarget)
{
    /*
    for (auto alienVec : aliens)
    {
        for (auto alien : alienVec)
        {
            if (alien == alienTarget)
            {
                alien = nullptr;
                break;
            }
        }
    }
    */
    
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            if (aliensShooters[j] == alienTarget)
            {
                int line = i - 1;
                for (;line >= 0; line--)
                {
                    if (aliens[line][j] != nullptr)
                    {
                        break;
                    }
                }

                if (line == -1)
                {
                    /*
                    auto iter = std::find(begin(aliensShooters), end(aliensShooters), alienTarget);
                    if (iter != aliensShooters.end())
                    {
                        aliensShooters.erase(iter);
                    }
                    */
                    //aliensShooters.erase(aliensShooters.begin()+j-1);
                    aliensShooters[j] = nullptr;
                }
                else
                {
                    aliensShooters[j] = aliens[line][j];
                }
            }
            if (aliens[i][j] == alienTarget)
            {
                aliens[i][j] = nullptr;
                break;
            }
        }
    }
    /*
    for (auto alienVec : aliens)
    {
        auto iter = std::find(begin(alienVec), end(alienVec), alienTarget);
        if (iter != alienVec.end())
        {
            alienVec.erase(iter);
            break;
        }
    }
    */
}

void Game::processInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            isRunning = false;
            break;
        }
    }

    // Keyboard status
    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

    if (keyboardState[SDL_SCANCODE_ESCAPE])
    {
        isRunning = false;
    }

    // Actor input
    isUpdatingActors = true;
    for (auto actor : actors)
    {
        actor->processInput(keyboardState);
    }
    isUpdatingActors = false;
}

void Game::update(float dt)
{
    // Update actors 
    isUpdatingActors = true;
    for (auto actor : actors)
    {
        actor->update(dt);
    }
    isUpdatingActors = false;

    //aliensShoot(dt);

    // Move pending actors to actors
    for (auto pendingActor : pendingActors)
    {
        actors.emplace_back(pendingActor);
    }
    pendingActors.clear();

    // Delete dead actors
    vector<Actor*> deadActors;
    for (auto actor : actors)
    {
        if (actor->getState() == Actor::ActorState::Dead)
        {
            deadActors.emplace_back(actor);
        }
    }
    for (auto deadActor : deadActors)
    {
        delete deadActor;
    }
}

void Game::render()
{
    renderer.beginDraw();
    renderer.draw();
    renderer.endDraw();
}

void Game::aliensShoot(float dt)
{
    if (delayShot <= 0)
    {
        int shooter = 0;
        do {
            shooter = rand() % 12;
        } while (aliensShooters[shooter] == nullptr);

        aliensShooters[shooter]->Shoot();

        delayShot = static_cast<float> (rand() % 3 + 1);
    }
    else
    {
        delayShot -= dt;
    }
}
