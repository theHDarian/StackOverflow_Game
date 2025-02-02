// internal
#include "physics_system.hpp"
#include "world_init.hpp"
#include <glm/trigonometric.hpp>
#include "components.hpp"
#include <glm/gtx/string_cast.hpp>
#include <bitset>

auto &motion_registry = registry.motions;
auto &dash_registry = registry.dashes;
ComponentContainer<WallCollider> &walls = registry.walls;
ComponentContainer<EnemyBullet> &eBullets = registry.enemyBullets;
ComponentContainer<Enemy> &enemies = registry.enemies;
ComponentContainer<PlayerBullet> &pBullets = registry.playerBullets;
ComponentContainer<Critter> &critters = registry.critters;
ComponentContainer<Door> &doors = registry.doors;
ComponentContainer<DebugComponent> &debug = registry.debugComponents;
ComponentContainer<Ignore> &ignores = registry.ignores;

void PhysicsSystem::step(float elapsed_ms)
{
	Entity player = registry.players.entities[0];
	Motion &m = registry.motions.get(player);
	CircleCollider &c = registry.circleColliders.get(player);
	float step_seconds = elapsed_ms / 1000.f;

	// Move motion entities that are not dashing
	for (uint i = 0; i < motion_registry.size(); i++)
	{
		Motion &motion = motion_registry.components[i];
		Entity entity = motion_registry.entities[i];
		if (registry.players.has(entity))
			continue;
		motion.position += motion.velocity * step_seconds;
		motion.velocity += motion.veer * step_seconds;

		// slightly broken
		if (glm::length(motion.velocity) > 0.001 && !registry.lasers.has(entity) && ((registry.enemyBullets.has(entity) && registry.enemyBullets.get(entity).bulletBounce > -1) || registry.playerBullets.has(entity)))
			motion.angle = atan2(motion.velocity.y, motion.velocity.x);

		if (registry.homes.has(entity))
		{
			if (registry.motions.has(registry.homes.get(entity).target))
			{
				vec2 target = registry.motions.get(registry.homes.get(entity).target).position;
				float intensity = registry.homes.get(entity).homingIntensity;
				target -= motion.position;
				float mag = glm::length(motion.velocity);
				motion.velocity = mag * glm::normalize(intensity * glm::normalize(target) + (1 - intensity) * glm::normalize(motion.velocity));
			}
			if (registry.playerBullets.has(entity) && registry.enemies.entities.size() > 0)
			{
				// Find a new target
				uint newTarget = 0;
				float minDist = 999999;
				for (uint i = 0; i < enemies.components.size(); i++)
				{
					if (ignores.get(entity).has(enemies.entities[i]))
						continue;
					Motion &emm = motion_registry.get(enemies.entities[i]);
					vec2 vecTo = emm.position - motion.position;
					if (glm::dot(vecTo, vecTo) < minDist)
					{
						minDist = glm::dot(vecTo, vecTo);
						newTarget = i;
					}
				}
				if (minDist < 999990)
					registry.homes.get(entity).target = enemies.entities[newTarget];
				if (minDist > 999990)
					registry.homes.remove(entity);
			}
		}

		if (registry.lasers.has(entity))
		{
			Laser &laser = registry.lasers.get(entity);
			EnemyBullet &eBullet = registry.enemyBullets.get(entity);
			if (registry.enemies.has(laser.start) && !registry.deleteds.has(laser.start))
			{
				Enemy &enemy = registry.enemies.get(laser.start);
				Motion &start = registry.motions.get(laser.start);
				if (enemy.rotationBehaviour == EnemyRotationBehavior::NONE || enemy.rotationBehaviour == EnemyRotationBehavior::REGULAR || enemy.rotationBehaviour == EnemyRotationBehavior::FACE_UP)
				{
					motion.angle += laser.rotation;
				}
				else if (enemy.rotationBehaviour == EnemyRotationBehavior::FACE_CENTER)
				{
					motion.angle = start.angle;
				}
				else if (enemy.rotationBehaviour == EnemyRotationBehavior::LASER_CONTROL)
				{
					motion.angle = start.angle + laser.rotation;
				}
				else if (enemy.rotationBehaviour == EnemyRotationBehavior::FACE_PLAYER) {
					motion.angle = laser.rotation * motion.angle + (1.f - laser.rotation) * start.angle;
				}
				if (laser.growth < 100.f || (eBullet.initialRange - eBullet.bulletRange > 1000))
					laser.length += laser.growth;
				if (registry.enemyGroups.has(laser.start))
				{
					EnemyGroup &group = registry.enemyGroups.get(laser.start);
					if (group.others.size() == 1)
					{
						// this is a twin laser
						Motion &otherMotion = registry.motions.get(group.others[0]);
						float dist = glm::distance(otherMotion.position, start.position);
						laser.length = min(dist, laser.length);
						vec2 diff = otherMotion.position - start.position;
						laser.rotation = atan2(diff.y, diff.x);
					}
				}
				vec2 goal = start.position + vec2(cos(motion.angle), sin(motion.angle)) * (10000.f);
				float currLength = laser.length;
				float maxFind = 9999999.f;
				for (uint i = 0; i < walls.components.size(); i++)
				{
					WallCollider &wall = walls.components[i];
					vec2 intersectionPoint;
					if (LineToLine(start.position, goal, wall.startPosition, wall.endPosition, intersectionPoint))
					{
						float dist = glm::distance(intersectionPoint, start.position);
						maxFind = min(maxFind, dist);
						currLength = min(currLength, dist);
						registry.collisions.emplace_with_duplicates(entity, walls.entities[i]);
					}
				}
				laser.maxLength = maxFind;
				motion.position = start.position + vec2(cos(motion.angle), sin(motion.angle)) * min(laser.length, currLength) * 0.5f;
				motion.scale.x = min(laser.length, currLength);
			}
			else
			{
				if (!registry.deleteds.has(entity))
					registry.deleteds.emplace(entity);
			}
		}
	}

	// Move dashing entities
	for (uint i = 0; i < registry.players.size(); i++)
	{
		Entity entity = registry.players.entities[i];
		Motion &motion = motion_registry.get(entity);
		// check if dashing entity will intersect a wall
		vec2 startPosition = motion.position;
		vec2 endPosition = motion.position + motion.velocity * step_seconds;
		bool hasCollided = false;
		vec2 closestIntersection;
		for (uint i = 0; i < walls.components.size(); i++)
		{
			WallCollider &wall = walls.components[i];
			vec2 intersectionPoint;
			if (LineToLine(startPosition, endPosition, wall.startPosition, wall.endPosition, intersectionPoint))
			{
				// get intersection point of the closest wall
				if (registry.circleColliders.has(entity))
				{
					if (!hasCollided || distanceSquared(intersectionPoint, motion.position) < distanceSquared(closestIntersection, motion.position))
					{
						closestIntersection = intersectionPoint;
						hasCollided = true;
					}
				}
				else
				{
					// std::cout << "Unhandled Dash Component Collision!!" << std::endl;
				}
			}
		}

		if (!hasCollided)
		{
			motion.position += motion.velocity * step_seconds;
		}
		else
		{ // stop at closest wall
			CircleCollider &circle = registry.circleColliders.get(entity);
			vec2 bounceBack = glm::normalize(-motion.velocity) * circle.radius;
			motion.position = closestIntersection + bounceBack;
		}
	}

	// player bullet vs wall check
	for (uint i = 0; i < registry.playerBullets.size(); i++)
	{
		Entity entity = registry.playerBullets.entities[i];
		PlayerBullet &pBullet = registry.playerBullets.components[i];
		Motion &motion = motion_registry.get(entity);

		if (pBullet.bulletSpeed <= 300)
			continue;

		// check if dashing entity will intersect a wall
		vec2 startPosition = motion.position - (motion.velocity - motion.veer * step_seconds) * step_seconds;
		vec2 endPosition = motion.position;
		bool hasCollided = false;
		vec2 closestIntersection;
		for (uint j = 0; j < walls.components.size(); j++)
		{
			WallCollider &wall = walls.components[j];
			vec2 intersectionPoint;
			if (LineToLine(startPosition, endPosition, wall.startPosition, wall.endPosition, intersectionPoint))
			{
				// get intersection point of the closest wall
				if (!hasCollided || distanceSquared(intersectionPoint, startPosition) < distanceSquared(closestIntersection, startPosition))
				{
					closestIntersection = intersectionPoint;
					hasCollided = true;
					break;
				}
			}
		}

		if (hasCollided)
		{ // stop at closest wall
			float radius = 5.f;
			if (registry.circleColliders.has(entity))
			{
				radius = registry.circleColliders.get(entity).radius;
			}
			vec2 bounceBack = vec2(0);
			if ((motion.velocity - motion.veer * step_seconds) != vec2(0))
			{
				bounceBack = glm::normalize(-(motion.velocity - motion.veer * step_seconds)) * radius / 2.f;
			}
			motion.position = closestIntersection + bounceBack;
		}
	}

	// enemy bullet vs wall check
	for (uint i = 0; i < registry.enemyBullets.size(); i++)
	{
		Entity entity = registry.enemyBullets.entities[i];
		EnemyBullet &eBullet = registry.enemyBullets.components[i];
		Motion &motion = motion_registry.get(entity);

		if (registry.lasers.has(entity))
			continue;
		if (eBullet.bulletSpeed <= 300)
			continue;

		// check if dashing entity will intersect a wall
		vec2 startPosition = motion.position - (motion.velocity - motion.veer * step_seconds) * step_seconds;
		vec2 endPosition = motion.position;
		bool hasCollided = false;
		vec2 closestIntersection;
		for (uint j = 0; j < walls.components.size(); j++)
		{
			WallCollider &wall = walls.components[j];
			vec2 intersectionPoint;
			if (LineToLine(startPosition, endPosition, wall.startPosition, wall.endPosition, intersectionPoint))
			{
				// get intersection point of the closest wall
				if (!hasCollided || distanceSquared(intersectionPoint, startPosition) < distanceSquared(closestIntersection, startPosition))
				{
					closestIntersection = intersectionPoint;
					hasCollided = true;
					break;
				}
			}
		}

		if (hasCollided)
		{ // stop at closest wall
			float radius = 5.f;
			if (registry.circleColliders.has(entity))
			{
				radius = registry.circleColliders.get(entity).radius;
			}
			vec2 bounceBack = vec2(0);
			if ((motion.velocity - motion.veer * step_seconds) != vec2(0))
			{
				bounceBack = glm::normalize(-(motion.velocity - motion.veer * step_seconds)) * radius / 2.f;
			}

			motion.position = closestIntersection + bounceBack;
		}
	}

	// Collision tests:

	// Player  -> Walls			(Circle to Line)
	for (uint i = 0; i < walls.components.size(); i++)
	{
		if (CircleToWall(player, walls.entities[i]))
		{
			registry.collisions.emplace_with_duplicates(player, walls.entities[i]);
		}
	}

	// Player to interactible ranges/objects
	for (uint i = 0; i < registry.interactables.components.size(); i++)
	{
		Entity interactable = registry.interactables.entities[i];
		if ((registry.circleColliders.has(interactable) && CircleToCircle(interactable, player)) ||
			(registry.aabbs.has(interactable) && AABBToCircle(interactable, player)))
		{
			registry.collisions.emplace_with_duplicates(player, interactable);
		}
	}

	// Player  -> EnemyBullets	(Circle to Circle for now)
	// EnemyBullets -> Walls	(Circle to wall for now)
	for (uint i = 0; i < eBullets.components.size(); i++)
	{
		if ((registry.circleColliders.has(eBullets.entities[i]) && AABBToCircle(player, eBullets.entities[i])) ||
			(registry.polyColliders.has(eBullets.entities[i]) && AABBToPoly(player, eBullets.entities[i])) ||
			(registry.lasers.has(eBullets.entities[i]) && AABBToLaser(player, eBullets.entities[i])))
		{
			registry.collisions.emplace_with_duplicates(player, eBullets.entities[i]);
		}
		for (uint j = 0; j < walls.components.size(); j++)
		{
			// Should check for collision for piledriver bullets too, because their speed gets set to 0 on collision
			if ((registry.circleColliders.has(eBullets.entities[i]) && CircleToWall(eBullets.entities[i], walls.entities[j])) ||
				(registry.polyColliders.has(eBullets.entities[i]) && PolyToWall(eBullets.entities[i], walls.entities[j])))
			{
				registry.collisions.emplace_with_duplicates(eBullets.entities[i], walls.entities[j]);
			}
		}
	}

	// PlayerBullets -> Walls	(Circle to Wall)
	for (uint i = 0; i < pBullets.components.size(); i++)
	{
		for (uint j = 0; j < walls.components.size(); j++)
		{
			if (CircleToWall(pBullets.entities[i], walls.entities[j]))
			{
				registry.collisions.emplace_with_duplicates(pBullets.entities[i], walls.entities[j]);
			}
		}
	}

	// Player  -> Enemies		(Circle to Circle)
	// Enemies -> PlayerBullets	(Circle to Circle)
	for (uint i = 0; i < enemies.components.size(); i++)
	{
		if ((registry.circleColliders.has(enemies.entities[i]) && CircleToCircle(player, enemies.entities[i])) ||
			registry.meshColliders.has(enemies.entities[i]) && (AABBToMesh(player, enemies.entities[i])) ||
			registry.aabbs.has(enemies.entities[i]) && (AABBToAABB(player, enemies.entities[i])))
		{
			registry.collisions.emplace_with_duplicates(player, enemies.entities[i]);
		}
		for (uint j = 0; j < pBullets.components.size(); j++)
		{
			if (registry.circleColliders.has(enemies.entities[i]) && CircleToCircle(enemies.entities[i], pBullets.entities[j]) ||
				(registry.meshColliders.has(enemies.entities[i]) && CircleToMesh(pBullets.entities[j], enemies.entities[i])))
			{
				if (!ignores.get(pBullets.entities[j]).has(enemies.entities[i]))
					registry.collisions.emplace_with_duplicates(enemies.entities[i], pBullets.entities[j]);
			}
		}
	}

	// Player -> doors
	for (uint i = 0; i < doors.components.size(); i++)
	{
		if (/*!doors.components[i].isPrev &&*/ CircleToLine(m.position, c.radius, doors.components[i].startPos, doors.components[i].endPos))
		{
			// spawn on side opposite to the door
			Room& room = registry.maps.components[0].currRoom;
			if (registry.mapRequests.components.size() <= 0 && !registry.nearbyInteractables.has(registry.doors.entities[i]) && (room.cleared || room.type == RoomType::TutorialRoom1)) {
				if (registry.interactables.has(registry.doors.entities[i]) 
					&& registry.interactables.get(registry.doors.entities[i]).interactType == InteractableType::DialogueInteractable) {
					registry.nearbyInteractables.emplace(registry.doors.entities[i]);
				}
				else {
					registry.interactables.get(registry.doors.entities[i]).timer -= elapsed_ms;
					if (registry.interactables.get(registry.doors.entities[i]).timer <= 0) {
						MapRequest& mapReq = registry.mapRequests.emplace(doors.entities[i], MapRequestType::ChangeRoom, doors.components[i].room, i);
						registry.interactables.get(registry.doors.entities[i]).timer = registry.interactables.get(registry.doors.entities[i]).base;
					}
				}
			}
				 // user has to press e to move doors
																				  // registry.dialogueRequests.emplace(registry.doors.entities[i]); // prompt user if want to change door
			// registry.mapRequests.emplace(doors.entities[i],MapRequestType::ChangeRoom,doors.components[i].room,i);
		}
		else {
			registry.interactables.get(registry.doors.entities[i]).timer = registry.interactables.get(registry.doors.entities[i]).base;
		}
	}

	// Critters
	for (uint i = 0; i < critters.components.size(); i++)
	{
		auto &crit = critters.components[i];
		if (crit.startled)
			continue;
		auto &cm = registry.motions.get(critters.entities[i]);
		bool shouldStartle = CheapCircleToCircle(m.position, c.radius, cm.position, crit.radius);
		if (!shouldStartle)
		{
			for (uint j = 0; j < pBullets.components.size(); j++)
			{
				auto &bm = registry.motions.get(pBullets.entities[j]);
				auto &bc = registry.circleColliders.get(pBullets.entities[j]);
				shouldStartle = shouldStartle || CheapCircleToCircle(bm.position, bc.radius, cm.position, crit.radius);
				if (shouldStartle)
					break;
			}
		}
		if (shouldStartle)
		{
			crit.startled = true;
			cm.velocity = crit.flee;
		}
	}
}

