// Header
#include "world_system.hpp"
#include "world_init.hpp"

// stlib
#include <cassert>
#include <sstream>
#include <glm/detail/func_trigonometric.inl>
#include <SDL.h>
#include <time.h>
#include "sound_system.hpp"
#include "physics_system.hpp"
#include "interactable_effects.h"
#include "components/presets/particle_presets.hpp"

// include these for now
// but may change to handle like render system does
#include "text_system.hpp"
#include "utils/random.hpp"
#include "utils/vector_operations.hpp"
#include <chrono>

using Clock = std::chrono::high_resolution_clock;

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

	// Destroy all created components
	registry.clear_all_components();

	// Close the window
	if (glfwWindowShouldClose(window))
		glfwDestroyWindow(window);
	glfwTerminate();
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
	glfwWindowHint(GLFW_REFRESH_RATE,60);
	//glfwWindowHint(GLFW_DECORATED,GLFW_FALSE); //make borderless window

	// Create the main window (for rendering, keyboard, and mouse input)
	int window_width_px,window_height_px;
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* vidMode = glfwGetVideoMode(monitor);
	//window_width_px = vidMode->width;
	//window_height_px = vidMode->height;
	 window_width_px = 1920;
	 window_height_px = 1080;
	window = glfwCreateWindow(window_width_px, window_height_px, "StackOverflow", monitor, nullptr);

	// FOR DEBUGGING AT SMALLER WINDOW SIZES
	//window_width_px = 1280;
	//window_height_px = 720;
	//window = glfwCreateWindow(window_width_px, window_height_px, "StackOverflow", nullptr, nullptr);
	 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	Entity ent = Entity();
	WindowState& windowState = registry.windowStates.emplace(ent);
	windowState.startTime = Clock::now();
	windowState.width = window_width_px;
	windowState.height = window_height_px;
	glfwSetWindowAspectRatio(window,windowState.width,windowState.height);

	if (window == nullptr) {
		fprintf(stderr, "Failed to glfwCreateWindow");
		return nullptr;
	}



	// Setting callbacks to member functions (that's why the redirect is needed)
	// Input is handled using GLFW, for more info see
	// http://www.glfw.org/docs/latest/input_guide.html
	glfwSetWindowUserPointer(window, this);

	std::string title = "StackOverflow";

	glfwSetWindowTitle(window, title.c_str());


	return window;
}

void WorldSystem::init(RenderSystem* renderer_arg, SoundSystem* soundPlayer_arg) {
	this->renderer = renderer_arg;
	// Playing background music indefinitely
	fprintf(stderr, "Loaded music\n");
	this->soundPlayer = soundPlayer_arg;


	// Set all states to default
	GameState& gameState = registry.gameStates.components[0];
	gameState.gameOver = false;
	gameState.gamePaused = false;
	gameState.dialogueScene = false;
	gameState.titleScreen = true;

	WindowState& wS = registry.windowStates.components[0];
	wS.currUnixTime = Clock::now();
	currentSpeed = 1.f;

	player = createPlayer(renderer,{wS.width / 2,wS.height/2});
	aimIndicator = createAimIndicator(renderer);
	cursor = createCursor();

	WindowState& ws = registry.windowStates.components[0];
	createTestFloor(renderer, { ws.width /2, ws.height/2 });
	createRoomBounds(renderer);

	//Entity title = createSkipDialogue();
	//registry.dialogueRequests.emplace(title);

	// mock interactable call instead of proper ui for now
	skipDialogue = createSkipDialogue();
	//registry.dialogueRequests.emplace(skipDialogue);

	//Entity skipDialogue2 = createSkipDialogue();
	//registry.dialogueRequests.emplace(skipDialogue2);
}
#pragma endregion

