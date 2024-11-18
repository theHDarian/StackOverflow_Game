#include "world_init.hpp"
#include "tiny_ecs_registry.hpp"
#include <glm/trigonometric.hpp>
#include "premades.hpp"
#include "ai_system.hpp"
#include "utils/random.hpp"
#include <glm/gtx/string_cast.hpp>
#include "components/presets/particle_presets.hpp"
#include "utils/vector_operations.hpp"

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
	motion.velocity = {0.f, 0.f};
	motion.scale = mesh.original_size * 70.f;

	Player &player = registry.players.emplace(entity);
	CircleCollider &cc = registry.circleColliders.emplace(entity);
	cc.radius = motion.scale.x / 2.5;

	AABBCollider &aabb = registry.aabbs.emplace(entity);
	aabb.topLeft = vec2(-motion.scale.x / 4.25, -motion.scale.y / 5);
	aabb.bottomRight = vec2(motion.scale.x / 4.25, motion.scale.y / 3.25);

	PlayerAttackData &shoot = registry.shoots.emplace(entity);

	registry.stackCompile.emplace(entity);

	// add player sprite
	Sprites &playerSprites = registry.sprites.emplace(entity);
	playerSprites.sprites[SPRITE_STATE::BASE] = "mcv1_base.png";
	playerSprites.sprites[SPRITE_STATE::DAMAGED] = "mcv1_hit.png";
	playerSprites.sprites[SPRITE_STATE::MOVING] = "mc_walk";
	RenderRequest &rr = registry.renderRequests.insert(
		entity,
		{"mcv1_base.png",
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE});
	// can play around with offset to try to align sprite
	rr.offset = vec2(-5, -5);

	auto &animate = registry.animations.emplace(entity);
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

	Animation &anim = registry.animations.get(ent);
	anim = Animation();

	// remove animation sequence here for now
	if (registry.animationSequences.has(ent))
	{
		registry.animationSequences.remove(ent);
	}
	if (registry.spriteTimers.has(ent))
	{
		registry.spriteTimers.remove(ent);
	}
	if (registry.invincibles.has(ent))
	{
		registry.invincibles.remove(ent);
	}

	return ent;
}

Entity createAimIndicator(RenderSystem *renderer)
{
	// add aim indicator
	auto aimIndicator = Entity();
	Motion &aimMotion = registry.motions.emplace(aimIndicator);
	aimMotion.scale = {30, 30};
	Sprites &indicatorSprites = registry.sprites.emplace(aimIndicator);
	indicatorSprites.sprites[SPRITE_STATE::BASE] = "aim_indicator.png";
	registry.renderRequests.insert(
		aimIndicator,
		{"aim_indicator.png",
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE});
	registry.gameUIs.emplace(aimIndicator);
	return aimIndicator;
}

Entity createCritter(RenderSystem *renderer, vec2 pos)
{
	auto critter = Entity();
	Motion &m = registry.motions.emplace(critter);
	m.position = pos;
	m.velocity = vec2(0);
	m.scale = vec2(40, 40);

	Critter &c = registry.critters.emplace(critter);
	float angle = (rand() % 101) / 100.f;
	angle = ((1 - angle) * 3.f * M_PI / 4.f) + ((angle)*M_PI / 4.f) + M_PI;
	c.flee = (rand() % 40 + 80.f) * vec2(cos(angle), sin(angle));

	Animation &a = registry.animations.emplace(critter);
	a.max_frames = 2;
	a.animation_countdown_base = 10000000;
	a.animation_countdown = 10000000;

	registry.renderRequests.insert(
		critter,
		{"critter_butterfly_green",
		 EFFECT_ASSET_ID::ANIMATE,
		 GEOMETRY_BUFFER_ID::SPRITE});

	return critter;
}