bool PhysicsSystem::CircleToCircle(Entity circleA, Entity circleB)
{
	// if (!registry.circleColliders.has(circleA) || !registry.circleColliders.has(circleB)) return false;
	Motion &motionA = registry.motions.get(circleA);
	Motion &motionB = registry.motions.get(circleB);

	CircleCollider &cA = registry.circleColliders.get(circleA);
	CircleCollider &cB = registry.circleColliders.get(circleB);

	return CheapCircleToCircle(motionA.position, cA.radius, motionB.position, cB.radius);
}

// ref: https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-detection (mostly the explanation, not the code)
bool PhysicsSystem::AABBToCircle(Entity aabb, Entity circle)
{
	Motion& motion1 = registry.motions.get(aabb);
	Motion& motion2 = registry.motions.get(circle);
	AABBCollider& aabbBox = registry.aabbs.get(aabb);

	float dist = distance(motion1.position, motion2.position);
	float angle = atan2(motion2.position.y - motion1.position.y, motion2.position.x - motion1.position.x);
	float clamped_x = clamp(dist * cos(angle), aabbBox.topLeft.x, aabbBox.bottomRight.x);
	float clamped_y = clamp(dist * sin(angle), aabbBox.topLeft.y, aabbBox.bottomRight.y);
	vec2 closestToCircle = { clamped_x + motion1.position.x, clamped_y + motion1.position.y };
	return distance(closestToCircle, motion2.position) <= registry.circleColliders.get(circle).radius;

	//Motion& mA = registry.motions.get(aabb);
	//Motion& mB = registry.motions.get(circle);

	//AABBCollider& ab = registry.aabbs.get(aabb);
	//CircleCollider& c = registry.circleColliders.get(circle);

	//vec2 topLeft = mB.position - mA.position + ab.topLeft;
	//vec2 topRight = mB.position - mA.position + ab.topLeft * vec2(-1, 1);
	//vec2 bottomLeft = mB.position - mA.position + ab.bottomRight * vec2(-1, 1);
	//vec2 bottomRight = mB.position - mA.position + ab.bottomRight;

	//if (glm::dot(topLeft, topLeft) < c.radius * c.radius)
	//	return true;
	//if (glm::dot(topRight, topRight) < c.radius * c.radius)
	//	return true;
	//if (glm::dot(bottomLeft, bottomLeft) < c.radius * c.radius)
	//	return true;
	//if (glm::dot(bottomRight, bottomRight) < c.radius * c.radius)
	//	return true;

	//if (!PointInAABB(mB.position, bottomRight + vec2(0, c.radius), topLeft + vec2(0, -c.radius)))
	//	return true;
	//if (!PointInAABB(mB.position, bottomRight + vec2(c.radius, 0), topLeft + vec2(-c.radius, 0)))
	//	return true;

	//return false;
}

