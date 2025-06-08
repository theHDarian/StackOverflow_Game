// internal
#include "ai_system.hpp"
#include "tiny_ecs_registry.hpp"
#include "common.hpp"
#include "actor_components.hpp"
#include "io_components.hpp"
#include "presets/particle_presets.hpp"
#include <iostream>
#include <random>
#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/spline.hpp>
#include <glm/gtx/norm.hpp>

// returns a vec4(min position, max position)
vec4 getRoomBounds(Entity entity)
{
	Map &map = registry.maps.components[0];
	vec2 roomStartPos = map.currRoom.roomStart;
	vec2 roomEndPos = map.currRoom.roomEnd;

	vec2 scale = abs(registry.motions.get(entity).scale);
	//scale = vec2(0); // TEST IF THIS IS OKAY Edit: this is not okay, enemies clip into walls
	float offset = 0.f;

	// can manually offset by 40 to avoid top wall clipping, because walls are off by 40 to let player walk up to wall
	offset = 40.f;

	vec2 min = vec2(roomStartPos.x + scale.x / 2.f, roomStartPos.y + scale.y/2.f + offset); 
	vec2 max = vec2(roomEndPos.x - scale.x / 2.f, roomEndPos.y - scale.y/2.f);
	
	return vec4(min, max);
}

vec2 boundPosition(vec2 position, Entity entity)
{
	vec4 roomBounds = getRoomBounds(entity);
	vec2 min = {roomBounds.x, roomBounds.y};
	vec2 max = {roomBounds.z, roomBounds.w};
	return glm::clamp(position, min, max);
}

void handleSpecialStates (const EnemyPattern &currPattern, Entity entity)
{

	switch (currPattern.specialState) {
		case SpecialStates::INVINCIBLE:
			if (!registry.invincibles.has(entity)) {
				auto& inv = registry.invincibles.emplace(entity);
					inv.countdown = currPattern.curDuration;
			} else {
				auto& inv = registry.invincibles.get(entity);
				if (inv.countdown < currPattern.curDuration) {
					inv.countdown = currPattern.curDuration;
				}
			}
		break;
		case SpecialStates::INVISIBLE:
			if (!registry.invisibles.has(entity)) {
				auto& inv = registry.invisibles.emplace(entity);

				inv.countdown = currPattern.curDuration;

			} else {
				auto& inv = registry.invisibles.get(entity);
				if (inv.countdown < currPattern.curDuration) {
					inv.countdown = currPattern.curDuration;
				}
			}
		break;
		case SpecialStates::PROTECTED:
			if (registry.vulnerabilities.has(entity)) {
				auto& vul = registry.vulnerabilities.get(entity);
				if (vul.countdown < currPattern.curDuration || vul.modifier != 0.5) {
					vul.countdown = currPattern.curDuration;
				}
				vul.modifier = checkTierThreshold(Pierce) ? 0.75 : 0.5;
			} else {
				auto& vul = registry.vulnerabilities.emplace(entity);
				vul.countdown = currPattern.curDuration;
				vul.modifier = checkTierThreshold(Pierce) ? 0.75 : 0.5;
			}
		break;
		case SpecialStates::VULNERABLE:
			if (registry.vulnerabilities.has(entity)) {
				auto& vul = registry.vulnerabilities.get(entity);
				if (vul.countdown < currPattern.curDuration || vul.modifier != 2.f) {
					vul.countdown = currPattern.curDuration;
				}
				vul.modifier = checkTierThreshold(Pierce) ? 1.75 : 1.5;
			} else {
                auto& vul = registry.vulnerabilities.emplace(entity);
                vul.countdown = currPattern.curDuration;
                vul.modifier = checkTierThreshold(Pierce) ? 1.75 : 1.5;
            }
		break;
		case SpecialStates::UNDERGROUND:
			if (!registry.moles.has(entity)) {
				auto& under = registry.moles.emplace(entity);
				under.countdown = currPattern.curDuration;
				auto& req = registry.soundRequests.emplace(Entity());
				req.type = SoundType::DiggingSound;
				req.ticks = currPattern.curDuration;
				req.sourceEntity = entity;
			} else {
				auto& under = registry.moles.get(entity);
				if (under.countdown < currPattern.curDuration) {
					under.countdown = currPattern.curDuration;
				}
			}
		break;
		case SpecialStates::REGENERATING:
			if (!registry.regenerates.has(entity)) {
				auto& regen = registry.regenerates.emplace(entity);
				regen.countdown = currPattern.curDuration;
				if (registry.healers.has(entity)) {
					regen.healAmount = registry.healers.get(entity).healPower;
				} else {
					regen.healAmount = registry.enemies.get(entity).maxHealth * 0.015;
				}
			}
		break;
		case SpecialStates::CLOAKED:
			if (!registry.cloaks.has(entity)) {
				auto& cloak = registry.cloaks.emplace(entity);
				cloak.countdown = currPattern.curDuration;
			} else {
				auto& cloak = registry.cloaks.get(entity);
				if (cloak.countdown < currPattern.curDuration) {
					cloak.countdown = currPattern.curDuration;
				}
			}
		break;
		case SpecialStates::CLEAR_ALL:
			if (registry.invincibles.has(entity)) {
				auto& inv = registry.invincibles.get(entity);
				inv.countdown = -1;
			}
			if (registry.invisibles.has(entity)) {
				auto& inv = registry.invisibles.get(entity);
				inv.countdown = -1;
			}
			if (registry.vulnerabilities.has(entity)) {
				auto& vul = registry.vulnerabilities.get(entity);
				vul.countdown = -1;
			}
			if (registry.moles.has(entity)) {
				auto& under = registry.moles.get(entity);
				under.countdown = -1;
				if (registry.persistentSounds.has(entity))
					registry.persistentSounds.get(entity).stopChannel(SoundType::DiggingSound);
			}
			if (registry.regenerates.has(entity)) {
				auto& under = registry.regenerates.get(entity);
				under.countdown = -1;
			}
			if (registry.cloaks.has(entity)) {
				auto& cloak = registry.cloaks.get(entity);
				cloak.countdown = -1;
			}
			break;
		case SpecialStates::CLEAR_INVINCIBLE:
			if (registry.invincibles.has(entity)) {
				auto& inv = registry.invincibles.get(entity);
				inv.countdown = -1;
			}
			break;
		case SpecialStates::CLEAR_INVISIBLE:
			if (registry.invisibles.has(entity)) {
				auto& inv = registry.invisibles.get(entity);
				inv.countdown = -1;
			}
			break;
		case SpecialStates::CLEAR_PROTECTED:
		case SpecialStates::CLEAR_VULNERABLE:
			if (registry.vulnerabilities.has(entity)) {
				auto& vul = registry.vulnerabilities.get(entity);
				vul.countdown = -1;
			}
			break;
		case SpecialStates::CLEAR_CLOAKED:
			if (registry.cloaks.has(entity)) {
				auto& cloak = registry.cloaks.get(entity);
				cloak.countdown = -1;
			}
			break;
		case SpecialStates::CLEAR_REGENERATING:
			if (registry.regenerates.has(entity)) {
				auto& under = registry.regenerates.get(entity);
				under.countdown = -1;
			}
			break;
		case SpecialStates::CLEAR_UNDERGROUND:
			if (registry.moles.has(entity)) {
				auto& under = registry.moles.get(entity);
				under.countdown = -1;
				if (registry.persistentSounds.has(entity))
					registry.persistentSounds.get(entity).stopChannel(SoundType::DiggingSound);
			}
			break;
		default: break;
	}
}

