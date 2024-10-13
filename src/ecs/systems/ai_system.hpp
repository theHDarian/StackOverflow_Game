#pragma once

#include <vector>

#include "tiny_ecs_registry.hpp"
#include "common.hpp"

class AISystem
{
public:
	void step(float elapsed_ms);
	static vec2 getMove(EnemyBehavior behavior);
private:
	static vec2 generateRandomPos();
	static vec2 getPlayerPos();

};