// internal
#include "physics_system.hpp"
#include "world_init.hpp"
#include <glm/trigonometric.hpp>

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
		// vec2 world_velocity;
		// world_velocity[0] = cos(motion.angle)*motion.velocity[0] - sin(motion.angle)*motion.velocity[1];
		// world_velocity[1] = sin(motion.angle) * motion.velocity[0] + cos(motion.angle) * motion.velocity[1];
		motion.position += motion.velocity * step_seconds;
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
		// if (CircleToPoly(player, eBullets.entities[i])) {
		// 	registry.collisions.emplace_with_duplicates(player, eBullets.entities[i]);
		// }
		if (CircleToCircle(player, eBullets.entities[i])) {
			registry.collisions.emplace_with_duplicates(player, eBullets.entities[i]);
		}
	}

	// Player  -> Enemies		(Circle to Circle)
	// Enemies -> PlayerBullets	(Circle to Circle)
	ComponentContainer<Enemy>& enemies = registry.enemies;
	ComponentContainer<PlayerBullet>& pBullets = registry.playerBullets;
	for (uint i = 0; i < enemies.components.size(); i++) {
		if (CircleToCircle(player, enemies.entities[i])) {

			// if enemy has damaged sprite, make it switch to damaged sprite on hit
			// note: this is just a demonstration of sprite switching,
			// won't necessarily have enemy on collision sprites
			if (registry.sprites.has(enemies.entities[i])) {
				auto& spriteMap = registry.sprites.get(enemies.entities[i]).sprites;
				if (spriteMap.count(SPRITE_STATE::DAMAGED))
					registry.renderRequests.get(enemies.entities[i]).used_texture = spriteMap[SPRITE_STATE::DAMAGED];
			}

			// the same can be done with the player
			if (registry.sprites.has(player)) {
				auto& spriteMap = registry.sprites.get(player).sprites;
				if (spriteMap.count(SPRITE_STATE::DAMAGED))
					registry.renderRequests.get(player).used_texture = spriteMap[SPRITE_STATE::DAMAGED];
			}

			registry.collisions.emplace_with_duplicates(player, enemies.entities[i]);
		}
		for (uint j = 0; j < pBullets.components.size(); j++) {
			if (CircleToCircle(enemies.entities[i], pBullets.entities[j])) {
				registry.collisions.emplace_with_duplicates(enemies.entities[i], pBullets.entities[j]);
			}
		}
	}
}


bool PhysicsSystem::CircleToCircle(Entity circleA, Entity circleB) {
	//if (!registry.circleColliders.has(circleA) || !registry.circleColliders.has(circleB)) return false;
	Motion& motionA = registry.motions.get(circleA);
	Motion& motionB = registry.motions.get(circleB);

	CircleCollider& cA = registry.circleColliders.get(circleA);
	CircleCollider& cB = registry.circleColliders.get(circleB);

	return (glm::distance(motionA.position, motionB.position) < cA.radius + cB.radius);
}

bool PhysicsSystem::CircleToWall(Entity circle, Entity wall) {
	//if (!registry.circleColliders.has(circle) || !registry.walls.has(wall)) return false;
	Motion& m = registry.motions.get(circle);
	CircleCollider& c = registry.circleColliders.get(circle);

	WallCollider& w = registry.walls.get(wall);

	return CircleToLine(m.position, c.radius, w.startPosition, w.endPosition);
}

// Returns true if the circle is intersecting a side of the polygon, 
// false if the circle is wholly inside the polygon
// (No case should arise where that happens though)
bool PhysicsSystem::CircleToPoly(Entity circle, Entity poly) {
	//if (!registry.circleColliders.has(circle) || !registry.polyColliders.has(poly)) return false;
	Motion& mA = registry.motions.get(circle);
	Motion& mB = registry.motions.get(poly);

	CircleCollider& c = registry.circleColliders.get(circle);
	PolyCollider& s = registry.polyColliders.get(poly);

	float ang = -glm::radians(mB.angle);
	vec2 mArot = {mA.position.x * cos(ang) - mA.position.y * sin(ang), mA.position.x * sin(ang) + mA.position.y * cos(ang)};

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
	vec2 a = p1 - p2;
	vec2 b = p3 - p2;
	vec2 c = (glm::dot(a, glm::normalize(b)) * glm::normalize(b));
	vec2 d = a - c;
	return (abs(glm::length(c) + glm::length(p3-p2-c) - glm::length(p3-p2)) < 0.01 && glm::length(d) < r);
}