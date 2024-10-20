// internal
#include "physics_system.hpp"
#include "world_init.hpp"
#include <glm/trigonometric.hpp>
#include "components/components.hpp"

void PhysicsSystem::step(float elapsed_ms)
{
	auto& motion_registry = registry.motions;
	float step_seconds = elapsed_ms / 1000.f;
	Entity player = registry.players.entities[0];
	auto& dash_registry = registry.dashes;
	ComponentContainer<WallCollider>& walls = registry.walls;

	// Move motion entities that are not dashing
	for(uint i = 0; i< motion_registry.size(); i++)
	{
		Motion& motion = motion_registry.components[i];
		Entity entity = motion_registry.entities[i];
		if (dash_registry.has(entity)) 
			continue;
		motion.position += motion.velocity * step_seconds;
		motion.velocity += motion.veer * step_seconds;

		//slightly broken
		if (registry.enemyBullets.has(entity) || registry.playerBullets.has(entity)) motion.angle = atan2(motion.velocity.y, motion.velocity.x);

		if (registry.homes.has(entity) && registry.motions.has(registry.homes.get(entity).target)) {
			vec2 target = registry.motions.get(registry.homes.get(entity).target).position;
			float intensity = registry.homes.get(entity).homingIntensity;
			target -= motion.position;
			float mag = glm::length(motion.velocity);
			motion.velocity = mag * (intensity * glm::normalize(target) + (1 - intensity) * glm::normalize(motion.velocity));
		}
	}

	// Move dashing entities
	for(uint i = 0; i< dash_registry.size(); i++)
	{
		Entity entity = dash_registry.entities[i];
		Motion& motion = motion_registry.get(entity);
		// check if dashing entity will intersect a wall
		vec2 startPosition = motion.position;
		vec2 endPosition = motion.position + motion.velocity * step_seconds;
		bool hasCollided = false;
		vec2 closestIntersection;
		for (uint i = 0; i < walls.components.size(); i++) {
			WallCollider& wall = walls.components[i];
			vec2 intersectionPoint;
			if (LineToLine(startPosition,endPosition, wall.startPosition,wall.endPosition,intersectionPoint)) {
				//get intersection point of the closest wall
				if (registry.circleColliders.has(entity)) {
					if (!hasCollided || glm::distance(intersectionPoint,motion.position) < glm::distance(closestIntersection,motion.position)) {
						closestIntersection = intersectionPoint;
					}
					hasCollided = true;
				} else {
					//std::cout << "Unhandled Dash Component Collision!!" << std::endl;
				}
			}
		}

		if (!hasCollided) {
			motion.position += motion.velocity * step_seconds;
		} else { //stop at closest wall
			CircleCollider& circle = registry.circleColliders.get(entity);
			vec2 bounceBack = glm::normalize(-motion.velocity) * circle.radius;
			motion.position = closestIntersection + bounceBack;
		}
	}


	// Collision tests:

	// Player  -> Walls			(Circle to Line)
	for (uint i = 0; i < walls.components.size(); i++) {
		if (CircleToWall(player, walls.entities[i])) {
			registry.collisions.emplace_with_duplicates(player, walls.entities[i]);
		}
	}

	// Player  -> EnemyBullets	(Circle to Circle for now)
	// EnemyBullets -> Walls	(Circle to wall for now)
	ComponentContainer<EnemyBullet>& eBullets = registry.enemyBullets;
	for (uint i = 0; i < eBullets.components.size(); i++) {
		if ((registry.circleColliders.has(eBullets.entities[i]) && CircleToCircle(player, eBullets.entities[i])) || 
			(registry.polyColliders.has(eBullets.entities[i]) && CircleToPoly(player, eBullets.entities[i]))) {
			registry.collisions.emplace_with_duplicates(player, eBullets.entities[i]);
		}
		for (uint j = 0; j < walls.components.size(); j++) {
			if ((registry.circleColliders.has(eBullets.entities[i]) && CircleToWall(eBullets.entities[i], walls.entities[j])) ||
				(registry.polyColliders.has(eBullets.entities[i]) && PolyToWall(eBullets.entities[i], walls.entities[j]))) {
				registry.collisions.emplace_with_duplicates(eBullets.entities[i], walls.entities[j]);
			}
		}
	}

	// Player  -> Enemies		(Circle to Circle)
	// Enemies -> PlayerBullets	(Circle to Circle)
	ComponentContainer<Enemy>& enemies = registry.enemies;
	ComponentContainer<PlayerBullet>& pBullets = registry.playerBullets;

	// PlayerBullets -> Walls	(Circle to Wall)
	for (uint i = 0; i < pBullets.components.size(); i++) {
		for (uint j = 0; j < walls.components.size(); j++) {
			if (CircleToWall(pBullets.entities[i], walls.entities[j])) {
				registry.collisions.emplace_with_duplicates(pBullets.entities[i], walls.entities[j]);
			}
		}
	}

	for (uint i = 0; i < enemies.components.size(); i++) {
		if (CircleToCircle(player, enemies.entities[i])) {
			registry.collisions.emplace_with_duplicates(player, enemies.entities[i]);
		}
		for (uint j = 0; j < pBullets.components.size(); j++) {
			if (CircleToCircle(enemies.entities[i], pBullets.entities[j])) {
				registry.collisions.emplace_with_duplicates(enemies.entities[i], pBullets.entities[j]);
			}
		}
	}



	 //Player  -> debugComponents (circle to poly)
	ComponentContainer<DebugComponent>& debug = registry.debugComponents;
	for (uint i = 0; i < debug.components.size(); i++) {
		if (CircleToPoly(player, debug.entities[i])) {
			registry.collisions.emplace_with_duplicates(player, debug.entities[i]);
		}
	}

	//Player -> doors
	ComponentContainer<Door>& doors = registry.doors;
	Motion& m = registry.motions.get(player);
	CircleCollider& c = registry.circleColliders.get(player);
	for (uint i = 0; i < doors.components.size(); i++) {
		if (!doors.components[i].isPrev && CircleToLine(m.position,c.radius,doors.components[i].startPos,doors.components[i].endPos)) {
			//spawn on side opposite to the door
			registry.changeRoomRequests.emplace(player,doors.components[i].room,i);
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

bool PhysicsSystem::PolyToWall(Entity poly, Entity wall) {
	
	//if (!registry.circleColliders.has(circle) || !registry.walls.has(wall)) return false;
	Motion& m = registry.motions.get(poly);

	WallCollider& w = registry.walls.get(wall);
	// Assumes a small circle around the center of the polygon as the collision point for walls
	return CircleToLine(m.position, registry.polyColliders.get(poly).minLength , w.startPosition, w.endPosition);
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
	if (CircleToLine({ mA.position.x - mB.position.x, mA.position.y - mB.position.y }, c.radius, s.offsetVertices[0], s.offsetVertices[s.offsetVertices.size()-1])) return true;
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

	// Circle center projected onto the line is between p2 and p3
	if (abs(glm::length(c) + glm::length(b - c) - glm::length(b)) < 0.01) {
		vec2 d = a - c;
		return (glm::length(d) < r);
	}

	// Circle is intersecting with p2 or p3
	if (glm::distance(p1, p2) < r || glm::distance(p1, p3) < r) return true;

	return false;
}
bool PhysicsSystem::LineToLine(vec2 line1Start,vec2 line1End, vec2 line2Start, vec2 line2End, vec2& intersectionPoint) {
	auto cross = [](const glm::vec2& v1, const glm::vec2& v2) { return v1.x * v2.y - v1.y * v2.x; };
    glm::vec2 r = line1End - line1Start, s = line2End - line2Start, pq = line2Start - line1Start;
    float rxs = cross(r, s);
    if (rxs == 0) return false; // Lines are parallel
    float t = cross(pq, s) / rxs, u = cross(pq, r) / rxs;
	intersectionPoint = line1Start + t * r;
    return (t >= 0 && t <= 1 && u >= 0 && u <= 1);
}
