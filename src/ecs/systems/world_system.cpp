// Header
#include "world_system.hpp"
#include "world_init.hpp"
#include "text_system.hpp"

// stlib
#include <cassert>
#include <sstream>
#include <iostream>
#include <glm/detail/func_trigonometric.inl>

#include "physics_system.hpp"

// include these for now
// but may change to handle like render system does
#include "text_system.hpp"

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
	if (playerHurtSound != nullptr)
		Mix_FreeChunk(playerHurtSound);
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
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	// Create the main window (for rendering, keyboard, and mouse input)
	int window_width_px,window_height_px;
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* vidMode = glfwGetVideoMode(monitor);
	window_width_px = vidMode->width;
	window_height_px = vidMode->height;
	window = glfwCreateWindow(window_width_px, window_height_px, "StackOverflow", monitor, nullptr);
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
	playerHurtSound = Mix_LoadWAV(audio_path("player_hurtv1.wav").c_str());
	salmonEatSound = Mix_LoadWAV(audio_path("eat_sound.wav").c_str());

	if (backgroundMusic == nullptr || playerHurtSound == nullptr || salmonEatSound == nullptr) {
		fprintf(stderr, "Failed to load sounds\n %s\n %s\n %s\n make sure the data directory is present",
			audio_path("music.wav").c_str(),
			audio_path("player_hurtv1.wav").c_str(),
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

	///////////////////////////////////
	// load text rendering
	if (initFreetypeLib() > 0) {
		std::cout << "Freetype loaded!" << std::endl;
	}

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
	//for (auto& owner : registry.collisionShapes.entities) {
	//	for (auto& shape : registry.collisionShapes.get(owner).shapes) {
	//		// very rough check to see if owner has moved (has velocity)
	//		// but doesn't account for change in angle, etc
	//		if ((registry.motions.get(owner).velocity.x > 0 || registry.motions.get(owner).velocity.y > 0) || owner == player) {
	//			auto& motion = registry.motions.get(shape);
	//			motion.angle = registry.motions.get(owner).angle;
	//			motion.position = registry.motions.get(owner).position;
	//			motion.velocity = registry.motions.get(owner).velocity;
	//		}
	//	}
	//}

	// place sprite timer progression here for now
	for (auto& entity : registry.spriteTimers.entities) {
		auto& spriteTimer = registry.spriteTimers.get(entity);
		spriteTimer.count_ms -= elapsed_ms_since_last_update;
		if (spriteTimer.count_ms <= 0) {
			registry.renderRequests.get(entity).used_texture = spriteTimer.nextSprite;
			registry.spriteTimers.remove(entity);
		}
	}

	vec2 dashDirection = registry.ioStates.components[0].lastInputAxis;

	movePlayer();
    //check dash related variables
    dash(dashDirection, elapsed_ms_since_last_update);

	shoot(elapsed_ms_since_last_update, getModifiedValue(BulletNum,registry.players.get(player).bulletCluster));

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
				registry.deleteEntityAndRelatedEntities(registry.playerBullets.entities[i]);
			}
		}
	}
	if (registry.enemyBullets.entities.size() > 0) {
		for (int i = (int)registry.enemyBullets.components.size()-1; i>=0; --i) {
			EnemyBullet& bullet = registry.enemyBullets.components[i];
			if ((bullet.bulletRange -= elapsed_ms_since_last_update) <= 0) {
				// remove enemy bullet
				registry.deleteEntityAndRelatedEntities(registry.enemyBullets.entities[i]);
			}
		}
	}

    //check invisibity countdown
    if (registry.invisibles.entities.size() > 0) {
        for (int i = (int)registry.invisibles.components.size()-1; i>=0; --i) {
            Invisible& entity = registry.invisibles.components[i];
            if ((entity.countdown -= elapsed_ms_since_last_update) <= 0) {
                registry.invisibles.remove(registry.invisibles.entities[i]);
            }
        }
    }


	WindowState& wS = registry.windowStates.components[0];
	if (registry.enemies.size() == 0) {
		createEnemy(renderer, vec2(wS.width * uniformDist(rng),wS.height * uniformDist(rng)), vec2(0, 0), EnemyAttackPattern::ALL_DIRECTION);
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
	GameState& gameState = registry.gameStates.components[0];
	gameState.gameOver = false;
	gameState.gamePaused = false;
	gameState.dialogueScene = false;

	WindowState& wS = registry.windowStates.components[0];
	printf("Restarting\n");

	// Reset the game speed
	currentSpeed = 1.f;

	// Remove all entities that we created
	// All that have a motion, we could also iterate over all fish, eels, ... but that would be more cumbersome
	while (registry.motions.entities.size() > 0)
	    registry.remove_all_components_of(registry.motions.entities.back());

	// Debugging for memory/component leaks
	registry.list_all_components();

	player = createPlayer(renderer,{wS.width / 2,wS.height/2});
	aimIndicator = createAimIndicator(renderer);

	// Test calls:
	createTestWall(renderer, {100,200}, {500, 200});
	createTestWall(renderer, {100,200}, {100, 600});

	//bounding walls
	
	createTestWall(renderer, {0,0}, {wS.width, 0});
	createTestWall(renderer, {wS.width,0}, {wS.width, wS.height});
	createTestWall(renderer, {wS.width, wS.height}, {0, wS.height});
	createTestWall(renderer, {0, wS.height}, {0,0});
	//createBlob(renderer, vec2(600, 300));

	//createTestFloor(renderer, { 1920/2,1080/2 });

	//createTestPoly(renderer, { 500,500 }, {
	//	{100, 0},
	//	{-50, 50},
	//	{-50, -50}
	//	}
	//	, 90);

	// this feels very bad, put as temp fix for getting window size for now
	WindowState& windowState = registry.windowStates.components[0];
	dialogueBox = createDialogueBox(vec2(windowState.width /2, windowState.height - windowState.height /8), vec2(windowState.width, windowState.height /4));
	pauseMenu = createPauseMenu(vec2(windowState.width / 2, windowState.height / 2), vec2(windowState.width, windowState.height / 4));
	gameOverMenu = createGameOverMenu(vec2(windowState.width / 2, windowState.height / 2), vec2(windowState.width, windowState.height / 4));
}

