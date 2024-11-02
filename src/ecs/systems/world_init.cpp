#include "world_init.hpp"
#include "tiny_ecs_registry.hpp"
#include <glm/trigonometric.hpp>
#include "premades.hpp"
#include "ai_system.hpp"

Entity createPlayer(RenderSystem *renderer, vec2 pos)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh &mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion &motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = mesh.original_size * 70.f;

	Player &player = registry.players.emplace(entity);
	CircleCollider &cc = registry.circleColliders.emplace(entity);
	cc.radius = motion.scale.x / 2.5;

	AABBCollider &aabb = registry.aabbs.emplace(entity);
	aabb.topLeft = vec2(-motion.scale.x / 4.25, -motion.scale.y / 5);
	aabb.bottomRight = vec2(motion.scale.x / 4.25, motion.scale.y / 3.25);

	PlayerAttackData &shoot = registry.shoots.emplace(entity);

	registry.stackCompile.emplace(entity);

	//add player sprite
	Sprites& playerSprites = registry.sprites.emplace(entity);
	playerSprites.sprites[SPRITE_STATE::BASE] = "mcv1_base.png";
	playerSprites.sprites[SPRITE_STATE::DAMAGED] = "mcv1_hit.png";
	playerSprites.sprites[SPRITE_STATE::MOVING] = "mc_walk";
	RenderRequest& rr = registry.renderRequests.insert(
		entity,
		{
			"mcv1_base.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE
		});
	// can play around with offset to try to align sprite
	rr.offset = vec2(-5, -5);

	auto& animate = registry.animations.emplace(entity);
	animate.max_frames = 6;

	return entity;
}

Entity resetPlayer()
{
	// Setting initial motion values
	assert(registry.players.entities.size() == 1);
	Entity ent = registry.players.entities[0];
	Motion &motion = registry.motions.get(ent);
	WindowState &ws = registry.windowStates.components[0];
	motion.position = {ws.width / 2, ws.height / 2};
	motion.angle = 0.f;
	motion.velocity = {0.f, 0.f};

	Player &player = registry.players.get(ent);
	player = Player();

	// reset stack and shoot
	PlayerAttackData &shoot = registry.shoots.get(ent);
	shoot = PlayerAttackData();

	StackCompile &sc = registry.stackCompile.get(ent);
	sc = StackCompile();

	Animation& anim = registry.animations.get(ent);
	anim = Animation();

	// remove animation sequence here for now
	if (registry.animationSequences.has(ent)) {
		registry.animationSequences.remove(ent);
	}
	if (registry.spriteTimers.has(ent)) { 
		registry.spriteTimers.remove(ent);
	}
	if (registry.invincibles.has(ent)) {
		registry.invincibles.remove(ent);
	}

	return ent;
}

Entity createAimIndicator(RenderSystem *renderer)
{
	// add aim indicator
	auto aimIndicator = Entity();
	Motion& aimMotion = registry.motions.emplace(aimIndicator);
	aimMotion.scale = {30,30};
	Sprites& indicatorSprites =  registry.sprites.emplace(aimIndicator);
	indicatorSprites.sprites[SPRITE_STATE::BASE] = "aim_indicator.png";
	registry.renderRequests.insert(
		aimIndicator,
		{"aim_indicator.png",
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE});
	registry.gameUIs.emplace(aimIndicator);
	return aimIndicator;
}

