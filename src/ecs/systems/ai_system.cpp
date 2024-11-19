// internal
#include "ai_system.hpp"
#include "tiny_ecs_registry.hpp"
#include "common.hpp"
#include "actor_components.hpp"
#include "io_components.hpp"
#include <iostream>
#include <random>
#include <glm/glm.hpp>

vec2 boundPosition(vec2 position, Entity entity)
{
	WindowState &windowState = registry.windowStates.components[0];
	int width = windowState.width;
	int height = windowState.height;
	float ratio1 =height / (float)width;
	float ratio2 = width / (float)height;
	vec2 scale = registry.motions.get(entity).scale;
	float minX = width / (6 * ratio2);
	float minY = height / (6 * ratio2) + scale.y / 2;
	float maxX = width - width / (6 * ratio2);
	float maxY = height - height / (6 * ratio2) - scale.y / 2;
	vec2 returnValue = position;
	returnValue.x = glm::clamp(position.x, minX, maxX);
	returnValue.y = glm::clamp(position.y, minY, maxY);
	return vec2(returnValue.x, returnValue.y);
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
		// std::cout << enemy.newPattern << std::endl;
		// std::cout << currPattern.name << "after update" << std::endl;
		// std::cout << currPattern.name << "after update" << std::endl;
		if (registry.boids.has(entity))
		{
			Boid &boid = registry.boids.get(entity);
			computeBoidVelocity(entity, boid);
			continue;
		}
		// THINKING
		// if (currPattern.type == EnemyBehavior::FOLLOW_PLAYER)
		// {
		// 	movement.posB = boundPosition(getMove(currPattern.type, entity), entity);
		// 	movement.posA = motion.position;
		// 	movement.distanceTraveled = 0.0f;
		// }
		if (currPattern.type == EnemyBehavior::FOLLOW_PLAYER || movement.distanceTraveled >= glm::distance(movement.posA, movement.posB) || enemy.newPattern == true)
		{
			// std::cout << currPattern.name << "after update" << std::endl;
			movement.posA = motion.position;
			// ACTING
			// std::cout << currPattern.name << "before getmove" << std::endl;
			movement.posB = boundPosition(getMove(currPattern.type, entity), entity);
			// std::cout << "x " << movement.posB[0] << " y " << movement.posB[1] <<std::endl;
			movement.distanceTraveled = 0.f;
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

void AISystem::updateState(Enemy &enemy, EnemyMovement movement, Entity entity)
{ // append and entity for more info on enemy for now
	vec2 playerPos = getPlayerPos();
	vec2 EnemyPos = movement.posA;
	vec2 EnemyPosMotion = registry.motions.get(entity).position;
	float distance = glm::distance(playerPos, EnemyPos);
	float closeDistance = 400.f;
	float hpPercent = static_cast<float>(enemy.currHealth) / static_cast<float>(enemy.maxHealth);
	EnemyPattern &currPattern = enemy.currEnemyPattern();
	bool reaction_found = false;
	float closeToBeeDistance = 100.f;
	bool closeToBee = false;

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
	if (registry.healers.has(entity))
	{
		reaction_found = updateHealerState(enemy, entity);
	}

	if (hpPercent < 0.25f)
	{
		// std::cout << "current enemy hp" << hpPercent << std::endl;
		auto reaction = getReactions(currPattern.reactions, ReactionType::TWENTYFIVE_HEALTH);
		if (reaction)
		{
			enemy.patternIndex = reaction->index;
			enemy.newPattern = true;
			reaction_found = true;
		}
	}
	// else if (closeToBee)
	// {

	// 	auto reaction = getReactions(currPattern.reactions, ReactionType::BEE_CLOSE);
	// 	if (reaction)
	// 	{
	// 		enemy.patternIndex = reaction->index;
	// 		reaction_found = true;
	// 		std::cout << "bee close!" << enemy.currEnemyPattern().name << std::endl;
	// 	}
	// }
	else if (hpPercent < 0.5f)
	{
		auto reaction = getReactions(currPattern.reactions, ReactionType::FIFTY_HEALTH);
		if (reaction)
		{
			enemy.patternIndex = reaction->index;
			enemy.newPattern = true;
			reaction_found = true;
		}
	}
	else if (distance < closeDistance)
	{
		auto reaction = getReactions(currPattern.reactions, ReactionType::PLAYER_CLOSE);
		if (reaction)
		{
			// std::cout << "got reaction for follow player" << std::endl;
			enemy.patternIndex = reaction->index;
			enemy.newPattern = true;
			reaction_found = true;
		}
	}
	else if (hpPercent < 0.75f)
	{
		auto reaction = getReactions(currPattern.reactions, ReactionType::SEVENTYFIVE_HEALTH);
		if (reaction)
		{
			enemy.patternIndex = reaction->index;
			enemy.newPattern = true;
			reaction_found = true;
		}
		// PLAYER BULLET CLOSE TO BE IMPELMENTED..
		// DEFAULT STATE (CHANGE BY DURATION)
	}
	if (!reaction_found && getReactions(currPattern.reactions, ReactionType::DURATION))
	{
		// std::cout << currPattern.name << " has " << currPattern.curDuration << " ms left" << std::endl;
		if (currPattern.curDuration < 0.f)
		{
			enemy.patternIndex = currPattern.next;
			enemy.newPattern = true;
			// std::cout << currPattern.next << " index currPattern.next" << std::endl;
			currPattern.curDuration = currPattern.maxDuration;
			// std::cout << "change to " << enemy.currEnemyPattern().name << std::endl;
		}
	}
}

bool AISystem::updateHealerState(Enemy &enemy, Entity entity)
{
	EnemyPattern &currPattern = enemy.currEnemyPattern();
	Healer &healer = registry.healers.get(entity);
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
	case EnemyBehavior::FOLLOW_PLAYER:
		// std::cout << "follow!" << std::endl;
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
	case EnemyBehavior::HEALING:
		return getTeamPos(entity);
	case EnemyBehavior::MERGE_BEE:
		return getCurrentPos(entity);
	case EnemyBehavior::SPAWNING:
		return getCurrentPos(entity);
	case EnemyBehavior::CHARGING:
		return getCharginPos(entity);
	default:
		return getCurrentPos(entity);
	};
};

vec2 AISystem::getCurrentPos(Entity entity)
{

	EnemyMovement &movement = registry.enemyMovement.get(entity);
	Motion &motion = registry.motions.get(entity);
	movement.speed = 100.f;
	return motion.position;
}

vec2 AISystem::getNextPatrolPos(Entity entity)
{
	Enemy &enemy = registry.enemies.get(entity);
	EnemyPattern &pattern = enemy.currEnemyPattern();
	if (pattern.type != EnemyBehavior::PATROLLING)
	{
		// std::cout << "Different EnemyBehavior!" << std::endl;
		return getCurrentPos(entity);
	}
	// std::cout << "current state: " << pattern.name << std::endl;
	pattern.pathIndex += 1;
	if (pattern.pathIndex >= pattern.path.size())
	{
		pattern.pathIndex = 0;
	}

	vec2 patrolFactor = pattern.path[pattern.pathIndex];

	WindowState &windowState = registry.windowStates.components[0];
	int width = windowState.width;
	int height = windowState.height;

	vec2 scale = registry.motions.get(entity).scale;
	float minX = 150.f + scale[0];
	float minY = 100.f + scale[1];
	float maxX = width - 150.f - scale[0];
	float maxY = height - 60.f - scale[1];

	float posX = minX + patrolFactor.x * (maxX - minX);
	float posY = minY + patrolFactor.y * (maxY - minY);

	posX = glm::clamp(posX, minX, maxX);
	posY = glm::clamp(posY, minY, maxY);

	return vec2(posX, posY);
}

vec2 AISystem::generateRandomPos(Entity entity)
{
	auto &window_registry = registry.windowStates;
	WindowState &windowState = window_registry.components[0];
	int width = windowState.width;
	int height = windowState.height;
	// std::cout << "width " << windowState.width << std::endl;
	// std::cout << "height " << windowState.height << std::endl;
	float pos_x = rand() % width;
	float pos_y = rand() % height;

	vec2 scale = registry.motions.get(entity).scale;
	float minX = 150.f + scale[0];
	float minY = 100.f + scale[1];
	float maxX = width - 150.f - scale[0];
	float maxY = height - 60.f - scale[1];
	pos_x = glm::clamp(pos_x, minX, maxX);
	pos_y = glm::clamp(pos_y, minY, maxY);
	return vec2(pos_x, pos_y);
}

vec2 AISystem::getCharginPos(Entity entity)
{
	auto &window_registry = registry.windowStates;
	WindowState &windowState = window_registry.components[0];
	int width = windowState.width;
	int height = windowState.height;
	vec2 playerPos = getPlayerPos();
	Motion &motion = registry.motions.get(entity);
	vec2 scale = motion.scale;
	// Calculate the direction from the enemy to the player
	vec2 direction = playerPos - motion.position;

	// Normalize the direction
	if (glm::length(direction) > 0)
	{
		direction = glm::normalize(direction);
	}

	// Define a charge distance (how far beyond the player the enemy charges)
	float chargeDistance = 100.0f; // Adjust this value as needed

	// Calculate the goal position for charging past the player
	vec2 goalPosition = playerPos + direction * chargeDistance;

	// Increase the enemy's speed for the charge
	EnemyMovement &movement = registry.enemyMovement.get(entity);
	movement.speed = 500.0f;

	float minX = 150.f + scale[0];
	float minY = 100.f + scale[1];
	float maxX = width - 150.f - scale[0];
	float maxY = height - 60.f - scale[1];

	goalPosition[0] = glm::clamp(goalPosition[0], minX, maxX);
	goalPosition[1] = glm::clamp(goalPosition[1], minY, maxY);

	return goalPosition;
}

vec2 AISystem::generateRandomPosInRadius(Entity entity, int radiusNear, int radiusFar)
{
	auto &window_registry = registry.windowStates;
	WindowState &windowState = window_registry.components[0];
	int width = windowState.width;
	int height = windowState.height;
	// std::cout << "width " << windowState.width << std::endl;
	// std::cout << "height " << windowState.height << std::endl;
	vec2 start = registry.motions.get(entity).position;
	float angle = 2.0f * M_PI * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
	float pos_x = cos(angle) * (radiusNear + rand() % (radiusFar - radiusNear)) + start.x;
	float pos_y = sin(angle) * (radiusNear + rand() % (radiusFar - radiusNear)) + start.y;

	vec2 scale = registry.motions.get(entity).scale;

	float minX = 150.f + scale[0];
	float minY = 100.f + scale[1];
	float maxX = width - 150.f - scale[0];
	float maxY = height - 60.f - scale[1];

	pos_x = glm::clamp(pos_x, minX, maxX);
	pos_y = glm::clamp(pos_y, minY, maxY);
	return vec2(pos_x, pos_y);
}

vec2 AISystem::getTeamPos(Entity entity)
{
	auto &window_registry = registry.windowStates;
	WindowState &windowState = window_registry.components[0];
	int width = windowState.width;
	int height = windowState.height;
	Motion &motion = registry.motions.get(entity);
	vec2 scale = motion.scale;
	if (registry.healers.has(entity))
	{
		Healer &healComponent = registry.healers.get(entity);
		Entity teammates = healComponent.targetEntity;
		if (teammates && registry.motions.has(teammates))
		{
			Motion &teammateMotion = registry.motions.get(teammates);
			Motion &healerMotion = registry.motions.get(entity);

			vec2 direction = teammateMotion.position - healerMotion.position;
			if (glm::length(direction) > 0)
			{
				direction = glm::normalize(direction);
			}
			float backDistance = 100.0f;

			vec2 goalPosition = teammateMotion.position - direction * backDistance;

			vec2 scale = healerMotion.scale;
			float minX = 150.f + scale.x;
			float minY = 100.f + scale.y;
			float maxX = width - 150.f - scale.x;
			float maxY = height - 60.f - scale.y;

			goalPosition.x = glm::clamp(goalPosition.x, minX, maxX);
			goalPosition.y = glm::clamp(goalPosition.y, minY, maxY);

			return goalPosition;
		}
	}
	return getCurrentPos(entity);
};

vec2 AISystem::getPlayerPos()
{
	auto &player_register = registry.players;
	Entity &entity = player_register.entities[0];
	Motion &motion = registry.motions.get(entity);
	return motion.position;
}

vec2 AISystem::evadeBullet(Entity entity)
{
	auto &window_registry = registry.windowStates;
	WindowState &windowState = window_registry.components[0];
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
	vec2 scale = motion_register.get(entity).scale;
	bestEscapePos[0] = glm::clamp(bestEscapePos[0], 0.f + scale[0], static_cast<float>(windowState.width) - scale[0]);
	bestEscapePos[1] = glm::clamp(bestEscapePos[1], 0.f + scale[1], static_cast<float>(windowState.height) - scale[1]);
	return bestEscapePos;
}

void AISystem::angryMode(Entity entity)
{
}

void AISystem::computeBoidVelocity(Entity entity, Boid &boid)
{
	Enemy &enemy = registry.enemies.get(entity);
	EnemyPattern &currentPattern = enemy.currEnemyPattern();
	if (currentPattern.type == EnemyBehavior::BOIDSGROUP)
	{
		boidComputeCoherence(entity, boid, 1.f);
		boidKeepBound(entity, boid);
		boid.velocity *= 0.8f;
	}
	else if (currentPattern.type == EnemyBehavior::BOIDSEXPLODE)
	{
		boidComputeSeperation(entity, boid, 1.f);
		boidKeepBound(entity, boid);
		boid.velocity *= 2.f;
	}
	else
	{
		boidWander(entity, boid);
		boidComputeCoherence(entity, boid, 0.01f);
		boidComputeSeperation(entity, boid, 0.05f);
		boidComputeAlignment(entity, boid, 0.02f);
		boidKeepBound(entity, boid);
	}

	float maxSpeed = 300.f;
	if (glm::length(boid.velocity) > maxSpeed)
	{
		boid.velocity = glm::normalize(boid.velocity) * maxSpeed;
	}
}

void AISystem::boidKeepBound(Entity entity, Boid &boid)
{
	WindowState &windowState = registry.windowStates.components[0];
	int width = windowState.width;
	int height = windowState.height;

	vec2 scale = registry.motions.get(entity).scale;
	float minX = 150.f + scale[0];
	float minY = 100.f + scale[1];
	float maxX = width - 150.f - scale[0];
	float maxY = height - 100.f - scale[1];
	float turnFactor = 1.0f;
	float momentumFactor = 50.f;
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

void AISystem::boidComputeCoherence(Entity entity, Boid &boid, float multiplier = 0.01f)
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
		float neighborRnge = 1000.f;
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

void AISystem::boidWander(Entity entity, Boid &boid)
{
	float wanderRadius = 50.0f;
	float wanderDistance = 100.0f;
	float wanderJitter = 5.0f;
	float wanderFactor = 0.1f;

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