void handleSpecialStates (const Reaction &reaction, Entity entity)
{
	switch (reaction.specialState) {
		case SpecialStates::INC_ANIM:
			std::cout << "INC" << std::endl;
			if (registry.animations.has(entity)) {
				auto& anim = registry.animations.get(entity);
				std::cout << "b4:" << anim.frame << std::endl;
				anim.frame = (anim.frame + 1) % anim.max_frames;
				std::cout << "aft:" << anim.frame << std::endl;
			}
			break;
		case SpecialStates::INVINCIBLE:
			if (!registry.invincibles.has(entity)) {
				auto& inv = registry.invincibles.emplace(entity);
				inv.max = registry.bosses.has( entity ) ? (int)registry.maps.components[0].currRegion* (Random::Float( 2500) + 2500.f) : Random::Float( 12000 ) + 3000;
				inv.countdown = inv.max;
			} else {
				auto& inv = registry.invincibles.get(entity);
				float countdown = registry.bosses.has( entity ) ? (int)registry.maps.components[0].currRegion* (Random::Float( 2500) + 2500.f) : Random::Float( 12000 ) + 3000;
				if (countdown > inv.countdown) {
					inv.countdown = countdown;
				}
			}
		break;
		case SpecialStates::INVISIBLE:
			if (!registry.invisibles.has(entity)) {
				auto inv = registry.invisibles.emplace(entity);
				inv.countdown =  registry.bosses.has( entity ) ? (int)registry.maps.components[0].currRegion* (Random::Float( 2500) + 2500.f) : Random::Float( 12000 ) + 3000;
			} else {
				auto& inv = registry.invisibles.get(entity);
				float countdown =  registry.bosses.has( entity ) ? (int)registry.maps.components[0].currRegion* (Random::Float( 2500) + 2500.f) : Random::Float( 12000 ) + 3000;
				if (countdown > inv.countdown) {
					inv.countdown = countdown;
				}
			}
		break;
		case SpecialStates::PROTECTED:
			if (registry.vulnerabilities.has(entity)) {
				auto& vul = registry.vulnerabilities.get(entity);
				vul.countdown =  registry.bosses.has( entity ) ? (int)registry.maps.components[0].currRegion* (Random::Float( 5000) + 5000.f) : Random::Float( 10000 ) + 10000;
				vul.modifier = checkTierThreshold(Pierce) ? 0.75 : 0.5;
			} else {
				auto& vul = registry.vulnerabilities.emplace(entity);
				float countdown =  registry.bosses.has( entity ) ? (int)registry.maps.components[0].currRegion* (Random::Float( 5000) + 5000.f) : Random::Float( 10000 ) + 10000;
				if (countdown > vul.countdown || vul.modifier != 0.5) {
					vul.countdown = countdown;
				}
				vul.modifier = checkTierThreshold(Pierce) ? 0.75 : 0.5;
			}
		break;
		case SpecialStates::VULNERABLE:
			if (registry.vulnerabilities.has(entity)) {
				auto& vul = registry.vulnerabilities.get(entity);
				vul.countdown =  registry.bosses.has( entity ) ? (int)registry.maps.components[0].currRegion* (Random::Float( 5000) + 5000.f) : Random::Float( 10000 ) + 10000;
				vul.modifier = checkTierThreshold(Pierce) ? 1.75 : 1.5;
			} else {
                auto& vul = registry.vulnerabilities.emplace(entity);
                float countdown =  registry.bosses.has( entity ) ? (int)registry.maps.components[0].currRegion* (Random::Float( 5000) + 5000.f) : Random::Float( 10000 ) + 10000;
				if (countdown > vul.countdown || vul.modifier != 2.f) {
					vul.countdown = countdown;
				}
                vul.modifier = checkTierThreshold(Pierce) ? 1.75 : 1.5;
            }
		break;
		case SpecialStates::UNDERGROUND:
			if (!registry.moles.has(entity)) {
				auto& under = registry.moles.emplace(entity);
				under.countdown =  registry.bosses.has( entity ) ? (int)registry.maps.components[0].currRegion* (Random::Float( 5000) + 5000.f) : Random::Float( 10000 ) + 10000;
				auto& req = registry.soundRequests.emplace(Entity());
				req.type = SoundType::DiggingSound;
				req.ticks = under.countdown;
				req.sourceEntity = entity;
			} else {
				auto& under = registry.moles.get(entity);
				float countdown =  registry.bosses.has( entity ) ? (int)registry.maps.components[0].currRegion* (Random::Float( 5000) + 5000.f) : Random::Float( 10000 ) + 10000;
				if (countdown > under.countdown) {
					under.countdown = countdown;
				}
			}
		break;
		case SpecialStates::REGENERATING:
			if (!registry.regenerates.has(entity)) {
				auto& under = registry.regenerates.emplace(entity);
				under.countdown =  registry.bosses.has( entity ) ? (int)registry.maps.components[0].currRegion* (Random::Float( 5000) + 5000.f) : Random::Float( 10000 ) + 10000;
				if (registry.healers.has(entity)) {
					under.healAmount = registry.healers.get(entity).healPower;
				} else {
					under.healAmount = registry.enemies.get(entity).maxHealth * 0.015;
				}
			} else {
				auto& under = registry.regenerates.get(entity);
				float countdown =  registry.bosses.has( entity ) ? (int)registry.maps.components[0].currRegion* (Random::Float( 5000) + 5000.f) : Random::Float( 10000 ) + 10000;
				if (countdown > under.countdown) {
					under.countdown = countdown;
				}
			}
		break;
		case SpecialStates::CLOAKED:
			if (!registry.cloaks.has(entity)) {
				auto& cloak = registry.cloaks.emplace(entity);
				cloak.countdown =  registry.bosses.has( entity ) ? (int)registry.maps.components[0].currRegion* (Random::Float( 5000) + 5000.f) : Random::Float( 10000 ) + 10000;
			} else {
				auto& cloak = registry.cloaks.get(entity);
				float countdown =  registry.bosses.has( entity ) ? (int)registry.maps.components[0].currRegion* (Random::Float( 5000) + 5000.f) : Random::Float( 10000 ) + 10000;
				if (countdown > cloak.countdown) {
					cloak.countdown = countdown;
				}
			}
		break;

		case SpecialStates::CLEAR_ALL:
			if (registry.invincibles.has(entity)) {
				auto& inv = registry.invincibles.get(entity);
				inv.countdown = -1;
			}
			if (registry.invisibles.has(entity)) {
				auto& inv = registry.invisibles.get(entity);
				inv.countdown = -1;
			}
			if (registry.vulnerabilities.has(entity)) {
				auto& vul = registry.vulnerabilities.get(entity);
				vul.countdown = -1;
			}
			if (registry.moles.has(entity)) {
				auto& under = registry.moles.get(entity);
				under.countdown = -1;
				if (registry.persistentSounds.has(entity))
					registry.persistentSounds.get(entity).stopChannel(SoundType::DiggingSound);
			}
			if (registry.regenerates.has(entity)) {
				auto& under = registry.regenerates.get(entity);
				under.countdown = -1;
			}
			if (registry.cloaks.has(entity)) {
				auto& cloak = registry.cloaks.get(entity);
				cloak.countdown = -1;
			}
		break;
		case SpecialStates::CLEAR_INVINCIBLE:
			if (registry.invincibles.has(entity)) {
				auto& inv = registry.invincibles.get(entity);
				inv.countdown = -1;
			}
		break;
		case SpecialStates::CLEAR_INVISIBLE:
			if (registry.invisibles.has(entity)) {
				auto& inv = registry.invisibles.get(entity);
				inv.countdown = -1;
			}
		break;
		case SpecialStates::CLEAR_PROTECTED:
		case SpecialStates::CLEAR_VULNERABLE:
			if (registry.vulnerabilities.has(entity)) {
				auto& vul = registry.vulnerabilities.get(entity);
				vul.countdown = -1;
			}
		break;
		case SpecialStates::CLEAR_CLOAKED:
			if (registry.cloaks.has(entity)) {
				auto& cloak = registry.cloaks.get(entity);
				cloak.countdown = -1;
			}
		break;
		case SpecialStates::CLEAR_REGENERATING:
			if (registry.regenerates.has(entity)) {
				auto& under = registry.regenerates.get(entity);
				under.countdown = -1;
			}
		break;
			case SpecialStates::CLEAR_UNDERGROUND:
			if (registry.moles.has(entity)) {
				auto& under = registry.moles.get(entity);
				under.countdown = -1;
				if (registry.persistentSounds.has(entity))
					registry.persistentSounds.get(entity).stopChannel(SoundType::DiggingSound);
			}
		break;
		default: break;
	}
}

