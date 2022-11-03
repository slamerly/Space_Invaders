#include "Game.h"
#include "Timer.h"
#include "Assets.h"
#include "AnimSpriteComponent.h"
#include "BackgroundSpriteComponent.h"
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
    Assets::loadTexture(renderer, "Res\\Farback01.png", "Farback01");
    Assets::loadTexture(renderer, "Res\\Farback02.png", "Farback02");
    Assets::loadTexture(renderer, "Res\\Stars.png", "Stars");
    Assets::loadTexture(renderer, "Res\\Ship01-SpaceInvaders.png", "Ship");
    Assets::loadTexture(renderer, "Res\\Laser.png", "Laser");
    Assets::loadTexture(renderer, "Res\\Alien_Laser.png", "LaserAlien");
    Assets::loadTexture(renderer, "Res\\Alien.png", "Alien");
    Assets::loadTexture(renderer, "Res\\UFO.png", "UFO");
    Assets::loadTexture(renderer, "Res\\Shield.png", "Shield");

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
    shipActive = new Ship();

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

    srand(time(NULL));

    // ALIEN TAB
    alienLoad();
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

    aliensShot(dt);
    aliensMovement();

    ufoPassage(dt);

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

    if (nbAliens <= 0 || shieldTouched)
    {
        alienReLoad();
        shieldTouched = false;
    }
}

void Game::render()
{
    renderer.beginDraw();
    renderer.draw();
    renderer.endDraw();
}

void Game::removeAlien(Alien* alienTarget)
{
    bool finish = false;
    nbAliens--;

    // Remove alientTarget
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            if (aliens[i][j] == alienTarget)
            {
                aliens[i][j] = nullptr;
                finish = true;
                break;
            }
        }
        if (finish)
        {
            finish = false;
            break;
        }
    }

    // If alientTarget is a Shooter, then replace the shooter
    for (int j = 0; j < 11; j++)
    {
        if (aliensShooters[j] == alienTarget)
        {
            aliensShooters[j] = nullptr;

            for (int line = 4; line >= 0; line--)
            {
                if (aliens[line][j] != nullptr)
                {
                    aliensShooters[j] = aliens[line][j];
                    break;
                }
            }
            if (aliensShooters[j] != nullptr)
                break;
        }
    }

    // If alientTarget is the leftmosht, the replace it
    if (alienTarget == alienLeft)
    {
        int li = 0;
        for (int co = 0; co < 11; co++)
        {
            for (li = 4; li >= 0; li--)
            {
                //std::cout << "test :" << li << ", " << co << std::endl;
                if (aliens[li][co] != nullptr)
                {
                    alienLeft = aliens[li][co];
                    //std::cout << li << ", " << co << std::endl;
                    break;
                }
            }
            if (alienLeft != alienTarget)
                break;
            else
                li = 4;
        }
    }

    // If alientTarget is the rightmosht, the replace it
    if (alienTarget == alienRight)
    {
        int li = 4;
        for (int co = 10; co >= 0; co--)
        {
            for (li = 4; li >= 0; li--)
            {
                if (aliens[li][co] != nullptr)
                {
                    alienRight = aliens[li][co];
                    break;
                }
            }
            if (alienRight != alienTarget)
                break;
            else
                li = 4;
        }

    }
}

void Game::removeShield(Shield* shieldTarget)
{
    for (int i = 0; i < shields.size(); i++)
    {
        if (shields[i] == shieldTarget)
            shields[i] = nullptr;
    }
}

void Game::shipDestroy()
{

    lifeCount--;
    if (lifeCount <= 0)
    {
        alienReLoad();
    }
    shipActive = new Ship();
}

void Game::aliensShot(float dt)
{
    if (delayShot <= 0)
    {
        int shooter = 0;
        do {
            shooter = rand() % 11;
        } while (aliensShooters[shooter] == nullptr);

        aliensShooters[shooter]->Shot();

        delayShot = (rand() % 30) * 0.1f;
    }
    else
    {
        delayShot -= dt;
    }
}

