// internal
#include "ai_system.hpp"
#include "tiny_ecs_registry.hpp"
#include "common.hpp"
#include "actor_components.hpp"
#include "io_components.hpp"
#include <iostream>



void AISystem::step(float elapsed_ms)
{
    auto &movement_registry = registry.enemyMovement;
    auto &enemy_registry = registry.enemies;
	// std::cout << enemy_registry.entities.size() << " is the size of enemy entity" << std::endl;
	for (Entity entity : enemy_registry.entities) {
		Enemy& enemy = enemy_registry.get(entity);
		EnemyPattern& currPattern = enemy.currEnemyPattern();
		// std::cout << movement_registry.entities.size() << " is the size of movement entity" << std::endl;
		// std::cout << currPattern.name << " initial" << std::endl;
		EnemyMovement& movement = movement_registry.get(entity);
		Motion& motion = registry.motions.get(entity);
		currPattern.curDuration -= elapsed_ms;
		// SENSING 
		updateState(enemy, movement);
		std::cout << currPattern.name << "after update" << std::endl;

		// THINKING 
		if(currPattern.type == EnemyBehavior::FOLLOW_PLAYER) {
			movement.posB = getMove(currPattern.type, entity);
			movement.posA = motion.position;
			movement.distanceTraveled = 0.0f;
		} else if (movement.distanceTraveled >= glm::distance(movement.posA, movement.posB)) {
            movement.posA = movement.posB;
			//ACTING 
			std::cout << currPattern.name << "before getmove" << std::endl;
            movement.posB = getMove(currPattern.type, entity);
			//std::cout << "x " << movement.posB[0] << " y " << movement.posB[1] <<std::endl;
            movement.distanceTraveled = 0.f;
        }


	}

}

Reaction* getReactions(std::vector<Reaction>& list, ReactionType target) {
	for (Reaction& reaction : list) {
		if (reaction.React == target) {
			return &reaction;
		}
	}
	return nullptr;
}

void AISystem::updateState(Enemy& enemy, EnemyMovement movement) {
	vec2 playerPos = getPlayerPos();
	vec2 EnemyPos = movement.posA;
	float distance = glm::distance(playerPos, EnemyPos);
	float closeDistance = 400.f;
	float hpPercent = enemy.currHealth / enemy.maxHealth;
	EnemyPattern& currPattern = enemy.currEnemyPattern();
	bool reaction_found = false;
	if (hpPercent < 0.25f) {
		auto reaction = getReactions(currPattern.reactions, ReactionType::TWENTYFIVE_HEALTH);
        if (reaction) {
            enemy.patternIndex = reaction->index;
            reaction_found = true;
        }

	} else if (hpPercent < 0.5f) {
		auto reaction = getReactions(currPattern.reactions, ReactionType::FIFTY_HEALTH);
        if (reaction) {
            enemy.patternIndex = reaction->index;
            reaction_found = true;
        }

	} else if (distance < closeDistance) {
		auto reaction = getReactions(currPattern.reactions, ReactionType::PLAYER_CLOSE);
        if (reaction) {
			std::cout << "got reaction for follow player" << std::endl;
            enemy.patternIndex = reaction->index;
            reaction_found = true;
        }

	} else if (hpPercent < 0.75f) {
		auto reaction = getReactions(currPattern.reactions, ReactionType::SEVENTYFIVE_HEALTH);
        if (reaction) {
            enemy.patternIndex = reaction->index;
            reaction_found = true;
        }
	// PLAYER BULLET CLOSE TO BE IMPELMENTED..
	// DEFAULT STATE (CHANGE BY DURATION)
	}
    if (!reaction_found) {
        std::cout << currPattern.name << " has " << currPattern.curDuration << " ms left" << std::endl;
        if (currPattern.curDuration < 0.f) {
            enemy.patternIndex = currPattern.next;
            std::cout << currPattern.next << " index currPattern.next" << std::endl;
            currPattern.curDuration = currPattern.maxDuration;
            std::cout << "change to " << enemy.currEnemyPattern().name << std::endl;
        }
    }
}


