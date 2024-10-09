#include "common.hpp"
#include "../utils/extended_stack.hpp"

struct WindowState {
    int width;
	int height;
    bool isRetinaDisplay;
};

struct IOState {
	bool shouldEnd;
	bool shouldRestart;
    bool shouldDash;
	bool shouldShoot;
	vec2 inputAxis;
	vec2 lastInputAxis = {1,1};
	vec2 mousePosition;

	ExtendedStack<int> pressedHorizontal;
	ExtendedStack<int> pressedVertical;
};

struct GameState {
	bool gamePaused;
	bool gameOver;
};