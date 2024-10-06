// Header
#include "world_system.hpp"
#include "world_init.hpp"

// stlib
#include <cassert>
#include <sstream>
#include <iostream>

#include "physics_system.hpp"

// Game configuration
const size_t MAX_NUM_EELS = 15;
const size_t MAX_NUM_FISH = 5;
const size_t EEL_SPAWN_DELAY_MS = 2000 * 3;
const size_t FISH_SPAWN_DELAY_MS = 5000 * 3;
uint ENEMY_NUM = 0;

// create the underwater world
WorldSystem::WorldSystem()
	: points(0) {
	// Seeding rng with random device
	rng = std::default_random_engine(std::random_device()());
}

WorldSystem::~WorldSystem() {
	
	// destroy music components
	if (backgroundMusic != nullptr)
		Mix_FreeMusic(backgroundMusic);
	if (salmonDeadSound != nullptr)
		Mix_FreeChunk(salmonDeadSound);
	if (salmonEatSound != nullptr)
		Mix_FreeChunk(salmonEatSound);

	Mix_CloseAudio();

	// Destroy all created components
	registry.clear_all_components();

	// Close the window
	glfwDestroyWindow(window);
}

// Debugging
namespace {
	void glfw_err_cb(int error, const char *desc) {
		fprintf(stderr, "%d: %s", error, desc);
	}
}

// World initialization
// Note, this has a lot of OpenGL specific things, could be moved to the renderer
GLFWwindow* WorldSystem::createWindow() {
	///////////////////////////////////////
	// Initialize GLFW
	glfwSetErrorCallback(glfw_err_cb);
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW");
		return nullptr;
	}

	//-------------------------------------------------------------------------
	// If you are on Linux or Windows, you can change these 2 numbers to 4 and 3 and
	// enable the glDebugMessageCallback to have OpenGL catch your mistakes for you.
	// GLFW / OGL Initialization
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_RESIZABLE, 0);

	// Create the main window (for rendering, keyboard, and mouse input)
	window = glfwCreateWindow(window_width_px, window_height_px, "Salmon Game Assignment", nullptr, nullptr);
	if (window == nullptr) {
		fprintf(stderr, "Failed to glfwCreateWindow");
		return nullptr;
	}

	// Setting callbacks to member functions (that's why the redirect is needed)
	// Input is handled using GLFW, for more info see
	// http://www.glfw.org/docs/latest/input_guide.html
	glfwSetWindowUserPointer(window, this);

	//////////////////////////////////////
	// Loading music and sounds with SDL
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		fprintf(stderr, "Failed to initialize SDL Audio");
		return nullptr;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
		fprintf(stderr, "Failed to open audio device");
		return nullptr;
	}

	backgroundMusic = Mix_LoadMUS(audio_path("music.wav").c_str());
	salmonDeadSound = Mix_LoadWAV(audio_path("death_sound.wav").c_str());
	salmonEatSound = Mix_LoadWAV(audio_path("eat_sound.wav").c_str());

	if (backgroundMusic == nullptr || salmonDeadSound == nullptr || salmonEatSound == nullptr) {
		fprintf(stderr, "Failed to load sounds\n %s\n %s\n %s\n make sure the data directory is present",
			audio_path("music.wav").c_str(),
			audio_path("death_sound.wav").c_str(),
			audio_path("eat_sound.wav").c_str());
		return nullptr;
	}

	return window;
}

void WorldSystem::init(RenderSystem* renderer_arg) {
	this->renderer = renderer_arg;
	// Playing background music indefinitely
	Mix_PlayMusic(backgroundMusic, -1);
	fprintf(stderr, "Loaded music\n");

	// Set all states to default
    restartGame();
}

// Update our game world
bool WorldSystem::step(float elapsed_ms_since_last_update) {
	// Updating window title with points
	std::stringstream title_ss;
	title_ss << "Points: " << points;
	glfwSetWindowTitle(window, title_ss.str().c_str());

	// Remove debug info from the last step
	while (registry.debugComponents.entities.size() > 0)
	    registry.remove_all_components_of(registry.debugComponents.entities.back());

	// Removing out of screen entities
	auto& motions_registry = registry.motions;

	// Remove entities that leave the screen on the left side
	// Iterate backwards to be able to remove without unterfering with the next object to visit
	// (the containers exchange the last element with the current)
	for (int i = (int)motions_registry.components.size()-1; i>=0; --i) {
	    Motion& motion = motions_registry.components[i];
		if (motion.position.x + abs(motion.scale.x) < 0.f) {
			if(!registry.players.has(motions_registry.entities[i])) // don't remove the player
				registry.remove_all_components_of(motions_registry.entities[i]);
		}
	}

	vec2 oldSpeed = registry.motions.get(player).velocity;
	if (oldSpeed[0] == 0 && oldSpeed[1] == 0) {
		oldSpeed = registry.ioStates.components[0].lastInputAxis;
	}

	// Processing inputs
	handleInput();

    //check dash related variables
    dash(oldSpeed, elapsed_ms_since_last_update);

	// Processing the salmon state
	assert(registry.screenStates.components.size() <= 1);
    ScreenState &screen = registry.screenStates.components[0];

    float min_counter_ms = 3000.f;
	// reduce window brightness if the salmon is dying
	screen.darken_screen_factor = 1 - min_counter_ms / 3000;

	return true;
}

