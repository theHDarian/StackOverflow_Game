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
	bool CircleToMesh(Entity circle, Entity mesh);
	bool PolyToWall(Entity poly, Entity wall);
	bool AABBToCircle(Entity aabb, Entity circle);
	bool AABBToPoly(Entity circle, Entity poly);
	bool AABBToMesh(Entity circle, Entity poly);

	bool CircleToLine(vec2 p1, float r, vec2 p2, vec2 p3);
	bool AABBToLine(vec2 maxxy, vec2 minxy, vec2 p1, vec2 p2);
	bool AABBToTriangle(vec2 maxxy, vec2 minxy, vec2 p1, vec2 p2, vec2 p3);
	bool LineToLine(vec2 line1Start,vec2 line1End, vec2 line2Start, vec2 line2End, vec2& intersectionPoint);
	int	 PointInAABB(const glm::vec2& p, const glm::vec2& max, const glm::vec2& min);
	bool PointInTriangle(vec2 p, vec2 p1, vec2 p2, vec2 p3);
	bool CheapCircleToCircle(vec2 p1, float r1, vec2 p2, float r2);
};