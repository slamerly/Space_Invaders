#pragma once
#include "MoveComponent.h"
#include <SDL_stdinc.h>

class InputComponent : 
	public MoveComponent
{
public:
	InputComponent(Actor* ownerP);
	InputComponent() = delete;
	InputComponent(const InputComponent&) = delete;
	InputComponent& operator=(const InputComponent&) = delete;

	void processInput(const Uint8* keyState);

	void setMaxForwardSpeed(float maxForwardSpeedP);
	void setMaxSideSpeed(float maxSideSpeedP);
	void setMaxAngularSpeed(float maxAngularSpeedP);
	void setForwardKey(int key);
	void setBackKey(int key);
	void setLeftKey(int key);
	void setRightKey(int key);
	void setClockwiseKey(int key);
	void setCounterClockwiseKey(int key);

private:
	float maxForwardSpeed;
	float maxSideSpeed;
	float maxAngularSpeed;
	
	int forwardKey;
	int backKey;
	int leftKey;
	int rightKey;
	int clockwiseKey;
	int counterClockwiseKey;
};