#include "InputComponent.h"
#include <SDL_scancode.h>

InputComponent::InputComponent(Actor* ownerP) :
	MoveComponent(ownerP),
	maxForwardSpeed(100.0f),
	maxSideSpeed(75.0f),
	maxAngularSpeed(1.0f),
	forwardKey(SDL_SCANCODE_W),
	backKey(SDL_SCANCODE_S),
	leftKey(SDL_SCANCODE_LEFT),
	rightKey(SDL_SCANCODE_RIGHT),
	clockwiseKey(SDL_SCANCODE_D),
	counterClockwiseKey(SDL_SCANCODE_A)
{
}

void InputComponent::processInput(const Uint8* keyState)
{
	/*
	float forwardSpeed = 0.0f;
	if (keyState[forwardKey])
	{
		forwardSpeed += maxForwardSpeed;
	}
	if (keyState[backKey])
	{
		forwardSpeed -= maxForwardSpeed;
	}
	setForwardSpeed(forwardSpeed);

	float angularSpeed = 0.0f;
	if (keyState[clockwiseKey])
	{
		angularSpeed -= maxAngularSpeed;
	}
	if (keyState[counterClockwiseKey])
	{
		angularSpeed += maxAngularSpeed;
	}
	setAngularSpeed(angularSpeed);
	*/
	float sideSpeed = 0.0f;
	if (keyState[leftKey])
	{
		sideSpeed -= maxSideSpeed;
	}
	if (keyState[rightKey])
	{
		sideSpeed += maxSideSpeed;
	}
	setSideSpeed(sideSpeed);

}

void InputComponent::setMaxForwardSpeed(float maxForwardSpeedP)
{
	maxForwardSpeed = maxForwardSpeedP;
}

void InputComponent::setMaxSideSpeed(float maxSideSpeedP)
{
	maxSideSpeed = maxSideSpeedP;
}

void InputComponent::setMaxAngularSpeed(float maxAngularSpeedP)
{
	maxAngularSpeed = maxAngularSpeedP;
}

void InputComponent::setForwardKey(int key)
{
	forwardKey = key;
}

void InputComponent::setBackKey(int key)
{
	backKey = key;
}

void InputComponent::setLeftKey(int key)
{
	leftKey = key;
}

void InputComponent::setRightKey(int key)
{
	rightKey;
}

void InputComponent::setClockwiseKey(int key)
{
	clockwiseKey = key;
}

void InputComponent::setCounterClockwiseKey(int key)
{
	counterClockwiseKey = key;
}