bool PhysicsSystem::CircleToWall(Entity circle, Entity wall)
{
	// if (!registry.circleColliders.has(circle) || !registry.walls.has(wall)) return false;
	Motion &m = registry.motions.get(circle);
	CircleCollider &c = registry.circleColliders.get(circle);

	WallCollider &w = registry.walls.get(wall);

	return CircleToLine(m.position, c.radius, w.startPosition, w.endPosition);
}

bool PhysicsSystem::PolyToWall(Entity poly, Entity wall)
{

	// if (!registry.circleColliders.has(circle) || !registry.walls.has(wall)) return false;
	Motion &m = registry.motions.get(poly);

	WallCollider &w = registry.walls.get(wall);
	// Assumes a small circle around the center of the polygon as the collision point for walls
	return CircleToLine(m.position, registry.polyColliders.get(poly).minLength, w.startPosition, w.endPosition);
}

// Returns true if the circle is intersecting a side of the polygon,
// false if the circle is wholly inside the polygon
// (No case should arise where that happens though)
bool PhysicsSystem::CircleToPoly(Entity circle, Entity poly)
{
	// if (!registry.circleColliders.has(circle) || !registry.polyColliders.has(poly)) return false;
	Motion &mA = registry.motions.get(circle);
	Motion &mB = registry.motions.get(poly);

	CircleCollider &c = registry.circleColliders.get(circle);
	PolyCollider &s = registry.polyColliders.get(poly);

	vec2 offset = {mA.position.x - mB.position.x, mA.position.y - mB.position.y};
	float ang = -(mB.angle);
	vec2 mArot = rotate(offset, ang);

	// Quick test to remove obviously not overlapping shapes
	if (!CheapCircleToCircle(mA.position, c.radius, mB.position, s.maxLength))
		return false;

	// Offset the circle position to be relative to the origin (like the polygon points)
	// Test the lines formed by every 2 adjacent polygon points against the circle
	if (CircleToLine(mArot, c.radius, s.offsetVertices[0], s.offsetVertices[s.offsetVertices.size() - 1]))
		return true;
	for (uint i = 1; i < s.offsetVertices.size(); i++)
	{
		if (CircleToLine(mArot, c.radius, s.offsetVertices[i], s.offsetVertices[i - 1]))
			return true;
	}
	return false;
}

