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

	return entity;
}

// Purely for testing walls, puts 2 fish at either end of the line segment
Entity createTestWall(RenderSystem* renderer, vec2 startPosition, vec2 endPosition) {
	createBlob(renderer, startPosition);
	createBlob(renderer, endPosition);

	auto entity = Entity();
	auto& wall = registry.walls.emplace(entity);
	wall.startPosition = startPosition;
	wall.endPosition = endPosition;

	return entity;
}

// Purely for testing polygons, puts fish at the vertices
Entity createTestPoly(RenderSystem* renderer, vec2 position, std::vector<vec2> points, float angle) {
	angle = glm::radians(angle);
	for (int i = 0; i < points.size(); i++) {
		vec2 mArot = { points[i].x * cos(angle) - points[i].y * sin(angle), points[i].x * sin(angle) + points[i].y * cos(angle) };
		createBlob(renderer, mArot + position);
	}
	auto entity = Entity();
	auto& poly = registry.polyColliders.emplace(entity);
	poly.offsetVertices = points;
	poly.setMaxLength();

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

	return entity;
}

Entity createEnemy(RenderSystem* renderer, vec2 pos, vec2 velocity, EnemyAttackPattern atkPattern) {
	auto entity = Entity();

	Mesh &mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	Motion &motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.position = pos;
	motion.velocity = velocity;
	motion.scale = vec2({ -EEL_BB_WIDTH, EEL_BB_HEIGHT });

	Enemy & enemy = registry.enemies.emplace(entity);
	enemy.attackCooldown = 5000;
	enemy.maxHealth = 1000;
	enemy.currHealth = enemy.maxHealth;
	enemy.speed = 100;
	enemy.state = 10;
	enemy.attackPattern = atkPattern;

	CircleCollider& cc = registry.circleColliders.emplace(entity);
	cc.radius = motion.scale.x;

	registry.renderRequests.insert(
		entity,
		{
			TEXTURE_ASSET_ID::EEL,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE
		});

	return entity;
};

Entity createBulletEnemy(RenderSystem* renderer, vec2 pos, vec2 velocity, float angle) {
	auto entity = Entity();

	Mesh &mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	EnemyBullet& bullet = registry.enemyBullets.emplace(entity);
	bullet.bulletSpeed = 1.f;
	bullet.bulletRange = 500.f;
	bullet.bulletBounce = 3;

	Motion &motion = registry.motions.emplace(entity);
	motion.angle = angle;
	motion.position = pos;
	motion.velocity = velocity * bullet.bulletSpeed;
	motion.scale = vec2({ -FISH_BB_WIDTH, FISH_BB_HEIGHT });

	CircleCollider& cc = registry.circleColliders.emplace(entity);
	cc.radius = abs(motion.scale.x);

	registry.renderRequests.insert(
		entity,
		{
			TEXTURE_ASSET_ID::FISH,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE
		});

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