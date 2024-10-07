// Header
#include "world_system.hpp"
#include "world_init.hpp"

// stlib
#include <cassert>
#include <sstream>
#include <iostream>
#include <glm/detail/func_trigonometric.inl>

#include "physics_system.hpp"

// Game configuration
const size_t MAX_NUM_EELS = 15;
const size_t MAX_NUM_FISH = 5;
const size_t EEL_SPAWN_DELAY_MS = 2000 * 3;
const size_t FISH_SPAWN_DELAY_MS = 5000 * 3;


#pragma region init
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
#pragma endregion

// Update our game world
bool WorldSystem::step(float elapsed_ms_since_last_update) {
	// Processing inputs
	handleInput();

	// Updating window title with points
	std::stringstream title_ss;
	title_ss << "Points: " << points;
	glfwSetWindowTitle(window, title_ss.str().c_str());

	// Remove debug info from the last step
	// comment this out for now
	//while (registry.debugComponents.entities.size() > 0)
	//    registry.remove_all_components_of(registry.debugComponents.entities.back());

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

	// simplistic way to have collision outlines follow their "owner" when the owner moves
	// potentially buggy implementation with poly outlines, but currently works with circles
	for (auto& owner : registry.collisionShapes.entities) {
		for (auto& shape : registry.collisionShapes.get(owner).shapes) {
			// very rough check to see if owner has moved (has velocity)
			// but doesn't account for change in angle, etc
			if ((registry.motions.get(owner).velocity.x > 0 || registry.motions.get(owner).velocity.y > 0) || owner == player) {
				auto& motion = registry.motions.get(shape);
				motion.angle = registry.motions.get(owner).angle;
				motion.position = registry.motions.get(owner).position;
				motion.velocity = registry.motions.get(owner).velocity;
			}
		}
	}

	// place sprite timer progression here for now
	for (auto& entity : registry.spriteTimers.entities) {
		auto& spriteTimer = registry.spriteTimers.get(entity);
		spriteTimer.count_ms -= elapsed_ms_since_last_update;
		if (spriteTimer.count_ms <= 0) {
			registry.renderRequests.get(entity).used_texture = spriteTimer.nextSprite;
			registry.spriteTimers.remove(entity);
		}
	}

	vec2 preDashSpeed = registry.motions.get(player).velocity;
	if (preDashSpeed[0] == 0 && preDashSpeed[1] == 0) {
		preDashSpeed = registry.ioStates.components[0].lastInputAxis;
	}

    //check dash related variables
    dash(preDashSpeed, elapsed_ms_since_last_update);

	shoot(elapsed_ms_since_last_update, getModifiedValue(BulletNum,registry.players.get(player).bulletCluster), getModifiedValue(BulletBurst,registry.players.get(player).maxBulletBurst));

	// Updating the invincibility timer
	if (registry.invincibles.entities.size() > 0) {
		for (Entity& invincible : registry.invincibles.entities) {
			float& invincible_timer = registry.invincibles.get(invincible).countdown;
			invincible_timer -= elapsed_ms_since_last_update;
			if (invincible_timer <= 0) {
				registry.invincibles.remove(invincible);
				//std::cout << "entity is no longer invincible" << std::endl;
			}
		}
	}

	// Updating the bullet ranges
	if (registry.playerBullets.entities.size() > 0) {
		for (int i = (int)registry.playerBullets.components.size()-1; i>=0; --i) {
			PlayerBullet& bullet = registry.playerBullets.components[i];
			if ((bullet.bulletRange -= elapsed_ms_since_last_update) <= 0) {
				registry.remove_all_components_of(registry.playerBullets.entities[i]);
			}
		}
	}
	if (registry.enemyBullets.entities.size() > 0) {
		// for (Entity& bullet : registry.enemyBullets.entities) {
		// 	float& range_timer = registry.enemyBullets.get(bullet).bulletRange;
		// 	range_timer -= elapsed_ms_since_last_update;
		// 	if (range_timer <= 0) {
		// 		registry.remove_all_components_of(bullet);
		// 	}
		// }
		for (int i = (int)registry.enemyBullets.components.size()-1; i>=0; --i) {
			EnemyBullet& bullet = registry.enemyBullets.components[i];
			if ((bullet.bulletRange -= elapsed_ms_since_last_update) <= 0) {
				registry.remove_all_components_of(registry.enemyBullets.entities[i]);
			}
		}
	}
	if (registry.enemies.size() == 0) {
		createEnemy(renderer, vec2(1280 * uniformDist(rng),720 * uniformDist(rng)), vec2(0, 0), EnemyAttackPattern::ALL_DIRECTION);
	}

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
	
	createTestWall(renderer, {100,200}, {400, 600});

	//createBlob(renderer, vec2(600, 300));

	//createTestPoly(renderer, { 500,500 }, {
	//	{100, 0},
	//	{-50, 50},
	//	{-50, -50}
	//	}
	//	, 90);

	// spawning 1 enemies to test
}