// Purely for testing walls, puts 2 fish at either end of the line segment
Entity createTestWall(RenderSystem *renderer, vec2 startPosition, vec2 endPosition)
{
	auto entity = Entity();

	auto &motion = registry.motions.emplace(entity);
	motion.position = (startPosition + endPosition) / 2.0f;
	motion.scale = vec2(glm::distance(startPosition, endPosition), 5);
	motion.angle = atan2(endPosition.y - startPosition.y, endPosition.x - startPosition.x);

	auto &wall = registry.walls.emplace(entity);
	wall.startPosition = startPosition;
	wall.endPosition = endPosition;

	registry.renderRequests.insert(
		entity,
		{"none",
		 EFFECT_ASSET_ID::EGG,
		 GEOMETRY_BUFFER_ID::DEBUG_LINE});

	return entity;
}
Entity createDoor(RenderSystem *renderer, vec2 startPos, vec2 endPos)
{
	auto entity = Entity();
	auto &motion = registry.motions.emplace(entity);
	motion.position = (startPos + endPos) / 2.0f;
	motion.scale = vec2(glm::distance(startPos, endPos), 5);
	motion.angle = atan2(endPos.y - startPos.y, endPos.x - startPos.x);

	auto &door = registry.doors.emplace(entity);
	door.startPos = startPos;
	door.endPos = endPos;

	registry.renderRequests.insert(
		entity,
		{"none",
		 EFFECT_ASSET_ID::EGG,
		 GEOMETRY_BUFFER_ID::DEBUG_LINE});
	vec3 c = registry.colors.emplace(entity);
	c.r = 0.0;
	c.g = 0.0;
	c.b = 1.0;

	return entity;
}

void createRoomBounds(RenderSystem *renderer)
{
	WindowState &wS = registry.windowStates.components[0];

	Entity bounds[4];
	bounds[0] = createTestWall(renderer, {0, 0}, {wS.width, 0});
	bounds[1] = createTestWall(renderer, {wS.width, 0}, {wS.width, wS.height});
	bounds[2] = createTestWall(renderer, {wS.width, wS.height}, {0, wS.height});
	bounds[3] = createTestWall(renderer, {0, wS.height}, {0, 0});

	for (Entity b : bounds)
	{
		registry.bounds.emplace(b);
	}
}

// draw a line from point A to B
// as a consequence of render system, each line is an entity for now
Entity drawLineAtoB(RenderSystem *renderer, vec2 a, vec2 b)
{
	// borrowing code from createLine(), may change later
	auto entity = Entity();

	// calculate how the line will look
	float length = distance(a, b);
	float angle = atan2(b.y - a.y, b.x - a.x);
	vec2 position = vec2(cos(angle) * length * 0.5 + a.x, sin(angle) * length * 0.5 + a.y);

	Motion &motion = registry.motions.emplace(entity);
	motion.angle = angle;
	motion.velocity = {0, 0};
	motion.position = position;
	motion.scale = vec2(length, 5);

	registry.renderRequests.insert(
		entity,
		{"none",
		 EFFECT_ASSET_ID::EGG,
		 GEOMETRY_BUFFER_ID::DEBUG_LINE});

	// std::cout << "created line from point a(" << a.x << ", " << a.y << ") to b(" << b.x << ", " << b.y << ") at (" << position.x << ", " << position.y << ")" << " angle: " << angle << ", length: " << length << std::endl;

	return entity;
}

// Purely for testing polygons
Entity createTestPoly(RenderSystem *renderer, vec2 position, std::vector<vec2> points, float angle)
{
	angle = glm::radians(angle);
	auto entity = Entity();

	auto &poly = registry.polyColliders.emplace(entity);
	poly.offsetVertices = points;
	poly.setPolyLengths();

	registry.debugComponents.emplace(entity);
	auto &motion = registry.motions.emplace(entity);
	motion.position = position;
	motion.angle = angle;

	auto &shapes = registry.collisionShapes.emplace(entity);

	// draw lines of polygon by drawing a line from point i to point i + 1
	for (int i = 0; i < points.size(); i++)
	{
		vec2 thisRotatedPoint = {points[i].x * cos(angle) - points[i].y * sin(angle), points[i].x * sin(angle) + points[i].y * cos(angle)};
		if (i == points.size() - 1)
		{
			vec2 nextRotatedPoint = vec2(points[0].x * cos(angle) - points[0].y * sin(angle), points[0].x * sin(angle) + points[0].y * cos(angle));
			auto line = drawLineAtoB(renderer, thisRotatedPoint + position, nextRotatedPoint + position);
			shapes.shapes.push_back(line);
		}
		else
		{
			vec2 nextRotatedPoint = vec2(points[i + 1].x * cos(angle) - points[i + 1].y * sin(angle), points[i + 1].x * sin(angle) + points[i + 1].y * cos(angle));
			auto line = drawLineAtoB(renderer, thisRotatedPoint + position, nextRotatedPoint + position);
			shapes.shapes.push_back(line);
		}
	}

	return entity;
}