Entity createCursor()
{
	auto cursor = Entity();
	Motion &cursorMotion = registry.motions.emplace(cursor);
	cursorMotion.scale = {64, 64};
	cursorMotion.position = {0, 0};
	Sprites &cursorSprites = registry.sprites.emplace(cursor);
	cursorSprites.sprites[SPRITE_STATE::BASE] = "cursor.png";
	registry.renderRequests.insert(
		cursor,
		{"CursorYB.png",
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE});
	registry.cursors.emplace(cursor);
	return cursor;
}

Entity createInteractable(RenderSystem *renderer, vec2 pos, InteractableItem item, std::vector<BulletStackEffect> effects)
{
	switch (item)
	{
	case InteractableItem::PopConsole:
		return createPopConsole(renderer, pos);
	case InteractableItem::Ram:
		return createRamStick(renderer, pos);
	case InteractableItem::Gardener:
		return createGardener(renderer, pos);
	case InteractableItem::BibleTree:
		return createBibleTree(renderer, pos);
	case InteractableItem::PushConsole:
		return createPushConsole(renderer, pos, effects);
	default:
		return Entity();
	}
}

Entity createPushConsole(RenderSystem *renderer, vec2 pos, std::vector<BulletStackEffect> effects)
{
	Entity console = Entity();
	Motion &m = registry.motions.emplace(console);
	m.position = pos;
	m.velocity = vec2(0);
	m.scale = 200.f * vec2(1, 1.4166666);

	auto &o = registry.objects.emplace(console);
	o.baseOffset = 20;

	CircleCollider &c = registry.circleColliders.get(registry.players.entities[0]);
	createWall(renderer, vec2(pos.x - 100 + c.radius * 2, pos.y + 20 - c.radius * 2), vec2(pos.x + 100 - c.radius * 2, pos.y + 20 - c.radius * 2));
	registry.backgrounds.emplace(console);

	CircleCollider &cc = registry.circleColliders.emplace(console);
	cc.radius = m.scale.y / 4;

	InteractableObject &object = registry.interactables.emplace(console);
	object.name = "PushStack";
	object.item = InteractableItem::PushConsole;
	// or maybe object type enum? This is not a unique id, just an object type identifier

	Animation &a = registry.animations.emplace(console);
	a.max_frames = 8;
	a.animation_countdown_base = 100;

	EffectStack &stack = registry.effectStacks.emplace(console);
	stack.stack = std::move(effects);

	registry.renderRequests.insert(
		console,
		{ "push_console",
		 EFFECT_ASSET_ID::ANIMATE,
		 GEOMETRY_BUFFER_ID::SPRITE});

	return console;
}

Entity createPopConsole(RenderSystem *renderer, vec2 pos)
{
	const Entity console = Entity();

	Motion &m = registry.motions.emplace(console);
	m.position = pos;
	m.velocity = vec2(0);
	m.scale = 200.f * vec2(1, 1.4166666);

	auto &o = registry.objects.emplace(console);
	o.baseOffset = 20;

	CircleCollider &c = registry.circleColliders.get(registry.players.entities[0]);
	createWall(renderer, vec2(pos.x - 100 + c.radius * 2, pos.y + 20 - c.radius * 2), vec2(pos.x + 100 - c.radius * 2, pos.y + 20 - c.radius * 2));
	registry.backgrounds.emplace(console);

	// can use aabb as near player range for now for pseudo-offsetting
	 AABBCollider& aabb = registry.aabbs.emplace(console);
	 aabb.topLeft = vec2(-m.scale.x / 8, -m.scale.y / 15);
	 aabb.bottomRight = vec2(m.scale.x / 8, m.scale.y / 3);

	//CircleCollider &cc = registry.circleColliders.emplace(console);
	//cc.radius = m.scale.y / 4;

	InteractableObject &object = registry.interactables.emplace(console);
	object.name = "PopStack";
	// or maybe object type enum? This is not a unique id, just an object type identifier

	Animation &a = registry.animations.emplace(console);
	a.max_frames = 8;
	a.animation_countdown_base = 100;

	registry.renderRequests.insert(
		console,
		{"pop_console",
		 EFFECT_ASSET_ID::ANIMATE,
		 GEOMETRY_BUFFER_ID::SPRITE});

	return console;
}

