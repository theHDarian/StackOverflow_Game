#pragma once
#include <time.h>

#include "common.hpp"
#include "utils/extended_stack.hpp"

struct WindowState {
    int width;
	int height;
    bool isRetinaDisplay;

	float fps;
	int numFramesThisSecond;
	time_t currUnixTime;
};

struct IOState {
	bool shouldEnd;
	bool shouldRestart;
    bool shouldDash;
	bool shouldShoot;
	vec2 inputAxis;
	vec2 lastInputAxis = {1,1};
	vec2 mousePosition;
	bool shouldShowDialogue = false;
	bool nextDialogue = false;

	ExtendedStack<int> pressedHorizontal;
	ExtendedStack<int> pressedVertical;
};

struct GameState {
	bool gamePaused;
	bool gameOver;
	bool dialogueScene = false;
};