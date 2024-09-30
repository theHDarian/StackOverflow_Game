#pragma once

// internal
#include "common.hpp"

// stlib
#include <vector>
#include <random>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_mixer.h>

#include "render_system.hpp"

// Container for all our entities and game logic. Individual rendering / update is
// deferred to the relative update() methods
class WorldSystem
{
public:
	WorldSystem();

	// Creates a window
	GLFWwindow* createWindow();

	// starts the game
	void init(RenderSystem* renderer);

	// Releases all associated resources
	~WorldSystem();

	// Steps the game ahead by ms milliseconds
	bool step(float elapsed_ms);

	// Check for collisions
	void handleCollisions();

	// Should the game be over ?
	bool isOver()const;

	void closeGame();
	void movePlayer(int key,int action, Entity& player);
private:
	// Input callback functions
	void onKey(int key, int, int action, int mod);
	void onMouseMove(vec2 pos);

	// restart level
	void restartGame();

	bool playerIsDead();

	// OpenGL window handle
	GLFWwindow* window;

	// Number of fish eaten by the salmon, displayed in the window title
	unsigned int points;

	// Game state
	RenderSystem* renderer;
	float currentSpeed;

	// Player Controls
	Entity player;
	vec2 mousePosition;

	// music references
	Mix_Music* backgroundMusic;
	Mix_Chunk* salmonDeadSound;
	Mix_Chunk* salmonEatSound;

	// C++ random number generator
	std::default_random_engine rng;
	std::uniform_real_distribution<float> uniformDist; // number between 0..1
};
