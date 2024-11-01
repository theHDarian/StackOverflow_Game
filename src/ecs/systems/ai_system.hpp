#pragma once

#include <vector>

#include "tiny_ecs_registry.hpp"
#include "common.hpp"

class AISystem
{
public:
	void step(float elapsed_ms);
	static vec2 getMove(EnemyBehavior behavior, Entity entity);
private:
	static vec2 generateRandomPos(Entity entity);
	static vec2 getPlayerPos();
	static vec2 getCurrentPos(Entity entity);
	static vec2 evadeBullet(Entity entity);
	static vec2 getNextPatrolPos(Entity entity);
	static void updateState(Enemy& enemy, EnemyMovement movement, Entity entity);
	static void angryMode(Entity entity);

};