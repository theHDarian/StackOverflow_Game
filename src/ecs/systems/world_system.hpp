#pragma once

// internal
#include "common.hpp"

// stlib
#include <vector>
#include <random>

#define SDL_MAIN_HANDLED

#include "render_system.hpp"

// forward declaration for sound system
class SoundSystem;

// Container for all our entities and game logic. Individual rendering / update is
// deferred to the relative update() methods
class WorldSystem
{
public:
	WorldSystem();

	// Creates a window
	GLFWwindow* createWindow();

	// starts the game
	void init(RenderSystem* renderer,  SoundSystem* soundPlayer_arg);

	// Releases all associated resources
	~WorldSystem();

	// Steps the game ahead by ms milliseconds
	bool step(float elapsed_ms);

	// Check for collisions
	void handleCollisions();

	// Should the game be over ?
	bool isOver()const;

	void closeGame();
	void handleInput();

	void clearDeleteQueue();

	void enemyBulletDeath(Entity e);


static float getModifiedValue(BulletEffectType bf, float value);
private:
	// restart level
	void restartGame();

	bool playerIsDead();
	void handlePlayerHit(Entity& other);
	
	void movePlayer();
    void dash(vec2 direction, float elapsed_ms_since_last_update);
    void shoot(float elapsed_ms_since_last_update, int cluster = 1);

	// OpenGL window handle
	GLFWwindow* window;

	// Number of fish eaten by the salmon, displayed in the window title
	unsigned int points;

	// Game state
	RenderSystem* renderer;
	float currentSpeed;

	// Player Controls
	Entity player;
	Entity aimIndicator;
	Entity cursor;
	Entity skipDialogue;

	SoundSystem* soundPlayer;

	// C++ random number generator
	std::default_random_engine rng;
	std::uniform_real_distribution<float> uniformDist; // number between 0..1

};