void AISystem::step(float elapsed_ms)
{
	auto &movement_registry = registry.enemyMovement;
	auto &enemy_registry = registry.enemies;
	// std::cout << enemy_registry.entities.size() << " is the size of enemy entity" << std::endl;
	for (Entity entity : enemy_registry.entities)
	{

		Enemy &enemy = enemy_registry.get(entity);
		EnemyPattern &currPattern = enemy.currEnemyPattern();
		enemy.newPattern = false;
		// std::cout << movement_registry.entities.size() << " is the size of movement entity" << std::endl;
		// std::cout << currPattern.name << " initial" << std::endl;
		EnemyMovement &movement = movement_registry.get(entity);
		Motion &motion = registry.motions.get(entity);
		currPattern.curDuration -= elapsed_ms;
		// SENSING
		updateState(enemy, movement, entity);
		// if (enemy.newPattern) {
		// 	currPattern = enemy.currEnemyPattern();
		// }
		handleSpecialStates( currPattern, entity );
		// std::cout << enemy.newPattern << std::endl;
		// std::cout << currPattern.name << "after update" << std::endl;

		if (registry.regenerates.has(entity) && !registry.instanceDamages.has(entity)) {
			auto& regen = registry.regenerates.get(entity);
			if ((regen.currHealInterval -= elapsed_ms) <= 0) {
				enemy.currHealth += regen.healAmount;
				if (enemy.currHealth > enemy.maxHealth) enemy.currHealth = enemy.maxHealth;
				regen.currHealInterval = regen.healInterval;
			}
		}


		if (registry.boids.has(entity))
		{
			Boid &boid = registry.boids.get(entity);
			computeBoidVelocity(entity, boid);
			if (!registry.wormHeads.has(entity)) continue;
		}

		if (currPattern.type == EnemyBehavior::ROLLING) {
			movement.posB = getRollingPos(entity);
		} else if (registry.wormBodies.has(entity) || registry.wormHeads.has(entity)) {
			auto& wormHead_registry = registry.wormHeads;
			auto& wormBody_registry = registry.wormBodies;

			// Update all segment locations
			if (wormHead_registry.has(entity)) {
				WormHead& head = wormHead_registry.get(entity);
				float dist = 0.f;
				// Move target location
				switch (currPattern.type) {
				case EnemyBehavior::WORM_FOLLOW:
					head.points[0] = moveTowards(head.points[0], registry.motions.get(registry.players.entities[0]).position, 1.5f * enemy.speedMultiplier);
					break;
				case EnemyBehavior::WORM_PATROL:
					// Worm will Teleport to first position in spline if not there
					// Remedy using WORM_GOTO
					//head.points[0] = catmullRomSplineLerp(currPattern.path, movement.t);
					//currPattern.pathIndex = currPattern.path.size() - 1;
					//movement.t += 0.01 * enemy.speedMultiplier;
					//if (movement.t > movement.points.size()) movement.t -= (float)currPattern.path.size();
					head.points[0] = moveTowards(head.points[0], lerpToRoom(currPattern.path[currPattern.pathIndex]), 3.f * enemy.speedMultiplier);
					if (glm::distance(lerpToRoom(currPattern.path[currPattern.pathIndex]), head.points[0]) < 0.5) getNextPatrolPos(entity);
					break;
				case EnemyBehavior::WORM_RANDOM:
					// TODO
					// Should choose 1-4 points ahead of worm to draw curved path
					head.points[0] = catmullRomSpline(movement.points, movement.t);
					movement.t += 0.01 * enemy.speedMultiplier;
					if (movement.t > movement.points.size()) movement.t -= (float)movement.points.size();
					break;
				case EnemyBehavior::WORM_GOTO:
					head.points[0] = moveTowards(head.points[0], lerpToRoom(currPattern.path[currPattern.path.size() - 1]), 5.f * enemy.speedMultiplier);
					currPattern.pathIndex = currPattern.path.size() - 1;
					movement.t = (movement.t > 1.f) ? 0.f : movement.t + 0.0001 * enemy.speedMultiplier;
					break;
				case EnemyBehavior::IDLE:
					movement.t = 0.f;
					break;
				default:
					if (registry.boids.has(entity)) head.points[0] = registry.boids.get(entity).position;
				}

				// Update constraints
				for (int i = 1; i < head.size + 1; i++) {
					//Pull the next segment to the previous one
					head.points[i] = constrainDistance(head.points[i], head.points[i - 1], head.constrainDistance);
				}
				if (head.anchor) {
					head.points[head.size] = constrainDistance(head.points[head.size], head.anchorPoint, head.constrainDistance);
					for (int i = head.size - 1; i >= 0; i--) {
						//Pull the next segment to the previous one
						head.points[i] = constrainDistance(head.points[i], head.points[i+1], head.constrainDistance);
					}
				}

				// Move head enemy
				vec2 direction = (head.points[0] - head.points[1]);
				motion.position = head.points[1] + 0.5f * direction;
				motion.angle = atan2(direction.y, direction.x);
				if (enemy.rotationBehaviour == EnemyRotationBehavior::FACE_UP) motion.angle = 0;
			}
			else {
				// Move body enemy
				WormBody& body = wormBody_registry.get(entity);
				WormHead& head = wormHead_registry.get(body.head);

				vec2 direction = (head.points[body.index] - head.points[body.index + 1]);
				motion.position = head.points[body.index + 1] + 0.5f * direction;
				motion.angle = atan2(direction.y, direction.x);
			}

		} else if (currPattern.type == EnemyBehavior::FOLLOWSCIENTIST || currPattern.type == EnemyBehavior::IDLE || currPattern.type == EnemyBehavior::FOLLOW_PLAYER || movement.distanceTraveled >= glm::distance(movement.posA, movement.posB) || enemy.newPattern == true)
		{
			// std::cout << currPattern.name << "after update" << std::endl;
			// if (registry.hand.has(entity) && currPattern.type == EnemyBehavior::IDLE) {
			// 	continue;
			// }
			movement.posA = motion.position;
			// ACTING
			// std::cout << currPattern.name << "before getmove" << std::endl;
			movement.posB = boundPosition(getMove(enemy.currEnemyPattern().type, entity), entity);
			// std::cout << "x " << movement.posB[0] << " y " << movement.posB[1] <<std::endl;
			movement.distanceTraveled = 0.f;
		}
		if (registry.enemyParts.has(entity)) {
			if (registry.enemyParts.get(entity).alwaysFollow) {
				Motion& motion = registry.motions.get(entity);
				if (registry.motions.has(registry.enemyParts.get(entity).parent)) {
					motion.position = registry.motions.get(registry.enemyParts.get(entity).parent).position + registry.enemyParts.get(entity).offset;
					// std::cout << "follow: " <<  motion.position.x << ", "<< motion.position.y <<std::endl;
				}
			}
		}

		if (registry.scientist.has(entity))
		{
			EnemyPattern &pattern = enemy.currEnemyPattern();
			RenderRequest &rr = registry.renderRequests.get(entity);
			if (pattern.type != EnemyBehavior::IDLE && pattern.type != EnemyBehavior::TELEPORT)
			{
				rr.texture_name = "scientist_walk";
				rr.used_effect = EFFECT_ASSET_ID::ANIMATE;
			}
			else
			{
				rr.texture_name = "scientist_idle.png";
				rr.used_effect = EFFECT_ASSET_ID::TEXTURED;
			}
		}
	}
}

Reaction *getReactions(std::vector<Reaction> &list, ReactionType target)
{
	for (Reaction &reaction : list)
	{
		if (reaction.React == target)
		{
			return &reaction;
		}
	}
	return nullptr;
}