// mesh enemy that doesn't do anything
Entity createBigC(RenderSystem *renderer, vec2 position)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh &mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::MESH_GB);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto &motion = registry.motions.emplace(entity);
	motion.angle = 0;
	motion.velocity = {0, 0};
	motion.position = position;

	// Setting initial values, scale is negative to make it face the opposite way
	motion.scale = vec2({700 * (1.923352 / 2.0f), 700});
	// motion.scale = vec2({ 500, 500 });

	registry.meshColliders.emplace(entity);

	auto &enemy = registry.enemies.emplace(entity);
	enemy.maxHealth = 1000;
	enemy.currHealth = enemy.maxHealth;

	EnemyMovement &movement = registry.enemyMovement.emplace(entity);
	movement.angularSpeed = 3;
	movement.posA = position;
	movement.posB = position;

	auto &boss = registry.bosses.emplace(entity);

	/*AttackData& atk = registry.attackDatas.emplace(entity);
	atk = none;*/

	registry.sprites.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{// registry.sprites.get(entity).sprites[SPRITE_STATE::BASE],
		 "none",
		 EFFECT_ASSET_ID::MESH,
		 GEOMETRY_BUFFER_ID::MESH_GB});

	return entity;
}

Entity createTestFloor(RenderSystem *renderer, vec2 pos)
{
	auto entity = Entity();

	Mesh &mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	Motion &motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.scale = vec2({2880 / 2, 1584 / 2});

	registry.backgrounds.emplace(entity);

	registry.renderRequests.insert(
		entity,
		{"blankFloor.png",
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE});

	return entity;
};


Entity createEnemy(RenderSystem *renderer, vec2 pos, EnemyType type)
{
	auto entity = Entity();

	// std::vector<AttackData> atkData = { threeBurst,twelveSpiralShot, threeHomingShot, twoPincerShot };
	Enemy &enemy = registry.enemies.emplace(entity);
	switch (type)
	{
		case EnemyType::TestRevampedEnemy:
		{
			enemy = TestEnemy();
			break;
		}
		case EnemyType::EasyEnemySentry:
		{
			enemy = EnemyEasySentry();
			break;
		}
		case EnemyType::BossBigC:
		{
			enemy = EnemyBigC();
			registry.bosses.emplace(entity);
			break;
		}
		case EnemyType::MediumEnemyCharge:
		{
			enemy = EnemyMediumCharge();
			break;
		}
	}

	Motion &motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.position = pos;
	motion.velocity = vec2(0, 0);
	motion.scale = enemy.scale;

	EnemyMovement &movement = registry.enemyMovement.emplace(entity);
	movement.posA = pos;
	// if (enemy.behavior == EnemyBehavior::PATROLLING) {
	// 	movement.posA = enemy.patrolPath[0];
	// } else {
	// 	movement.posA = pos;
	// }
	//std::cout << "building enemy with type: " << enemy.currEnemyPattern().name << std::endl;
	movement.posB = AISystem::getMove(enemy.currEnemyPattern().type, entity);
	// std::cout<< movement.posA.x << movement.posA.y  << " " << movement.posB.x << movement.posB.y << std::endl;
	movement.speed = 100.0f;
	movement.distanceTraveled = 0.0f;

	registry.bursts.emplace(entity);

	if (enemy.sprite.geometryId == GEOMETRY_BUFFER_ID::SPRITE)
	{
		CircleCollider &cc = registry.circleColliders.emplace(entity);
		cc.radius = abs(min(motion.scale.x, motion.scale.y)) / 2.5;
		Mesh &mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
		registry.meshPtrs.emplace(entity, &mesh);
	}
	else
	{
		// std::cout << "should have create bigC, the png is:" << sprite.texturePath << std::endl;
		registry.meshColliders.emplace(entity);
		Mesh &mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::MESH_GB);
		registry.meshPtrs.emplace(entity, &mesh);
	}

	registry.renderRequests.insert(
		entity,
		{
			enemy.sprite.texturePath,
			enemy.sprite.effectId,
			enemy.sprite.geometryId,
			// true,
			// vec2(-12, 0) // manually set an offset for now
		});

	// need to also add an animate component
	if (enemy.sprite.effectId == EFFECT_ASSET_ID::ANIMATE) {
		auto& animate = registry.animations.emplace(entity);
		animate.max_frames = 5; // this works only for bee for now, but texture arrays also seem to auto-mod, may not be needed?
		animate.animation_countdown = 20;
		animate.animation_countdown_base = animate.animation_countdown;
	}

	return entity;
};

