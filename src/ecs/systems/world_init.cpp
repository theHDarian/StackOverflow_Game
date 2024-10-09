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
	motion.scale = mesh.original_size * 100.f;

	Player& player = registry.players.emplace(entity);
	player.baseSpeed = 200;
	CircleCollider& cc = registry.circleColliders.emplace(entity);
	cc.radius = motion.scale.x/2;

    Shoots& shoot = registry.shoots.emplace(entity);

	registry.stackCompile.emplace(entity);
	registry.sprites.emplace(entity);
	registry.sprites.get(entity).sprites[SPRITE_STATE::BASE] = TEXTURE_ASSET_ID::MC_BASE;
	registry.sprites.get(entity).sprites[SPRITE_STATE::DAMAGED] = TEXTURE_ASSET_ID::MC_HIT;
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

	registry.debugComponents.emplace(entity);
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

	auto enemy = registry.enemies.emplace(entity);
	enemy.attackCooldown = 5000;
	enemy.maxHealth = 1000;
	enemy.currHealth = enemy.maxHealth;
	enemy.speed = 100;
	enemy.state = 10;
	enemy.attackPattern = EnemyAttackPattern::SINGLE_SHOT;

	Shoots &shoot = registry.shoots.emplace(entity);
	shoot.maxBulletBurst = 3;
	shoot.maxFiringInterval = 3000.0f;
	shoot.bulletSpeed = 300;


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

Entity createEnemy(RenderSystem* renderer, vec2 pos, vec2 velocity, EnemyAttackPattern atkPattern) {
	auto entity = Entity();

	Mesh &mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);


	Motion &motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.position = pos;
	motion.velocity = velocity;
	motion.scale = vec2({ -EEL_BB_WIDTH, EEL_BB_HEIGHT });

	Enemy& enemy = registry.enemies.emplace(entity);
	enemy.attackCooldown = 5000;
	enemy.maxHealth = 100;
	enemy.currHealth = enemy.maxHealth;
	enemy.speed = 100;
	enemy.state = 10;
	enemy.attackPattern = atkPattern;

	Shoots &shoot = registry.shoots.emplace(entity);
	shoot.maxBulletBurst = 3;
	shoot.maxFiringInterval = 3000.0f;
	shoot.bulletSpeed = 200;

	CircleCollider& cc = registry.circleColliders.emplace(entity);
	cc.radius = abs(motion.scale.x)/2;

	registry.renderRequests.insert(
		entity,
		{
			TEXTURE_ASSET_ID::EEL,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE
		});

	Entity c = createCollisionCircle(renderer, pos, motion.angle, motion.velocity, cc.radius);
	auto& shapes = registry.collisionShapes.emplace(entity);
	shapes.shapes.push_back(c);

	return entity;
};

Entity createBulletEnemy(RenderSystem* renderer, vec2 pos, vec2 velocity, float angle) {
	auto entity = Entity();

	Mesh &mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	EnemyBullet& bullet = registry.enemyBullets.emplace(entity);
	bullet.bulletSpeed = 1.0f;
	bullet.bulletRange = 50000.f;
	bullet.bulletBounce = 3;

	Motion &motion = registry.motions.emplace(entity);
	motion.angle = angle;
	motion.position = pos;
	motion.velocity = velocity * bullet.bulletSpeed;


	motion.scale = bullet.bulletSize; // Ensure scale is initialized

    Invisible& inv = registry.invisibles.emplace(entity);
    float vel = sqrt(pow(motion.velocity.x, 2) + pow(motion.velocity.y, 2));
    inv.countdown = (75.0f/vel) * 1000.0f;

	CircleCollider& cc = registry.circleColliders.emplace(entity);
	cc.radius = motion.scale.x / 2;


	auto& spriteComponent = registry.sprites.emplace(entity);
	spriteComponent.sprites[SPRITE_STATE::BASE] = TEXTURE_ASSET_ID::FISH;

	registry.renderRequests.insert(
		entity,
		{
			spriteComponent.sprites[SPRITE_STATE::BASE],
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE
		});

	Entity c = createCollisionCircle(renderer, pos, motion.angle, motion.velocity, cc.radius);
	auto& shapes = registry.collisionShapes.emplace(entity);
	shapes.shapes.push_back(c);

	return entity;
}

Entity createEnemyBullet(RenderSystem* renderer, vec2 pos, vec2 velocity, float speed) {
	auto entity = Entity();

	Mesh &mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	EnemyBullet& bullet = registry.enemyBullets.emplace(entity);
	bullet.bulletSpeed = speed;
	bullet.bulletRange = 50000.f;
	bullet.bulletBounce = 3;

	Motion &motion = registry.motions.emplace(entity);
	motion.angle = atan2(velocity.y, velocity.x);
	motion.position = pos;
	motion.velocity = velocity * bullet.bulletSpeed;

    Invisible& inv = registry.invisibles.emplace(entity);
    inv.countdown = (75.0f/bullet.bulletSpeed) * 1000.0f;


	motion.scale = bullet.bulletSize; // Ensure scale is initialized

	CircleCollider& cc = registry.circleColliders.emplace(entity);
	cc.radius = motion.scale.x / 2;


	auto& spriteComponent = registry.sprites.emplace(entity);
	spriteComponent.sprites[SPRITE_STATE::BASE] = TEXTURE_ASSET_ID::FISH;

	registry.renderRequests.insert(
		entity,
		{
			spriteComponent.sprites[SPRITE_STATE::BASE],
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE
		});

	Entity c = createCollisionCircle(renderer, pos, motion.angle, motion.velocity, cc.radius);
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

Entity createPlayerBullet(RenderSystem* renderer, vec2 position, vec2 direction)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial values
	PlayerBullet& bullet = registry.playerBullets.emplace(entity);
	bullet.damage = getModifiedValue(BulletDamage, bullet.damage);
	bullet.bulletSpeed = getModifiedValue(ProjectileSpeed, bullet.bulletSpeed);
	bullet.bulletRange = getModifiedValue(BulletRange, bullet.bulletRange);
	bullet.bulletSize = getModifiedValue(ProjectileSize,  bullet.bulletSize);
	bullet.bulletPierce = getModifiedValue(Pierce, bullet.bulletPierce);
	bullet.bulletBounce = getModifiedValue(Bounce, bullet.bulletBounce);

    Invisible& inv = registry.invisibles.emplace(entity);
    inv.countdown = (75.0f/bullet.bulletSpeed) * 1000.0f;

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = atan2(direction.y, direction.x);
	motion.velocity = direction * bullet.bulletSpeed;
	motion.position = position;
	motion.scale = vec2(bullet.bulletSize, bullet.bulletSize); // Ensure scale is initialized

	CircleCollider& cc = registry.circleColliders.emplace(entity);
	cc.radius = motion.scale.x / 2;


	auto& spriteComponent = registry.sprites.emplace(entity);
	spriteComponent.sprites[SPRITE_STATE::BASE] = TEXTURE_ASSET_ID::FISH;

	registry.renderRequests.insert(
		entity,
		{
			spriteComponent.sprites[SPRITE_STATE::BASE],
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE
		});

	return entity;
}

float getModifiedValue(BulletEffectType bf, float value)
{
	Entity& player = registry.players.entities[0];
	return max(registry.stackCompile.get(player).minimums[bf], (value + registry.stackCompile.get(player).additives[bf]) * registry.stackCompile.get(player).multiplicatives[bf]);
}


