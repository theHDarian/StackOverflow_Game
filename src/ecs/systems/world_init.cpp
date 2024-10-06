#include "world_init.hpp"
#include "tiny_ecs_registry.hpp"

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
Entity createCollisionCircle(RenderSystem* renderer, vec2 position, float angle, vec2 velocity, float rad) {
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	//auto& entityMotion = registry.motions.get(e);
	auto& motion = registry.motions.emplace(entity);
	motion.angle = angle;
	motion.velocity = velocity;
	motion.position = position;
	motion.scale = vec2(rad * 2, rad * 2);

	registry.renderRequests.insert(
		entity,
		{
			TEXTURE_ASSET_ID::CIRCLE,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE
		});

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