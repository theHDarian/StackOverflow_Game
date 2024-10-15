#include "world_init.hpp"
#include "tiny_ecs_registry.hpp"
#include <glm/trigonometric.hpp>
#include "ai_system.hpp"

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
	motion.scale = mesh.original_size * 50.f;

	Player& player = registry.players.emplace(entity);
	player.baseSpeed = 400;
	CircleCollider& cc = registry.circleColliders.emplace(entity);
	cc.radius = motion.scale.x/2.5;

    PlayerAttackData& shoot = registry.shoots.emplace(entity);

	registry.stackCompile.emplace(entity);

	//add player sprite
	Sprites& playerSprites = registry.sprites.emplace(entity);
	playerSprites.sprites[SPRITE_STATE::BASE] = TEXTURE_ASSET_ID::MC_BASE;
	playerSprites.sprites[SPRITE_STATE::DAMAGED] = TEXTURE_ASSET_ID::MC_HIT;
	registry.renderRequests.insert(
		entity,
		{
			playerSprites.sprites[SPRITE_STATE::BASE],
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE
		});

	return entity;
}
Entity createAimIndicator(RenderSystem* renderer) {
	//add aim indicator
	auto aimIndicator = Entity();
	Motion& aimMotion = registry.motions.emplace(aimIndicator);
	aimMotion.scale = {30,30};
	Sprites& indicatorSprites =  registry.sprites.emplace(aimIndicator);
	indicatorSprites.sprites[SPRITE_STATE::BASE] = TEXTURE_ASSET_ID::AIM_INDICATOR;
	registry.renderRequests.insert(
		aimIndicator,
		{
			indicatorSprites.sprites[SPRITE_STATE::BASE],
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE
		}
	);
	return aimIndicator;
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
	poly.setPolyLengths();

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
	enemy.state = 10;

	registry.sprites.emplace(entity);
	registry.sprites.get(entity).sprites[SPRITE_STATE::BASE] = TEXTURE_ASSET_ID::PUFFERFISH;
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

Entity createTestFloor(RenderSystem* renderer, vec2 pos) {
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.scale = vec2({ 2880 /2, 1584 /2 });

	registry.backgrounds.emplace(entity);

	registry.renderRequests.insert(
		entity,
		{
			TEXTURE_ASSET_ID::FLOOR,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE
		});

	return entity;
};

Entity createEnemy(RenderSystem* renderer, vec2 pos, vec2 velocity, std::vector<AttackData> atkData, EnemyBehavior behavior) {
	auto entity = Entity();

	Mesh &mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	Motion &motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.position = pos;
	motion.velocity = velocity;
	motion.scale = vec2({ 288.0f/2, 240.0f/2 });

	Enemy& enemy = registry.enemies.emplace(entity);
	enemy.attackCooldown = 5000;
	enemy.maxHealth = 50;
	enemy.currHealth = enemy.maxHealth;
	enemy.state = 10;
	enemy.behavior = behavior;
	enemy.attackData = atkData;

	EnemyMovement& movement = registry.enemyMovement.emplace(entity);
	movement.posA = pos;
	movement.posB = AISystem::getMove(behavior);
	std::cout<< movement.posA.x << movement.posA.y  << " " << movement.posB.x << movement.posB.y << std::endl;
	movement.speed = 100.0f;
	movement.distanceTraveled = 0.0f;

	AttackData& atk = registry.attackDatas.emplace(entity);
	atk = atkData[0];
	for (int i = 0; i < atkData.size(); i++) {
		if ((atkData[i].attackType == EnemyAttackPattern::BURST || atkData[i].attackType == EnemyAttackPattern::SPRAY) && !registry.bursts.has(entity)) {
			registry.bursts.emplace(entity);
		}
	}

	CircleCollider& cc = registry.circleColliders.emplace(entity);
	cc.radius = abs(min(motion.scale.x, motion.scale.y))/2.5;

	registry.renderRequests.insert(
		entity,
		{
			TEXTURE_ASSET_ID::PUFFERFISH,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE
		});

	return entity;
};

Entity createEnemyBullet(RenderSystem* renderer, vec2 pos, vec2 velocity, vec2 veer, AttackData atkData) {
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	EnemyBullet& bullet = registry.enemyBullets.emplace(entity);
	bullet.bulletSpeed = atkData.speed;
	bullet.bulletRange = atkData.bulletRange;
	bullet.bulletBounce = atkData.bulletBounce;
	bullet.bulletEffects.push_back(atkData.defaultEffect);

	Motion& motion = registry.motions.emplace(entity);
	motion.angle = atan2(velocity.y, velocity.x);
	motion.position = pos;
	motion.velocity = velocity * bullet.bulletSpeed;
	motion.scale = atkData.size; // Ensure scale is initialized
	motion.veer = veer;

	if (atkData.homing > 0.0) {
		HomingBullet& homing = registry.homes.emplace(entity);
		homing.homingIntensity = atkData.homing;
		homing.target = registry.players.entities[0];
	}

	Invisible& inv = registry.invisibles.emplace(entity);
	inv.countdown = (75.0f / bullet.bulletSpeed) * 1000.0f;

	auto& spriteComponent = registry.sprites.emplace(entity);

	if (atkData.shape == RECTANGLE) {
		PolyCollider& pc = registry.polyColliders.emplace(entity);
		pc.offsetVertices = {
			{motion.scale.x / 2,motion.scale.y / 2},
			{motion.scale.x / 2,-motion.scale.y / 2},
			{-motion.scale.x / 2,motion.scale.y / 2},
			{-motion.scale.x / 2,-motion.scale.y / 2}
		};
		pc.maxLength = glm::length(vec2(motion.scale.x / 2, motion.scale.y / 2));
		pc.minLength = min(motion.scale.x / 2, motion.scale.y / 2);
		//pc.setPolyLengths();
		spriteComponent.sprites[SPRITE_STATE::BASE] = TEXTURE_ASSET_ID::ENEMY_BULLET_SQUARE;
	}
	else if (atkData.shape == TRIANGLE) {
		PolyCollider& pc = registry.polyColliders.emplace(entity);
		pc.offsetVertices = {
			{motion.scale.x / 2, 0},
			{-motion.scale.x / 2,-motion.scale.y / 2},
			{-motion.scale.x / 2, motion.scale.y / 2}
		};
		pc.maxLength = glm::length(vec2(-motion.scale.x / 2, -motion.scale.y / 2));
		pc.minLength = min(motion.scale.x / 2, motion.scale.y / 2);
		//pc.setPolyLengths();
		spriteComponent.sprites[SPRITE_STATE::BASE] = TEXTURE_ASSET_ID::ENEMY_BULLET_TRIANGLE;
	}
	else {
		CircleCollider& cc = registry.circleColliders.emplace(entity);
		cc.radius = motion.scale.x / 2;
		spriteComponent.sprites[SPRITE_STATE::BASE] = TEXTURE_ASSET_ID::ENEMY_BULLET_CIRCLE;
	}

	registry.renderRequests.insert(
		entity,
		{
			spriteComponent.sprites[SPRITE_STATE::BASE],
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

// draws dialogue box
// may end up setting globals for box position later?
Entity createDialogueBox(vec2 position, vec2 scale) {
	Entity entity = Entity();

	// copies code from draw line as a box for now
	// consider doing a check of "should I render now"? Or hide entity?
	auto& rr = registry.renderRequests.insert(
		entity, { TEXTURE_ASSET_ID::TEXTURE_COUNT,
				 EFFECT_ASSET_ID::EGG,
				 GEOMETRY_BUFFER_ID::DEBUG_LINE });
	rr.show = false;

	registry.uis.emplace(entity);

	Motion& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.position = position;
	motion.scale = scale;

	// temp colour
	auto& color = registry.colors.emplace(entity);
	color.r = 0.0;
	color.b = 1.0;
	color.g = 1.0;

	// attach 1 text render request
	auto& text = registry.textRenderRequests.emplace(entity);
	text.color = vec3(1, 1, 1);

	// want to place at top of dialogue box
	// with current text projection matrix being "flipped" coords
	// temp fix for getting window size for now
	WindowState& windowState = registry.windowStates.components[0];
	text.x = windowState.width - scale.x + 25; // 25 is just some padding
	text.y = windowState.height - position.y + scale.y / 4; // place text slightly above middle of box
	text.scale = 0.5; // for some reason, scale should be small
	text.text = "hello this is test dialogue!";

	// attach list of dialogue lines
	// probably shouldn't be attached to box, but to some dialogue state entity?
	auto& lines = registry.dialogueLines.emplace(entity);
	lines.lines.push_back("hello, this is a dialogue box.\npress e to go to next dialogue");
	lines.lines.push_back("when dialogue is happening, there shouldn't be any fighting going on\n as a temp fix for that, the game is paused while dialogue is happening");
	lines.lines.push_back("but also note the dialogue \"paused \" state is separate from the game paused state!\n(press esc to pause the game right now and see)");
	lines.lines.push_back("oh hey there's no more dialogue after this, so pressing e again won't open another dialogue box\ngoodbye");

	return entity;
}

// not a real menu right now; just to show the game is paused
Entity createPauseMenu(vec2 position, vec2 scale) {
	Entity entity = Entity();

	// copies code from draw line as a box for now
	auto& rr = registry.renderRequests.insert(
		entity, { TEXTURE_ASSET_ID::TEXTURE_COUNT,
				 EFFECT_ASSET_ID::EGG,
				 GEOMETRY_BUFFER_ID::DEBUG_LINE });
	rr.show = false;

	registry.uis.emplace(entity);

	Motion& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.position = position;
	motion.scale = scale;

	// temp colour
	auto& color = registry.colors.emplace(entity);
	color.r = 0.0;
	color.b = 0.0;
	color.g = 0.9;

	// attach 1 text render request
	auto& text = registry.textRenderRequests.emplace(entity);
	text.color = vec3(1, 1, 1);

	WindowState& windowState = registry.windowStates.components[0];
	// note: position is not center, but start of text rendering
	// need a mechanism to figure out text line size
	text.x = position.x / 2.5;
	text.y = position.y; 
	text.scale = 1.5; 
	text.text = "Game Paused";

	return entity;
}

// not a real menu right now; just to show the game is over
Entity createGameOverMenu(vec2 position, vec2 scale) {
	Entity entity = Entity();

	// copies code from draw line as a box for now
	auto& rr = registry.renderRequests.insert(
		entity, { TEXTURE_ASSET_ID::TEXTURE_COUNT,
				 EFFECT_ASSET_ID::EGG,
				 GEOMETRY_BUFFER_ID::DEBUG_LINE });
	rr.show = false;

	registry.uis.emplace(entity);

	Motion& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.position = position;
	motion.scale = scale;

	// temp colour
	auto& color = registry.colors.emplace(entity);
	color.r = 0.0;
	color.b = 0.0;
	color.g = 0.0;

	// attach 1 text render request
	auto& text = registry.textRenderRequests.emplace(entity);
	text.color = vec3(1, 1, 1);

	WindowState& windowState = registry.windowStates.components[0];
	text.x = windowState.width - scale.x + 25;
	text.y = windowState.height - position.y + scale.y / 4;
	text.scale = 1.5;
	text.text = "Game Over \npress R to restart";

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
	spriteComponent.sprites[SPRITE_STATE::BASE] = TEXTURE_ASSET_ID::MC_BULLET;

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