void AISystem::updateState(Enemy &enemy, const EnemyMovement& movement, Entity entity)
{ // append and entity for more info on enemy for now
	vec2 playerPos = getPlayerPos();
	vec2 EnemyPos = movement.posA;
	vec2 EnemyPosMotion = registry.motions.get(entity).position;
	float distance = glm::distance(playerPos, EnemyPos);
	float farDistance = 800.f;
	float closeDistance = 400.f;
	float reallyCloseDistance = 100.f;
	float hpPercent = static_cast<float>(enemy.currHealth) / static_cast<float>(enemy.maxHealth);
	EnemyPattern &currPattern = enemy.currEnemyPattern();
	bool reaction_found = false;
	float closeToBeeDistance = 100.f;
	bool closeToBee = false;

	//std::cout << currPattern.name << std::endl;


	if (registry.bees.has(entity))
	{
		if (registry.bees.get(entity).nearbyBees.size() == 0)
		{
			auto reaction = getReactions(currPattern.reactions, ReactionType::NO_BEES);
			if (reaction)
			{
				enemy.patternIndex = reaction->index;
				enemy.newPattern = true;
				reaction_found = true;
			}
		}
		for (Entity bee : registry.bees.entities)
		{
			if (registry.bees.get(entity).maxMerge == registry.bees.get(entity).mergeCount)
			{
				break;
			}
			if (registry.bees.get(bee).maxMerge == registry.bees.get(bee).mergeCount)
			{
				continue;
			}
			if (bee != entity)
			{
				closeToBee = (glm::distance(EnemyPosMotion, registry.motions.get(bee).position) < closeToBeeDistance);
				BeeEnemy &beeComponent = registry.bees.get(entity);
				BeeEnemy &otherBeeComponent = registry.bees.get(bee);
				int mergeTotal = beeComponent.mergeCount + otherBeeComponent.mergeCount;
				// beeComponent.
				Motion &motion = registry.motions.get(entity);
				if (closeToBee && mergeTotal <= beeComponent.maxMerge)
				{
					registry.bees.get(entity).nearbyBees.insert(bee);
					auto reaction = getReactions(currPattern.reactions, ReactionType::BEE_CLOSE);
					if (reaction)
					{
						enemy.patternIndex = reaction->index;
						enemy.newPattern = true;
						reaction_found = true;
						BeeEnemy &beeComponent = registry.bees.get(entity);
						BeeEnemy &otherBeeComponent = registry.bees.get(bee);
					}
					if (registry.bees.get(entity).nearbyBees.size() == 0)
					{
						auto reaction = getReactions(currPattern.reactions, ReactionType::NO_BEES);
					}
					break;
				}
			}
		}
	}
	if (registry.scientist.has(entity))
	{
		Scientist& scien = registry.scientist.get(entity);
		if (scien.shield && registry.enemies.has(scien.shield))
		{

			Enemy& shield = registry.enemies.get(scien.shield);
			// std::cout << shield.currHealth << "shield health" << std::endl;
			if (shield.currHealth <= 0)
			{
				// std::cout << "got here" << std::endl;
				auto reaction = getReactions(currPattern.reactions, ReactionType::SHIELDBREAK);
				if (reaction)
				{
					enemy.patternIndex = reaction->index;
					enemy.newPattern = true;
					reaction_found = true;
				}
			}
		}
	}
	if (registry.hand.has(entity))
	{
		EnemyPattern& pattern = enemy.currEnemyPattern();
		RenderRequest& rr = registry.renderRequests.get(entity);
		if (pattern.type == EnemyBehavior::CHARGING)
		{
			rr.texture_name = "hand_idletocharge";
			rr.used_effect = EFFECT_ASSET_ID::ANIMATE;
			if (registry.animations.get(entity).frame == -1 && !registry.animationSequences.has(entity))
			{
				// registry.animations.get(entity).frame == 1;
				AnimationSequence& as = registry.animationSequences.emplace(entity);
				as.nextEffect = EFFECT_ASSET_ID::TEXTURED;
				as.nextSprite = "hand_charging.png";
				// std::cout << registry.animations.get(entity).frame << std::endl;
			}
		}
		else if (pattern.type == EnemyBehavior::PATROLLING)
		{
			rr.texture_name = "hand_shooting_laser.png";
			rr.used_effect = EFFECT_ASSET_ID::TEXTURED;
			registry.animations.get(entity).frame = -1;
		}
		else
		{
			rr.texture_name = "hand_idle.png";
			rr.used_effect = EFFECT_ASSET_ID::TEXTURED;
			registry.animations.get(entity).frame = -1;
		}
	}
	if (registry.healers.has(entity))
	{
		reaction_found = updateHealerState(enemy, entity);
	}

	if (hpPercent < 0.1f)
	{
		auto reaction = getReactions(currPattern.reactions, ReactionType::TEN_HEALTH);
		if (reaction)
		{
			std::cout << "10% HP" << std::endl;
			enemy.patternIndex = reaction->index;
			enemy.newPattern = true;
			reaction_found = true;
			handleSpecialStates(*reaction, entity);
		}
	}
	else if (hpPercent < 0.2f)
	{
		auto reaction = getReactions(currPattern.reactions, ReactionType::TWENTY_HEALTH);
		if (reaction)
		{
			std::cout << "20% HP" << std::endl;
			enemy.patternIndex = reaction->index;
			enemy.newPattern = true;
			reaction_found = true;
			handleSpecialStates(*reaction, entity);
		}
	}
	else if (hpPercent < 0.25f)
	{
		auto reaction = getReactions(currPattern.reactions, ReactionType::TWENTYFIVE_HEALTH);
		if (reaction)
		{
			std::cout << "25% HP" << std::endl;
			enemy.patternIndex = reaction->index;
			enemy.newPattern = true;
			reaction_found = true;
			handleSpecialStates(*reaction, entity);
		}
	}
	else if (hpPercent < 0.3f)
	{
		auto reaction = getReactions(currPattern.reactions, ReactionType::THIRTY_HEALTH);
		if (reaction)
		{
			std::cout << "30% HP" << std::endl;
			enemy.patternIndex = reaction->index;
			enemy.newPattern = true;
			reaction_found = true;
			handleSpecialStates(*reaction, entity);
		}
	}
	else if (hpPercent < 0.4f)
	{
		auto reaction = getReactions(currPattern.reactions, ReactionType::FORTY_HEALTH);
		if (reaction)
		{
			std::cout << "40% HP" << std::endl;
			enemy.patternIndex = reaction->index;
			enemy.newPattern = true;
			reaction_found = true;
			handleSpecialStates(*reaction, entity);
		}
	}

	else if (hpPercent < 0.5f)
	{
		auto reaction = getReactions(currPattern.reactions, ReactionType::FIFTY_HEALTH);
		if (reaction)
		{
			std::cout << "50% HP" << std::endl;
			enemy.patternIndex = reaction->index;
			enemy.newPattern = true;
			reaction_found = true;
			handleSpecialStates(*reaction, entity);
		}
	}

	else if (hpPercent < 0.6f)
	{
		auto reaction = getReactions(currPattern.reactions, ReactionType::SIXTY_HEALTH);
		if (reaction)
		{
			std::cout << "60% HP" << std::endl;
			enemy.patternIndex = reaction->index;
			enemy.newPattern = true;
			reaction_found = true;
			handleSpecialStates(*reaction, entity);
		}
	}
	else if (hpPercent < 0.7f)
	{
		auto reaction = getReactions(currPattern.reactions, ReactionType::SEVENTY_HEALTH);
		if (reaction)
		{
			std::cout << "70% HP" << std::endl;
			enemy.patternIndex = reaction->index;
			enemy.newPattern = true;
			reaction_found = true;
			handleSpecialStates(*reaction, entity);
		}
	}
	else if (hpPercent < 0.75f)
	{
		auto reaction = getReactions(currPattern.reactions, ReactionType::SEVENTYFIVE_HEALTH);
		if (reaction)
		{
			std::cout << "75% HP" << std::endl;
			enemy.patternIndex = reaction->index;
			enemy.newPattern = true;
			reaction_found = true;
			handleSpecialStates(*reaction, entity);
		}
	}
	else if (hpPercent < 0.8f)
	{
		auto reaction = getReactions(currPattern.reactions, ReactionType::EIGHTY_HEALTH);
		if (reaction)
		{
			std::cout << "80% HP" << std::endl;
			enemy.patternIndex = reaction->index;
			enemy.newPattern = true;
			reaction_found = true;
			handleSpecialStates(*reaction, entity);
		}
	}
	else if (hpPercent < 0.9f)
	{
		auto reaction = getReactions(currPattern.reactions, ReactionType::NINETY_HEALTH);
		if (reaction)
		{
			std::cout << "90% HP" << std::endl;
			enemy.patternIndex = reaction->index;
			enemy.newPattern = true;
			reaction_found = true;
			handleSpecialStates(*reaction, entity);
		}
	}

	if (!reaction_found && getReactions(currPattern.reactions, ReactionType::FINISH_PATROL) && currPattern.pathIndex == currPattern.path.size() - 1) {
		auto reaction = getReactions(currPattern.reactions, ReactionType::FINISH_PATROL);

		vec2 patrolFactor = currPattern.path[currPattern.path.size() - 1];
		vec2 endPoint = lerpToRoom(patrolFactor);

		//std::cout << glm::to_string(EnemyPos) << ", " << glm::to_string(endPoint) << ", " << glm::distance(EnemyPos, endPoint) << std::endl;
		if (glm::distance(EnemyPos, endPoint) < 0.001 || (registry.wormHeads.has(entity) && glm::distance(registry.wormHeads.get(entity).points[0], endPoint) < 0.05)) {
			enemy.patternIndex = reaction->index;
			enemy.newPattern = true;
			reaction_found = true;
			handleSpecialStates(*reaction, entity);
		}
	}
	if (!reaction_found && distance < closeDistance)
	{
		if (distance < reallyCloseDistance)
		{
			auto reaction = getReactions(currPattern.reactions, ReactionType::PLAYER_REALLY_CLOSE);
			if (reaction)
			{
				// std::cout << "got reaction for follow player" << std::endl;
				enemy.patternIndex = reaction->index;
				enemy.newPattern = true;
				reaction_found = true;
				handleSpecialStates(*reaction, entity);
			}
		}
		else {
			auto reaction = getReactions(currPattern.reactions, ReactionType::PLAYER_CLOSE);
			if (reaction)
			{
				// std::cout << "got reaction for follow player" << std::endl;
				enemy.patternIndex = reaction->index;
				enemy.newPattern = true;
				reaction_found = true;
				handleSpecialStates(*reaction, entity);
			}
		}
	}
	if (!reaction_found && distance > farDistance)
	{
		auto reaction = getReactions(currPattern.reactions, ReactionType::PLAYER_FAR);
		if (reaction)
		{
			// std::cout << "got reaction for follow player" << std::endl;
			enemy.patternIndex = reaction->index;
			enemy.newPattern = true;
			reaction_found = true;
			handleSpecialStates(*reaction, entity);
		}
	}
	if (!reaction_found && (currPattern.reactions.size() == 0 || getReactions(currPattern.reactions, ReactionType::DURATION)))
	{
		// std::cout << currPattern.name << " has " << currPattern.curDuration << " ms left" << std::endl;
		auto reaction = getReactions(currPattern.reactions, ReactionType::DURATION);
		if (currPattern.curDuration < 0.f)
		{
			enemy.patternIndex = currPattern.next;
			enemy.newPattern = true;
			// std::cout << currPattern.next << " index currPattern.next" << std::endl;
			currPattern.curDuration = currPattern.maxDuration;
			if (reaction) handleSpecialStates(*reaction, entity);
			// std::cout << "change to " << enemy.currEnemyPattern().name << std::endl;
		}
	}
	if (!reaction_found && getReactions(currPattern.reactions, ReactionType::PLAYER_BULLET_CLOSE))
	{
		float closestPBullet = 100000.f;
		if (!registry.spawnings.has(entity)) {
			for (Entity e : registry.playerBullets.entities) {
				Motion& m = registry.motions.get(e);
				closestPBullet = min(closestPBullet, glm::distance(m.position, EnemyPos));
			}
		}

		auto reaction = getReactions(currPattern.reactions, ReactionType::PLAYER_BULLET_CLOSE);
		if (reaction && closestPBullet < reallyCloseDistance)
		{
			// std::cout << "got reaction for follow player" << std::endl;
			enemy.patternIndex = reaction->index;
			enemy.newPattern = true;
			reaction_found = true;
			handleSpecialStates(*reaction, entity);
		}
	}
}

