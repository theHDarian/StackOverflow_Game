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
	static vec2 getCharginPos(Entity entity);
	static vec2 getTeamPos(Entity entity);
	static void updateState(Enemy& enemy, EnemyMovement movement, Entity entity);
	static bool updateHealerState(Enemy &enemy, Entity entity);
	static void angryMode(Entity entity);
	static void computeBoidVelocity(Entity entity , Boid& boid);
	static void boidKeepBound(Entity entity, Boid& boid);
	static void boidComputeCoherence(Entity entity, Boid& boid);
	static void boidComputeSeperation(Entity entity, Boid& boid);
	static void boidComputeAlignment(Entity entity, Boid& boid);
	static void boidWander(Entity entity, Boid &boid);
};