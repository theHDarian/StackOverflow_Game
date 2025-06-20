#pragma once
#include <time.h>
#include <chrono>

#include "common.hpp"
#include "utils/extended_stack.hpp"

struct WindowState {
    int width;
	int height;
    bool isRetinaDisplay;

	int fps;
	int numFramesThisSecond;
	std::chrono::steady_clock::time_point currUnixTime;

	std::chrono::steady_clock::time_point startTime;
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
	int hoveringMenuChoice = 0;
	int lastHoverMenuChoice = 0;
	bool showFPS = true;
	bool clickedButton = false;
	int activeMenu = -1;
	bool confirmedOption = false;
	bool pressedEsc = false;
	bool lockControls = false;
	bool isFullscreen = true;
	bool isBorderless = false;

	ExtendedStack<int> pressedHorizontal;
	ExtendedStack<int> pressedVertical;
};

struct GameState {
	bool gamePaused;
	bool gameOver;
	bool hardMode = true; // hard mode is 25% faster
	bool dialogueScene = false;
	bool cutScene = false;
	int dialogueChoice = -1;
	float currentVolume = 0.4f;
	float previousVolume = 0.4f;
	float currentSfxVolume = 0.3f;
	float previousSfxVolume = 0.3f;
	bool seenLockedDoor = false;
	bool titleScreen = false;
	bool resetRoom = false;
	bool loading = true; // this is ONLY here to prevent awkward half second of non-black screen on game start
						// when fix ui later properly can remove
};

struct Camera {
	float zoom = 1.0f;
	vec2 lookAtPos = vec2(0);
	float startZoom = 1.0f;
	vec2 startPos = vec2(0);
	float elapsedTime = 0;
	Entity target; // camera will follow target if no transitions ongoing, default is player
};

enum CameraRequestType {
	ChangeZoom,
	ChangeLookAt,
	ChangeLookAtAndZoom,
	ChangeTarget,
	HoldCamera, // keep camera frozen for X ms
	ChangeTargetAndZoom
};

struct CameraRequest {
	CameraRequestType type;
	float newZoom; // so can lerp from current zoom to new zoom
	vec2 newLookAt; // look to a static position
	float transitionTime = 1000; // how long camera transition should take, in ms
	Entity newTarget; // new target camera should follow, must have a motion attached. Will also transition camera position over to target first.
};

struct GameReport {
	int roomsCleared = 0; // in case differ from map's, i.e. each region's counts resets
	std::chrono::steady_clock::time_point gameStartTime; // different from window, used to calculate run time
	std::string name;
};