// Update our game world
bool WorldSystem::step(float elapsed_ms_since_last_update) {
	// Remove debug info from the last step
	// comment this out for now
	//while (registry.debugComponents.entities.size() > 0)
	//    registry.remove_all_components_of(registry.debugComponents.entities.back());

	// Removing out of screen entities
	auto& motions_registry = registry.motions;

	// Remove entities that leave the screen on the left side
	// Iterate backwards to be able to remove without unterfering with the next object to visit
	// (the containers exchange the last element with the current)
	// for (int i = (int)motions_registry.components.size()-1; i>=0; --i) {
	//     Motion& motion = motions_registry.components[i];
	// 	if (motion.position.x + abs(motion.scale.x) < 0.f) {
	// 		if(!registry.players.has(motions_registry.entities[i])) // don't remove the player
	// 			registry.remove_all_components_of(motions_registry.entities[i]);
	// 	}
	// }
	vec2 dashDirection = registry.ioStates.components[0].lastInputAxis;
	GameState& gameState = registry.gameStates.components[0];
	if (!gameState.dialogueScene && !gameState.cutScene) {
		movePlayer();
		//check dash related variables
		dash(dashDirection, elapsed_ms_since_last_update);
		shoot(elapsed_ms_since_last_update, getModifiedValue(BulletNum, registry.players.get(player).bulletCluster));


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
		//check invisibity countdown
		if (registry.invisibles.entities.size() > 0) {
			for (int i = (int)registry.invisibles.components.size()-1; i>=0; --i) {
				Invisible& entity = registry.invisibles.components[i];
				if ((entity.countdown -= elapsed_ms_since_last_update) <= 0) {
					registry.invisibles.remove(registry.invisibles.entities[i]);
				}
			}
		}
		// Updating the bullet ranges
		if (registry.playerBullets.entities.size() > 0) {
			for (int i = (int)registry.playerBullets.components.size()-1; i>=0; --i) {
				PlayerBullet& bullet = registry.playerBullets.components[i];
				if ((bullet.bulletRange -= elapsed_ms_since_last_update) <= 0) {
					if (!registry.deleteds.has(registry.playerBullets.entities[i]))
						registry.deleteds.emplace(registry.playerBullets.entities[i]);
				}
			}
		}

		if (registry.enemyBullets.entities.size() > 0) {
			for (int i = (int)registry.enemyBullets.components.size()-1; i>=0; --i) {
				EnemyBullet& bullet = registry.enemyBullets.components[i];
				if ((bullet.bulletRange -= elapsed_ms_since_last_update) <= 0) {
					// remove enemy bullet
					if (!registry.deleteds.has(registry.enemyBullets.entities[i]))
						registry.deleteds.emplace(registry.enemyBullets.entities[i]);
				}
			}
		}
	}


	//check damage countdown
	if (registry.damageds.entities.size() > 0) {
		for (int i = (int)registry.damageds.components.size()-1; i>=0; --i) {
			Damaged& entity = registry.damageds.components[i];
			if ((entity.countdown -= elapsed_ms_since_last_update) <= 0) {
				registry.damageds.remove(registry.damageds.entities[i]);
			}
		}
	}

	// Critter management
	if (registry.critters.entities.size() > 0) {
		for (int i = (int)registry.critters.components.size() - 1; i >= 0; --i) {
			if (registry.deleteds.has(registry.critters.entities[i])) continue;
			auto& critter = registry.critters.components[i];
			if (critter.startled) {
				critter.life -= elapsed_ms_since_last_update;
				if (registry.animations.get(registry.critters.entities[i]).animation_countdown_base > 100) {
					registry.animations.get(registry.critters.entities[i]).animation_countdown = 50;
					registry.animations.get(registry.critters.entities[i]).animation_countdown_base = 50;
				}
				if (critter.life <= 0) registry.deleteds.emplace(registry.critters.entities[i]);
			}
			else {
				float time = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - registry.windowStates.components[0].startTime).count();
				registry.animations.get(registry.critters.entities[i]).frame = (sin(time/(300.f + i * 25.f) + 3.f * i) < -0.99);
			}
		}
	}

	// Boss parts
	if (!registry.bosses.entities.size() > 0) 
	{
		for (int i = (int)registry.bossParts.components.size() - 1; i >= 0; --i) {
			registry.deleteds.emplace(registry.bossParts.entities[i]);
		}
	}

	// interactible object management placed here and hard coded for now 
	// can consider: each behaviour type is component, when choice X is selected then enact that behaviour
	for (InteractableReaction& reaction : registry.interactableReactions.components) {
		InteractableObject& object = registry.interactables.get(reaction.object);
		if (object.name.compare("PopStack") == 0) { // the choices are known implicitly by person who wrote object script for now
			if (reaction.choice == 0) { // yes
				object.dialogueCount++;
				resetStack(player, renderer);
			}
			else if (reaction.choice == 1) { // no
				// not incrementing allows player to keep asking to pop until pop, but potentially finicky
			}
		}

		if (object.name.compare("BibleTree") == 0) {
			if (reaction.choice == 0) { // yes
				object.dialogueCount++;
			}
			else if (reaction.choice == 1) { // no
				// not incrementing allows player to keep asking to pop until pop, but potentially finicky
			}
		}

		if (object.name.compare("SkipTutorial") == 0) {
			IOState& iostate = registry.ioStates.components[0];
			if (reaction.choice == 0) { // yes
				iostate.tutorialOn = false;
			}
			else if (reaction.choice == 1) { // no
				iostate.tutorialOn = true;
			}
			registry.mapRequests.emplace(player, MapRequestType::RestartGame);
		}

		if (object.name.compare("LockedDoor") == 0) {
			assert(registry.doors.has(reaction.object));

			StackCompile& stack = registry.stackCompile.get(player);
			if (reaction.choice == 0) {
				if (stack.useKey()) {
					soundPlayer->playDoorOpenSound();
					object.name = "OpenDoor";
					object.interactType = InteractableType::ActionInteractable;
					reaction.choice = -1;
				} else { // does not have key, but attempted opening
					DialogueRequest& req = registry.dialogueRequests.emplace(reaction.object);
					req.choice = 2; // use this as temporary way to get back to dialogue system
					if (!gameState.seenLockedDoor) {
						req.choice = 3;
						gameState.seenLockedDoor = true;
					}
				}
			}
		}

		if (object.name.compare("OpenDoor") == 0) {
			assert(registry.doors.has(reaction.object));
			
			if (reaction.choice == 0) {
				registry.mapRequests.emplace(reaction.object, MapRequestType::ChangeRoom, registry.doors.get(reaction.object).room, registry.doors.get(reaction.object).doorIndex);
			}
		}

		if (object.item == InteractableItem::Ram) {
			if (reaction.choice == 0) {
				DialogueRequest& req = registry.dialogueRequests.emplace(reaction.object);
				extendStack( player, 5);
				object.dialogueCount++;
				registry.deleteds.emplace(reaction.object);
			}
		}
		if (object.item == PushConsole) {
			if (reaction.choice == 0) {
				EffectStack& stack = registry.effectStacks.get(reaction.object);
				addEffect(player, stack.stack);
				object.dialogueCount++;
			}
		}
	}

	registry.interactableReactions.clear();

	// place sprite timer progression here for now
	for (auto& entity : registry.spriteTimers.entities) {
		auto& spriteTimer = registry.spriteTimers.get(entity);
		spriteTimer.count_ms -= elapsed_ms_since_last_update;
		if (spriteTimer.count_ms <= 0) {
			//std::cout << " got hit, switch back to normal " << std::endl;
			registry.renderRequests.get(entity).texture_name = spriteTimer.nextSprite;
			registry.renderRequests.get(entity).used_effect = spriteTimer.nextEffect;
			registry.spriteTimers.remove(entity);
		}
	}

	return true;
}