// Compute collisions between entities
void WorldSystem::handleCollisions() {
	// Loop over all collisions detected by the physics system
	auto& collisionsRegistry = registry.collisions;
	for (uint i = 0; i < collisionsRegistry.components.size(); i++) {
		// The entity and its collider
		Entity entity = collisionsRegistry.entities[i];
		Entity entity_other = collisionsRegistry.components[i].other;

		// Player centric collision handling
		if (registry.players.has(entity)) {
			//Player& player = registry.players.get(entity);

			// Checking Player - Deadly collisions
			if (registry.enemies.has(entity_other)) {
				// initiate death unless already dying
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

		// Enemy bullet centric handling
		if (registry.enemyBullets.has(entity)) {
			if (registry.walls.has(entity_other)) {
				if (registry.enemyBullets.get(entity).bulletBounce > 0) {
					// Bounce / reflect the enemy bullet against the wall
					Motion& motion = registry.motions.get(entity);
					WallCollider& wall = registry.walls.get(entity_other);

					vec2 a = motion.position - wall.startPosition;
					vec2 b = wall.endPosition - wall.startPosition;
					vec2 c = (glm::dot(a, glm::normalize(b)) * glm::normalize(b));
					vec2 n = glm::normalize(a - c);

					motion.velocity = motion.velocity - 2 * (glm::dot(motion.velocity, n)) * n;

					// Assumes bullet flies towards facing direction
					motion.angle = atan(motion.velocity.y / motion.velocity.x);

					registry.enemyBullets.get(entity).bulletBounce -= 1;
				}
				else {
					registry.remove_all_components_of(entity);
				}
			}
		}

		// Player bullet centric handling
		if (registry.playerBullets.has(entity)) {
			if (registry.walls.has(entity_other)) {
				if (registry.playerBullets.get(entity).bulletBounce > 0) {
					// Bounce / reflect the enemy bullet against the wall
					Motion& motion = registry.motions.get(entity);
					WallCollider& wall = registry.walls.get(entity_other);

					vec2 a = motion.position - wall.startPosition;
					vec2 b = wall.endPosition - wall.startPosition;
					vec2 c = (glm::dot(a, glm::normalize(b)) * glm::normalize(b));
					vec2 n = glm::normalize(a - c);

					motion.velocity = motion.velocity - 2 * (glm::dot(motion.velocity, n)) * n;

					// Assumes bullet flies towards facing direction
					motion.angle = atan(motion.velocity.y/motion.velocity.x);

					registry.playerBullets.get(entity).bulletBounce -= 1;
				}
				else {
					registry.remove_all_components_of(entity);
				}
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

	//game playing
	movePlayer(input.inputAxis);
}

void WorldSystem::dash(vec2 preDashSpeed, float elapsed_ms_since_last_update) {
    Player& pl = registry.players.get(player);
    if (pl.currDashCharges < getModifiedValue(PlayerNumDash, pl.maxDashCharges)) {
        pl.currDashCooldown -= elapsed_ms_since_last_update;
        if (pl.currDashCooldown <= 0) {
            pl.currDashCharges++;
            pl.currDashCooldown = getModifiedValue(PlayerDashCDR, pl.dashCooldown);
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
    	registry.invincibles.get(player).countdown = max(registry.invincibles.get(player).countdown, input.shouldDash);
        player_motion.velocity = pl.dashSpeed * glm::normalize(preDashSpeed);
    }
    else if (input.shouldDash <= 0.0f) {
        // dash is over, remove invincibility and restore speed
        player_motion.velocity = preDashSpeed;
        pl.currDashCharges--;
        input.shouldDash = 0.0f;
        return;
    }
}

void WorldSystem::shoot(float elapsed_ms_since_last_update, int cluster, int burst) {
	IOState& input = registry.ioStates.components[0];
	Player& pl = registry.players.get(player);
	if (!input.shouldShoot) {
		if (elapsed_ms_since_last_update > 50 && (pl.currBulletBurst < pl.maxBulletBurst)) {
			pl.currBulletBurst++;
		}
		return;
	}
	if (pl.currFiringInterval > 0) {
		pl.currFiringInterval -= elapsed_ms_since_last_update;
	}
	if (pl.bulletBurstCooldown > 0) {
		pl.bulletBurstCooldown -= elapsed_ms_since_last_update;
	}
	if (pl.currFiringInterval <= 0) {
		pl.currBulletBurst = getModifiedValue(BulletBurst, pl.maxBulletBurst);
		pl.currFiringInterval = (1 / getModifiedValue(FireRate, 1000 / pl.maxFiringInterval)) * 1000;
	}
	if (pl.bulletBurstCooldown <= 0 && pl.currBulletBurst > 0) {
		//convert interval from ms to rounds per second for getModifiedValue, then back to ms
		pl.currBulletBurst--;
		pl.bulletBurstCooldown = min(
			50.0f,
			((1 / getModifiedValue(FireRate, 1000 / pl.maxFiringInterval)) * 1000) / getModifiedValue(
				BulletBurst, pl.maxBulletBurst)
		);
		// create bullet

		vec2 playerPos = registry.motions.get(player).position;
		vec2 bulletDir = glm::normalize(input.mousePosition - registry.motions.get(player).position);
		vec2 bulletPos = playerPos + bulletDir * 100.f;

		if (cluster == 1) {
			createPlayerBullet(renderer, bulletPos, bulletDir);
			return;
		}

		// Calculate the offset between bullets for cluster shots

		float offSet = radians(getModifiedValue(BulletSpread, 30)) / cluster;
		// Create a rotation matrix
		glm::mat2 rotationMatrix = glm::mat2(
			glm::cos(offSet), -glm::sin(offSet),
			glm::sin(offSet),  glm::cos(offSet)
		);


		if (cluster == 2) {
			createPlayerBullet(renderer, bulletPos, bulletDir*rotationMatrix);
			createPlayerBullet(renderer, bulletPos, bulletDir*glm::transpose(rotationMatrix));
			return;
		}

		for (int i = 0; i < cluster; i++) {
			if (i == 0) {
				createPlayerBullet(renderer, bulletPos, bulletDir);
				continue;
			}
			for (int j = 0; j < i; j++) {
				if (i % 2 == 0) {
					bulletDir = bulletDir * rotationMatrix;
				}
				else {
					bulletDir = bulletDir * glm::transpose(rotationMatrix);
				}
			}
			createPlayerBullet(renderer, bulletPos, bulletDir);
		}
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


