// Header
#include "world_system.hpp"
#include "world_init.hpp"

// stlib
#include <cassert>
#include <sstream>
//#include <glm/detail/func_trigonometric.inl>
#include <SDL.h>
#include <time.h>
#include "sound_system.hpp"
#include "physics_system.hpp"
#include "interactable_effects.h"
#include "components/presets/particle_presets.hpp"

#include <glm/gtx/compatibility.hpp>

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
	//glfwWindowHint(GLFW_SAMPLES, 4);

	// Create the main window (for rendering, keyboard, and mouse input)
	int window_width_px,window_height_px;
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* vidMode = glfwGetVideoMode(monitor);
	window_width_px = vidMode->width;
	window_height_px = window_width_px * (1080.f/1920.f);
	// window_width_px = 1280;
	// window_height_px = 720;
	window_width_px = 1920;
	window_height_px = 1080;
	//window = glfwCreateWindow(window_width_px, window_height_px, "StackOverflow", monitor, nullptr);

	// FOR DEBUGGING AT SMALLER WINDOW SIZES
	//window_width_px = 1280;
	//window_height_px = 720;
	 window = glfwCreateWindow(window_width_px, window_height_px, "StackOverflow", nullptr , nullptr);
	 
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

	Room& currRoom = registry.maps.components[0].currRoom;
	/*vec2 roomSize = { 1920,1080 };*/
	// note: walls only align to floor if fixed to middle of screen rn
	vec2 roomCenter = { wS.width / 2, wS.height / 2 };

	WindowState& ws = registry.windowStates.components[0];
	createWallThickness({ ws.width / 2, ws.height / 2}, currRoom.preset.roomSize);
	createFloor(renderer, { ws.width /2, ws.height/2 }, currRoom.preset.roomSize);

	createRoomBounds(renderer, roomCenter, currRoom.preset.roomSize);

	// mock interactable call instead of proper ui for now
	skipDialogue = createSkipDialogue();

	Camera& camera = registry.cameras.emplace(player);
	camera.target = player;

	registry.gameReports.emplace(player);
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
		//check Mole countdown
		if (registry.moles.entities.size() > 0) {
			for (int i = (int)registry.moles.components.size()-1; i>=0; --i) {
				Mole& entity = registry.moles.components[i];
				if ((entity.countdown -= elapsed_ms_since_last_update) <= 0) {
					registry.moles.remove(registry.moles.entities[i]);
				}
			}
		}

		//check regen countdown
		if (registry.regenerates.entities.size() > 0) {
            for (int i = (int)registry.regenerates.components.size()-1; i>=0; --i) {
                Regenerate& entity = registry.regenerates.components[i];
                if ((entity.countdown -= elapsed_ms_since_last_update) <= 0) {
                    registry.regenerates.remove(registry.regenerates.entities[i]);
                }
            }
        }

		//check vulnerability countdown
		if (registry.vulnerabilities.entities.size() > 0) {
			for (int i = (int)registry.vulnerabilities.components.size()-1; i>=0; --i) {
				auto& entity = registry.vulnerabilities.components[i];
				if ((entity.countdown -= elapsed_ms_since_last_update) <= 0 || registry.instanceDamages.has(registry.vulnerabilities.entities[i])) {
					registry.vulnerabilities.remove(registry.vulnerabilities.entities[i]);
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
				Entity entity = registry.enemyBullets.entities[i];
				if ((bullet.bulletRange -= elapsed_ms_since_last_update) <= 0) {
					// remove enemy bullet
					if (!registry.deleteds.has(registry.enemyBullets.entities[i])) {
						Motion& motion = registry.motions.get(entity);
						ParticleProps props = enemyBulletDeathParticle;
						props.position.variation = VecOp::rotate(motion.scale,motion.angle);
						for (const BulletStackEffect &effect : bullet.bulletEffects)
						{
							BulletEffectType type = effect.type;
							if (type == BulletEffectType::Inert)
								continue;
							if(enemyBulletParticleColors.count(type) > 0) {
								props.colorEffects.push_back({enemyBulletParticleColors.at(type),effect.value});
							} else {
								printf("Warning: enemy bullet color not defined\n");
							}
						}
						if (!props.colorEffects.empty())
						{
							props.position.variation = VecOp::rotate(motion.scale, motion.angle);
							EmitParticle &ep = registry.emitParticles.emplace(Entity(),PExplode,props,150,2);
							ep.defaultPos = motion.position;
						}
						registry.deleteds.emplace(registry.enemyBullets.entities[i]);
					}
				}
			}
		}
	}


	//check damage countdown
	if (!registry.damageds.entities.empty()) {
		for (int i = (int)registry.damageds.components.size()-1; i>=0; --i) {
			Damaged& entity = registry.damageds.components[i];
			if ((entity.countdown -= elapsed_ms_since_last_update) <= 0) {
				registry.damageds.remove(registry.damageds.entities[i]);
			}
		}
	}

	if (!registry.burnTicked.entities.empty()) {
		for (int i = (int)registry.burnTicked.components.size() - 1; i >= 0; --i) {
			BurnTick& entity = registry.burnTicked.components[i];
			if ((entity.countdown -= elapsed_ms_since_last_update) <= 0) {
				registry.burnTicked.remove(registry.burnTicked.entities[i]);
			}
		}
	}

	//check spawn countdown
	if (!registry.spawnings.entities.empty()) {
		for (int i = (int)registry.spawnings.components.size()-1; i>=0; --i) {
			Spawning& entity = registry.spawnings.components[i];
			if ((entity.countdown -= elapsed_ms_since_last_update) <= 0) {
				if (registry.gaugeVisuals.has(registry.spawnings.entities[i])) {
					registry.gaugeVisuals.remove(registry.spawnings.entities[i]); // remove "spawn effect" after done spawning
				}
				registry.spawnings.remove(registry.spawnings.entities[i]);
			}
		}
	}

	//check cloak countdown
	if (!registry.cloaks.entities.empty()) {
		for (int i = (int)registry.cloaks.components.size()-1; i>=0; --i) {
			Cloaked& entity = registry.cloaks.components[i];
			if ((entity.countdown -= elapsed_ms_since_last_update) <= 0) {
				registry.cloaks.remove(registry.cloaks.entities[i]);
			}
		}
	}

	// Critter management
	if (!registry.critters.entities.empty()) {
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
	// if (!registry.bosses.entities.size() > 0)
	// {
	// 	for (int i = (int)registry.bossParts.components.size() - 1; i >= 0; --i) {
	// 		if (!registry.deleteds.has(registry.bossParts.entities[i]))
	// 			registry.deleteds.emplace(registry.bossParts.entities[i]);
	// 	}
	// }

	// Worm body deletion
	if (registry.wormBodies.entities.size() > 0)
	{
		for (int i = (int)registry.wormBodies.components.size() - 1; i >= 0; --i) {
			if (!registry.deleteds.has(registry.wormBodies.entities[i]) && registry.deleteds.has(registry.wormBodies.components[i].head))
				registry.deleteds.emplace(registry.wormBodies.entities[i]);
		}
	}

	interact(elapsed_ms_since_last_update, player, renderer, soundPlayer);

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
	gameState.resetRoom = true;
	gameState.currentVolume = gameState.previousVolume;
	soundPlayer->stopPersistentSounds();
	soundPlayer->stopGameOverSound();

	if (!registry.mapRequests.has(player)) {
		std ::cout << "Restarting game" << std::endl;
		MapRequest& mapReq = registry.mapRequests.emplace(player, MapRequestType::RestartGame);
	}
	
	registry.cameras.components[0].zoom = 1.0f;
	registry.cameras.components[0].startZoom = 1.0f;
	registry.cameras.components[0].startPos = vec2(0);
	registry.cameras.components[0].elapsedTime = 0;
	registry.cameras.components[0].target = player;
	registry.cameraRequests.clear();

	registry.ioStates.components[0].shouldRestart = false;
	registry.ioStates.components[0].lockControls = false;

	// Reset the game speed
	currentSpeed = 1.f;

	registry.maps.components[0].currRoom.dialogueDone = true;

	registry.interactableInDialogue.clear();

	//DialogueRequest& resetReq = registry.dialogueRequests.emplace(player);
	//resetReq.type = DialogueRequestType::ResetDialogue;

	// mock interactable call instead of proper ui for now
	registry.dialogueRequests.emplace(skipDialogue);

	Entity player = resetPlayer();
	StackUI& stackUI = registry.stackUI.components[0];
	stackUI.updateStackUISize(registry.stackCompile.get(player).baseStackSize);

	// reset report
	GameReport& report = registry.gameReports.get(player);
	report.gameStartTime = Clock::now();
	report.roomsCleared = 0;

	UIRequest& uireq = registry.uiRequests.emplace_with_duplicates(player);
	uireq.type = UIRequestType::ResetUI;
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
			if (!registry.invincibles.has(entity) && !registry.spawnings.has(entity_other) && !registry.moles.has(entity_other) && !registry.roomWideBuffers.has(entity_other)
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
			Room& room = registry.maps.components[0].currRoom;
			if (registry.interactables.has(entity_other) && (room.cleared || room.type == RoomType::TutorialRoom1)) {
				// bad singleton implementation: only interested in one E so just io system can just grab most recent one
				// consider grabbing nearest one instead
				if (!registry.interactableReactions.has(entity_other))
					registry.nearbyInteractables.emplace(entity_other);
			}
		}

		// Enemy bullet centric handling
		if (registry.enemyBullets.has(entity)) {
			if (registry.walls.has(entity_other) && !registry.lasers.has(entity)) {
				Motion& motion = registry.motions.get(entity);
				WallCollider& wall = registry.walls.get(entity_other);
				EnemyBullet& bullet = registry.enemyBullets.get(entity);
				if (registry.enemyBullets.get(entity).bulletBounce > 0) {
					// Bounce / reflect the enemy bullet against the wall
					vec2 a = motion.position - wall.startPosition;
					vec2 b = wall.endPosition - wall.startPosition;
					vec2 c = (glm::dot(a, glm::normalize(b)) * glm::normalize(b));
					vec2 n = glm::normalize(a - c);

					motion.position -= normalize(motion.velocity) * max(motion.scale.x,motion.scale.y)/2.f;
					motion.velocity = motion.velocity - 2 * (glm::dot(motion.velocity, n)) * n;
					motion.veer = motion.veer - 2 * (glm::dot(motion.veer, n)) * n;

					// Assumes bullet flies towards facing direction
					motion.angle = atan2(motion.velocity.y, motion.velocity.x);

					registry.enemyBullets.get(entity).bulletBounce -= 1;
				}
				else if (registry.enemyBullets.get(entity).bulletBounce < -1) {
					motion.velocity = vec2(0.f);
					motion.veer = vec2(0.f);
				} else {
					if (!registry.deleteds.has(entity)) {
						//emit wall collision particle
						ParticleProps props = enemyBulletDeathParticle;
						props.position.variation = VecOp::rotate(motion.scale,motion.angle);
						for (const BulletStackEffect &effect : bullet.bulletEffects)
						{
							BulletEffectType type = effect.type;
							if (type == BulletEffectType::Inert)
								continue;
							if(enemyBulletParticleColors.count(type) > 0) {
								props.colorEffects.push_back({enemyBulletParticleColors.at(type),effect.value});
							} else {
								printf("Warning: enemy bullet color not defined\n");
							}
						}
						if (!props.colorEffects.empty())
						{
							// only take into account y scale, so the long rectangle bullets won't have death particles end up in the middle of room
							props.position.variation = VecOp::rotate(vec2(0,motion.scale.y), motion.angle);
							EmitParticle &ep = registry.emitParticles.emplace(Entity(),PWallCollision,props,75,2);
							//get impact direction using the velocity of bullet projected onto the normal axis of the wall and take the negative
							ep.defaultPos = motion.position;
							vec2 a = wall.endPosition-wall.startPosition;
							vec2 b = -motion.velocity;
							vec2 p = dot(a,b)/dot(a,a)*a;
							ep.impactDirection = normalize(b-p);
						}
						registry.deleteds.emplace(entity);
					}
				}
			}
		}

		// Player bullet centric handling
		if (registry.playerBullets.has(entity)) {
			PlayerBullet &bullet = registry.playerBullets.get(entity);
			if (registry.walls.has(entity_other)) {
				Motion& motion = registry.motions.get(entity);
				WallCollider& wall = registry.walls.get(entity_other);
				if (bullet.bulletBounce > 0) {
					if (checkTierThreshold(Bounce) && registry.enemies.entities.size() > 0) {
						motion.position -= normalize(motion.velocity) * max(motion.scale.x, motion.scale.y) / 2.f;
						Entity randomEnemy = Random::ListItem(registry.enemies.entities);
						Motion& rem = registry.motions.get(randomEnemy);

						vec2 goTo = glm::normalize(rem.position - motion.position);
						motion.velocity = goTo * glm::length(motion.velocity);
						motion.veer = vec2(0);
						//motion.angle = atan2(motion.velocity.y, motion.velocity.x);
					}
					else {
						// Bounce / reflect the enemy bullet against the wall
						vec2 a = motion.position - wall.startPosition;
						vec2 b = wall.endPosition - wall.startPosition;
						vec2 c = (glm::dot(a, glm::normalize(b)) * glm::normalize(b));
						vec2 n = glm::normalize(a - c);

						motion.position -= normalize(motion.velocity) * max(motion.scale.x, motion.scale.y) / 2.f;
						motion.velocity = motion.velocity - 2 * (glm::dot(motion.velocity, n)) * n;
						motion.veer = motion.veer - 2 * (glm::dot(motion.veer, n)) * n;
						// Assumes bullet flies towards facing direction
						//motion.angle = atan2(motion.velocity.y, motion.velocity.x);
					}
					bullet.bulletBounce -= 1;
					bullet.bulletRange = getModifiedValue(BulletRange, PlayerBullet().bulletRange) / 1.5f; //refresh range
					registry.ignores.get(entity).clear();
				}
				else {
					if (!registry.deleteds.has(entity)) {
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
						registry.deleteds.emplace(entity);
					}
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
		restartGame();
	}

	//move cursor
	Motion& cursorMotion = registry.motions.get(cursor);
	cursorMotion.position = input.mousePosition;

	//change musicVolume
	GameState& gameState = registry.gameStates.components[0];
	soundPlayer->setMusicVolume(gameState.currentVolume);
	soundPlayer->setSFXVolume(gameState.currentSfxVolume);

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
            pl.currDashCooldown = getModifiedValue(PlayerDashRecharge, pl.dashCooldown);
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

	WindowState& windowState = registry.windowStates.components[0];
	Camera& camera = registry.cameras.components[0];
	Room& room = registry.maps.components[0].currRoom;
	float clampAmount = 1.0f;

	// need to shift mouse by player actual pos -> translate into world pos
	// ideally, this is only done once in the move player and the mouse position is passed to shoot
	// (actual mouse position shouldn't be changed though, the ui mouse picking needs it to be w/ respect to window)
	vec2 offset = { windowState.width / 2, windowState.height / 2 };
	vec2 mousePositionWorld = input.mousePosition - offset + player_motion.position;
	vec2 bulletDir = glm::normalize(mousePositionWorld - player_motion.position);

	//std::cout << "Mouse pos: " << mousePositionWorld.x << ", " << mousePositionWorld.y << std::endl;
	//std::cout << "Player pos: " << player_motion.position.x << ", " << player_motion.position.y << std::endl;
	
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

	vec2 bulletPos = playerPos + bulletDir;
	float angle = atan2(bulletDir.y, bulletDir.x);
	float angleOffset = radians(getModifiedValue(BulletAccuracy, 20));

	//// Tom's thingy
	//if (checkTierThreshold(FireRate) && Random::Float() < 0.15f && registry.enemies.entities.size() - registry.boids.entities.size() > 0) {
	//	float randomDirection = 2.f * M_PI * Random::Float();
	//	vec2 randomOffset = vec2(cos(randomDirection), sin(randomDirection)) * (float)(rand() % 200);
	//	createNTentaclePlayerBullet(renderer, 1, bulletPos, bulletDir, 400);
	//}

	if (pl.currFiringInterval <= 0) {
		pl.currBulletBurst = getModifiedValue(BulletBurst, pl.maxBulletBurst);
		pl.currFiringInterval = getModifiedValue(FireRate, pl.maxFiringInterval);
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

		if (checkTierThreshold(BulletNum)) {
			createNGenericPlayerBullet(renderer, 4 * (1 + getEffectValueTierThresholdDifference(BulletNum)), bulletPos, { cos(angle), sin(angle) }, 500);
		}

		if (cluster % 2 == 0) {
			for (int i = 0; i < cluster / 2; i++) {
				float a1 = angle + (i + 0.5) * angleOffset;
				float a2 = angle - (i + 0.5) * angleOffset;
				createPlayerBullet(renderer, bulletPos, { cos(a1), sin(a1) });
				createPlayerBullet(renderer, bulletPos, { cos(a2), sin(a2) });
				soundPlayer->playPlayerShootSound(max(250.0f, min(
					50.0f,
					((1 / getModifiedValue(FireRate, 1000 / pl.maxFiringInterval)) * 1000) / getModifiedValue(
						BulletBurst, pl.maxBulletBurst))));
			}
		} else {
			createPlayerBullet(renderer, bulletPos, bulletDir);
			soundPlayer->playPlayerShootSound(max(250.0f, min(
				50.0f,
				((1 / getModifiedValue(FireRate, 1000 / pl.maxFiringInterval)) * 1000) / getModifiedValue(
					BulletBurst, pl.maxBulletBurst))));
			for (int i = 0; i < (cluster - 1) / 2; i++) {
				float a1 = angle + (i + 1) * angleOffset;
				float a2 = angle - (i + 1) * angleOffset;
				createPlayerBullet(renderer, bulletPos, { cos(a1), sin(a1) });
				createPlayerBullet(renderer, bulletPos, { cos(a2), sin(a2) });
				soundPlayer->playPlayerShootSound(max(250.0f, min(
					50.0f,
					((1 / getModifiedValue(FireRate, 1000 / pl.maxFiringInterval)) * 1000) / getModifiedValue(
						BulletBurst, pl.maxBulletBurst))));
			}
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

	WindowState& windowState = registry.windowStates.components[0];
	Camera& camera = registry.cameras.components[0];
	Room& room = registry.maps.components[0].currRoom;
	float clampAmount = 1.0f;
	vec2 offset = { windowState.width / 2, windowState.height / 2 };
	vec2 mousePositionWorld = input.mousePosition - offset + player_motion.position;
	vec2 bulletDir = glm::normalize(mousePositionWorld - player_motion.position);
	vec2 diff = mousePositionWorld - player_motion.position;
	float range = 50.0f;
	aimMotion.angle = atan(diff.y,diff.x)+M_PI/4;
	aimMotion.position = player_motion.position + glm::normalize(diff) * range;
}

void WorldSystem::handlePlayerHit(Entity& other) {
	std::vector<BulletStackEffect> effects;
	if (registry.enemyBullets.has(other)) {
		EnemyBullet& eBullet = registry.enemyBullets.get(other);
		effects = eBullet.bulletEffects;
		if (eBullet.isSpecial) { // kept effect same as original code
			registry.maps.components[0].currRoom.preset.numSpecialBulletsToSpawn -= effects.size();
		}
	}
	else if (registry.enemies.has(other)) {
		// Boid touch player, it die
		if (registry.boids.has(other) && !registry.deleteds.has(other)) registry.deleteds.emplace(other);
		// Handle case of boid worm
		if (registry.wormHeads.has(other) && registry.wormBodies.entities.size() > 0)
		{
			for (int i = (int)registry.wormBodies.components.size() - 1; i >= 0; --i) {
				if (!registry.deleteds.has(registry.wormBodies.entities[i]) && registry.deleteds.has(registry.wormBodies.components[i].head))
					registry.deleteds.emplace(registry.wormBodies.entities[i]);
			}
		}
		Enemy& e = registry.enemies.get(other);
		effects.insert(effects.end(), e.collisionBullet.begin(), e.collisionBullet.end());
	}

	if (checkTierThreshold(PlayerDashRecharge) && effects.size() == 1 && effects[0].type == Inert) {
		soundPlayer->playPlayerDodgeSound(Random::Int(2));
		ParticleProps props = playerTrail;
		registry.emitParticles.replace(player, PExplode, props, 100, 1);
		if (!registry.invincibles.has(player)) {
			registry.invincibles.emplace(player);
		}
		return;
	}

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
	if (registry.enemyBullets.has(other) && registry.enemyBullets.get(other).bulletEffects[0].type == Lightning) {
		soundPlayer->playPlayerZappedSound();
	} else
		soundPlayer->playPlayerHurtSound();

	//add player invincibility frames
	if (!registry.invincibles.has(player)) {
		registry.invincibles.emplace(player);
	}

	// check tutorial condition - unlock controls if hit in tutorial
	if (registry.ioStates.components[0].lockControls) {
		DialogueRequest& req = registry.dialogueRequests.emplace(registry.players.entities[0]);
		req.type = DialogueRequestType::StoryDialogue;
		registry.ioStates.components[0].lockControls = false;
	}

	//add to stack for enemy bullets
	for (int i = 0; i < effects.size(); i++) {
		if (effects[i].type == BulletEffectType::Pop) {
			InteractableRequest& req = registry.interactableRequests.emplace(Entity());
			req.type = InteractableRequestType::PopStack;
			req.choice = effects[i].value;
		}
		else {
			bool success = registry.stackCompile.get(player).add(effects[i]);
			if (!success) {
				GameState& gameState = registry.gameStates.components[0];
				gameState.gameOver = true;
				gameState.currentVolume *= 0.15f;
				soundPlayer->playGameOverSound();
				soundPlayer->stopPersistentSounds();
				soundPlayer->setMusicVolume(gameState.currentVolume);
				UIRequest& req = registry.uiRequests.emplace_with_duplicates(player);
				req.type = UIRequestType::GameOverReport;
			}
		}
	}

	// emit particles and send stack notifs
	ParticleProps props;
	if (registry.enemyBullets.has(other) && registry.enemyBullets.get(other).bulletEffects[0].type == Lightning) {
		if (registry.enemyBullets.get(other).bulletEffects[0].value == 1) {
			//yellow is multiplicative
			props = playerZappedYellow;
		}
		else {
			props = playerZappedBlue;
		}
	}
	else {
		props = playerDamaged;
	}
	// don't send notif if game is over for now
	if (!registry.gameStates.components[0].gameOver) {
		UIRequest& req = registry.uiRequests.emplace_with_duplicates(player);
		req.type = UIRequestType::StackNotifBullet;
		req.effects = effects;
	}
	registry.emitParticles.replace(player, PExplode, props, 100, 1);
}

void WorldSystem::clearDeleteQueue() {
	for (int i = registry.deleteds.size() - 1; i >= 0; i--) {
		Entity e = registry.deleteds.entities[i];

		// right now, all our entities that fade will also emit particles (enemies)
		// but should be generalized for more things in the future
		if (!registry.fades.has(e) || registry.fades.get(e).time <= 0) {
			if (registry.enemyBullets.has(e) && !registry.gameStates.components[0].resetRoom) {
				enemyBulletDeath(e);
			}

			// Player bullet explodes if ProjectileSize threshold
			if (registry.playerBullets.has(e) && !registry.playerBullets.get(e).generic && checkTierThreshold(ProjectileSize) && !registry.gameStates.components[0].resetRoom) {
				Motion& pBM = registry.motions.get(e);
				createNGenericPlayerBullet(renderer, 3 + (rand() % (2 + getEffectValueTierThresholdDifference(ProjectileSize))), pBM.position, pBM.velocity);
			}
			registry.deleteEntityAndRelatedEntities(e);
		}
	}

	if (registry.gameStates.components[0].resetRoom) {
		if (registry.enemyBullets.entities.size() == 0) {
			registry.gameStates.components[0].resetRoom = false;
		}
		//causes empty room error, check if this is needed
		// else {
		// 	if (!registry.mapRequests.has(player)) {
		// 		std::cout << "clearing enemy bullets" << std::endl;
		// 		MapRequest& mapReq = registry.mapRequests.emplace(player, MapRequestType::RestartGame);
		// 	}
		// }
		
	}
}

void WorldSystem::enemyBulletDeath(Entity e) {
	auto& eb = registry.enemyBullets.get(e);
	auto& ebm = registry.motions.get(e);

	if (eb.onDeath == EnemyBulletDeath::NONE) return;
	if (eb.onDeath == EnemyBulletDeath::EXPLODE) {
		Entity ed = createEnemyBulletDeath(renderer, ebm.position, vec2(0), EnemyBulletDeath::EXPLODE);
		Motion& edMotion = registry.motions.get(ed);
		ParticleProps props = enemyBulletExplosion;
		auto& ep = registry.emitParticles.emplace(Entity(),PExplode,props,100,400);
		ep.defaultPos = edMotion.position;
		soundPlayer->playExplosionSound(2);
		return;
	}
	else if (eb.onDeath == EnemyBulletDeath::CLUSTER) {
		createEnemyBulletDeath(renderer, ebm.position, vec2( 1, 1), EnemyBulletDeath::CLUSTER);
		createEnemyBulletDeath(renderer, ebm.position, vec2( 1,-1), EnemyBulletDeath::CLUSTER);
		createEnemyBulletDeath(renderer, ebm.position, vec2(-1, 1), EnemyBulletDeath::CLUSTER);
		createEnemyBulletDeath(renderer, ebm.position, vec2(-1,-1), EnemyBulletDeath::CLUSTER);
		//soundPlayer->playExplosionSound(0);
		return;
	}
}