// Reset the world state to its initial state
void WorldSystem::restartGame() {
	printf("Restarting\n");
	// Debugging for memory/component leaks
	// registry.list_all_components();
	GameState& gameState = registry.gameStates.components[0];
	gameState.gameOver = false;
	gameState.gamePaused = false;
	gameState.dialogueScene = false;
	gameState.cutScene = false;
	gameState.seenLockedDoor = false; 
	gameState.loading = false;
	gameState.dialogueChoice = -1;

	//std::cout << ("MyString") << std::endl;
	//std::cout << std::hash<std::string>{}("MyString") << std::endl;

	// Reset the game speed
	currentSpeed = 1.f;

	Entity player = resetPlayer();
	StackUI& stackUI = registry.stackUI.components[0];
	stackUI.updateStackUISize(registry.stackCompile.get(player).baseStackSize);

	// resetting dialogue related stuff
	DialogueLines& lines = registry.dialogueLines.components[0];
	lines = DialogueLines();
	// clear choices here for now
	for (int i = registry.dialogueChoices.size() - 1; i >= 0; i--) {
		Entity e = registry.dialogueChoices.entities[i];
		registry.deleteEntityAndRelatedEntities(e);
	}
	registry.maps.components[0].currRoom.dialogueDone = true;

	// mock interactable call instead of proper ui for now
	//Entity skipDialogue2 = createSkipDialogue();
	registry.dialogueRequests.emplace(skipDialogue);

	//registry.mapRequests.emplace(player,MapRequestType::RestartGame);
}

