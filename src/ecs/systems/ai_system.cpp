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

    for (int i = 0; i < movement_registry.size(); i++) {
        Entity& entity = movement_registry.entities[i];
        EnemyMovement& movement = movement_registry.get(entity);
        Enemy& enemy = enemy_registry.get(entity);
		Motion& motion = registry.motions.get(entity);

		if(enemy.behavior == EnemyBehavior::FOLLOW_PLAYER) {
			movement.posB = getMove(enemy.behavior);
			movement.posA = motion.position;
			movement.distanceTraveled = 0.0f;
		} else if (movement.distanceTraveled >= glm::distance(movement.posA, movement.posB)) {
			//pick new destination
            movement.posA = movement.posB; 
            movement.posB = getMove(enemy.behavior);
			std::cout<< movement.posA.x << movement.posA.y  << " " << movement.posB.x << movement.posB.y << std::endl;
            movement.distanceTraveled = 0.f;
        }
    }
}

vec2 AISystem::getMove(EnemyBehavior behavior) {
	// path finding hasnt been implemented
	switch(behavior) {
		case EnemyBehavior::RANDOM:
			return generateRandomPos();
		
		case EnemyBehavior::FOLLOW_PLAYER:
			return getPlayerPos();
		
		case EnemyBehavior::PATHFINDING:
			return vec2{0, 0};
		default:
			return vec2{ 0, 0 };
	}
}

vec2 AISystem::generateRandomPos() {
	auto& window_registry = registry.windowStates;
	WindowState& windowState = window_registry.components[0];
	int width = windowState.width;
	int height = windowState.height;

	float pos_x = rand() % width;
	float pos_y = rand() % height;

	return vec2(pos_x, pos_y);

}

vec2 AISystem::getPlayerPos() {
	auto& player_register = registry.players;
	Entity& entity = player_register.entities[0];
	Motion& motion = registry.motions.get(entity);
	return motion.position;
}