bool PhysicsSystem::CircleToMesh(Entity circle, Entity mesh)
{
	// if (!registry.circleColliders.has(circle) || !registry.polyColliders.has(poly)) return false;
	Motion &mA = registry.motions.get(circle);
	Motion &mB = registry.motions.get(mesh);

	CircleCollider &c = registry.circleColliders.get(circle);
	Mesh m = *registry.meshPtrs.get(mesh);

	// Translate, scale, rotate circle position to match mesh conditions
	vec2 offset = {mA.position.x - mB.position.x, mA.position.y - mB.position.y};
	offset = vec2(offset.x / mB.scale.x, offset.y / mB.scale.y);
	offset = rotate(offset, -mB.angle);

	// Scale radius to match mesh scale
	float r = c.radius / max(mB.scale.x, mB.scale.y);

	// Quick test to remove obviously not overlapping shapes
	if (!CheapCircleToCircle(mA.position, c.radius, mB.position, max(mB.scale.x, mB.scale.y)))
		return false;

	for (uint i = 0; i < m.vertex_indices.size(); i += 3)
	{
		if (!CheapCircleToTriangle(offset, r,
								   m.vertices[m.vertex_indices[i + 0]].position,
								   m.vertices[m.vertex_indices[i + 1]].position,
								   m.vertices[m.vertex_indices[i + 2]].position))
			continue;
		if (PointInTriangle(offset,
							m.vertices[m.vertex_indices[i + 0]].position,
							m.vertices[m.vertex_indices[i + 1]].position,
							m.vertices[m.vertex_indices[i + 2]].position))
			return true;
		if (CircleToLine(offset, r, m.vertices[m.vertex_indices[i + 0]].position, m.vertices[m.vertex_indices[i + 1]].position))
			return true;
		if (CircleToLine(offset, r, m.vertices[m.vertex_indices[i + 1]].position, m.vertices[m.vertex_indices[i + 2]].position))
			return true;
		if (CircleToLine(offset, r, m.vertices[m.vertex_indices[i + 2]].position, m.vertices[m.vertex_indices[i + 0]].position))
			return true;
	}
	return false;
}