bool AISystem::updateHealerState(Enemy &enemy, Entity entity)
{
	EnemyPattern &currPattern = enemy.currEnemyPattern();
	Healer& healer = registry.healers.get(entity);
	bool reaction_found = false;
	if (healer.targetEntity)
	{
		if (registry.enemies.has(healer.targetEntity))
		{
			Enemy &targetEnemy = registry.enemies.get(healer.targetEntity);
			if (targetEnemy.currHealth > 0)
			{
				return true;
			}
		}
	}
	bool foundHurtEnemy = false;
	for (Entity otherEntity : registry.enemies.entities)
	{
		if (otherEntity != entity)
		{
			Enemy &otherEnemy = registry.enemies.get(otherEntity);
			float otherHpPercent = static_cast<float>(otherEnemy.currHealth) / static_cast<float>(otherEnemy.maxHealth);

			// Look for an enemy that is hurt (health < max health)
			if (otherHpPercent < 1.0f)
			{
				auto reaction = getReactions(currPattern.reactions, ReactionType::TEAM_HURT);
				if (reaction)
				{
					enemy.patternIndex = reaction->index;
					enemy.newPattern = true;
					reaction_found = true;
					healer.targetEntity = otherEntity;
					foundHurtEnemy = true;
				}
				break;
			}
		}
	}
	if (!foundHurtEnemy)
	{

		auto idleReaction = getReactions(currPattern.reactions, ReactionType::DURATION);
		if (idleReaction)
		{
			enemy.patternIndex = idleReaction->index;
			enemy.newPattern = true;
			reaction_found = true;
		}
	}
	return reaction_found;
}

vec2 AISystem::getMove(EnemyBehavior behavior, Entity entity)
{
	if ((registry.enemyParts.has(entity) && registry.enemyParts.get(entity).alwaysFollow) || registry.roomWideBuffers.has(entity))
	{
		return getCurrentPos(entity);
	}
	// path finding hasnt been implemented
	switch (behavior)
	{
	case EnemyBehavior::RANDOM:
		// std::cout << "random!" << std::endl;
		return generateRandomPos(entity);
	case EnemyBehavior::RANDOM_NEAR:
		// std::cout << "random!" << std::endl;
		return generateRandomPosInRadius(entity, 100, 200);
	case EnemyBehavior::RANDOM_FAR:
		// std::cout << "random!" << std::endl;
		return generateRandomPosInRadius(entity, 500, 1000);
	case EnemyBehavior::ROLLING:
		return getRollingPos(entity);
	case EnemyBehavior::FOLLOW_PLAYER:
		// std::cout << "follow!" << std::endl;
		return getPlayerPos();
	case EnemyBehavior::ROOK_FOLLOW:
		// std::cout << "rook follow!" << std::endl;
		return getRookPos(entity);
	case EnemyBehavior::EVADEBULLET:
		// std::cout << "evade!" << std::endl;
		return evadeBullet(entity);
	case EnemyBehavior::ROTATE_IN_PLACE:
		return getCurrentPos(entity);
	case EnemyBehavior::IDLE:
		return getCurrentPos(entity);
	case EnemyBehavior::HEALING:
		return getTeamPos(entity);
	case EnemyBehavior::MERGE_BEE:
		return getCurrentPos(entity);
	case EnemyBehavior::SPAWNING:
		return getCurrentPos(entity);
	case EnemyBehavior::CHARGING:
		return getCharginPos(entity);
	case EnemyBehavior::RECOIL:
		return getRecoilPos(entity);
	case EnemyBehavior::TELEPORT:
		return getTeleportPos(entity);
	case EnemyBehavior::FOLLOWSCIENTIST:
		return getScientistPos(entity);
	case EnemyBehavior::FOLLOWPARENT:
		return getParentPos(entity);
	case EnemyBehavior::GRANTINGBUFFS:
		return getTeamPos(entity);
	case EnemyBehavior::GRANTINGBUFFSAOE:
    	return getTeamPos(entity);
	case EnemyBehavior::PATROLLING:
		return getNextPatrolPos(entity);
	default:
		return getCurrentPos(entity);
	};
};

vec2 AISystem::getParentPos(Entity entity)
{
	auto& ep = registry.enemyParts.get(entity);
	Motion &motion = registry.motions.get(ep.parent);
	return motion.position + ep.offset;
}


vec2 AISystem::getScientistPos(Entity entity)
{

	Entity scienEntity = registry.scientist.entities[0];
	Scientist &scien = registry.scientist.components[0];
	Motion &motionScientist = registry.motions.get(scienEntity);

	return motionScientist.position + vec2(100.f, 0);
}

vec2 AISystem::getCurrentPos(Entity entity)
{
	EnemyMovement &movement = registry.enemyMovement.get(entity);
	Motion &motion = registry.motions.get(entity);
	// This resets movement speed to ignore speedModifier...
	//movement.speed = 100.f;
	return motion.position;
}

vec2 AISystem::getRookPos(Entity entity)
{
	vec2 dir = (getPlayerPos() - getCurrentPos(entity));
	bool dir2 = (abs(dir.x) > abs(dir.y));
	return getCurrentPos(entity) + (dir * vec2(dir2, !dir2));
}

vec2 AISystem::getTeleportPos(Entity entity)
{
	Enemy &enemy = registry.enemies.get(entity);
	EnemyPattern &pattern = enemy.currEnemyPattern();
	if (pattern.type == EnemyBehavior::TELEPORT)
	{
		// std::cout << "tele" << pattern.name << std::endl;
		// if (registry.hand.has(entity) && pattern.name == "T")
		// {
		// 	std::cout << "should tele to scientist" << std::endl;
		// 	Entity scien = registry.scientist.entities[0];
		// 	Motion &scienMotion = registry.motions.get(scien);

		// 	return scienMotion.position - vec2(100.f, 0);
		// }

		if (pattern.path.size() > 0)
		{
			vec2 positionFactor = pattern.path[0];
			vec4 roomBounds = getRoomBounds(entity);
			vec2 min = {roomBounds.x, roomBounds.y};
			vec2 max = {roomBounds.z, roomBounds.w};

			return glm::lerp(min, max, positionFactor);
		}
		else
		{
			return generateRandomPos(entity);
		}
	}
	else
	{
		return getCurrentPos(entity);
	}
}