Entity createRamStick(RenderSystem *renderer, vec2 pos)
{
	auto entity = Entity();
	Motion &motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.scale = vec2(100, 50);
	motion.angle = 0;
	auto &object = registry.objects.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{"ram.png",
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE});
	auto &interact = registry.interactables.emplace(entity);
	interact.name = "Ram";
	interact.item = InteractableItem::Ram;
	registry.circleColliders.emplace(entity).radius = motion.scale.x / 2;

	return entity;
}

Entity createGardener(RenderSystem *renderer, vec2 pos)
{
	const Entity gardener = Entity();

	Motion &m = registry.motions.emplace(gardener);
	m.position = pos;
	m.velocity = vec2(0);
	m.scale = 300.f * vec2(1, 1);

	auto &o = registry.objects.emplace(gardener);
	o.baseOffset = 20;

	Animation &a = registry.animations.emplace(gardener);
	a.max_frames = 4;
	a.animation_countdown_base = 300;

	registry.renderRequests.insert(
		gardener,
		{"gardener",
		 EFFECT_ASSET_ID::ANIMATE,
		 GEOMETRY_BUFFER_ID::SPRITE});

	InteractableObject &object = registry.interactables.emplace(gardener);
	object.name = "Gardener";

	CircleCollider &cc = registry.circleColliders.emplace(gardener);
	cc.radius = m.scale.y / 4;

	return gardener;
}

Entity createBibleTree(RenderSystem *renderer, vec2 pos)
{
	const Entity tree = Entity();

	pos -= vec2(25, 100);

	Motion &m = registry.motions.emplace(tree);
	m.position = pos;
	m.velocity = vec2(0);
	m.scale = 400.f * vec2(1.2777777, 1);

	auto &o = registry.objects.emplace(tree);
	o.baseOffset = 190;

	CircleCollider &c = registry.circleColliders.get(registry.players.entities[0]);
	createWall(renderer, vec2(pos.x - 13 + c.radius * 2, pos.y + 190 - c.radius * 2), vec2(pos.x + 45 - c.radius * 2, pos.y + 190 - c.radius * 2));

	registry.backgrounds.emplace(tree);

	createCritter(renderer, pos + vec2(-200, -180));
	createCritter(renderer, pos + vec2(93, -185));
	createCritter(renderer, pos + vec2(20, -205));
	createCritter(renderer, pos + vec2(-116, -140));
	createCritter(renderer, pos + vec2(-45, -85));
	createCritter(renderer, pos + vec2(48, -163));
	createCritter(renderer, pos + vec2(-68, -20));
	createCritter(renderer, pos + vec2(68, 0));
	createCritter(renderer, pos + vec2(-128, -68));
	createCritter(renderer, pos + vec2(225, -116));
	createCritter(renderer, pos + vec2(156, -106));

	registry.renderRequests.insert(
		tree,
		{"bandedtree.png",
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE});

	InteractableObject &object = registry.interactables.emplace(tree);
	object.name = "BibleTree";

	CircleCollider &cc = registry.circleColliders.emplace(tree);
	cc.radius = 200; // hard code for now, can't seem to see if use scale??

	return tree;
}

Entity createWall(RenderSystem *renderer, vec2 startPosition, vec2 endPosition)
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

	WindowState &ws = registry.windowStates.components[0];

	auto &door = registry.doors.emplace(entity);
	door.startPos = startPos;
	door.endPos = endPos;
	door.side = (door.startPos.y == door.endPos.y) ? (door.startPos.y < ws.height / 2.f) ? 'T' : 'B' : (door.startPos.x < ws.width / 2.f) ? 'L'
																																		  : 'R';
	std::cout << glm::to_string(startPos) << ", " << glm::to_string(endPos) << ", " << door.side << std::endl;

	InteractableObject &object = registry.interactables.emplace(entity);
	object.name = "LockedDoor";

	registry.renderRequests.insert(
		entity,
		{"none",
		 EFFECT_ASSET_ID::EGG,
		 GEOMETRY_BUFFER_ID::DEBUG_LINE});

	// bad fix for interactable object system to figure out door index
	door.doorIndex = registry.doors.components.size() - 1;

	return entity;
}