// Formula for distance from a point to a line defined by 2 points, taken from wikipedia
bool PhysicsSystem::CircleToLine(vec2 p1, float r, vec2 p2, vec2 p3)
{
	vec2 a = p1 - p2;
	vec2 b = p3 - p2;
	vec2 c = (glm::dot(a, glm::normalize(b)) * glm::normalize(b));

	// Circle is intersecting with p2 or p3
	if (CheapCircleToCircle(p1, 0, p2, r) || CheapCircleToCircle(p1, 0, p3, r))
		return true;

	// Circle center projected onto the line is between p2 and p3
	// Costly check, could use refining but idk how yet lol
	if (abs(glm::length(c) + glm::length(b - c) - glm::length(b)) < 0.01)
	{
		vec2 d = a - c;
		return (glm::dot(d, d) < r * r);
	}

	return false;
}

bool PhysicsSystem::AABBToAABB(Entity aabb1, Entity aabb2)
{
	Motion &mA = registry.motions.get(aabb1);
	Motion &mB = registry.motions.get(aabb2);

	AABBCollider &ab1 = registry.aabbs.get(aabb1);
	AABBCollider &ab2 = registry.aabbs.get(aabb2);

	vec2 max1 = mA.position + ab1.bottomRight;
	vec2 max2 = mB.position + ab2.bottomRight;
	vec2 min1 = mA.position + ab1.topLeft;
	vec2 min2 = mB.position + ab2.topLeft;

	return ((min1.y < max2.y) && (min2.y < max1.y) && (min1.x < max2.x) && (min2.x < max1.x));
}

