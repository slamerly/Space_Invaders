#pragma once
#include <vector>
#include "Window.h"
#include "Renderer.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "Alien.h"
#include "Ship.h"

using std::vector;

class Game
{
//Begin singleton
public:
	static Game& instance()
	{
		static Game inst;
		return inst;
	}

	Game(const Game&) = delete;
	Game& operator = (const Game&) = delete;
	Game(const Game&&) = delete;
	Game& operator = (const Game&&) = delete;

private:
	//Game() = default;
	Game() : isRunning(true), isUpdatingActors(false){}

//End singleton

public:
	bool initialize();
	void load();
	void loop();
	void unload();
	void close();

	void addActor(Actor* actor);
	void removeActor(Actor* actor);

	Renderer& getRenderer() { return renderer; }

	// Game specific
	vector<vector<Alien*>> getAliens() { return aliens; }
	void removeAlien(Alien* alienTarget);
	Ship* getShipActive() { return shipActive; }
	void shipDestroy();

private:
	void processInput();
	void update(float dt);
	void render();

	Window window {};
	Renderer renderer;
	bool isRunning { true };

	bool isUpdatingActors;
	vector<Actor*> actors;
	vector<Actor*> pendingActors;

	// Game specific
	void aliensShot(float dt);
	void aliensMovement();

	vector<vector<Alien*>> aliens;
	vector<Alien*> aliensShooters;
	int nbAliens = 5 * 11;
	float delayShot = 0.0f;
	Alien* alienLeft = nullptr;
	Alien* alienRight = nullptr;
	Ship* shipActive = nullptr;
};

