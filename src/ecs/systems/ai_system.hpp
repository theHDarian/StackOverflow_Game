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
	static vec2 generateRandomPosInRadius(Entity entity, int radiusNear, int radiusFar);
	static vec2 getPlayerPos();
	static vec2 getCurrentPos(Entity entity);
	static vec2 evadeBullet(Entity entity);
	static vec2 getNextPatrolPos(Entity entity);
	static vec2 getCharginPos(Entity entity);
	static vec2 getRecoilPos(Entity entity);
	static vec2 getTeamPos(Entity entity);
	static vec2 getTeleportPos(Entity entity);
	static void updateState(Enemy& enemy, EnemyMovement movement, Entity entity);
	static bool updateHealerState(Enemy &enemy, Entity entity);
	static void angryMode(Entity entity);
	static void computeBoidVelocity(Entity entity , Boid& boid);
	static void boidKeepBound(Entity entity, Boid& boid, float minx, float miny, float maxx, float maxy);
	static void boidComputeCoherence(Entity entity, Boid& boid, float multiplier, float range);
	static void boidComputeSeperation(Entity entity, Boid& boid, float multiplier);
	static void boidComputeAlignment(Entity entity, Boid& boid, float multiplier);
	static void boidWander(Entity entity, Boid &boid, float multiplier);
	static void boidFollowPlayer(Entity entity, Boid &boid, float multiplier);
	static void boidCircleRoom(Entity entity, Boid& boid, float multiplier, float angularSpeed);
	static void boidEvadePlayer(Entity entity, Boid &boid, float multiplier);
	static void boidComputeAllFactor(Entity entity, Boid &boid, float multiplierCoherence,float multiplierSeperation, float multiplierAlignment, float range);
};