Entity createDoorSymbol(RenderSystem *renderer, vec2 position, float angle, vec2 scale, float symbolAngle, vec3 axis, vec3 offset, vec2 spriteOffset)
{
	auto entity = Entity();
	Motion &motion = registry.motions.emplace(entity);
	motion.position = position + spriteOffset;
	motion.angle = angle;
	motion.scale = vec2(40.f, 80) * normalize(vec2(2 * scale.x / 120.f, 2 * scale.y / 66.f)) * 2.f;

	DoorSymbol &symbol = registry.doorSymbols.emplace(entity);
	symbol.angle = symbolAngle;
	symbol.axis = axis;
	symbol.offset = offset;

	registry.backgrounds.emplace(entity);

	RenderRequest &rr = registry.renderRequests.insert(entity,
													   {"door_symbols",
														EFFECT_ASSET_ID::ROOM_BOUND,
														GEOMETRY_BUFFER_ID::SPRITE});
	return entity;
}

void createRoomBounds(RenderSystem *renderer)
{
	WindowState &ws = registry.windowStates.components[0];

	Entity bounds[4];
	float spriteOffset = 30.f;

	struct WallPos
	{
		vec2 colliderStart;
		vec2 colliderEnd;
		vec2 spritePosition;
		vec2 spriteScale;
		float spriteAngle;
		vec3 offset;
		vec2 symbolOffset;
	};
	vec2 windowDimensions = {ws.width - 80, ws.height + 270};
	float ratio1 = ws.height / (float)ws.width;
	float ratio2 = ws.width / (float)ws.height;
	std::vector<WallPos> wallPositions = {
		{// top

		 vec2(0, ws.height / (6 * ratio2)), // manually offset by approx height of player sprite
		 vec2(ws.width, ws.height / (6 * ratio2)),
		 vec2(ws.width / 2.f, -ws.height / (6 * ratio1 * (1 + 0.5 * ratio1))),
		 vec2(ws.width - ws.width / 6, ws.height / (6 * ratio2)),
		 0,
		 vec3(0),
		 vec2(0, -spriteOffset)},
		{// right
		 vec2(ws.width - ws.width / (6 * ratio2), 0),
		 vec2(ws.width - ws.width / (6 * ratio2), ws.height),
		 vec2(ws.width / (6 * (1 + ratio1 * 0.5)), ws.height / 2),
		 vec2(ws.height + ws.height / (6 * ratio1), ws.width / (12 * ratio2)),
		 glm::radians(270.f),
		 vec3(0, 0, 50),
		 vec2(-spriteOffset, 0)},
		{													   // bottom
		 vec2(ws.width, ws.height - ws.height / (6 * ratio2)), // manually offset by mc feet
		 vec2(0, ws.height - ws.height / (6 * ratio2)),
		 vec2(ws.width / 2, ws.height + ws.height / (6 * ratio1 * (1 + 0.5 * ratio1))),
		 vec2(ws.width - ws.width / 6, ws.height / (6 * ratio2)),
		 glm::radians(180.f),
		 vec3(0),
		 vec2(0, spriteOffset)},
		// NOTE: left and righ wall require some weird z offset
		{// left
		 vec2(ws.width / (6 * ratio2), ws.height),
		 vec2(ws.width / (6 * ratio2), 0),
		 vec2(ws.width - ws.width / (6 * (1 + ratio1 * 0.5)), ws.height / 2),
		 vec2(ws.height + ws.height / (6 * ratio1), ws.width / (12 * ratio2)),
		 glm::radians(90.f),
		 vec3(0, 0, 50),
		 vec2(spriteOffset, 0)}};
	for (auto &p : wallPositions)
	{
		auto entity = Entity();

		auto &motion = registry.motions.emplace(entity);
		motion.position = p.spritePosition;
		motion.scale = p.spriteScale;
		motion.angle = p.spriteAngle;
		std::cout << motion.angle << std::endl;

		auto &wall = registry.walls.emplace(entity);
		wall.startPosition = p.colliderStart;
		wall.endPosition = p.colliderEnd;

		Bound &b = registry.bounds.emplace(entity);
		b.angle = glm::radians(-90.f);
		b.axis = vec3(1, 0, 0);
		b.offset = p.offset;
		b.side = (p.colliderStart.y == p.colliderEnd.y) ? (p.colliderStart.y < ws.height / 2.f) ? 'T' : 'B' : (p.colliderStart.x < ws.width / 2.f) ? 'L'
																																				   : 'R';

		auto &anim = registry.animations.emplace(entity);
		anim.animate = false;
		anim.max_frames = 3;

		RenderRequest &rr = registry.renderRequests.insert(
			entity,
			{(p.colliderStart.y == p.colliderEnd.y) ? "wall_horizontal" : "wall_vertical",
			 EFFECT_ASSET_ID::ROOM_BOUND,
			 GEOMETRY_BUFFER_ID::SPRITE});
		registry.backgrounds.emplace(entity);

		// add door symbol for each wall
		createDoorSymbol(renderer, motion.position, motion.angle, motion.scale, b.angle, b.axis, b.offset, p.symbolOffset);
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

Entity createTestFloor(RenderSystem *renderer, vec2 pos)
{
	auto entity = Entity();

	Mesh &mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	Motion &motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.scale = vec2({2880 / 1.75, 1584 / 1.75});

	registry.backgrounds.emplace(entity);

	registry.renderRequests.insert(
		entity,
		{"blankFloor.png",
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE});

	return entity;
};

float getRandomFloat(float min, float max)
{
	return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

Entity createEnemy(RenderSystem *renderer, vec2 pos, EnemyType type)
{
	auto entity = Entity();

	EnemyMovement &movement = registry.enemyMovement.emplace(entity);
	movement.posA = pos;

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
		movement.angularSpeed = 20;
		break;
	}
	case EnemyType::BossBeehiveGun:
	{
		enemy = BossBeehiveSentry();
		registry.bossParts.emplace(entity);
		break;
	}
	case EnemyType::MediumEnemyCharge:
	{
		enemy = EnemyMediumCharge();
		break;
	}
	case EnemyType::OneBee:
	{

		enemy = Bee1();
		registry.bees.emplace(entity);
		break;
	}
	case EnemyType::TwoBee:
	{
		enemy = Bee2();
		BeeEnemy &bee = registry.bees.emplace(entity);
		bee.mergeCount = 2;

		break;
	}
	case EnemyType::ThreeBee:
	{
		enemy = Bee3();
		BeeEnemy &bee = registry.bees.emplace(entity);
		bee.mergeCount = 3;
		break;
	}
	case EnemyType::MediumEnemyTank:
	{
		enemy = EnemyMediumTank();
		break;
	}
	case EnemyType::LaserEnemyTank:
	{
		enemy = EnemyLaserTank();
		break;
	}
	case EnemyType::HardEnemyTank:
	{
		enemy = EnemyHardTank();
		break;
	}
	case EnemyType::BeeHive:
	{
		enemy = EnemyMediumBeeHive();
		break;
	}
	case EnemyType::BossBeehiveMain:
	{
		enemy = BossBeeHive();
		registry.bosses.emplace(entity);
		break;
	}
	case EnemyType::EasyEnemySkull:
	{
		enemy = EnemyEasySkull();
		break;
	}
	case EnemyType::Snail:
	{
		enemy = EnemyEasyTrail();
		break;
	}
	case EnemyType::EvilSnail:
	{
		enemy = EnemyHardTrail();
		break;
	}
	case EnemyType::HardEnemyBoid:
	{
		enemy = EnemyHardBoid();
		Boid &boid = registry.boids.emplace(entity);
		boid.position = pos;
		float randomX = getRandomFloat(-150.f, 150.f);
		float randomY = getRandomFloat(-150.f, 150.f);
		boid.velocity = vec2(randomX, randomY);
		break;
	}
	case EnemyType::HardEnemyBoidBio:
	{
		enemy = EnemyHardBoidBio();
		Boid &boid = registry.boids.emplace(entity);
		boid.position = pos;
		float randomX = getRandomFloat(-150.f, 150.f);
		float randomY = getRandomFloat(-150.f, 150.f);
		boid.velocity = vec2(randomX, randomY);
		break;
	}
	case EnemyType::MediumEnemyBoar:
	{
		enemy = EnemyMediumBoar();
		break;
	}
	case EnemyType::MediumEnemyHealer:
	{
		enemy = EnemyMediumHeal();
		Healer &healer = registry.healers.emplace(entity);
		healer.coolDown = 0.f;
	}
	};

	Motion &motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.position = pos;
	motion.velocity = vec2(0, 0);
	motion.scale = enemy.scale;

	// if (enemy.behavior == EnemyBehavior::PATROLLING) {
	// 	movement.posA = enemy.patrolPath[0];
	// } else {
	// 	movement.posA = pos;
	// }
	// std::cout << "building enemy with type: " << enemy.currEnemyPattern().name << std::endl;
	movement.posB = AISystem::getMove(enemy.currEnemyPattern().type, entity);
	// std::cout<< movement.posA.x << movement.posA.y  << " " << movement.posB.x << movement.posB.y << std::endl;
	movement.speed = 100.0f * enemy.speedMultiplier;
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
			true,
			enemy.sprite.offset // manually set an offset for now
		});

	// need to also add an animate component
	if (enemy.sprite.effectId == EFFECT_ASSET_ID::ANIMATE)
	{
		auto &animate = registry.animations.emplace(entity);
		animate.animate = enemy.sprite.animationType;
		animate.max_frames = enemy.sprite.max_Frames; // this works only for bee for now, but texture arrays also seem to auto-mod, may not be needed?
		animate.animation_countdown = enemy.sprite.countdown;
		animate.animation_countdown_base = animate.animation_countdown;
	}

	if (type == EnemyType::BossBeehiveMain)
	{
		createEnemy(renderer, pos + vec2(97, -95), EnemyType::BossBeehiveGun);
		createEnemy(renderer, pos + vec2(-118, 72), EnemyType::BossBeehiveGun);
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
	if (atkData.onDeath != EnemyBulletDeath::NONE)
		bullet.onDeath = atkData.onDeath;

	Motion &motion = registry.motions.emplace(entity);
	motion.angle = atan2(velocity.y, velocity.x);
	motion.position = pos;
	motion.velocity = velocity * bullet.bulletSpeed;
	motion.scale = atkData.size; // Ensure scale is initialized
	motion.veer = veer;

	if (bullet.bulletEffects[0].type == BulletEffectType::Key)
	{
		motion.scale = 16.f * vec2(2.8, 1);
		motion.velocity = velocity * 400.f;
		motion.veer = vec2(0);
		bullet.bulletBounce = 10;
		bullet.bulletPierce = 0;
		bullet.bulletRange = 5000;

		PolyCollider &pc = registry.polyColliders.emplace(entity);
		pc.offsetVertices = {
			{motion.scale.x / 2, motion.scale.y / 2},
			{motion.scale.x / 2, -motion.scale.y / 2},
			{-motion.scale.x / 2, -motion.scale.y / 2},
			{-motion.scale.x / 2, motion.scale.y / 2}};
		pc.maxLength = glm::length(vec2(motion.scale.x / 2, motion.scale.y / 2));
		pc.minLength = min(motion.scale.x / 2, motion.scale.y / 2);

		registry.renderRequests.insert(
			entity,
			{"enemy_bullet_key.png",
			 EFFECT_ASSET_ID::TEXTURED,
			 GEOMETRY_BUFFER_ID::SPRITE});

		ParticleProps props = enemyBullet;
		props.colors.push_back(enemyBulletColors.at(Key));
		props.position.variation = VecOp::rotate(motion.scale, motion.angle);
		EmitParticle &ep = registry.emitParticles.emplace(entity, PBulletTrail, props, 10000, Random::Int(3) + 5);

		return entity;
	}

	if (atkData.homing > 0.0)
	{
		HomingBullet &homing = registry.homes.emplace(entity);
		homing.homingIntensity = atkData.homing;
		homing.target = registry.players.entities[0];
	}

	// Invisible &inv = registry.invisibles.emplace(entity);
	//// inv.countdown = (75.0f / (bullet.bulletSpeed)) * 1000.0f;
	// inv.countdown = 200.0f;

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
		// pc.setPolyLengths();
		auto &spriteComponent = registry.sprites.emplace(entity);
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
		// pc.setPolyLengths();
		auto &spriteComponent = registry.sprites.emplace(entity);
		spriteComponent.sprites[SPRITE_STATE::BASE] = "enemy_bullet_triangle.png";
		renderShape = "enemy_bullet_triangle.png";
	}
	else
	{
		CircleCollider &cc = registry.circleColliders.emplace(entity);
		cc.radius = motion.scale.x / 2;
		renderShape = "enemy_bullet_circle.png";
	}

	registry.renderRequests.insert(
		entity,
		{renderShape,
		 EFFECT_ASSET_ID::BULLET,
		 GEOMETRY_BUFFER_ID::SPRITE});

	// bullet trail
	ParticleProps props = enemyBullet;
	for (const BulletStackEffect &effect : bullet.bulletEffects)
	{
		BulletEffectType type = effect.type;
		if (type == BulletEffectType::Inert)
			continue;
		props.colors.push_back(enemyBulletColors.at(type));
	}
	if (!props.colors.empty())
	{
		props.position.variation = VecOp::rotate(motion.scale, motion.angle);
		EmitParticle &ep = registry.emitParticles.emplace(entity, PBulletTrail, props, 10000, Random::Int(3) + 5);
	}

	return entity;
}