vec2 AISystem::getNextPatrolPos(Entity entity)
{
	Enemy &enemy = registry.enemies.get(entity);
	EnemyPattern &pattern = enemy.currEnemyPattern();
	//if (pattern.type != EnemyBehavior::PATROLLING)
	//{
	//	// std::cout << "Different EnemyBehavior!" << std::endl;
	//	return getCurrentPos(entity);
	//}
	// std::cout << "current state: " << pattern.name << std::endl;
	pattern.pathIndex += 1;
	pattern.pathIndex %= pattern.path.size();

	vec2 patrolFactor = pattern.path[pattern.pathIndex];

	// printf("%.1f %.1f\n", patrolFactor.x,patrolFactor.y);

	vec4 roomBounds = getRoomBounds(entity);
	vec2 min = {roomBounds.x, roomBounds.y};
	vec2 max = {roomBounds.z, roomBounds.w};

	//std::cout << glm::to_string(glm::lerp(min, max, patrolFactor)) << std::endl;

	//return glm::lerp(min, max, patrolFactor);
	return lerpToRoom(patrolFactor);
}

vec2 AISystem::generateRandomPos(Entity entity)
{
	vec4 roomBounds = getRoomBounds(entity);
	vec2 min = {roomBounds.x, roomBounds.y};
	vec2 max = {roomBounds.z, roomBounds.w};

	return glm::lerp(min, max, Random::Vec2(vec2(1)));
}

vec2 AISystem::getCharginPos(Entity entity)
{
	vec2 playerPos = getPlayerPos();
	Motion &motion = registry.motions.get(entity);
	vec2 scale = motion.scale;
	vec2 direction = playerPos - motion.position;

	if (glm::length2(direction) > 0)
	{
		direction = glm::normalize(direction);
	}
	float chargeDistance = 100.0f;

	vec2 goalPosition = playerPos + direction * chargeDistance;
	EnemyMovement &movement = registry.enemyMovement.get(entity);
	movement.speed = 500.0f;

	vec4 roomBounds = getRoomBounds(entity);
	vec2 min = {roomBounds.x, roomBounds.y};
	vec2 max = {roomBounds.z, roomBounds.w};

	goalPosition = glm::clamp(goalPosition, min, max);

	return goalPosition;
}

vec2 AISystem::getRecoilPos(Entity entity)
{
	vec2 playerPos = getPlayerPos();
	Motion &motion = registry.motions.get(entity);
	vec2 scale = motion.scale;
	vec2 direction = motion.position - playerPos;

	if (glm::length2(direction) > 0)
	{
		direction = glm::normalize(direction);
	}

	float chargeDistance = 300.0f;

	vec2 goalPosition = motion.position + direction * chargeDistance;

	EnemyMovement &movement = registry.enemyMovement.get(entity);
	movement.speed = 300.0f;

	vec4 roomBounds = getRoomBounds(entity);
	vec2 min = {roomBounds.x, roomBounds.y};
	vec2 max = {roomBounds.z, roomBounds.w};

	goalPosition = glm::clamp(goalPosition, min, max);

	return goalPosition;
}

vec2 AISystem::generateRandomPosInRadius(Entity entity, int radiusNear, int radiusFar)
{
	vec2 start = registry.motions.get(entity).position;
	float angle = 2.0f * M_PI * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
	float pos_x = cos(angle) * (radiusNear + rand() % (radiusFar - radiusNear)) + start.x;
	float pos_y = sin(angle) * (radiusNear + rand() % (radiusFar - radiusNear)) + start.y;

	vec4 roomBounds = getRoomBounds(entity);
	vec2 min = {roomBounds.x, roomBounds.y};
	vec2 max = {roomBounds.z, roomBounds.w};

	return glm::clamp(vec2(pos_x, pos_y), min, max);
}

vec2 AISystem::getTeamPos(Entity entity)
{
	Motion &motion = registry.motions.get(entity);
	vec2 scale = motion.scale;
	if (registry.healers.has(entity))
	{
		Healer& healComponent = registry.healers.get(entity);
		Entity teammates = healComponent.targetEntity;
		if (teammates && registry.motions.has(teammates))
		{
			Motion &teammateMotion = registry.motions.get(teammates);
			Motion &healerMotion = registry.motions.get(entity);

			vec2 direction = teammateMotion.position - healerMotion.position;
			if (glm::length2(direction) > 0)
			{
				direction = glm::normalize(direction);
			}
			float backDistance = 100.0f;

			vec2 goalPosition = teammateMotion.position - direction * backDistance;

			vec4 roomBounds = getRoomBounds(entity);
			vec2 min = {roomBounds.x, roomBounds.y};
			vec2 max = {roomBounds.z, roomBounds.w};

			goalPosition = glm::clamp(goalPosition, min, max);

			return goalPosition;
		}
	} else if (registry.buffers.has(entity))
	{
		Buffer& buffer = registry.buffers.get(entity);
		Entity teammates = buffer.targetEntity;
		if (teammates != NULL && registry.motions.has(teammates))
		{
			Motion &teammateMotion = registry.motions.get(teammates);
			Motion &bufferMotion = registry.motions.get(entity);

			vec2 direction = teammateMotion.position - bufferMotion.position;
			if (glm::length2(direction) > 0)
			{
				direction = glm::normalize(direction);
			}
			float backDistance = buffer.range * 0.5f;

			vec2 goalPosition = teammateMotion.position - direction * backDistance;

			vec4 roomBounds = getRoomBounds(entity);
			vec2 min = {roomBounds.x, roomBounds.y};
			vec2 max = {roomBounds.z, roomBounds.w};

			goalPosition = glm::clamp(goalPosition, min, max);

			return goalPosition;
		}
	}

	return generateRandomPos(entity);
};

vec2 AISystem::getPlayerPos()
{
	auto &player_register = registry.players;
	Entity &entity = player_register.entities[0];
	Motion &motion = registry.motions.get(entity);
	return motion.position;
}

vec2 AISystem::getRollingPos(Entity entity)
{
	auto& movement_register = registry.enemyMovement;
	auto& motion_register = registry.motions;
	EnemyMovement& movement = movement_register.get(entity);
	Motion& enemyMotion = motion_register.get(entity);

	vec4 roomBounds = getRoomBounds(entity);
	vec2 min = { roomBounds.x, roomBounds.y };
	vec2 max = { roomBounds.z, roomBounds.w };

	vec2 direction = movement.posB - movement.posA;
	float distance = glm::length(max - min);

	// Wall collisions and bouncing
	vec2 norm = vec2(0, 0);
	if (enemyMotion.position.x <= min.x && direction.x < 0.0) {
		norm = vec2(1, 0);
	}
	else if (enemyMotion.position.x >= max.x && direction.x > 0.0) {
		norm = vec2(-1, 0);
	}
	else if (enemyMotion.position.y <= min.y && direction.y < 0.0) {
		norm = vec2(0, 1);
	}
	else if (enemyMotion.position.y >= max.y && direction.y > 0.0) {
		norm = vec2(0, -1);
	}
	if (norm != vec2(0, 0)) {

		vec2 newDirection = glm::reflect(direction, norm);
		vec2 intersection = vec2((norm.x < 0.0) ? max.x : (norm.x == 0.0) ? enemyMotion.position.x : min.x, 
							     (norm.y < 0.0) ? max.y : (norm.y == 0.0) ? enemyMotion.position.y : min.y);

		//std::cout << glm::to_string(movement.posB) << " : " << glm::to_string(min) << " : " << glm::to_string(max) << std::endl;
		//std::cout << glm::to_string(intersection) << " : " << glm::to_string(norm) << " : " << glm::to_string(direction) << " : " << glm::to_string(glm::reflect(direction, norm)) << std::endl;
		//std::cout << glm::to_string(enemyMotion.position) << ", " << glm::to_string(intersection + newDirection * distance) << std::endl;

		movement.posA = intersection;
		return intersection + newDirection * distance;
	}

	return movement.posB;
}