bool PhysicsSystem::AABBToPoly(Entity aabb, Entity poly)
{
	// if (!registry.circleColliders.has(circle) || !registry.polyColliders.has(poly)) return false;
	Motion &mA = registry.motions.get(aabb);
	Motion &mB = registry.motions.get(poly);

	AABBCollider &ab = registry.aabbs.get(aabb);
	PolyCollider &s = registry.polyColliders.get(poly);

	vec2 offset = {mA.position.x - mB.position.x, mA.position.y - mB.position.y};

	// Quick test to remove obviously not overlapping shapes
	if (!CheapCircleToCircle(mA.position, max(mA.scale.x, mA.scale.y), mB.position, s.maxLength))
		return false;

	// Offset the circle position to be relative to the origin (like the polygon points)
	// Test the lines formed by every 2 adjacent polygon points against the circle
	// if (AABBToLine(offset + ab.bottomRight, offset + ab.topLeft, rotate(s.offsetVertices[0], mB.angle), rotate(s.offsetVertices[s.offsetVertices.size() - 1], mB.angle))) return true;
	// for (uint i = 1; i < s.offsetVertices.size(); i++) {
	//	if (AABBToLine(offset + ab.bottomRight, offset + ab.topLeft, rotate(s.offsetVertices[i], mB.angle), rotate(s.offsetVertices[i-1], mB.angle))) return true;
	//}

	// Cheap hacky alternative, just check if a poly vertex is in AABB OR center is in AABB
	if (!PointInAABB(vec2(0), offset + ab.bottomRight, offset + ab.topLeft))
		return true;
	for (uint i = 0; i < s.offsetVertices.size(); i++)
	{
		if (!PointInAABB(rotate(s.offsetVertices[i], mB.angle), offset + ab.bottomRight, offset + ab.topLeft))
			return true;
	}
	return false;
}