Entity createEnemyBulletDeath(RenderSystem *renderer, vec2 pos, vec2 velocity, EnemyBulletDeath onDeath)
{
	auto entity = Entity();

	Mesh &mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	EnemyBullet &bullet = registry.enemyBullets.emplace(entity);
	bullet.bulletBounce = 0;
	bullet.bulletPierce = 0;
	bullet.bulletEffects = {blunt};
	bullet.shape = CIRCLE;

	Motion &motion = registry.motions.emplace(entity);
	motion.position = pos;

	if (onDeath == EnemyBulletDeath::EXPLODE)
	{
		bullet.bulletSpeed = 0;
		bullet.bulletRange = 200;
		motion.velocity = velocity * bullet.bulletSpeed;
		motion.scale = {240, 240}; // Ensure scale is initialized
	}
	else if (onDeath == EnemyBulletDeath::CLUSTER)
	{
		bullet.bulletSpeed = 200;
		bullet.bulletRange = 1000;
		motion.velocity = velocity * bullet.bulletSpeed;
		motion.scale = {30, 30}; // Ensure scale is initialized
		motion.veer = -motion.velocity * 0.8f;
	}

	auto &spriteComponent = registry.sprites.emplace(entity);

	CircleCollider &cc = registry.circleColliders.emplace(entity);
	cc.radius = motion.scale.x / 2;

	registry.renderRequests.insert(
		entity,
		{// spriteComponent.sprites[SPRITE_STATE::BASE],
		 "enemy_bullet_circle.png",
		 EFFECT_ASSET_ID::BULLET,
		 GEOMETRY_BUFFER_ID::SPRITE});

	// bullet trail (if we decide to add effects in the future)
	ParticleProps props = enemyBullet;
	for (const BulletStackEffect &effect : bullet.bulletEffects)
	{
		BulletEffectType type = effect.type;
		if (type == BulletEffectType::Inert)
			continue;
		props.colors.push_back(enemyBulletColors.at(type));
	}
	if (!props.colors.empty())
	{
		props.position.variation = VecOp::rotate(motion.scale, motion.angle);
		EmitParticle &ep = registry.emitParticles.emplace(entity, PBulletTrail, props, 10000, Random::Int(3) + 5);
	}

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

Entity createLightningBullet(RenderSystem *renderer, vec2 pos)
{
	auto entity = Entity();

	Mesh &mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	EnemyBullet &bullet = registry.enemyBullets.emplace(entity);
	bullet.bulletSpeed = 450;
	bullet.bulletRange = 5000;
	bullet.bulletBounce = 10;
	bullet.bulletPierce = 10000;
	bullet.bulletEffects = {lightning};
	bullet.shape = RECTANGLE;

	Motion &motion = registry.motions.emplace(entity);
	float angle = (rand() % 100 / 100.f) * 2 * M_PI;
	motion.angle = angle;
	motion.position = pos;
	motion.velocity = 450.f * vec2(cos(angle), sin(angle));
	motion.scale = 25.f * vec2(2, 1); // Ensure scale is initialized
	motion.veer = {0, 0};

	PolyCollider &pc = registry.polyColliders.emplace(entity);
	pc.offsetVertices = {
		{motion.scale.x / 2, motion.scale.y / 3},
		{motion.scale.x / 2, -motion.scale.y / 3},
		{-motion.scale.x / 2, -motion.scale.y / 3},
		{-motion.scale.x / 2, motion.scale.y / 3}};
	pc.maxLength = glm::length(vec2(motion.scale.x / 2, motion.scale.y / 2));
	pc.minLength = min(motion.scale.x / 2, motion.scale.y / 2);
	// pc.setPolyLengths();

	auto &animate = registry.animations.emplace(entity);
	animate.max_frames = 2;
	animate.animation_countdown_base = 50;

	registry.renderRequests.insert(
		entity,
		{"lightning_bullet",
		 EFFECT_ASSET_ID::ANIMATE,
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

	if (getModifiedValue(Homing, 0) > 0)
	{
		HomingBullet &home = registry.homes.emplace(entity);
		home.homingIntensity = getModifiedValue(Homing, 0);
	}

	// Invisible &inv = registry.invisibles.emplace(entity);
	// inv.countdown = (75.0f / bullet.bulletSpeed) * 1000.0f;

	// Initialize the motion
	auto &motion = registry.motions.emplace(entity);
	motion.angle = atan2(direction.y, direction.x);
	motion.velocity = direction * bullet.bulletSpeed;
	motion.position = position;
	motion.scale = vec2(bullet.bulletSize, bullet.bulletSize); // Ensure scale is initialized

	CircleCollider &cc = registry.circleColliders.emplace(entity);
	cc.radius = motion.scale.x / 2;

	registry.ignores.emplace(entity);

	auto &spriteComponent = registry.sprites.emplace(entity);
	spriteComponent.sprites[SPRITE_STATE::BASE] = "player_bullet.png";

	registry.renderRequests.insert(
		entity,
		{"player_bullet.png",
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE});

	// add bullet trail
	ParticleProps props = playerBulletTrail;
	props.position.variation = VecOp::rotate(motion.scale, motion.angle);
	EmitParticle &ep = registry.emitParticles.emplace(entity, PBulletTrail, props, 10000, Random::Int(2) + 3);
	return entity;
}

Entity createSkipDialogue()
{
	Entity entity = Entity();
	InteractableObject &object = registry.interactables.emplace(entity);
	object.name = "SkipTutorial";
	registry.menuUIs.emplace(entity);
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
	float prob = (1.0f / registry.enemies.components.size()); // reduce probability to spawn if there are more enemies
	Map &map = registry.maps.components[0];

	if (atkData.rareBulletEffects.size() > 0 && Random::Float() < prob && map.currRoom.preset.numSpecialBulletsToSpawn > 0)
	{
		registry.maps.components[0].currRoom.preset.numSpecialBulletsToSpawn--;
		if (Random::Float() < 0.15) return { key };
		return atkData.rareBulletEffects;
	}
	return {atkData.defaultEffect};
}