vec2 AISystem::getMove(EnemyBehavior behavior, Entity entity) {
	// path finding hasnt been implemented
	switch(behavior) {
		case EnemyBehavior::RANDOM:
			//std::cout << "random!" << std::endl;
			return generateRandomPos(entity);
		case EnemyBehavior::FOLLOW_PLAYER:
			//std::cout << "follow!" << std::endl;
			return getPlayerPos();
		case EnemyBehavior::PATROLLING:
			return getNextPatrolPos(entity);
		case EnemyBehavior::EVADEBULLET:
			// std::cout << "evade!" << std::endl;
			return evadeBullet(entity);
		case EnemyBehavior::ROTATE_IN_PLACE:
			return getCurrentPos(entity);
		case EnemyBehavior::IDLE:
			return getCurrentPos(entity);
		default:
			return vec2{ 0, 0 };
	};
};

vec2 AISystem::getCurrentPos(Entity entity) {
	EnemyMovement movement = registry.enemyMovement.get(entity);
	return movement.posA;
}

vec2 AISystem::getNextPatrolPos(Entity entity) {
	Enemy& enemy = registry.enemies.get(entity);
	EnemyPattern& pattern = enemy.currEnemyPattern();
	std::cout << "current state: " << pattern.name << std::endl;
	pattern.pathIndex += 1;
	if (pattern.path.size() - 1 <= pattern.pathIndex) {
		pattern.pathIndex = 0;
	}
	return pattern.path[pattern.pathIndex];
	
}

vec2 AISystem::generateRandomPos(Entity entity) {
	auto& window_registry = registry.windowStates;
	WindowState& windowState = window_registry.components[0];
	int width = windowState.width;
	int height = windowState.height;
	// std::cout << "width " << windowState.width << std::endl;
	// std::cout << "height " << windowState.height << std::endl;
	float pos_x = rand() % width;
	float pos_y = rand() % height;
	vec2 scale = registry.motions.get(entity).scale;
	pos_x = glm::clamp(pos_x, 0.f + scale[0], static_cast<float>(width) - scale[0]);
	pos_y = glm::clamp(pos_y, 0.f + scale[1], static_cast<float>(height) - scale[1]);
	return vec2(pos_x, pos_y);

}

vec2 AISystem::getPlayerPos() {
	auto& player_register = registry.players;
	Entity& entity = player_register.entities[0];
	Motion& motion = registry.motions.get(entity);
	return motion.position;
}

vec2 AISystem::evadeBullet(Entity entity) {
	auto& window_registry = registry.windowStates;
	WindowState& windowState = window_registry.components[0];
	auto& motion_register = registry.motions;
	auto& pBullet_register = registry.playerBullets;
	auto& movement_register = registry.enemyMovement;
	EnemyMovement& movement = movement_register.get(entity);
	if (pBullet_register.entities.size() == 0) {
		return movement.posA;
	}
	vec2 bestEscapePos = movement.posA;
	float closestDistance = 0;
	for (Entity ent : pBullet_register.entities) {
		float danger_radius = 500.f;
		Motion& bulletMotion = motion_register.get(ent);
		Motion& enemyMotion = motion_register.get(entity);
		vec2 bulletPos = bulletMotion.position;
		vec2 enemyPos = enemyMotion.position;
		float distance = glm::distance(bulletPos, enemyPos);
		if (distance < danger_radius) {
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
			if (dist > dist1 && dist1 > closestDistance) {
				bestEscapePos = escapePos;
				closestDistance = dist;
			} else if (dist1 > dist && dist1 > closestDistance) {
				bestEscapePos = escapePos1;
				closestDistance = dist1;
			}
		}
	}
	vec2 scale = motion_register.get(entity).scale;
	bestEscapePos[0] = glm::clamp(bestEscapePos[0], 0.f + scale[0], static_cast<float>(windowState.width) - scale[0]);
	bestEscapePos[1] = glm::clamp(bestEscapePos[1], 0.f + scale[1], static_cast<float>(windowState.height) - scale[1]);
	return bestEscapePos;
}