vec2 AISystem::evadeBullet(Entity entity)
{
	auto &motion_register = registry.motions;
	auto &pBullet_register = registry.playerBullets;
	auto &movement_register = registry.enemyMovement;
	EnemyMovement &movement = movement_register.get(entity);
	if (pBullet_register.entities.size() == 0)
	{
		return movement.posA;
	}
	vec2 bestEscapePos = movement.posA;
	float closestDistance = 0;
	for (Entity ent : pBullet_register.entities)
	{
		float danger_radius = 500.f;
		Motion &bulletMotion = motion_register.get(ent);
		Motion &enemyMotion = motion_register.get(entity);
		vec2 bulletPos = bulletMotion.position;
		vec2 enemyPos = enemyMotion.position;
		float distance = glm::distance(bulletPos, enemyPos);
		if (distance < danger_radius)
		{
			vec2 futureBulletPos = bulletMotion.position + bulletMotion.velocity * 0.1f;
			vec2 direction = glm::normalize(enemyMotion.position - futureBulletPos);
			// Right and Left Perpendicular of Bullet Direction
			vec2 perpDirection = vec2(-bulletMotion.velocity[1], bulletMotion.velocity[0]);
			vec2 perpDirection1 = vec2(bulletMotion.velocity[1], -bulletMotion.velocity[0]);

			perpDirection = glm::normalize(perpDirection);
			perpDirection1 = glm::normalize(perpDirection1);

			vec2 escapePos = enemyPos + perpDirection * 100.f;
			vec2 escapePos1 = enemyPos + perpDirection1 * 100.f;

			float dist = glm::distance(escapePos, futureBulletPos);
			float dist1 = glm::distance(escapePos1, futureBulletPos);
			if (dist > dist1 && dist1 > closestDistance)
			{
				bestEscapePos = escapePos;
				closestDistance = dist;
			}
			else if (dist1 > dist && dist1 > closestDistance)
			{
				bestEscapePos = escapePos1;
				closestDistance = dist1;
			}
		}
	}

	vec4 roomBounds = getRoomBounds(entity);
	vec2 min = {roomBounds.x, roomBounds.y};
	vec2 max = {roomBounds.z, roomBounds.w};
	bestEscapePos = glm::clamp(bestEscapePos, min, max);
	return bestEscapePos;
}

void AISystem::angryMode(Entity entity)
{
}

void AISystem::computeBoidVelocity(Entity entity, Boid &boid)
{
	Enemy &enemy = registry.enemies.get(entity);
	EnemyPattern &currentPattern = enemy.currEnemyPattern();
	float maxSpeed = 470.f;
	if (currentPattern.type == EnemyBehavior::BOIDSGROUP)
	{
		boidComputeCoherence(entity, boid, 1.f, 400.f);
		boid.velocity *= 0.8f;
		if (glm::length2(boid.velocity) > boid.maxSpeed * boid.maxSpeed)
		{
			boid.velocity = glm::normalize(boid.velocity) * boid.maxSpeed;
		}
		boidKeepBound(entity, boid, 0, 0, 0, 0);
	}
	else if (currentPattern.type == EnemyBehavior::BOIDSEXPLODE)
	{
		boidComputeSeperation(entity, boid, 1.f);
		boid.velocity *= 2.f;
		maxSpeed = 700.f;
		if (glm::length2(boid.velocity) > maxSpeed * maxSpeed)
		{
			boid.velocity = glm::normalize(boid.velocity) * boid.maxSpeed;
		}
		boidKeepBound(entity, boid, 0, 0, 0, 0);
	}
	else if (currentPattern.type == EnemyBehavior::BOIDSWARMPLAYER)
	{
		boidFollowPlayer(entity, boid, 0.05f);
		boidWander(entity, boid, 0.05f);

		// boidComputeCoherence(entity, boid, 0.01f);
		boidComputeSeperation(entity, boid, 0.2f);
		// boidComputeAlignment(entity, boid, 0.02f);
		if (glm::length2(boid.velocity) > boid.maxSpeed * boid.maxSpeed)
		{
			boid.velocity = glm::normalize(boid.velocity) * boid.maxSpeed;
		}
		boidKeepBound(entity, boid, 0, 0, 0, 0);
	}
	else if (currentPattern.type == EnemyBehavior::BOIDSFISH)
	{
		boidCircleRoom(entity, boid, 0.3f, 0.05f);
		// boidComputeCoherence(entity, boid, 0.015f, 1000.f);
		// boidComputeSeperation(entity, boid, 0.5f);
		// boidComputeAlignment(entity, boid, 0.01f);
		boidComputeAllFactor(entity, boid, 0.01f, 0.5f, 0.01f, 700.f);

		if (glm::length2(boid.velocity) > boid.maxSpeed * boid.maxSpeed)
		{
			boid.velocity = glm::normalize(boid.velocity) * boid.maxSpeed;
		}
		boidEvadePlayer(entity, boid, 100.0f);
		boidKeepBound(entity, boid, 0, 0, 0, 0);
	}
	else
	{
		boidWander(entity, boid, 0.1f);
		// boidComputeCoherence(entity, boid, 0.02f, 700.f);
		// boidComputeSeperation(entity, boid, 0.05f);
		// boidComputeAlignment(entity, boid, 0.02f);
		boidComputeAllFactor(entity, boid, 0.02f, 0.05f, 0.02f, 700.f);
		if (glm::length2(boid.velocity) > boid.maxSpeed * boid.maxSpeed)
		{
			boid.velocity = glm::normalize(boid.velocity) * boid.maxSpeed;
		}
		boidKeepBound(entity, boid, 0, 0, 0, 0);
	}
}

void AISystem::boidKeepBound(Entity entity, Boid &boid, float minx, float miny, float maxx, float maxy)
{
	WindowState &windowState = registry.windowStates.components[0];
	Map &map = registry.maps.components[0];
	vec2 roomCenter = vec2(windowState.width, windowState.height) / 2.f;
	vec2 roomStartPos = map.currRoom.roomStart;
	vec2 roomEndPos = map.currRoom.roomEnd;

	vec2 scale = registry.motions.get(entity).scale;

	float minX = roomStartPos.x + minx + scale[0];
	float minY = roomStartPos.y + miny + scale[1];
	float maxX = roomEndPos.x - maxx - scale[0];
	float maxY = roomEndPos.y - maxy - scale[1];
	float turnFactor = 1.0f;
	float momentumFactor = 150.f;
	vec2 position = boid.position;

	if (position[0] < minX)
	{
		boid.velocity[0] += glm::abs(boid.velocity[0]) * turnFactor + momentumFactor;
	}
	if (position[0] > maxX)
	{
		boid.velocity[0] -= glm::abs(boid.velocity[0]) * turnFactor + momentumFactor;
	}
	if (position[1] < minY)
	{
		boid.velocity[1] += glm::abs(boid.velocity[1]) * turnFactor + momentumFactor;
	}
	if (position[1] > maxY)
	{
		boid.velocity[1] -= glm::abs(boid.velocity[1]) * turnFactor + momentumFactor;
	}
}

void AISystem::boidComputeCoherence(Entity entity, Boid &boid, float multiplier = 0.01f, float range = 1000.f)
{
	float centeringFactor = multiplier;
	vec2 center = vec2{0, 0};
	int numNeighbors = 0;

	vec2 position = boid.position;
	for (Entity other : registry.boids.entities)
	{
		if (other == entity)
		{
			continue;
		};
		Boid &otherBoid = registry.boids.get(other);
		vec2 otherPos = otherBoid.position;
		float neighborRnge = range;
		float distance = glm::distance(position, otherPos);

		if (distance < neighborRnge)
		{
			center += otherPos;
			numNeighbors++;
		}
	}

	if (numNeighbors > 0)
	{
		center /= numNeighbors;
		vec2 cohesionVelocity = (center - position) * centeringFactor;
		boid.velocity += cohesionVelocity;
	}
}

void AISystem::boidComputeSeperation(Entity entity, Boid &boid, float multiplier = 0.05f)
{
	float minDistance = 20.f;
	float avoidFactor = multiplier;
	vec2 move = vec2(0, 0);
	vec2 position = boid.position;
	for (Entity other : registry.boids.entities)
	{
		if (other == entity)
		{
			continue;
		};
		Boid &otherBoid = registry.boids.get(other);
		vec2 otherPos = otherBoid.position;
		float distance = glm::distance(position, otherPos);

		if (distance < minDistance)
		{
			move[0] += boid.position[0] - otherPos[0];
			move[1] += boid.position[1] - otherPos[1];
		};
	}
	boid.velocity[0] += move[0] * avoidFactor;
	boid.velocity[1] += move[1] * avoidFactor;
}

void AISystem::boidComputeAlignment(Entity entity, Boid &boid, float multiplier = 0.02f)
{
	vec2 avgVelocity = vec2(0, 0);
	int numNeighbors = 0;
	float matchingFactor = multiplier;

	vec2 position = boid.position;
	for (Entity other : registry.boids.entities)
	{
		if (other == entity)
		{
			continue;
		};
		Boid &otherBoid = registry.boids.get(other);
		vec2 otherPos = otherBoid.position;
		float neighborRnge = 200.f;
		float distance = glm::distance(position, otherPos);

		if (distance < neighborRnge)
		{
			avgVelocity[0] += otherBoid.velocity[0];
			avgVelocity[1] += otherBoid.velocity[1];
			numNeighbors += 1;
		}
	}

	if (numNeighbors > 0)
	{
		avgVelocity[0] /= numNeighbors;
		avgVelocity[1] /= numNeighbors;

		boid.velocity[0] += (avgVelocity[0] - boid.velocity[0]) * matchingFactor;
		boid.velocity[1] += (avgVelocity[1] - boid.velocity[1]) * matchingFactor;
	}
}