bool PhysicsSystem::AABBToMesh(Entity aabb, Entity mesh)
{
	// if (!registry.circleColliders.has(circle) || !registry.polyColliders.has(poly)) return false;
	Motion &mA = registry.motions.get(aabb);
	Motion &mB = registry.motions.get(mesh);

	AABBCollider &ab = registry.aabbs.get(aabb);
	Mesh m = *registry.meshPtrs.get(mesh);

	vec2 offset = {mA.position.x - mB.position.x, mA.position.y - mB.position.y};
	offset = vec2(offset.x / mB.scale.x, offset.y / mB.scale.y);
	vec2 br = vec2(ab.bottomRight.x / mB.scale.x, ab.bottomRight.y / mB.scale.y);
	vec2 tl = vec2(ab.topLeft.x / mB.scale.x, ab.topLeft.y / mB.scale.y);

	// Quick test to remove obviously not overlapping shapes
	if (!CheapCircleToCircle(mA.position, max(mA.scale.x, mA.scale.y), mB.position, max(mB.scale.x / 2, mB.scale.y / 2)))
		return false;

	// Offset the circle position to be relative to the origin (like the polygon points)
	// Test the lines formed by every 2 adjacent polygon points against the circle
	for (uint i = 0; i < m.vertex_indices.size(); i += 3)
	{
		if (!CheapCircleToTriangle(offset, max(mA.scale.x, mA.scale.y),
								   m.vertices[m.vertex_indices[i + 0]].position,
								   m.vertices[m.vertex_indices[i + 1]].position,
								   m.vertices[m.vertex_indices[i + 2]].position))
			continue;
		if (AABBToTriangle(offset + br, offset + tl,
						   rotate(m.vertices[m.vertex_indices[i + 0]].position, mB.angle),
						   rotate(m.vertices[m.vertex_indices[i + 1]].position, mB.angle),
						   rotate(m.vertices[m.vertex_indices[i + 2]].position, mB.angle)))
			return true;
	}
	return false;
}

bool PhysicsSystem::AABBToTriangle(vec2 maxxy, vec2 minxy, vec2 p1, vec2 p2, vec2 p3)
{

	if (AABBToLine(maxxy, minxy, p1, p2))
		return true;
	if (AABBToLine(maxxy, minxy, p1, p3))
		return true;
	if (AABBToLine(maxxy, minxy, p2, p3))
		return true;
	return false;
}

bool PhysicsSystem::AABBToLaser(Entity aabb, Entity laser)
{
	if (registry.deleteds.has(laser))
	{
		return false;
	}

	Motion &mA = registry.motions.get(aabb);
	Motion &mB = registry.motions.get(laser);

	AABBCollider &ab = registry.aabbs.get(aabb);
	Laser &l = registry.lasers.get(laser);
	Motion &ls = registry.motions.get(l.start);

	vec2 laserDir = vec2(cos(mB.angle), sin(mB.angle));
	vec2 leftOffset = vec2(-laserDir.y, laserDir.x) * mB.scale.y / 2.0f;
	vec2 rightOffset = -leftOffset;

	return AABBToLine(mA.position + ab.bottomRight, mA.position + ab.topLeft, ls.position + leftOffset, ls.position + leftOffset + laserDir * mB.scale.x) || AABBToLine(mA.position + ab.bottomRight, mA.position + ab.topLeft, ls.position + rightOffset, ls.position + rightOffset + laserDir * mB.scale.x);
}

// Uses https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
bool PhysicsSystem::AABBToLine(vec2 maxxy, vec2 minxy, vec2 p1, vec2 p2)
{
	int p1Code = PointInAABB(p1, maxxy, minxy);
	int p2Code = PointInAABB(p2, maxxy, minxy);
	float x = 0.0f, y = 0.0f;

	while (true)
	{
		if (!(p1Code) || !(p2Code))
		{
			// bitwise OR is 0: either points inside window; trivially accept and exit loop
			return true;
		}
		else if (p1Code & p2Code)
		{
			// bitwise AND is not 0: both points share an outside zone (LEFT, RIGHT, TOP,
			// or BOTTOM), so both must be outside window; exit loop (accept is false)
			return false;
		}
		else
		{
			// failed both tests, so calculate the line segment to clip
			// from an outside point to an intersection with clip edge

			// At least one endpoint is outside the clip rectangle; pick it.
			int outcodeOut = (p2Code > p1Code) ? p2Code : p1Code;

			// Now find the intersection point;
			// use formulas:
			//   slope = (y1 - y0) / (x1 - x0)
			//   x = x0 + (1 / slope) * (ym - y0), where ym is ymin or ymax
			//   y = y0 + slope * (xm - x0), where xm is xmin or xmax
			// No need to worry about divide-by-zero because, in each case, the
			// outcode bit being tested guarantees the denominator is non-zero
			if (outcodeOut & 0b1000)
			{ // point is above the clip window
				x = p1.x + (p2.x - p1.x) * (maxxy.y - p1.y) / (p2.y - p1.y);
				y = maxxy.y;
			}
			else if (outcodeOut & 0b0100)
			{ // point is below the clip window
				x = p1.x + (p2.x - p1.x) * (minxy.y - p1.y) / (p2.y - p1.y);
				y = minxy.y;
			}
			else if (outcodeOut & 0b0010)
			{ // point is to the right of clip window
				y = p1.y + (p2.y - p1.y) * (maxxy.x - p1.x) / (p2.x - p1.x);
				x = maxxy.x;
			}
			else if (outcodeOut & 0b0001)
			{ // point is to the left of clip window
				y = p1.y + (p2.y - p1.y) * (minxy.x - p1.x) / (p2.x - p1.x);
				x = minxy.x;
			}

			// Now we move outside point to intersection point to clip
			// and get ready for next pass.
			if (outcodeOut == p1Code)
			{
				p1 = vec2(x, y);
				p1Code = PointInAABB(p1, maxxy, minxy);
			}
			else
			{
				p2 = vec2(x, y);
				p2Code = PointInAABB(p2, maxxy, minxy);
			}
		}
	}

	return false;
}

