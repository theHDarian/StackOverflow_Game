#pragma once

#include <vector>

#include "tiny_ecs_registry.hpp"
#include "common.hpp"

class AISystem
{
public:
	void step(float elapsed_ms);
	vec2 getMove(EnemyBehavior behavior);
	vec2 generateRandomPos();
	vec2 getPlayerPos();
};