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
private:
	// restart level
	void restartGame();

	bool playerIsDead();
	void handleInput();
	void movePlayer(vec2 inputAxis);
    void dash(vec2 preDashSpeed, float elapsed_ms_since_last_update);

	void shoot(float elapsed_ms_since_last_update, int cluster = 1, int burst = 1);

	float getModifiedValue(BulletEffectType bf, float value);

	// OpenGL window handle
	GLFWwindow* window;

	// Number of fish eaten by the salmon, displayed in the window title
	unsigned int points;

	// Game state
	RenderSystem* renderer;
	float currentSpeed;
	Entity dialogueBox;

	// Player Controls
	Entity player;

	// music references
	Mix_Music* backgroundMusic;
	Mix_Chunk* salmonDeadSound;
	Mix_Chunk* salmonEatSound;

	// C++ random number generator
	std::default_random_engine rng;
	std::uniform_real_distribution<float> uniformDist; // number between 0..1

};