// Reset the world state to its initial state
void WorldSystem::restartGame() {
	// Debugging for memory/component leaks
	registry.list_all_components();
	printf("Restarting\n");

	// Reset the game speed
	currentSpeed = 1.f;

	// Remove all entities that we created
	// All that have a motion, we could also iterate over all fish, eels, ... but that would be more cumbersome
	while (registry.motions.entities.size() > 0)
	    registry.remove_all_components_of(registry.motions.entities.back());

	// Debugging for memory/component leaks
	registry.list_all_components();

	player = createPlayer(renderer,{0,0});


	// Test calls:
	
	//createTestWall(renderer, {500,10}, {1000, 600});

	createTestPoly(renderer, { 500,500 }, {
		{100, 0},
		{-50, 50},
		{-50, -50}
		}
		, 90);

	// spawning 1 enemies to test
	if (ENEMY_NUM <= 0) {
		createEnemy(renderer, vec2(1280 - 100.f, 50.f + uniformDist(rng) * (720 - 100.f)), vec2(0, 0), EnemyAttackPattern::DOUBLE_SHOT);
		ENEMY_NUM = 1;
	}
}

// Compute collisions between entities
void WorldSystem::handleCollisions() {
	// Loop over all collisions detected by the physics system
	auto& collisionsRegistry = registry.collisions;
	for (uint i = 0; i < collisionsRegistry.components.size(); i++) {
		// The entity and its collider
		Entity entity = collisionsRegistry.entities[i];
		Entity entity_other = collisionsRegistry.components[i].other;

		// for now, we are only interested in collisions that involve the salmon
		if (registry.players.has(entity)) {
			//Player& player = registry.players.get(entity);

			// Checking Player - Deadly collisions
			if (registry.enemies.has(entity_other)) {
				// initiate invincibility unless already invincible
				if (!registry.invincibles.has(entity)) {
					// Scream, reset timer, and make the salmon sink
					registry.invincibles.emplace(entity);
					Mix_PlayChannel(-1, salmonDeadSound, 0);
				}
			}

			// Check Player -> EnemyBullets collision
			if (registry.enemyBullets.has(entity_other)) {
				// initiate invincibility unless already invincible
				if (!registry.invincibles.has(entity)) {
					// Scream, reset timer, and make the salmon sink
					registry.invincibles.emplace(entity);
					Mix_PlayChannel(-1, salmonDeadSound, 0);
					EnemyBullet& eBullet = registry.enemyBullets.get(entity_other);
					for (int i = 0; i < eBullet.bulletEffects.size(); i++) {
						registry.stackCompile.get(player).add(eBullet.bulletEffects[i]);
					}
				}
			}


			// Checking Player -> Wall collision
			// If found, move the player position away from the wall by radius in direction reflection of projection
			if (registry.walls.has(entity_other)) {
				Motion& motion = registry.motions.get(entity);
				CircleCollider& circle = registry.circleColliders.get(entity);
				WallCollider& wall = registry.walls.get(entity_other);

				vec2 a = motion.position - wall.startPosition;
				vec2 b = wall.endPosition - wall.startPosition;
				vec2 c = (glm::dot(a, glm::normalize(b)) * glm::normalize(b));
				vec2 d = a - c;
				motion.position = (wall.startPosition + c + glm::normalize(d) * (circle.radius));
			}
		}
	}

	// Remove all collisions from this simulation step
	registry.collisions.clear();
}

void WorldSystem::handleInput() {
	assert(registry.ioStates.components.size() <= 1);
    IOState &input = registry.ioStates.components[0];
	if (input.shouldEnd) {
		input.shouldEnd = false;
		closeGame();
	}
	if (input.shouldRestart) {
		input.shouldRestart = false;
		restartGame();
	}
	movePlayer(input.inputAxis);

	if(registry.motions.has(player)) {
		Motion& playerMotion = registry.motions.get(player);
		vec2 diff = input.mousePosition - playerMotion.position;
		float angle = atan2(diff[1],diff[0]);
		//playerMotion.angle = angle;
	}

}

void WorldSystem::dash(vec2 oldSpeed, float elapsed_ms_since_last_update) {
    Player& pl = registry.players.get(player);
    if (pl.currDashCharges < pl.maxDashCharges) {
        pl.currDashCooldown -= elapsed_ms_since_last_update;
        if (pl.currDashCooldown <= 0) {
            pl.currDashCharges++;
            pl.currDashCooldown = pl.dashCooldown;
        }
    }
    IOState& input = registry.ioStates.components[0];
    if (!input.shouldDash) {
        return;
    }
    if (pl.currDashCharges <= 0) {
        input.shouldDash = 0.0f;
        return;
    }
    Motion& player_motion = registry.motions.get(player);
            // save the current speed and direction
    if ((input.shouldDash -= elapsed_ms_since_last_update) > 0.0f) {
        if (!registry.invincibles.has(player))
            registry.invincibles.emplace(player);
        player_motion.velocity = pl.dashSpeed * glm::normalize(oldSpeed);
    }
    else if (input.shouldDash <= 0.0f) {
        // dash is over, remove invincibility and restore speed
        if (registry.invincibles.has(player))
            registry.invincibles.remove(player);
        player_motion.velocity = oldSpeed;
        pl.currDashCharges--;
        input.shouldDash = 0.0f;
        return;
    }
}


void WorldSystem::closeGame() {
	glfwSetWindowShouldClose(window,1);
}

// Should the game be over ?
bool WorldSystem::isOver() const {
	return bool(glfwWindowShouldClose(window));
}

void WorldSystem::movePlayer(vec2 inputAxis) {
	Motion& player_motion = registry.motions.get(player);
	if (glm::length(inputAxis) <= 0.0f) {
		player_motion.velocity = {0,0};
	} else {
		player_motion.velocity = glm::normalize(inputAxis) * getModifiedValue(PlayerSpeed, registry.players.get(player).baseSpeed);
	}
}

float WorldSystem::getModifiedValue(BulletEffectType bf, float value)
{
	return max(registry.stackCompile.get(player).minimums[bf], (value + registry.stackCompile.get(player).additives[bf]) * registry.stackCompile.get(player).multiplicatives[bf]);
}