Entity createEnemyBullet(RenderSystem *renderer, vec2 pos, vec2 velocity, vec2 veer, AttackData atkData)
{
	auto entity = Entity();

	Mesh &mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	EnemyBullet &bullet = registry.enemyBullets.emplace(entity);
	bullet.bulletSpeed = atkData.speed;
	bullet.bulletRange = atkData.bulletRange;
	bullet.bulletBounce = atkData.bulletBounce;
	bullet.bulletPierce = atkData.bulletPierce;
	bullet.bulletEffects = getBulletEffects(atkData);
	bullet.shape = atkData.shape;

	Motion &motion = registry.motions.emplace(entity);
	motion.angle = atan2(velocity.y, velocity.x);
	motion.position = pos;
	motion.velocity = velocity * bullet.bulletSpeed;
	motion.scale = atkData.size; // Ensure scale is initialized
	motion.veer = veer;

	if (atkData.homing > 0.0)
	{
		HomingBullet &homing = registry.homes.emplace(entity);
		homing.homingIntensity = atkData.homing;
		homing.target = registry.players.entities[0];
	}

	Invisible &inv = registry.invisibles.emplace(entity);
	// inv.countdown = (75.0f / (bullet.bulletSpeed)) * 1000.0f;
	inv.countdown = 200.0f;

	auto &spriteComponent = registry.sprites.emplace(entity);

	std::string renderShape;
	if (atkData.shape == RECTANGLE)
	{
		PolyCollider &pc = registry.polyColliders.emplace(entity);
		pc.offsetVertices = {
			{motion.scale.x / 2, motion.scale.y / 2},
			{motion.scale.x / 2, -motion.scale.y / 2},
			{-motion.scale.x / 2, -motion.scale.y / 2},
			{-motion.scale.x / 2, motion.scale.y / 2}};
		pc.maxLength = glm::length(vec2(motion.scale.x / 2, motion.scale.y / 2));
		pc.minLength = min(motion.scale.x / 2, motion.scale.y / 2);
		pc.setPolyLengths();
		spriteComponent.sprites[SPRITE_STATE::BASE] = "enemy_bullet_square.png";
		renderShape = "enemy_bullet_square.png";
	}
	else if (atkData.shape == TRIANGLE)
	{
		PolyCollider &pc = registry.polyColliders.emplace(entity);
		pc.offsetVertices = {
			{motion.scale.x / 2, 0},
			{-motion.scale.x / 2, -motion.scale.y / 2},
			{-motion.scale.x / 2, motion.scale.y / 2}};
		pc.maxLength = glm::length(vec2(-motion.scale.x / 2, -motion.scale.y / 2));
		pc.minLength = min(motion.scale.x / 2, motion.scale.y / 2);
		pc.setPolyLengths();
		spriteComponent.sprites[SPRITE_STATE::BASE] = "enemy_bullet_triangle.png";
		renderShape = "enemy_bullet_triangle.png";
	}
	else
	{
		CircleCollider &cc = registry.circleColliders.emplace(entity);
		cc.radius = motion.scale.x / 2;
		spriteComponent.sprites[SPRITE_STATE::BASE] = TEXTURE_ASSET_ID::ENEMY_BULLET_CIRCLE;
		renderShape = "enemy_bullet_circle.png";
	}

	registry.renderRequests.insert(
		entity,
		{// spriteComponent.sprites[SPRITE_STATE::BASE],
		 renderShape,
		 EFFECT_ASSET_ID::BULLET,
		 GEOMETRY_BUFFER_ID::SPRITE});

	return entity;
}

