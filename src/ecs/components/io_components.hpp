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
	bool nextDialogue = false;
	bool debugMode = false;
	bool tutorialOn = false;
	int hoveringDialogueChoice = -1;
	int lastHoverDialogueChoice = -1;

	ExtendedStack<int> pressedHorizontal;
	ExtendedStack<int> pressedVertical;
};

struct GameState {
	bool gamePaused;
	bool gameOver;
	bool dialogueScene = false;
	bool cutScene = false;
	int dialogueChoice = -1;
	float currentVolume = 0.25f;
};