// Compute collisions between entities
void WorldSystem::handleCollisions() {
	auto& collisionsRegistry = registry.collisions;
	for (uint i = 0; i < collisionsRegistry.components.size(); i++) {
		Entity entity = collisionsRegistry.entities[i];
		Entity entity_other = collisionsRegistry.components[i].other;

		// Player centric collision handling
		if (registry.players.has(entity)) {
			// Checking Player - Deadly collisions
			if (!registry.invincibles.has(entity)
				&& (registry.enemies.has(entity_other) || registry.enemyBullets.has(entity_other))
			) {
				handlePlayerHit(entity_other);
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
				// Player center projects onto the wall;
				if (abs(glm::length(c) + glm::length(b - c) - glm::length(b)) < 0.01) {
					motion.position = (wall.startPosition + c + glm::normalize(d) * (circle.radius));
				}
				// Player circle collides with startPosition
				else if (glm::length(a) < circle.radius) {
					motion.position = (wall.startPosition + glm::normalize(a) * (circle.radius));
				}
				// Player circle collides with endPosition
				else if (glm::length(motion.position - wall.endPosition) < circle.radius) {
					motion.position = (wall.endPosition + glm::normalize(motion.position - wall.endPosition) * (circle.radius));
				}
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
					registry.deleteEntityAndRelatedEntities(entity);
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
					registry.deleteEntityAndRelatedEntities(entity);
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

	GameState& gameState = registry.gameStates.components[0];
	registry.renderRequests.get(gameOverMenu).show = gameState.gameOver;

	if (!gameState.gameOver) {
		registry.renderRequests.get(pauseMenu).show = gameState.gamePaused;
		if (input.shouldShowDialogue && input.nextDialogue && !gameState.gamePaused) {
			input.nextDialogue = false;
			std::string nextLine = registry.dialogueLines.get(dialogueBox).next();
			std::cout << " dialogue line " << nextLine << std::endl;
			if (strcmp(nextLine.c_str(), "<end>") != 0) {
				registry.renderRequests.get(dialogueBox).show = true;
				registry.textRenderRequests.get(dialogueBox).text = nextLine;
			}
			// no more lines of dialogue
			else {
				input.shouldShowDialogue = false;
				registry.renderRequests.get(dialogueBox).show = false;
				gameState.dialogueScene = false;
			}
		}
	}

}

void WorldSystem::dash(vec2 direction, float elapsed_ms_since_last_update) {
	// Tick Dash Charge Timer
    Player& pl = registry.players.get(player);
    if (pl.currDashCharges < getModifiedValue(PlayerNumDash, pl.maxDashCharges)) {
		if (pl.currDashCooldown > 0) {
        	pl.currDashCooldown -= elapsed_ms_since_last_update;
		} else {
            pl.currDashCharges++;
            pl.currDashCooldown = getModifiedValue(PlayerDashCDR, pl.dashCooldown);
        }
    }
	// Player wants to start a new dash
	IOState& input = registry.ioStates.components[0];
	Motion& playerMotion = registry.motions.get(player);
	if (input.shouldDash) {
		input.shouldDash = false;

		if (!registry.dashes.has(player) && pl.currDashCharges > 0) {
			pl.currDashCharges--;
			Dash& dash = registry.dashes.emplace(player);
			dash.dashDirection = direction;
		}
	}
	// Tick dash timer
	if (registry.dashes.has(player)) {
		Dash& dash = registry.dashes.get(player);
		dash.endTimer -= elapsed_ms_since_last_update;
		if (dash.endTimer <= 0) {
			registry.dashes.remove(player);
			playerMotion.velocity = {0,0};
		} else {
			// Tick IFrame timer
			if (!registry.invincibles.has(player))
            	registry.invincibles.emplace(player);
			registry.invincibles.get(player).countdown = max(registry.invincibles.get(player).countdown, dash.endTimer);

			// Update Velocity
			playerMotion.velocity = pl.dashSpeed * glm::normalize(dash.dashDirection);
			// std::cout << playerMotion.velocity.x << " " << playerMotion.velocity.y << std::endl;
		}
	}
}

void WorldSystem::shoot(float elapsed_ms_since_last_update, int cluster) {
	IOState& input = registry.ioStates.components[0];
	PlayerAttackData& pl = registry.shoots.get(player);
    Motion& player_motion = registry.motions.get(player);
    vec2 playerPos = player_motion.position;
    vec2 bulletDir = glm::normalize(input.mousePosition - player_motion.position);
    player_motion.scale.x = bulletDir.x < 0 ? -abs(player_motion.scale.x) : abs(player_motion.scale.x);
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
		vec2 bulletPos = playerPos + bulletDir;

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

void WorldSystem::movePlayer() {
	IOState &input = registry.ioStates.components[0];
	vec2 inputAxis = input.inputAxis;
	Motion& player_motion = registry.motions.get(player);
	if (glm::length(inputAxis) <= 0.0f) {
		player_motion.velocity = {0,0};
	} else {
		player_motion.velocity = glm::normalize(inputAxis) * getModifiedValue(PlayerSpeed, registry.players.get(player).baseSpeed);
	}

	//move aim indicator
	Motion& aimMotion = registry.motions.get(aimIndicator);
	vec2 mousePos = input.mousePosition;
	vec2 diff = mousePos - player_motion.position;
	float range = 50.0f;
	aimMotion.angle = atan(diff.y,diff.x)+M_PI/4;
	aimMotion.position = player_motion.position + glm::normalize(diff) * range;
	
}

float WorldSystem::getModifiedValue(BulletEffectType bf, float value)
{
	return max(registry.stackCompile.get(player).minimums[bf], (value + registry.stackCompile.get(player).additives[bf]) * registry.stackCompile.get(player).multiplicatives[bf]);
}

void WorldSystem::handlePlayerHit(Entity& other) {
	//change sprite
	auto& spriteMap = registry.sprites.get(player).sprites;
	if (spriteMap.count(SPRITE_STATE::DAMAGED) && !registry.invincibles.has(player)) {
		registry.renderRequests.get(player).used_texture = spriteMap[SPRITE_STATE::DAMAGED];
		if (!registry.spriteTimers.has(player)) {
			auto& spriteTimer = registry.spriteTimers.emplace(player);
			spriteTimer.count_ms = 100;
			spriteTimer.nextSprite = spriteMap[SPRITE_STATE::BASE];
		}
	}
	//play hit sound
	Mix_PlayChannel(-1, playerHurtSound, 0);
	//add player invincibility frames
	if (!registry.invincibles.has(player))
		registry.invincibles.emplace(player);

	//add to stack for enemy bullets
	if (registry.enemyBullets.has(other)) {
		EnemyBullet& eBullet = registry.enemyBullets.get(other);
		for (int i = 0; i < eBullet.bulletEffects.size(); i++) {
			bool success = registry.stackCompile.get(player).add(eBullet.bulletEffects[i]);
			if (!success) {
				registry.gameStates.components[0].gameOver = true;
			}
		}
	}
}