int PhysicsSystem::PointInAABB(const glm::vec2 &p, const glm::vec2 &max, const glm::vec2 &min)
{
	int code = 0;
	if (p.x < min.x) // to the left aabb
		code |= 0b0001;
	else if (p.x > max.x) // to the right aabb
		code |= 0b0010;
	if (p.y < min.y) // below the aabb
		code |= 0b0100;
	else if (p.y > max.y) // above the aabb
		code |= 0b1000;
	return code;
}

bool PhysicsSystem::PointInTriangle(vec2 p, vec2 p1, vec2 p2, vec2 p3)
{
	auto area = [](const glm::vec2 &v1, const glm::vec2 &v2, const glm::vec2 &v3)
	{
		return 0.5 * abs((v2.x - v1.x) * (v3.y - v1.y) - (v3.x - v1.x) * (v2.y - v1.y));
	};

	if (abs(area(p, p1, p2) + area(p, p1, p3) + area(p, p2, p3) - area(p1, p2, p3)) < 0.0001)
		return true;
	return false;
}

bool PhysicsSystem::CheapCircleToCircle(vec2 p1, float r1, vec2 p2, float r2)
{
	float sum = (r1 + r2);
	vec2 offset = p1 - p2;
	return (glm::dot(offset, offset) < sum * sum);
}

bool PhysicsSystem::CheapCircleToTriangle(vec2 p, float r, vec2 a, vec2 b, vec2 c)
{
	float dotA = glm::dot(a, a);
	float dotB = glm::dot(b, b);
	float dotC = glm::dot(c, c);

	float d = 2 * (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
	float ux = (dotA * (b.y - c.y) + dotB * (c.y - a.y) + dotC * (a.y - b.y)) / d;
	float uy = (dotA * (c.x - b.x) + dotB * (a.x - c.x) + dotC * (b.x - a.x)) / d;
	vec2 u = vec2(ux, uy);

	float tr = glm::distance(u, a);

	// std::cout << glm::to_string(a) << ", " << glm::to_string(b) << ", " << glm::to_string(c) << std::endl;
	// std::cout << glm::to_string(u) << ", " << tr << std::endl;

	return CheapCircleToCircle(p, r, u, tr);
}

vec2 PhysicsSystem::rotate(vec2 v, float angle)
{
	return {v.x * cos(angle) - v.y * sin(angle), v.x * sin(angle) + v.y * cos(angle)};
}

float PhysicsSystem::distanceSquared(vec2 v1, vec2 v2)
{
	vec2 diff = v1 - v2;
	return glm::dot(diff, diff);
}

bool PhysicsSystem::LineToLine(vec2 line1Start, vec2 line1End, vec2 line2Start, vec2 line2End, vec2 &intersectionPoint)
{
	auto cross = [](const glm::vec2 &v1, const glm::vec2 &v2)
	{ return v1.x * v2.y - v1.y * v2.x; };
	glm::vec2 r = line1End - line1Start, s = line2End - line2Start, pq = line2Start - line1Start;
	float rxs = cross(r, s);
	if (rxs == 0)
		return false; // Lines are parallel
	float t = cross(pq, s) / rxs, u = cross(pq, r) / rxs;
	intersectionPoint = line1Start + t * r;
	return (t >= 0 && t <= 1 && u >= 0 && u <= 1);
}
