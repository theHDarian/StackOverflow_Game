// internal
#include "physics_system.hpp"
#include "world_init.hpp"

// Returns the local bounding coordinates scaled by the current size of the entity
vec2 get_bounding_box(const Motion& motion)
{
	// abs is to avoid negative scale due to the facing direction.
	return { abs(motion.scale.x), abs(motion.scale.y) };
}

// This is a SUPER APPROXIMATE check that puts a circle around the bounding boxes and sees
// if the center point of either object is inside the other's bounding-box-circle. You can
// surely implement a more accurate detection
bool collides(const Motion& motion1, const Motion& motion2)
{
	vec2 dp = motion1.position - motion2.position;
	float dist_squared = dot(dp,dp);
	const vec2 other_bonding_box = get_bounding_box(motion1) / 2.f;
	const float other_r_squared = dot(other_bonding_box, other_bonding_box);
	const vec2 my_bonding_box = get_bounding_box(motion2) / 2.f;
	const float my_r_squared = dot(my_bonding_box, my_bonding_box);
	const float r_squared = max(other_r_squared, my_r_squared);
	if (dist_squared < r_squared)
		return true;
	return false;
}

void PhysicsSystem::step(float elapsed_ms)
{
	// Move based on how much time has passed, this is to (partially) avoid
	// having entities move at different speed based on the machine.
	auto& motion_registry = registry.motions;
	for(uint i = 0; i< motion_registry.size(); i++)
	{
		Motion& motion = motion_registry.components[i];
		Entity entity = motion_registry.entities[i];
		float step_seconds = elapsed_ms / 1000.f;
		//have velocity be relative to local rotation angle
		vec2 world_velocity;
		world_velocity[0] = cos(motion.angle)*motion.velocity[0] - sin(motion.angle)*motion.velocity[1];
		world_velocity[1] = sin(motion.angle) * motion.velocity[0] + cos(motion.angle) * motion.velocity[1];
		motion.position += world_velocity * step_seconds;
		//(void)elapsed_ms; // placeholder to silence unused warning until implemented
	}


	// Collision tests:

	Entity player = registry.players.entities[0];

	// Player  -> Walls			(Circle to Line)
	ComponentContainer<WallCollider>& walls = registry.walls;
	for (uint i = 0; i < walls.components.size(); i++) {
		if (CircleToWall(player, walls.entities[i])) {
			registry.collisions.emplace_with_duplicates(player, walls.entities[i]);
		}
	}

	// Player  -> EnemyBullets	(Circle to Poly)
	ComponentContainer<EnemyBullet>& eBullets = registry.enemyBullets;
	for (uint i = 0; i < eBullets.components.size(); i++) {
		if (CircleToPoly(player, eBullets.entities[i])) {
			registry.collisions.emplace_with_duplicates(player, eBullets.entities[i]);
		}
	}

	// Player  -> Enemies		(Circle to Circle)
	// Enemies -> PlayerBullets	(Circle to Circle)
	ComponentContainer<Enemy>& enemies = registry.enemies;
	ComponentContainer<PlayerBullet>& pBullets = registry.playerBullets;
	for (uint i = 0; i < enemies.components.size(); i++) {
		if (CircleToCircle(player, enemies.entities[i])) {
			registry.collisions.emplace_with_duplicates(player, enemies.entities[i]);
		}
		for (uint j = 0; j < pBullets.components.size(); j++) {
			if (CircleToCircle(eBullets.entities[i], pBullets.entities[i])) {
				registry.collisions.emplace_with_duplicates(enemies.entities[i], pBullets.entities[i]);
			}
		}
	}
}


bool PhysicsSystem::CircleToCircle(Entity circleA, Entity circleB) {
	Motion& motionA = registry.motions.get(circleA);
	Motion& motionB = registry.motions.get(circleB);

	CircleCollider& cA = registry.circleColliders.get(circleA);
	CircleCollider& cB = registry.circleColliders.get(circleB);

	return (glm::distance(motionA.position, motionB.position) < cA.radius + cB.radius);
}

bool PhysicsSystem::CircleToWall(Entity circle, Entity wall) {
	Motion& m = registry.motions.get(circle);
	CircleCollider& c = registry.circleColliders.get(circle);

	WallCollider& w = registry.walls.get(wall);

	return CircleToLine(m.position, c.radius, w.startPosition, w.endPosition);
}

// Returns true if the circle is intersecting a side of the polygon, 
// false if the circle is wholly inside the polygon
// (No case should arise where that happens though)
bool PhysicsSystem::CircleToPoly(Entity circle, Entity sat) {
	Motion& mA = registry.motions.get(circle);
	Motion& mB = registry.motions.get(sat);

	CircleCollider& c = registry.circleColliders.get(circle);
	PolyCollider& s = registry.polyColliders.get(sat);

	// Quick test to remove obviously not overlapping shapes
	if (glm::distance(mA.position, mB.position) > c.radius + s.maxLength) return false;

	// Offset the circle position to be relative to the origin (like the polygon points)
	// Test the lines formed by every 2 adjacent polygon points against the circle
	for (uint i = 1; i < s.offsetVertices.size(); i++) {
		if (CircleToLine({ mA.position.x - mB.position.x, mA.position.y - mB.position.y }, c.radius, s.offsetVertices[i], s.offsetVertices[i - 1])) return true;
	}
	return false;
}

// Formula for distance from a point to a line defined by 2 points, taken from wikipedia
bool PhysicsSystem::CircleToLine(vec2 p1, float r, vec2 p2, vec2 p3) {
	float dist = abs((p3.y - p2.y) * p1.x - (p3.x - p2.x) * p1.y + p3.x * p2.y - p2.x * p3.y);
	dist /= sqrt((p3.y - p2.y) * (p3.y - p2.y) + (p3.x - p2.x) * (p3.x - p2.x));
	return (dist < r);
}