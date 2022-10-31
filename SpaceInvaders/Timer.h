#pragma once

constexpr static unsigned int FPS = 60;
constexpr static unsigned int FRAME_DELAY = 1000 / FPS;

class Timer
{
public:
	Timer() = default;
	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;

	// Compute delta time as the number of milliseconds since last frame
	// Clamp it so debug won't think delta time is really high during breakpoints
	unsigned int computeDeltaTime();

	// Wait if the game run faster than the decided FPS
	void delayTime();

private:
	// unsigned int valeur toujours positive
	const unsigned int MAX_DT = 50;

	// Time in milliseconds when frame starts
	unsigned int frameStart{ 0 };

	// Last frame start time in milliseconds
	unsigned int lastFrame{ 0 };

	// Time it tooks to run the loop. Used to cap framerate.
	unsigned int frameTime{ 0 };
};