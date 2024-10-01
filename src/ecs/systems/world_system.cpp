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
	auto key_redirect = [](GLFWwindow* wnd, int _0, int _1, int _2, int _3) { ((WorldSystem*)glfwGetWindowUserPointer(wnd))->onKey(_0, _1, _2, _3); };
	auto cursor_pos_redirect = [](GLFWwindow* wnd, double _0, double _1) { ((WorldSystem*)glfwGetWindowUserPointer(wnd))->onMouseMove({ _0, _1 }); };
	glfwSetKeyCallback(window, key_redirect);
	glfwSetCursorPosCallback(window, cursor_pos_redirect);

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

	// Processing the salmon state
	assert(registry.screenStates.components.size() <= 1);
    ScreenState &screen = registry.screenStates.components[0];

    float min_counter_ms = 3000.f;
	for (Entity entity : registry.deathTimers.entities) {
		// progress timer
		DeathTimer& counter = registry.deathTimers.get(entity);
		counter.counter_ms -= elapsed_ms_since_last_update;
		if(counter.counter_ms < min_counter_ms){
		    min_counter_ms = counter.counter_ms;
		}

		// restart the game once the death timer expired
		if (counter.counter_ms < 0) {
			registry.deathTimers.remove(entity);
			screen.darken_screen_factor = 0;
            restartGame();
			return true;
		}
	}
	// reduce window brightness if the salmon is dying
	screen.darken_screen_factor = 1 - min_counter_ms / 3000;

	for (Entity entity : registry.lightUp.entities) {
		// progress timer
		LightUp& counter = registry.lightUp.get(entity);
		counter.counter_ms -= elapsed_ms_since_last_update;

		std::cout << "LightUp Timer: " << counter.counter_ms << std::endl;

		// restart the game once the death timer expired
		if (counter.counter_ms < 0) {
			registry.lightUp.remove(entity);
		}
	}

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

	player = createSalmon(renderer,{0,0});
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
				// initiate death unless already dying
				if (!registry.deathTimers.has(entity)) {
					// Scream, reset timer, and make the salmon sink
					registry.deathTimers.emplace(entity);
					Mix_PlayChannel(-1, salmonDeadSound, 0);

					Motion& player_motion = registry.motions.get(entity);
					player_motion.angle = M_PI;
					player_motion.velocity = {0,100};
					vec3& player_color = registry.colors.get(entity);
					player_color = {1.0f,0.0f,0.0f};
				}
			}
			// Checking Player - Eatable collisions
			else if (registry.eatables.has(entity_other)) {
				if (!registry.deathTimers.has(entity)) {
					// chew, count points, and set the LightUp timer
					registry.remove_all_components_of(entity_other);
					Mix_PlayChannel(-1, salmonEatSound, 0);
					++points;

					if (!registry.lightUp.has(entity))
						registry.lightUp.emplace(entity);
					else {
						registry.lightUp.get(entity).counter_ms = 3000.0f;
					}
				}
			}
		}
	}

	// Remove all collisions from this simulation step
	registry.collisions.clear();
}

void WorldSystem::closeGame() {
	glfwSetWindowShouldClose(window,1);
}

// Should the game be over ?
bool WorldSystem::isOver() const {
	return bool(glfwWindowShouldClose(window));
}

void WorldSystem::movePlayer(int key, int action, Entity& player) {
	Motion& player_motion = registry.motions.get(player);
	float speed = 100.0f;
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		if (key == GLFW_KEY_LEFT)
			player_motion.velocity = {-speed,player_motion.velocity[1]};
		else if (key == GLFW_KEY_RIGHT)
			player_motion.velocity = {speed,player_motion.velocity[1]};
		else if (key == GLFW_KEY_UP)
			player_motion.velocity = {player_motion.velocity[0],-speed}; //up is negative
		else if (key == GLFW_KEY_DOWN)
			player_motion.velocity = {player_motion.velocity[0],speed};
	} else if (action == GLFW_RELEASE) { //on release, reset to zero or based on other held key
		if (key == GLFW_KEY_LEFT)
			player_motion.velocity = {glm::max(player_motion.velocity[0],0.0f),player_motion.velocity[1]};
		else if (key == GLFW_KEY_RIGHT)
			player_motion.velocity = {glm::min(player_motion.velocity[0],0.0f),player_motion.velocity[1]};
		else if (key == GLFW_KEY_UP)
			player_motion.velocity = {player_motion.velocity[0],glm::max(0.0f,player_motion.velocity[1])}; //up is negative
		else if (key == GLFW_KEY_DOWN)
			player_motion.velocity = {player_motion.velocity[0],glm::min(0.0f,player_motion.velocity[1])};
	}
}

// On key callback
void WorldSystem::onKey(int key, int, int action, int mod) {
	// Close game
	if (key == GLFW_KEY_ESCAPE) {
		closeGame();
	}

	// Resetting game
	if (action == GLFW_RELEASE && key == GLFW_KEY_R) {
		int w, h;
		glfwGetWindowSize(window, &w, &h);

        restartGame();
	}

	//Player movement
	if(!playerIsDead()) {
		movePlayer(key,action,player);
	}
	

	// Debugging
	if (key == GLFW_KEY_D) {
		if (action == GLFW_RELEASE)
			debugging.in_debug_mode = false;
		else
			debugging.in_debug_mode = true;
	}

	// Control the current speed with `<` `>`
	if (action == GLFW_RELEASE && (mod & GLFW_MOD_SHIFT) && key == GLFW_KEY_COMMA) {
		currentSpeed -= 0.1f;
		printf("Current speed = %f\n", currentSpeed);
	}
	if (action == GLFW_RELEASE && (mod & GLFW_MOD_SHIFT) && key == GLFW_KEY_PERIOD) {
		currentSpeed += 0.1f;
		printf("Current speed = %f\n", currentSpeed);
	}
	currentSpeed = fmax(0.f, currentSpeed);
}

bool WorldSystem::playerIsDead() {
	return registry.deathTimers.has(player);
}

void WorldSystem::onMouseMove(vec2 mousePosition) {
	this->mousePosition = mousePosition;
	
	//rotate player to face cursor
	if(!playerIsDead() && registry.motions.has(player)) {
		Motion& player_motion = registry.motions.get(player);
		vec2 diff = mousePosition - player_motion.position;
		float angle = atan2(diff[1],diff[0]);
		player_motion.angle = angle;
	}
	
}