float getRandomInRange(float min, float max)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(min, max);
	return dis(gen);
}

void AISystem::boidWander(Entity entity, Boid &boid, float multiplier)
{
	float wanderRadius = 50.0f;
	float wanderDistance = 100.0f;
	float wanderJitter = 5.0f;
	float wanderFactor = multiplier;

	boid.wanderAngle += getRandomInRange(-wanderJitter, wanderJitter);

	vec2 wanderTarget = vec2(
		cos(boid.wanderAngle) * wanderRadius,
		sin(boid.wanderAngle) * wanderRadius);

	vec2 ahead = boid.velocity;
	ahead = glm::normalize(ahead) * wanderDistance;
	vec2 target = boid.position + ahead + wanderTarget;

	vec2 desiredVelocity = target - boid.position;

	boid.velocity += desiredVelocity * wanderFactor;
}

void AISystem::boidFollowPlayer(Entity entity, Boid &boid, float multiplier)
{
	vec2 playerPos = getPlayerPos();
	vec2 position = boid.position;

	//vec2 directionToPlayer = playerPos - position;
	//float distanceToPlayer = glm::length(directionToPlayer);

	vec2 center = (boid.position + playerPos) * 0.5f;
	vec2 cohesionToPlayer = (center - position) * multiplier;

	boid.velocity += cohesionToPlayer;
}

void AISystem::boidEvadePlayer(Entity entity, Boid &boid, float multiplier)
{
	vec2 playerPos = getPlayerPos();
	vec2 position = boid.position;
	float evadeRadius = 150.f;

	vec2 directionToPlayer = playerPos - position;
	float distanceToPlayer = glm::length2(directionToPlayer);

	if (distanceToPlayer < evadeRadius * evadeRadius)
	{
		vec2 fleeDirection = -glm::normalize(directionToPlayer);

		vec2 fleeVelocity = fleeDirection * multiplier;

		boid.velocity += fleeVelocity;

		float panicBoost = 2.5f;
		if (glm::length2(boid.velocity) < panicBoost * multiplier * panicBoost * multiplier)
		{
			boid.velocity = glm::normalize(boid.velocity) * panicBoost * multiplier;
		}

		float maxSpeed = 400.f;
		if (glm::length2(boid.velocity) > maxSpeed * maxSpeed)
		{
			boid.velocity = glm::normalize(boid.velocity) * maxSpeed;
		}
	}
}

void AISystem::boidCircleRoom(Entity entity, Boid &boid, float multiplier, float angularSpeed)
{

	WindowState &windowState = registry.windowStates.components[0];
	Room &room = registry.maps.components[0].currRoom;
	vec2 roomCenter = (room.roomStart + room.roomEnd) / 2.f;

	float radius = glm::min(room.preset.roomSize.x, room.preset.roomSize.y) / 3.5f;

	vec2 position = boid.position;
	vec2 directionToCenter = position - roomCenter;

	float currentAngle = atan2(directionToCenter.y, directionToCenter.x);

	float newAngle = currentAngle + angularSpeed;

	vec2 targetPosition = roomCenter + vec2(cos(newAngle) * radius, sin(newAngle) * radius);
	vec2 desiredVelocity = targetPosition - position;
	boid.velocity += desiredVelocity * multiplier;
}

void AISystem::boidComputeAllFactor(Entity entity, Boid &boid, float multiplierCoherence, float multiplierSeperation, float multiplierAlignment, float range)
{
	float centeringFactor = multiplierCoherence;
	vec2 center = vec2{0, 0};
	int numNeighbors = 0;
	// ------- SPERATION ----- //
	float minDistance = 20.f;
	float avoidFactor = multiplierSeperation;
	vec2 move = vec2(0, 0);
	vec2 position = boid.position;
	//------ ALIGNMENT ---- //
	vec2 avgVelocity = vec2(0, 0);
	int numNeighborsAlignemnt = 0;
	float matchingFactor = multiplierAlignment;
	for (Entity other : registry.boids.entities)
	{
		if (other == entity)
		{
			continue;
		};
		// -------- COHERENCE ----------- //
		Boid &otherBoid = registry.boids.get(other);
		vec2 otherPos = otherBoid.position;
		float neighborRnge = range;
		float distance = glm::distance(position, otherPos);

		if (distance < neighborRnge)
		{
			center += otherPos;
			numNeighbors++;
		}

		// --------- SEPERATION --------- //

		if (distance < minDistance)
		{
			move[0] += boid.position[0] - otherPos[0];
			move[1] += boid.position[1] - otherPos[1];
		};

		// ---------- ALIGNMENT ----------- //
		float neighborRngeAlignment = 200.f;

		if (distance < neighborRngeAlignment)
		{
			avgVelocity[0] += otherBoid.velocity[0];
			avgVelocity[1] += otherBoid.velocity[1];
			numNeighborsAlignemnt += 1;
		}
	}
	// -------------- COHERENCE ------------//
	if (numNeighbors > 0)
	{
		center /= numNeighbors;
		vec2 cohesionVelocity = (center - position) * centeringFactor;
		boid.velocity += cohesionVelocity;
	}
	// ------------------ SEPERATION ------------------ //

	boid.velocity[0] += move[0] * avoidFactor;
	boid.velocity[1] += move[1] * avoidFactor;
	// -------------    ALIGNMENT    ----------//

	if (numNeighborsAlignemnt > 0)
	{
		avgVelocity[0] /= numNeighbors;
		avgVelocity[1] /= numNeighbors;

		boid.velocity[0] += (avgVelocity[0] - boid.velocity[0]) * matchingFactor;
		boid.velocity[1] += (avgVelocity[1] - boid.velocity[1]) * matchingFactor;
	}
}

bool AISystem::LineToLine(vec2 line1Start, vec2 line1End, vec2 line2Start, vec2 line2End, vec2& intersectionPoint)
{
	auto cross = [](const glm::vec2& v1, const glm::vec2& v2)
		{ return v1.x * v2.y - v1.y * v2.x; };
	glm::vec2 r = line1End - line1Start, s = line2End - line2Start, pq = line2Start - line1Start;
	float rxs = cross(r, s);
	if (rxs == 0)
		return false; // Lines are parallel
	float t = cross(pq, s) / rxs, u = cross(pq, r) / rxs;
	intersectionPoint = line1Start + t * r;
	return (t >= 0 && t <= 1 && u >= 0 && u <= 1);
}

vec2 AISystem::constrainDistance(vec2 point, vec2 anchor, float distance) {
	return (glm::normalize(point - anchor) * distance) + anchor;
}

vec2 AISystem::catmullRomSplineLerp(const std::vector<glm::vec2>& cp, float t)
{
	// indices of the relevant control points
	int i0 = glm::clamp<int>(t - 1, 0, cp.size() - 1);
	int i1 = glm::clamp<int>(t + 0, 0, cp.size() - 1);
	int i2 = glm::clamp<int>(t + 1, 0, cp.size() - 1);
	int i3 = glm::clamp<int>(t + 2, 0, cp.size() - 1);

	// parameter on the local curve interval
	float local_t = glm::fract(t);

	return glm::catmullRom(lerpToRoom(cp[i0]), lerpToRoom(cp[i1]), lerpToRoom(cp[i2]), lerpToRoom(cp[i3]), local_t);
}

vec2 AISystem::catmullRomSpline(const std::vector<glm::vec2>& cp, float t)
{
	// indices of the relevant control points
	int i0 = glm::clamp<int>(t - 1, 0, cp.size() - 1);
	int i1 = glm::clamp<int>(t, 0, cp.size() - 1);
	int i2 = glm::clamp<int>(t + 1, 0, cp.size() - 1);
	int i3 = glm::clamp<int>(t + 2, 0, cp.size() - 1);

	// parameter on the local curve interval
	float local_t = glm::fract(t);

	return glm::catmullRom(cp[i0], cp[i1], cp[i2], cp[i3], local_t);
}

vec2 AISystem::lerpToRoom(vec2 point) {
	Map& map = registry.maps.components[0];
	return glm::lerp(map.currRoom.roomStart, map.currRoom.roomEnd, point);
}

 vec2 AISystem::moveTowards(vec2 current, vec2 target, float maxDistanceDelta)
{
	vec2 a = target - current;
	float magnitude = glm::length(a);
	if (magnitude <= maxDistanceDelta || magnitude == 0.f)
	{
		return target;
	}
	return current + a / magnitude * maxDistanceDelta;
}
