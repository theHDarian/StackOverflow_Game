#include "world_init.hpp"
#include "tiny_ecs_registry.hpp"
#include <glm/trigonometric.hpp>
#include "bullet_effects.hpp"

Entity createPlayer(RenderSystem* renderer, vec2 pos)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = mesh.original_size * 300.f;
	motion.scale.y *= -1; // point front to the right


	// create an empty Salmon component for our character
	Player& player = registry.players.emplace(entity);
	player.baseSpeed = 200;

	CircleCollider& cc = registry.circleColliders.emplace(entity);
	cc.radius = motion.scale.x/2;

	registry.stackCompile.emplace(entity);
	registry.sprites.emplace(entity);
	registry.sprites.get(entity).sprites[SPRITE_STATE::BASE] = TEXTURE_ASSET_ID::EEL;
	registry.sprites.get(entity).sprites[SPRITE_STATE::DAMAGED] = TEXTURE_ASSET_ID::FISH;
	registry.renderRequests.insert(
		entity,
		{
			registry.sprites.get(entity).sprites[SPRITE_STATE::BASE],
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE
		});

	Entity c = createCollisionCircle(renderer, pos, motion.angle, motion.velocity, cc.radius);
	auto& shapes = registry.collisionShapes.emplace(entity);
	shapes.shapes.push_back(c);

	return entity;
}

// circle outline for circle collision
Entity createCollisionCircle(RenderSystem* renderer, vec2 position, float angle, vec2 velocity, float radius) {
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	auto& motion = registry.motions.emplace(entity);
	motion.angle = angle;
	motion.velocity = velocity;
	motion.position = position;
	motion.scale = vec2(radius * 2, radius * 2);

	registry.renderRequests.insert(
		entity,
		{
			TEXTURE_ASSET_ID::CIRCLE,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE
		});

	return entity;
}

// Purely for testing walls, puts 2 fish at either end of the line segment
Entity createTestWall(RenderSystem* renderer, vec2 startPosition, vec2 endPosition) {
	drawLineAtoB(renderer, startPosition, endPosition);

	auto entity = Entity();
	auto& wall = registry.walls.emplace(entity);
	wall.startPosition = startPosition;
	wall.endPosition = endPosition;

	return entity;
}

// draw a line from point A to B
// as a consequence of render system, each line is an entity for now
Entity drawLineAtoB(RenderSystem* renderer, vec2 a, vec2 b) {
	// borrowing code from createLine(), may change later
	auto entity = Entity();

	// calculate how the line will look
	float length = distance(a, b);
	float angle = atan2(b.y - a.y, b.x - a.x);
	vec2 position = vec2(cos(angle) * length * 0.5 + a.x, sin(angle) * length * 0.5 + a.y);

	Motion& motion = registry.motions.emplace(entity);
	motion.angle = angle;
	motion.velocity = { 0, 0 };
	motion.position = position;
	motion.scale = vec2(length, 5);

	registry.renderRequests.insert(
		entity, { TEXTURE_ASSET_ID::TEXTURE_COUNT,
				 EFFECT_ASSET_ID::EGG,
				 GEOMETRY_BUFFER_ID::DEBUG_LINE });

	//std::cout << "created line from point a(" << a.x << ", " << a.y << ") to b(" << b.x << ", " << b.y << ") at (" << position.x << ", " << position.y << ")" << " angle: " << angle << ", length: " << length << std::endl;

	return entity;
}

// Purely for testing polygons
Entity createTestPoly(RenderSystem* renderer, vec2 position, std::vector<vec2> points, float angle) {
	angle = glm::radians(angle);
	auto entity = Entity();

	auto& poly = registry.polyColliders.emplace(entity);
	poly.offsetVertices = points;
	poly.setMaxLength();

	// make this test poly a "bullet" so that 
	// circle-poly collision can be detected
	registry.enemyBullets.emplace(entity);
	auto& motion = registry.motions.emplace(entity);
	motion.position = position;
	motion.angle = angle;
	
	auto& shapes = registry.collisionShapes.emplace(entity);

	// draw lines of polygon by drawing a line from point i to point i + 1
	for (int i = 0; i < points.size(); i++) {
		vec2 thisRotatedPoint = { points[i].x * cos(angle) - points[i].y * sin(angle), points[i].x * sin(angle) + points[i].y * cos(angle) };
		if (i == points.size() - 1) {
			vec2 nextRotatedPoint = vec2(points[0].x * cos(angle) - points[0].y * sin(angle), points[0].x * sin(angle) + points[0].y * cos(angle));
			auto line = drawLineAtoB(renderer, thisRotatedPoint + position, nextRotatedPoint + position);
			shapes.shapes.push_back(line);
		}
		else {
			vec2 nextRotatedPoint = vec2(points[i + 1].x * cos(angle) - points[i + 1].y * sin(angle), points[i + 1].x * sin(angle) + points[i + 1].y * cos(angle));
			auto line = drawLineAtoB(renderer, thisRotatedPoint + position, nextRotatedPoint + position);
			shapes.shapes.push_back(line);
		}
	}

	return entity;
}

// basic enemy that doesn't do anything
Entity createBlob(RenderSystem* renderer, vec2 position) {
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0;
	motion.velocity = { 0, 0 };
	motion.position = position;

	// Setting initial values, scale is negative to make it face the opposite way
	motion.scale = vec2({ 100, 100 });

	CircleCollider& cc = registry.circleColliders.emplace(entity);
	cc.radius = motion.scale.x/2;

	registry.enemies.emplace(entity);
	registry.sprites.emplace(entity);
	registry.sprites.get(entity).sprites[SPRITE_STATE::BASE] = TEXTURE_ASSET_ID::EEL;
	registry.sprites.get(entity).sprites[SPRITE_STATE::DAMAGED] = TEXTURE_ASSET_ID::FISH;
	registry.renderRequests.insert(
		entity,
		{
			registry.sprites.get(entity).sprites[SPRITE_STATE::BASE],
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE
		});

	Entity c = createCollisionCircle(renderer, position, motion.angle, motion.velocity, cc.radius);
	auto& shapes = registry.collisionShapes.emplace(entity);
	shapes.shapes.push_back(c);

	return entity;
}

Entity createLine(vec2 position, vec2 scale)
{
	Entity entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	registry.renderRequests.insert(
		entity, {TEXTURE_ASSET_ID::TEXTURE_COUNT,
				 EFFECT_ASSET_ID::EGG,
				 GEOMETRY_BUFFER_ID::DEBUG_LINE});

	// Create motion
	Motion &motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = {0, 0};
	motion.position = position;
	motion.scale = scale;

	registry.debugComponents.emplace(entity);
	return entity;
}