// Compute collisions between entities
void WorldSystem::handleCollisions() {
	auto& collisionsRegistry = registry.collisions;
	for (uint i = 0; i < collisionsRegistry.components.size(); i++) {
		Entity entity = collisionsRegistry.entities[i];
		Entity entity_other = collisionsRegistry.components[i].other;

		if (registry.deleteds.has(entity) || registry.deleteds.has(entity_other))
			continue;

		// Player centric collision handling
		if (registry.players.has(entity)) {
			// Checking Player - Deadly collisions
			if (!registry.invincibles.has(entity)
				&& (registry.enemies.has(entity_other) || registry.enemyBullets.has(entity_other))) {
				handlePlayerHit(entity_other);
				if (registry.enemyBullets.has(entity_other) 
					&& (!registry.deleteds.has(entity_other))
					&& (registry.enemyBullets.get(entity_other).bulletPierce -= 1) < 0) {
					registry.deleteds.emplace(entity_other);
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

			// check if player is within detection radius of interactible
			// this is for when player is near and has to press E to interact
			if (registry.interactables.has(entity_other)) {
				// bad singleton implementation: only interested in one E so just io system can just grab most recent one
				// consider grabbing nearest one instead
				if (!registry.interactableReactions.has(entity_other))
					registry.nearbyInteractables.emplace(entity_other);
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
					motion.veer = motion.veer - 2 * (glm::dot(motion.veer, n)) * n;

					// Assumes bullet flies towards facing direction
					motion.angle = atan2(motion.velocity.y, motion.velocity.x);

					registry.enemyBullets.get(entity).bulletBounce -= 1;
				}
				else {
					if (!registry.deleteds.has(entity))
						registry.deleteds.emplace(entity);
				}
			}
		}

		// Player bullet centric handling
		if (registry.playerBullets.has(entity)) {
			if (registry.walls.has(entity_other)) {
				Motion& motion = registry.motions.get(entity);
				WallCollider& wall = registry.walls.get(entity_other);
				if (registry.playerBullets.get(entity).bulletBounce > 0) {
					// Bounce / reflect the enemy bullet against the wall
					vec2 a = motion.position - wall.startPosition;
					vec2 b = wall.endPosition - wall.startPosition;
					vec2 c = (glm::dot(a, glm::normalize(b)) * glm::normalize(b));
					vec2 n = glm::normalize(a - c);

					motion.velocity = motion.velocity - 2 * (glm::dot(motion.velocity, n)) * n;
					motion.veer = motion.veer - 2 * (glm::dot(motion.veer, n)) * n;
					// Assumes bullet flies towards facing direction
					motion.angle = atan2(motion.velocity.y, motion.velocity.x);

					registry.playerBullets.get(entity).bulletBounce -= 1;
					registry.ignores.get(entity).clear();
				}
				else {
					//emit wall collision particle
					ParticleProps props = playerBulletCollision;
					props.position.variation = VecOp::rotate(motion.scale,motion.angle);
					EmitParticle& ep = registry.emitParticles.emplace(Entity(),PWallCollision,props,150,2);
					//get impact direction using the velocity of bullet projected onto the normal axis of the wall and take the negative
					ep.defaultPos = motion.position;
					vec2 a = wall.endPosition-wall.startPosition;
					vec2 b = -motion.velocity;
					vec2 p = dot(a,b)/dot(a,a)*a;
					ep.impactDirection = normalize(b-p);
					if (!registry.deleteds.has(entity)) {
						registry.deleteds.emplace(entity);
					}
				}

			}
		}
	}

	// Remove all collisions from this simulation step
	registry.collisions.clear();
}

void WorldSystem::playCutscene() {
	// idea: wait until all animation sequences are done
	if (registry.animationSequences.components.size() == 0) {
		GameState& gameState = registry.gameStates.components[0];
		gameState.cutScene = false;
		Map& map = registry.maps.components[0];
		map.currRoom.cutsceneCount++;
		map.currRoom.cutSceneDone = true;
	}
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

	//move cursor
	Motion& cursorMotion = registry.motions.get(cursor);
	cursorMotion.position = input.mousePosition;

	//change volume
	GameState& gameState = registry.gameStates.components[0];
	soundPlayer->setVolume(gameState.currentVolume);
	
	// clear nearby interactables here for now
	registry.nearbyInteractables.clear();
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
			ParticleProps props = playerTrail;
			props.velocity.base = -(pl.dashSpeed * glm::normalize(dash.dashDirection)) * 0.1f;

			registry.emitParticles.replace(player, ParticleRequestType::PPlayerTrail,props,dash.endTimer, Random::Int(20) + 60);
			soundPlayer->playPlayerDashSound();
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
			if (!registry.invincibles.has(player)) {
				registry.invincibles.emplace(player);
				registry.invincibles.get(player).countdown = dash.endTimer;
				registry.invincibles.get(player).max = dash.endTimer;
			} else {
				registry.invincibles.get(player).countdown = max(registry.invincibles.get(player).countdown, dash.endTimer);
			}

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
	if (pl.currFiringInterval > 0) {
		pl.currFiringInterval -= elapsed_ms_since_last_update;
	}
	if (pl.bulletBurstCooldown > 0) {
		pl.bulletBurstCooldown -= elapsed_ms_since_last_update;
	}
	if (!input.shouldShoot) {
		if (elapsed_ms_since_last_update > 50 && (pl.currBulletBurst < pl.maxBulletBurst)) {
			pl.currBulletBurst++;
		}
		return;
	}

	if (pl.currFiringInterval <= 0) {
		pl.currBulletBurst = getModifiedValue(BulletBurst, pl.maxBulletBurst);
		pl.currFiringInterval = (1 / getModifiedValue(FireRate, 1000 / pl.maxFiringInterval)) * 1000;
	}
	int channel = 1;  // Use a specific channel, e.g., channel 1

	soundPlayer->playPlayerShootSound(max(250.0f, min(
			50.0f,
			((1 / getModifiedValue(FireRate, 1000 / pl.maxFiringInterval)) * 1000) / getModifiedValue(
				BulletBurst, pl.maxBulletBurst))));

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
	RenderRequest& rr = registry.renderRequests.get(player);
	if (glm::length(inputAxis) <= 0.0f) {
		if (!registry.spriteTimers.has(player) && !registry.animationSequences.has(player)) {
			rr.used_effect = EFFECT_ASSET_ID::TEXTURED;
			rr.texture_name = registry.sprites.get(player).sprites[SPRITE_STATE::BASE];
		}
		player_motion.velocity = {0,0};
	} else {
		if (!registry.spriteTimers.has(player) && !registry.animationSequences.has(player)) {
			rr.used_effect = EFFECT_ASSET_ID::ANIMATE;
			rr.texture_name = registry.sprites.get(player).sprites[SPRITE_STATE::MOVING];
			// hard code this for now -- annoying
			registry.animations.get(player).max_frames = 6;
		}
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
	Entity& pl = registry.players.entities[0];
	return max(registry.stackCompile.get(pl).minimums[bf], (value + registry.stackCompile.get(pl).additives[bf]) * registry.stackCompile.get(pl).multiplicatives[bf]);
}

void WorldSystem::handlePlayerHit(Entity& other) {
	//change sprite
	auto& spriteMap = registry.sprites.get(player).sprites;
	if (spriteMap.count(SPRITE_STATE::DAMAGED) && !registry.invincibles.has(player)) {
		registry.renderRequests.get(player).texture_name = spriteMap[SPRITE_STATE::DAMAGED];
		registry.renderRequests.get(player).used_effect = EFFECT_ASSET_ID::TEXTURED;
		if (!registry.spriteTimers.has(player)) {
			auto& spriteTimer = registry.spriteTimers.emplace(player);
			spriteTimer.count_ms = 250;
			spriteTimer.nextSprite = spriteMap[SPRITE_STATE::BASE];
			spriteTimer.nextEffect = EFFECT_ASSET_ID::TEXTURED;
		}
	}

	//play hit sound
	soundPlayer->playPlayerHurtSound();

	//add player invincibility frames
	if (!registry.invincibles.has(player)) {
		registry.invincibles.emplace(player);
		ParticleProps props = playerDamaged;
		registry.emitParticles.replace(player,PExplode, props,100, 1);
	}

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
	else if (registry.enemies.has(other)) {
		Enemy& e = registry.enemies.get(other);
		bool success = registry.stackCompile.get(player).add(e.collisionBullet);
		if (!success) {
			registry.gameStates.components[0].gameOver = true;
		}
	}
}

void WorldSystem::clearDeleteQueue() {
	for (int i = registry.deleteds.size() - 1; i >= 0; i--) {
		Entity e = registry.deleteds.entities[i];
		// right now, all our entities that fade will also emit particles (enemies)
		// but should be generalized for more things in the future
		if (!registry.fades.has(e) || registry.fades.get(e).time <= 0) {
			if (registry.enemyBullets.has(e)) {
				enemyBulletDeath(e);
			}
			registry.deleteEntityAndRelatedEntities(e);
		}
	}
}

void WorldSystem::enemyBulletDeath(Entity e) {
	auto& eb = registry.enemyBullets.get(e);
	auto& ebm = registry.motions.get(e);
	if (eb.onDeath == EnemyBulletDeath::NONE) return;
	if (eb.onDeath == EnemyBulletDeath::EXPLODE) {
		createEnemyBulletDeath(renderer, ebm.position, vec2(0), EnemyBulletDeath::EXPLODE);
		soundPlayer->playExplosionSound(2);
		return;
	}
	else if (eb.onDeath == EnemyBulletDeath::CLUSTER) {
		createEnemyBulletDeath(renderer, ebm.position, vec2( 1, 1), EnemyBulletDeath::CLUSTER);
		createEnemyBulletDeath(renderer, ebm.position, vec2( 1,-1), EnemyBulletDeath::CLUSTER);
		createEnemyBulletDeath(renderer, ebm.position, vec2(-1, 1), EnemyBulletDeath::CLUSTER);
		createEnemyBulletDeath(renderer, ebm.position, vec2(-1,-1), EnemyBulletDeath::CLUSTER);
		soundPlayer->playExplosionSound(0);
		return;
	}
}


