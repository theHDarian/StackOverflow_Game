#pragma once

#include "common.hpp"
#include "tiny_ecs.hpp"
#include "components.hpp"
#include "tiny_ecs_registry.hpp"

// A simple physics system that moves rigid bodies and checks for collision
class PhysicsSystem
{
public:
	void step(float elapsed_ms);

	PhysicsSystem()
	{
	}
private:
	bool CircleToCircle(Entity circleA, Entity circleB);
	bool CircleToWall(Entity circle, Entity wall);
	bool CircleToPoly(Entity circle, Entity sat);
	bool PolyToWall(Entity poly, Entity wall);
	bool CircleToLine(vec2 p1, float r, vec2 p2, vec2 p3);
};