void Game::aliensMovement()
{
    //std::cout << alienLeft->getPosition().x << ", " << alienLeft->getPosition().y << std::endl;

    //std::cout << nbAliens << std::endl;
    //std::cout << aliens[4][0]->getMove().getSideSpeed() << std::endl;

    // Acceleration with the number of Aliens alive
    switch (nbAliens)
    {
    case 27:
        for (auto &alienVec : aliens)
        {
            for (auto alien : alienVec)
            {
                if (alien != nullptr && (alien->getMove().getSideSpeed() == 25 || alien->getMove().getSideSpeed() == -25))
                {
                    alien->getMove().setSideSpeed(alien->getMove().getSideSpeed() * 2);
                }
            }
        }
        break;
    case 13:
        for (auto &alienVec : aliens)
        {
            for (auto alien : alienVec)
            {
                if (alien != nullptr && (alien->getMove().getSideSpeed() == 50 || alien->getMove().getSideSpeed() == -50))
                {
                    alien->getMove().setSideSpeed(alien->getMove().getSideSpeed() * 2);
                }
            }
        }
        break;
    case 6:
        for (auto &alienVec : aliens)
        {
            for (auto alien : alienVec)
            {
                if (alien != nullptr && (alien->getMove().getSideSpeed() == 100 || alien->getMove().getSideSpeed() == -100))
                {
                    alien->getMove().setSideSpeed(alien->getMove().getSideSpeed() * 2);
                }
            }
        }
        break;
    default:
        break;
    }

    // Scroll down aliens
    if ( !scrolldown && ((alienLeft->getPosition().x <= 55) || (alienRight->getPosition().x >= WINDOW_WIDTH - 55)))
    {
        for (auto& alienVec : aliens)
        {
            for (auto alien : alienVec)
            {
                if (alien != nullptr)
                {
                    alien->setPosition({ alien->getPosition().x, alien->getPosition().y + 24 });
                    alien->getMove().setSideSpeed(alien->getMove().getSideSpeed() * -1);

                    // End game
                    if(alien->getPosition().y >= 575)
                    {
                        shieldTouched = true;
                    }
                }
            }
        }
        scrolldown = true;
    }

    if (scrolldown && ((alienLeft->getPosition().x > 55) || (alienRight->getPosition().x < WINDOW_WIDTH - 55)))
        scrolldown = false;

}

void Game::alienLoad()
{
    float initY = 100;
    float initX = 192;
    for (int li = 0; li < 5; li++)
    {
        initX = 192;
        vector<Alien*> vectorTempAlien;
        for (int co = 0; co < 11; co++)
        {
            Alien* tempAlien = new Alien();
            tempAlien->setPosition({ initX, initY });
            vectorTempAlien.emplace_back(tempAlien);
            initX += 64;
        }
        aliens.emplace_back(vectorTempAlien);
        initY += 48;
    }
    alienLeft = aliens[4][0];
    alienRight = aliens[4][10];
    aliensShooters = aliens[4];

    initX = 234;
    for (int i = 0; i < 4; i++)
    {
        shields.emplace_back(new Shield());
        shields[i]->setPosition({ initX, 625 });
        initX += 184;
    }
}

void Game::alienReLoad()
{
    float initY = 100;
    float initX = 192;
    for (int li = 0; li < 5; li++)
    {
        initX = 192;
        for (int co = 0; co < 11; co++)
        {
            if (aliens[li][co] == nullptr)
            {
                aliens[li][co] = new Alien();
            }
            aliens[li][co]->setPosition({ initX, initY });
            aliens[li][co]->getMove().setSideSpeed(-25);
            initX += 64;
        }
        initY += 48;
    }
    alienLeft = aliens[4][0];
    alienRight = aliens[4][10];
    aliensShooters = aliens[4];
    lifeCount = 3;
    nbAliens = 55;

    initX = 234;
    for (int i = 0; i < 4; i++)
    {
        if (shields[i] == nullptr)
        {
            shields[i] = new Shield();
            shields[i]->setPosition({ initX, 625 });
        }
        shields[i]->setLife(10);
        initX += 184;
    }
}

void Game::ufoPassage(float dt)
{
    if (ufoTimer <= 0)
    {
        if (saucer == nullptr)
        {
            saucer = new UFO();
            int direction = rand() % 2;
            switch (direction)
            {
            case 0:
                saucer->setPosition({ 56, 50 });
                saucer->getMove().setSideSpeed(300.0f);
                break;
            case 1:
                saucer->setPosition({ WINDOW_WIDTH - 56, 50 });
                saucer->getMove().setSideSpeed(-300.0f);
                break;
            default:
                break;
            }

            ufoTimer = rand() % (2500 + 1) + 500;
        }
        else
        {
            saucer->setState(Actor::ActorState::Dead);
            saucer = nullptr;
        }
    }
    else
    {
        ufoTimer -= dt;
    }

    if(saucer != nullptr && (saucer->getPosition().x <= 55 || saucer->getPosition().x >= WINDOW_WIDTH - 55))
    {
        saucer->setState(Actor::ActorState::Dead);
        saucer = nullptr;
    }
}