Entity createEnemyLaser(RenderSystem *renderer, vec2 pos, float angle, Entity start, AttackData atkData)
{
	auto entity = Entity();

	Mesh &mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	EnemyBullet &bullet = registry.enemyBullets.emplace(entity);
	bullet.bulletSpeed = 0;
	bullet.bulletRange = atkData.bulletRange;
	bullet.bulletBounce = 0;
	bullet.bulletPierce = 10000;
	bullet.bulletEffects = getBulletEffects(atkData);
	bullet.shape = RECTANGLE;

	Motion &motion = registry.motions.emplace(entity);
	motion.angle = angle;
	motion.position = pos;
	motion.velocity = {0, 0};
	motion.scale = {0, atkData.size.y}; // Ensure scale is initialized
	motion.veer = {0, 0};

	auto &spriteComponent = registry.sprites.emplace(entity);

	Laser &laser = registry.lasers.emplace(entity);
	laser.start = start;
	laser.length = 0;
	laser.growth = atkData.veer.x;
	laser.rotation = atkData.veer.y;

	registry.renderRequests.insert(
		entity,
		{"enemy_bullet_square.png",
		 EFFECT_ASSET_ID::BULLET,
		 GEOMETRY_BUFFER_ID::SPRITE});

	return entity;
}

Entity createLine(vec2 position, vec2 scale)
{
	Entity entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	registry.renderRequests.insert(
		entity,
		{"none",
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



Entity createPlayerBullet(RenderSystem *renderer, vec2 position, vec2 direction)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh &mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial values
	PlayerBullet &bullet = registry.playerBullets.emplace(entity);
	bullet.damage = getModifiedValue(BulletDamage, bullet.damage);
	bullet.bulletSpeed = getModifiedValue(ProjectileSpeed, bullet.bulletSpeed);
	bullet.bulletRange = getModifiedValue(BulletRange, bullet.bulletRange);
	bullet.bulletSize = getModifiedValue(ProjectileSize, bullet.bulletSize);
	bullet.bulletPierce = getModifiedValue(Pierce, bullet.bulletPierce);
	bullet.bulletBounce = getModifiedValue(Bounce, bullet.bulletBounce);

	Invisible &inv = registry.invisibles.emplace(entity);
	inv.countdown = (75.0f / bullet.bulletSpeed) * 1000.0f;

	// Initialize the motion
	auto &motion = registry.motions.emplace(entity);
	motion.angle = atan2(direction.y, direction.x);
	motion.velocity = direction * bullet.bulletSpeed;
	motion.position = position;
	motion.scale = vec2(bullet.bulletSize, bullet.bulletSize); // Ensure scale is initialized

	CircleCollider &cc = registry.circleColliders.emplace(entity);
	cc.radius = motion.scale.x / 2;


	auto& spriteComponent = registry.sprites.emplace(entity);
	spriteComponent.sprites[SPRITE_STATE::BASE] = "player_bullet.png";

	registry.renderRequests.insert(
		entity,
		{"player_bullet.png",
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE});

	return entity;
}

float getModifiedValue(BulletEffectType bf, float value)
{
	Entity &player = registry.players.entities[0];
	return max(registry.stackCompile.get(player).minimums[bf], (value + registry.stackCompile.get(player).additives[bf]) * registry.stackCompile.get(player).multiplicatives[bf]);
}

std::vector<BulletStackEffect> getBulletEffects(AttackData atkData)
{
	// TODO add logic from room data about whether a bullet should be default effect or special effects
	if (atkData.rareBulletEffects.size() > 0)
	{
		return atkData.rareBulletEffects;
	}
	return {atkData.defaultEffect};
}

