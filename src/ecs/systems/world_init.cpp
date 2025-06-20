#include "world_init.hpp"
#include "tiny_ecs_registry.hpp"
#include <glm/trigonometric.hpp>
#include "premades.hpp"
#include "ai_system.hpp"
#include "utils/random.hpp"
#include <glm/glm.hpp>
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
	case InteractableItem::Ramlet:
		return createRamlet(renderer, pos);
	case InteractableItem::Gardener:
		return createGardener(renderer, pos);
	case InteractableItem::BibleTree:
		return createBibleTree(renderer, pos);
	case InteractableItem::PushConsole:
		return createPushConsole(renderer, pos, effects);
	case InteractableItem::HoneyCanister:
		return createHoneyCanister(renderer, pos);
	case InteractableItem::Baru:
		return createBaru(renderer, pos);
	case InteractableItem::WishGranter:
		return createWishGranter(renderer, pos);
	case InteractableItem::OracleCrab:
		return createOracleCrab(renderer, pos);
	case InteractableItem::FightConsole:
		return createFightConsole(renderer, pos, effects);
	case InteractableItem::WhiteBoard:
		return createWhiteBoard(renderer, pos);
	case InteractableItem::Desk:
		return createDesk(renderer, pos);
	case InteractableItem::Phone:
		return createPhone(renderer, pos);
	case InteractableItem::Swarm:
		return createSwarm(renderer, pos);
	case InteractableItem::Mouse:
		return createDeskWithMouse(renderer, pos);
	case InteractableItem::OracleTurret:
		return  createOracleTurret(renderer, pos);
	case InteractableItem::Oven:
		return CreateOven(renderer, pos);
	case InteractableItem::Optimizer:
		return createOptimizer(renderer, pos);
	case InteractableItem::Inverter:
		return createInverter(renderer, pos);
	default:
		assert(false && "Invalid InteractableItem");
		return Entity();
	}
}

Entity createPhone(RenderSystem *renderer, vec2 pos)
{
	Entity entity = Entity();

	Motion &m = registry.motions.emplace(entity);
	m.position = pos;
	m.velocity = vec2(0);
	m.scale = {384 / 4.f, 384 / 4.f};

	registry.backgrounds.emplace(entity);

	registry.renderRequests.insert(
		entity,
		{"callchip.png",
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE});

	InteractableObject &object = registry.interactables.emplace(entity);
	object.name = "Phone";
	object.item = InteractableItem::Phone;

	CircleCollider &cc = registry.circleColliders.emplace(entity);
	cc.radius = 50.f;

	return entity;
}

Entity createDesk(RenderSystem *renderer, vec2 pos)
{
	Entity entity = Entity();

	Motion &m = registry.motions.emplace(entity);
	m.position = pos;
	m.velocity = vec2(0);
	m.scale = {378 / 1.5f, 291 / 1.5f};

	auto &o = registry.objects.emplace(entity);
	o.baseOffset = m.scale.y / 3.f;

	createWall(renderer, vec2(pos.x - m.scale.x / 2.f, pos.y + m.scale.y / 2.f - m.scale.y / 4.f),
			   vec2(pos.x + m.scale.x / 2.f, pos.y + m.scale.y / 2.f - m.scale.y / 4.f));

	createWall(renderer, vec2(pos.x - m.scale.x / 2.f, pos.y + m.scale.y / 4.f),
			   vec2(pos.x - m.scale.x / 2.f, pos.y - m.scale.y / 2.f));

	// registry.backgrounds.emplace(entity);

	registry.renderRequests.insert(
		entity,
		{"desk.png",
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE});

	InteractableObject &object = registry.interactables.emplace(entity);
	object.name = "Desk";

	CircleCollider &cc = registry.circleColliders.emplace(entity);
	cc.radius = 150.f;

	createProp(renderer, pos + vec2(0, o.baseOffset * 1.2), "chair.png", vec2(189 / 2.5f, 295 / 2.5f), vec2(1));
	createJournal(renderer, pos + vec2(m.scale.x / 4, 0));

	return entity;
}

Entity createDeskWithMouse ( RenderSystem *renderer, vec2 pos )
{
	Entity entity = createProp3D( renderer, pos, "Mouse1.png", vec2(240 / 1.5f, 192/ 1.5f), vec2(130 / 1.5f, 0), 0, EFFECT_ASSET_ID::TEXTURED );

	// registry.backgrounds.emplace(entity);

	InteractableObject &object = registry.interactables.emplace(entity);
	object.name = "Mouse";
	object.item = InteractableItem::Mouse;

	CircleCollider &cc = registry.circleColliders.emplace(entity);
	cc.radius = 150.f;

	return entity;
}

Entity createOracleTurret ( RenderSystem *renderer, vec2 pos )
{
	Entity entity = createProp3D( renderer, pos, "Turret.png", vec2(192, 264), vec2(110, 35), 35, EFFECT_ASSET_ID::TEXTURED );

	// registry.backgrounds.emplace(entity);

	InteractableObject &object = registry.interactables.emplace(entity);
	object.name = "OracleTurret";
	object.item = InteractableItem::OracleTurret;

	CircleCollider &cc = registry.circleColliders.emplace(entity);
	cc.radius = 150.f;

	return entity;
}

Entity CreateOven ( RenderSystem *renderer, vec2 pos )
{
	Entity entity = createProp3D( renderer, pos, "oven.png", vec2(200, 200), vec2(100, 60), 60, EFFECT_ASSET_ID::TEXTURED );

	// registry.backgrounds.emplace(entity);

	InteractableObject &object = registry.interactables.emplace(entity);
	object.name = "Oven";
	object.item = InteractableItem::Oven;

	CircleCollider &cc = registry.circleColliders.emplace(entity);
	cc.radius = 150.f;

	return entity;
}


// if make this a prop, would disappear because too tiny and is object.
// consider changing type
Entity createJournal(RenderSystem *renderer, vec2 pos)
{
	Entity entity = Entity();

	Motion &m = registry.motions.emplace(entity);
	m.position = pos;
	m.velocity = vec2(0);
	m.scale = vec2(47 / 1.2f, 48 / 1.2f);

	auto &o = registry.objects.emplace(entity);
	o.baseOffset = 1000;

	registry.renderRequests.insert(
		entity,
		{"journal.png",
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE});

	return entity;
}

Entity createWhiteBoard(RenderSystem *renderer, vec2 pos)
{
	Entity entity = Entity();

	Motion &m = registry.motions.emplace(entity);
	m.position = pos;
	m.velocity = vec2(0);
	m.scale = {576 / 1.5f, 300 / 1.5f};

	auto &o = registry.objects.emplace(entity);
	o.baseOffset = m.scale.y / 3.f;

	createWall(renderer, vec2(pos.x - m.scale.x / 2.f, pos.y + m.scale.y / 2.f - m.scale.y / 4.f),
			   vec2(pos.x + m.scale.x / 2.f, pos.y + m.scale.y / 2.f - m.scale.y / 4.f));

	registry.backgrounds.emplace(entity);

	registry.renderRequests.insert(
		entity,
		{"controls.png",
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE});

	InteractableObject &object = registry.interactables.emplace(entity);
	object.name = "WhiteBoard";

	CircleCollider &cc = registry.circleColliders.emplace(entity);
	cc.radius = 150.f;

	return entity;
}

// add names of effects
void createEffectString (InteractableObject &object, std::vector<BulletStackEffect> effects) {
	vec3 color;
	size_t next = 0;
	std::string effectsString = "";

	// first merge effects
	effects = mergeEffects(effects);

	for (int i = 0; i < effects.size(); i++) {
		effectsString += getFormattedBulletEffectString(effects[i]);
		color = bulletEffectColors.at(effects[i].type);

		// Remove later when change key to 1 colour
		if (effects[i].type == Key) {
			color = COLOR_YELLOW;
		}
		
		if (i == effects.size() - 1) {
			object.decorations.at(0).push_back(TextDecorationSpan{ next, effectsString.length() - 1, color });
		}
		else {
			effectsString += ", ";
			object.decorations.at(0).push_back(TextDecorationSpan{ next, effectsString.length() - 2, color });
		}
		
		next = effectsString.length();
	}

	object.scriptVariables.push_back(effectsString);


	//std::vector<std::tuple<std::string, vec3>> effectStringList;
	//int effectCount = 1;
	//std::string effectsString = "";
	//for (int i = 1; i < effects.size(); i++) {
	//	if (effects[i - 1].type == Key || effects[i - 1].type == Inert || effects[i - 1 ].type == None || effects[i - 1].type == Lightning) {
	//		effectStringList.emplace_back(effects[i-1].name, effects[i-1].type == Key ? COLOR_YELLOW : bulletEffectColors.at (effects[i-1].type));
	//	} else {
	//		if ((effects[i-1].name == effects[i].name && effectCount < 3)) {
	//			effectCount++;
	//		} else {
	//			std::string direction = effects[i-1].name.find("Down") != std::string::npos ? " -" : " +";
	//			effectStringList.emplace_back(effects[i-1].name + direction +  std::to_string(effectCount), bulletEffectColors.at (effects[i-1].type));
	//			effectCount = 1;
	//		}
	//	}
	//}
	//for (int index = 0; index < effectStringList.size(); ++index) {
	//	// std::cout << std::get<0>(effectStringList[index]) << std::endl;
	//	vec3 color = std::get<1>(effectStringList[index]);
	//	if (index == effectStringList.size() - 1) {
	//		effectsString += std::get<0>(effectStringList[index]);
	//		object.decorations.at(0).push_back(TextDecorationSpan{ next, effectsString.length() - 1, color });
	//	} else {
	//		effectsString += std::get<0>(effectStringList[index]) + ", ";
	//		object.decorations.at(0).push_back(TextDecorationSpan{ next, effectsString.length() - 2, color });
	//	}
	//	next = effectsString.length();
	//}
	//object.scriptVariables.push_back(effectsString);
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
	object.decorations.push_back({});

	createEffectString(object, effects);

	Animation &a = registry.animations.emplace(console);
	a.max_frames = 8;
	a.animation_countdown_base = 100;

	EffectStack &stack = registry.effectStacks.emplace(console);
	stack.stack = std::move(effects);

	registry.renderRequests.insert(
		console,
		{"push_console",
		 EFFECT_ASSET_ID::ANIMATE,
		 GEOMETRY_BUFFER_ID::SPRITE});

	return console;
}



Entity createFightConsole(RenderSystem *renderer, vec2 pos, std::vector<BulletStackEffect> effects)
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
	//AABBCollider &aabb = registry.aabbs.emplace(console);
	//aabb.topLeft = vec2(-m.scale.x / 8, -m.scale.y / 15);
	//aabb.bottomRight = vec2(m.scale.x / 8, m.scale.y / 3);

	 CircleCollider &cc = registry.circleColliders.emplace(console);
	 cc.radius = m.scale.y / 4;

	 InteractableObject& object = registry.interactables.emplace(console);
	 object.name = "FightConsole";
	 object.item = InteractableItem::FightConsole;
	 object.decorations.push_back({});

	createEffectString(object, effects);


	Animation &a = registry.animations.emplace(console);
	a.max_frames = 1;
	a.animation_countdown_base = 100;

	EffectStack &stack = registry.effectStacks.emplace(console);
	stack.stack = std::move(effects);

	registry.renderRequests.insert(
		console,
		{"fight_console",
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
	//AABBCollider &aabb = registry.aabbs.emplace(console);
	//aabb.topLeft = vec2(-m.scale.x / 8, -m.scale.y / 15);
	//aabb.bottomRight = vec2(m.scale.x / 8, m.scale.y / 3);

	 CircleCollider &cc = registry.circleColliders.emplace(console);
	 cc.radius = m.scale.y / 4;

	InteractableObject &object = registry.interactables.emplace(console);
	object.name = "PopStack";
	object.item = PopConsole;
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

Entity createOptimizer(RenderSystem *renderer, vec2 pos)
{
	const Entity console = Entity();

	Motion &m = registry.motions.emplace(console);
	m.position = pos;
	m.velocity = vec2(0);
	m.scale = vec2(784,456) / 1.4f;

	auto &o = registry.objects.emplace(console);
	o.baseOffset = 20;

	CircleCollider &c = registry.circleColliders.get(registry.players.entities[0]);
	// createWall(renderer, vec2(pos.x - 100 + c.radius * 2, pos.y + 20 - c.radius * 2), vec2(pos.x + 100 - c.radius * 2, pos.y + 20 - c.radius * 2));
	registry.backgrounds.emplace(console);

	// can use aabb as near player range for now for pseudo-offsetting
	//AABBCollider &aabb = registry.aabbs.emplace(console);
	//aabb.topLeft = vec2(-m.scale.x / 8, -m.scale.y / 15);
	//aabb.bottomRight = vec2(m.scale.x / 8, m.scale.y / 3);

	CircleCollider &cc = registry.circleColliders.emplace(console);
	cc.radius = m.scale.y / 4;

	InteractableObject &object = registry.interactables.emplace(console);
	object.name = "Optimizer";
	object.item = Optimizer;
	// or maybe object type enum? This is not a unique id, just an object type identifier

	registry.renderRequests.insert(
		console,
		{"robot_surgeon.png",
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE});

	return console;
}

Entity createInverter(RenderSystem *renderer, vec2 pos)
{
	const Entity console = Entity();

	Motion &m = registry.motions.emplace(console);
	m.position = pos;
	m.velocity = vec2(0);
	m.scale = vec2(784,456) / 1.4f;

	auto &o = registry.objects.emplace(console);
	o.baseOffset = 20;

	CircleCollider &c = registry.circleColliders.get(registry.players.entities[0]);
	// createWall(renderer, vec2(pos.x - 100 + c.radius * 2, pos.y + 20 - c.radius * 2), vec2(pos.x + 100 - c.radius * 2, pos.y + 20 - c.radius * 2));
	registry.backgrounds.emplace(console);

	// can use aabb as near player range for now for pseudo-offsetting
	//AABBCollider &aabb = registry.aabbs.emplace(console);
	//aabb.topLeft = vec2(-m.scale.x / 8, -m.scale.y / 15);
	//aabb.bottomRight = vec2(m.scale.x / 8, m.scale.y / 3);

	CircleCollider &cc = registry.circleColliders.emplace(console);
	cc.radius = m.scale.y / 4;

	InteractableObject &object = registry.interactables.emplace(console);
	object.name = "Inverter";
	object.item = Inverter;
	// or maybe object type enum? This is not a unique id, just an object type identifier

	registry.renderRequests.insert(
		console,
		{"robot_surgeon.png",
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE});

	return console;
}


Entity createHoneyCanister(RenderSystem *renderer, vec2 pos)
{
	auto entity = createProp3D(renderer, pos, "HoneyCanisterFull.png", vec2(200, 300), vec2(40, 80), 110);
	InteractableObject &object = registry.interactables.emplace(entity);
	object.name = "HoneyCanister";
	object.item = InteractableItem::HoneyCanister;
	registry.circleColliders.emplace(entity).radius = 100;
	//AABBCollider &aabb = registry.aabbs.emplace(entity);
	//aabb.topLeft = vec2(-100, -150);
	//aabb.bottomRight = vec2(100, 150);

	return entity;
}

Entity createUnInterableSwarm(RenderSystem *renderer, vec2 pos)
{
	auto entity = Entity();
	Motion &motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.velocity = vec2(0);
	motion.scale = vec2(244, 240);
	motion.angle = 0;

	auto &object = registry.objects.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{"hivemind_boids",
		 EFFECT_ASSET_ID::ANIMATE,
		 GEOMETRY_BUFFER_ID::SPRITE});
	Animation &a = registry.animations.emplace(entity);
	a.max_frames = 50;
	a.animation_countdown_base = 100;
	auto &interact = registry.interactables.emplace(entity);
	return entity;
}

Entity createSwarm(RenderSystem *renderer, vec2 pos)
{
	auto entity = Entity();
	Motion &motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.velocity = vec2(0);
	motion.scale = vec2(244, 240);
	motion.angle = 0;

	auto &object = registry.objects.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{"hivemind_boids",
		 EFFECT_ASSET_ID::ANIMATE,
		 GEOMETRY_BUFFER_ID::SPRITE});

	for (int i = 0; i < 2; i++)
	{
		vec2 newPos = pos;
		if (i == 0)
		{
			newPos.x += 80;
			newPos.y += 120;
		}
		else
		{
			newPos.x -= 80;
			newPos.y += 120;
		}
		auto e = createUnInterableSwarm(renderer, newPos);
	}

	Animation &a = registry.animations.emplace(entity);
	a.max_frames = 50;
	a.animation_countdown_base = 100;
	auto &interact = registry.interactables.emplace(entity);
	interact.name = "Swarm";
	interact.item = InteractableItem::Swarm;
	registry.circleColliders.emplace(entity).radius = motion.scale.x;

	return entity;
}

Entity createBaru(RenderSystem *renderer, vec2 pos)
{
	auto entity = createProp3D(renderer, pos, "Brau1589.png", vec2(1380, 960), vec2(0, 0), 300);
	InteractableObject &object = registry.interactables.emplace(entity);
	WallCollider& wall = registry.walls.emplace(Entity());
	wall.startPosition = pos + vec2(-400, 300);
	wall.endPosition = pos + vec2(500, 300);
	WallCollider& wall2 = registry.walls.emplace(Entity());
	wall2.startPosition = pos + vec2(-400, -300);
	wall2.endPosition = pos + vec2(-400, 300);
	WallCollider& wall3 = registry.walls.emplace(Entity());
	wall3.startPosition = pos + vec2(500, 300);
	wall3.endPosition = pos + vec2(-400, -300);
	object.name = "Baru";
	object.item = InteractableItem::Baru;
	registry.circleColliders.emplace(entity).radius = 450;
	// AABBCollider &aabb = registry.aabbs.emplace(entity);
	// aabb.topLeft = vec2(-750, -375);
	// aabb.bottomRight = vec2(750, 375);

	return entity;
}

Entity createWishGranter(RenderSystem *renderer, vec2 pos)
{
	auto entity = createProp3D(renderer, pos, "wishGranter", vec2(132, 200), vec2(0, 0), 0, ANIMATE);
	InteractableObject &object = registry.interactables.emplace(entity);
	object.name = "WishGranter";
	object.item = InteractableItem::WishGranter;
	registry.circleColliders.emplace(entity).radius = 100;
	//AABBCollider &aabb = registry.aabbs.emplace(entity);
	//aabb.topLeft = vec2(-100, -150);
	//aabb.bottomRight = vec2(100, 150);
	Animation &a = registry.animations.emplace(entity);
	a.max_frames = 4;
	a.animation_countdown_base = 200;

	return entity;
}

Entity createOracleCrab(RenderSystem *renderer, vec2 pos)
{
	auto entity = createProp3D(renderer, pos, "BrokenCrab.png", vec2(200, 200), vec2(0, 0), 0);
	InteractableObject &object = registry.interactables.emplace(entity);
	object.name = "OracleCrab";
	object.item = InteractableItem::OracleCrab;
	registry.circleColliders.emplace(entity).radius = 100;
	//AABBCollider &aabb = registry.aabbs.emplace(entity);
	//aabb.topLeft = vec2(-100, -150);
	//aabb.bottomRight = vec2(100, 150);

	return entity;
}

Entity createRamStick(RenderSystem *renderer, vec2 pos)
{
	auto entity = Entity();
	Motion &motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.scale = vec2(100*1.2, 50*1.2);
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
	auto &effect = registry.emitParticles.emplace(entity, PBulletTrail, playerBulletTrail, 999999, 1);

	return entity;
}

Entity createRamlet(RenderSystem *renderer, vec2 pos)
{
	auto entity = Entity();
	Motion &motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.scale = vec2(100*0.6, 50*0.6);
	motion.angle = 0;
	auto &object = registry.objects.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{"ram.png",
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE});
	auto &interact = registry.interactables.emplace(entity);
	interact.name = "Ramlet";
	interact.item = InteractableItem::Ramlet;
	registry.circleColliders.emplace(entity).radius = motion.scale.x / 2;
	auto &effect = registry.emitParticles.emplace(entity, PBulletTrail, playerBulletTrail, 999999, 1);

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
	a.max_frames = 25;
	a.animation_countdown_base = 50;

	registry.renderRequests.insert(
		gardener,
		{"gardener",
		 EFFECT_ASSET_ID::ANIMATE,
		 GEOMETRY_BUFFER_ID::SPRITE});

	InteractableObject &object = registry.interactables.emplace(gardener);
	object.name = "Gardener";
	object.item = InteractableItem::Gardener;

	CircleCollider &cc = registry.circleColliders.emplace(gardener);
	cc.radius = m.scale.y / 4;

	createProp(renderer, pos + vec2(-400, 0), "lily_planter.png", vec2(264, 480), vec2(0.8, 0.9));
	createProp(renderer, pos + vec2(+400, 0), "carrot_planter.png", vec2(264, 480), vec2(0.8, 0.9));

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
	object.item = InteractableItem::BibleTree;

	CircleCollider &cc = registry.circleColliders.emplace(tree);
	cc.radius = 200; // hard code for now, can't seem to see if use scale??

	return tree;
}

// For creating top-down props with four walls (like planters)
Entity createProp(RenderSystem *renderer, vec2 pos, std::string filename, vec2 scale, vec2 shrink, bool solid)
{
	Entity e = Entity();

	Motion &m = registry.motions.emplace(e);
	m.position = pos;
	m.velocity = vec2(0);
	m.scale = scale;

	scale *= shrink;
	if (solid) {
		Entity ew = createWall(renderer, pos + vec2(+scale.x / 2.f, +scale.y / 2.f), pos + vec2(+scale.x / 2.f, -scale.y / 2.f));
		createWall(renderer, pos + vec2(-scale.x / 2.f, +scale.y / 2.f), pos + vec2(-scale.x / 2.f, -scale.y / 2.f));
		createWall(renderer, pos + vec2(+scale.x / 2.f, +scale.y / 2.f), pos + vec2(-scale.x / 2.f, +scale.y / 2.f));
		createWall(renderer, pos + vec2(+scale.x / 2.f, -scale.y / 2.f), pos + vec2(-scale.x / 2.f, -scale.y / 2.f));
		Parent& p = registry.parents.emplace(ew);
		p.children.push_back(e);
	}

	auto &object = registry.objects.emplace(e);
	object.baseOffset;

	registry.renderRequests.insert(
		e,
		{filename,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE});
	return e;
}

// For creating side-on props with a wall (like pop console or tree)
Entity createProp3D(RenderSystem *renderer, vec2 pos, std::string filename, vec2 scale, vec2 wallOffset, float baseOffset, EFFECT_ASSET_ID effect)
{
	Entity e = Entity();

	Motion &m = registry.motions.emplace(e);
	m.position = pos;
	m.velocity = vec2(0);
	m.scale = scale;

	Entity ew = createWall(renderer, pos + wallOffset * vec2(-1, 1), pos + wallOffset);
	Parent &p = registry.parents.emplace(ew);
	p.children.push_back(e);

	auto &object = registry.objects.emplace(e);
	object.baseOffset = baseOffset;

	registry.renderRequests.insert(
		e,
		{filename,
		 effect,
		 GEOMETRY_BUFFER_ID::SPRITE});
	return e;
}

Entity createFloorDeco(RenderSystem *renderer, vec2 pos, std::string filename)
{
	Entity e = Entity();

	Motion &m = registry.motions.emplace(e);
	m.position = pos;
	m.velocity = vec2(0);
	m.scale = vec2(192 * ((rand() % 2 == 1) ? -1 : 1), 192);

	registry.backgrounds.emplace(e);

	auto &anim = registry.animations.emplace(e);
	anim.animate = false;
	anim.max_frames = 16;
	anim.frame = rand() % 16;

	registry.renderRequests.insert(
		e,
		{filename,
		 EFFECT_ASSET_ID::ANIMATE,
		 GEOMETRY_BUFFER_ID::SPRITE});
	return e;
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
	door.side = (door.startPos.y == door.endPos.y) ? (door.startPos.y < ws.height / 2.f) ? 'B' : 'T' : (door.startPos.x < ws.width / 2.f) ? 'L'
																																		  : 'R';

	registry.roomSizeScaleds.emplace(entity, "Door");
	std::cout << glm::to_string(startPos) << ", " << glm::to_string(endPos) << ", " << door.side << std::endl;

	InteractableObject &object = registry.interactables.emplace(entity);
	object.name = "LockedDoor";
	object.base = 500;
	object.timer = 500;

	registry.renderRequests.insert(
		entity,
		{"none",
		 EFFECT_ASSET_ID::EGG,
		 GEOMETRY_BUFFER_ID::DEBUG_LINE});

	// bad fix for interactable object system to figure out door index
	door.doorIndex = registry.doors.components.size() - 1;

	return entity;
}

Entity createWallThickness(vec2 pos, vec2 scale)
{
	auto entity = Entity();

	Motion &motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.scale = scale + vec2(200, 200 / 1.33);

	registry.backgrounds.emplace(entity);
	registry.roomSizeScaleds.emplace(entity, "WallThickness");

	registry.renderRequests.insert(
		entity,
		{"wall_border.png",
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE});

	return entity;
}

Entity createDoorSymbol(RenderSystem *renderer, char side, float angle, vec2 scale, float symbolAngle, vec3 axis, vec3 offset, vec2 spriteOffset)
{
	auto entity = Entity();
	Motion &motion = registry.motions.emplace(entity);
	Map &map = registry.maps.components[0];
	WindowState &ws = registry.windowStates.components[0];
	vec2 position = vec2(ws.width, ws.height) / 2.f;
	float of = 200.f;
	if (side == 'T')
		position += vec2(0, map.currRoom.preset.roomSize.y / 2 + of);
	if (side == 'R')
		position += vec2(map.currRoom.preset.roomSize.x / 2 + of, 0);
	if (side == 'B')
		position += vec2(0, -map.currRoom.preset.roomSize.y / 2 - of);
	if (side == 'L')
		position += vec2(-map.currRoom.preset.roomSize.x / 2 - of, 0);
	motion.position = position;
	motion.angle = 0;
	motion.scale = vec2(120.f, 120);
	registry.roomSizeScaleds.emplace(entity, "DoorSymbol");

	DoorSymbol &symbol = registry.doorSymbols.emplace(entity);
	symbol.angle = 0;
	symbol.axis = axis;
	symbol.offset = vec3(0);
	symbol.side = side;

	registry.gameOverlayUIs.emplace(entity);

	RenderRequest &rr = registry.renderRequests.insert(entity,
													   {"door_symbols",
														EFFECT_ASSET_ID::ANIMATE,
														GEOMETRY_BUFFER_ID::SPRITE});

	Animation &anim = registry.animations.emplace(entity);
	anim.max_frames = 100;
	anim.animate = false;
	anim.animation_countdown = 1000;
	anim.animation_countdown_base = 1000;
	return entity;
}
Entity createDoors(RenderSystem *renderer, vec2 position, float angle, vec2 scale, float doorAngle, vec3 axis, vec3 offset, char side)
{
	auto entity = Entity();
	Motion &motion = registry.motions.emplace(entity);
	float offsetAmount = -40.f;
	vec2 offsetPos = vec2(0);
	vec2 scaleOffset;
	if (side == 'T')
		offsetPos.y = -offsetAmount;
	if (side == 'R')
		offsetPos.x = offsetAmount;
	if (side == 'B')
		offsetPos.y = offsetAmount;
	if (side == 'L')
		offsetPos.x = -offsetAmount;
	motion.position = position + offsetPos;
	motion.angle = angle;
	float angleOffset = 0.66;
	if (angle == (M_PI / 2) || angle == (M_PI / 2 + M_PI))
		angleOffset = 1.0f;
	// this is a factor of the wall's scaling, the wall's dimensions are 120x66 so if we want the door to be 4 tiles long it should be
	//  motion.scale = vec2(336,400.f) * vec2(1,tan(radians(125.f)/2))* normalize(vec2(2 * scale.x / 120.f, 2 * scale.y / 66.f));
	//  if (side == 'R' || side == 'L') motion.scale.x *= 6.5f/3.8f;
	if (side == 'L' || side == 'R')
	{
		motion.scale = vec2(336, 264) * normalize(vec2(3, 0.5555)) * 1.07f;
	}
	else
	{
		motion.scale = vec2(336, 264) * normalize(vec2(1920, 1080)) * 0.7f;
	}
	// motion.scale = vec2(336,264) * vec2(1,0.1);

	DoorSymbol &symbol = registry.doorSymbols.emplace(entity);
	symbol.angle = doorAngle;
	symbol.axis = axis;
	symbol.offset = offset;
	symbol.door = true;
	symbol.side = side;
	registry.roomSizeScaleds.emplace(entity, "DoorSprite");

	registry.backgrounds.emplace(entity);

	auto &anim = registry.animations.emplace(entity);
	anim.animate = false;
	anim.max_frames = 3;

	RenderRequest &rr = registry.renderRequests.insert(entity,
													   {"doors",
														EFFECT_ASSET_ID::ROOM_BOUND,
														GEOMETRY_BUFFER_ID::SPRITE});
	return entity;
}

void createRoomBounds(RenderSystem *renderer, vec2 roomCenter, vec2 roomSize)
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
	vec2 floorScale = roomSize;
	vec2 floorPosition = roomCenter;
	float wallThickness = 100.f;
	float doorwidth = 100.f;

	std::vector<WallPos> wallPositions = {
		{// top
		 vec2(floorPosition.x - floorScale.x * 1.1 / 2.f, floorPosition.y - floorScale.y / 2.f),
		 vec2(floorPosition.x + floorScale.x * 1.1 / 2.f, floorPosition.y - floorScale.y / 2.f),
		 vec2(floorPosition.x, floorPosition.y + floorScale.y),
		 vec2(floorScale.x * 1.1, wallThickness),
		 glm::radians(180.f),
		 vec3(0, 0, 50),
		 vec2(0, spriteOffset)},
		{// right
		 vec2(floorPosition.x + floorScale.x / 2.f - wallThickness * 1.1 / 2 - 25, floorPosition.y - floorScale.y * 2 / 2.f),
		 vec2(floorPosition.x + floorScale.x / 2.f - wallThickness * 1.1 / 2 - 25, floorPosition.y + floorScale.y * 2 / 2.f),
		 vec2(floorPosition.x + floorScale.x / 2 * 1.1, floorPosition.y), // not sure why 1.1, is magic number rn
		 vec2(floorScale.y * 2, wallThickness),
		 glm::radians(90.f),
		 vec3(0, 0, 50),
		 vec2(spriteOffset, 0)},
		{// bottom
		 vec2(floorPosition.x - floorScale.x * 1.1 / 2.f, floorPosition.y + floorScale.y / 2.f - wallThickness / 2.f),
		 vec2(floorPosition.x + floorScale.x * 1.1 / 2.f, floorPosition.y + floorScale.y / 2.f - wallThickness / 2.f),
		 vec2(floorPosition.x, floorPosition.y - floorScale.y),
		 vec2(floorScale.x * 1.1, wallThickness),
		 0,
		 vec3(0, 0, 50),
		 vec2(0, -spriteOffset)},
		{// left
		 vec2(floorPosition.x - floorScale.x / 2.f + wallThickness * 1.1 / 2 + 25, floorPosition.y - floorScale.y * 2 / 2.f),
		 vec2(floorPosition.x - floorScale.x / 2.f + wallThickness * 1.1 / 2 + 25, floorPosition.y + floorScale.y * 2 / 2.f),
		 vec2(floorPosition.x - floorScale.x / 2 * 1.1, floorPosition.y),
		 vec2(floorScale.y * 2, wallThickness),
		 glm::radians(270.f),
		 vec3(0, 0, 50),
		 vec2(-spriteOffset, 0)},

	};
	for (auto &p : wallPositions)
	{
		auto entity = Entity();

		registry.roomSizeScaleds.emplace(entity, "Bound");

		auto &motion = registry.motions.emplace(entity);
		motion.position = p.spritePosition;
		motion.scale = p.spriteScale;

		// std::cout << "Position: " << p.spritePosition.x << ", " << p.spritePosition.y << std::endl;
		// std::cout << "Scale: " << p.spriteScale.x << ", " << p.spriteScale.y << std::endl;

		// std::cout << "Start: " << p.colliderStart.x << ", " << p.colliderStart.y << std::endl;
		// std::cout << "End: " << p.colliderEnd.x << ", " << p.colliderEnd.y << std::endl;

		motion.angle = p.spriteAngle;
		// std::cout << motion.angle << std::endl;

		auto &wall = registry.walls.emplace(entity);
		wall.startPosition = p.colliderStart;
		wall.endPosition = p.colliderEnd;

		Bound &b = registry.bounds.emplace(entity);
		b.angle = glm::radians(-90.f);
		b.axis = vec3(1, 0, 0);
		b.offset = p.offset;
		b.side = (p.colliderStart.y == p.colliderEnd.y) ? ((p.colliderStart.y < ws.height / 2.f) ? 'B' : 'T') : (p.colliderStart.x < ws.width / 2.f) ? 'L'
																																					 : 'R';

		RenderRequest &rr = registry.renderRequests.insert(
			entity,
			{"walls",
			 EFFECT_ASSET_ID::ROOM_BOUND,
			 GEOMETRY_BUFFER_ID::SPRITE,
			 true,
			 vec2(0),
			 vec2(168, 384)});
		registry.backgrounds.emplace(entity);

		// add door symbol for each wall
		createDoorSymbol(renderer, b.side, motion.angle, motion.scale, b.angle, b.axis, b.offset, p.symbolOffset);
	}
	for (auto &p : wallPositions)
	{
		float angle = -M_PI / 2.f;
		vec3 axis = vec3(1, 0, 0);
		vec3 offset = p.offset;
		char side = (p.colliderStart.y == p.colliderEnd.y) ? ((p.colliderStart.y < ws.height / 2.f) ? 'B' : 'T') : (p.colliderStart.x < ws.width / 2.f) ? 'L'
																																						: 'R';

		createDoors(renderer, p.spritePosition, p.spriteAngle, p.spriteScale, angle, axis, offset, side);
	}

	createDoor(renderer, {ws.width / 2 - doorwidth / 2, wallPositions[0].colliderStart.y + 45}, {ws.width / 2 + doorwidth / 2, wallPositions[0].colliderStart.y + 45});
	createDoor(renderer, {wallPositions[1].colliderStart.x - 30, ws.height / 2 - doorwidth / 2}, {wallPositions[1].colliderStart.x - 30, ws.height / 2 + doorwidth / 2});
	createDoor(renderer, {ws.width / 2 - doorwidth / 2, wallPositions[2].colliderStart.y - 30}, {ws.width / 2 + doorwidth / 2, wallPositions[2].colliderStart.y - 30});
	createDoor(renderer, {wallPositions[3].colliderStart.x + 30, ws.height / 2 - doorwidth / 2}, {wallPositions[3].colliderStart.x + 30, ws.height / 2 + doorwidth / 2});

	// update room start/end pos based on colliders
	Room &room = registry.maps.components[0].currRoom;
	room.roomStart = vec2(wallPositions[3].colliderStart.x, wallPositions[0].colliderStart.y);
	room.roomEnd = vec2(wallPositions[1].colliderStart.x, wallPositions[2].colliderStart.y);
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

Entity createFloor(RenderSystem *renderer, vec2 pos, vec2 scale)
{
	auto entity = Entity();

	Mesh &mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	Motion &motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.scale = scale;

	registry.backgrounds.emplace(entity);
	registry.roomSizeScaleds.emplace(entity, "Floor");

	registry.renderRequests.insert(
		entity,
		{"Floor.png",
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE,
		 true,
		 vec2(0),
		 vec2(1440.f / 1.5f)});

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
	case EnemyType::EnemyTestLightningRotate:
	{
		enemy = TestLightningRotate();
		break;
	}
	case EnemyType::EnemyTestLightningShuffle:
	{
		enemy = TestLightningShuffle();
		break;
	}
	case EnemyType::EnemyTestEffectBlunt:
	{
		enemy = TestEffectBlunt();
		break;
	}
	case EnemyType::EnemyTestEffectPlusOne:
	{
		enemy = TestEffectPlusOne();
		break;
	}
	case EnemyType::EnemyTestEffectPlusTwo:
	{
		enemy = TestEffectPlusTwo();
		break;
	}
	case EnemyType::EnemyTestEffectPlusThree:
	{
		enemy = TestEffectPlusThree();
		break;
	}
	case EnemyType::EnemyTestEffectMinusOne:
	{
		enemy = TestEffectMinusOne();
		break;
	}
	case EnemyType::EnemyTestEffectMinusTwo:
	{
		enemy = TestEffectMinusTwo();
		break;
	}
	case EnemyType::EnemyTestEffectMinusThree:
	{
		enemy = TestEffectMinusThree();
		break;
	}
	case EnemyType::EnemyPufferfish:
	{
		enemy = Pufferfish();
		break;
	}
	case EnemyType::EnemyQuadshooter:
	{
		enemy = Quadshooter();
		break;
	}
		case EnemyType::EnemyQuadshooterElite: {
		enemy = EliteQuadshooter();
		break;
	}
		case EnemyType::TutorialEnemyQuadshooter:
	{
		enemy = TutorialQuadshooter();
		break;
	}
	case EnemyType::BossBigC:
	{
		enemy = BossBigCCore();
		auto &boss = registry.bosses.emplace(entity);
		boss.name = "\"Centurion\"";
		break;
	}
		case EnemyType::BossBigCShield:
	{
		enemy = BigC();
		auto& bp = registry.bossParts.emplace(entity);
		bp.showHpBar = false;
		break;
	}
		case EnemyType::EnemySmallCShield: {
		enemy = SmallC();
		auto& ep = registry.enemyParts.emplace(entity);
		ep.offset = {0,0};
		ep.alwaysFollow = true;
		registry.specialRotators.emplace(entity);
		break;
	}
	case EnemyType::BossBeehiveGun:
	{
		enemy = BeehiveGun();
		auto& bp = registry.bossParts.emplace(entity);
		bp.showHpBar = false;
		break;
	}
	case EnemyType::EnemyMagnet:
	{
		enemy = Magnet();
		break;
	}case EnemyType::EnemyMage:
	{
		enemy = Mage();
		// Healer& healer = registry.healers.emplace(entity);
		// healer.coolDown = 1000.f;
		// healer.healPower = 15.f;
		break;
	}
	case EnemyType::EnemyOneBee:
	{

		enemy = OneBee();
		registry.bees.emplace(entity);
		break;
	}
	case EnemyType::EnemyTwoBee:
	{
		enemy = TwoBee();
		BeeEnemy &bee = registry.bees.emplace(entity);
		bee.mergeCount = 2;

		break;
	}
	case EnemyType::EnemyThreeBee:
	{
		enemy = ThreeBee();
		BeeEnemy &bee = registry.bees.emplace(entity);
		bee.mergeCount = 3;
		break;
	}
	case EnemyType::EnemyCrab:
	{
		enemy = Crab();
		break;
	}
	case EnemyType::EnemyLaserCrab:
	{
		enemy = LaserCrab();
		break;
	}
	case EnemyType::EnemyEvilCrab:
	{
		enemy = EvilCrab();
		break;
	}
	case EnemyType::EnemyBeeHive:
	{
		enemy = BeeHive();
		break;
	}
	case EnemyType::BossBeehiveMain:
	{
		enemy = BossBeeHive();
		auto &boss = registry.bosses.emplace(entity);
		boss.name = "The Main Hive";
		break;
	}
	case EnemyType::EnemySkull:
	{
		enemy = Skull();
		break;
	}
	case EnemyType::EnemySmallMole:
	{
		enemy = SmallMole();
		break;
	}
	case EnemyType::EnemySurfaceMole: {
		enemy = SurfaceMole();
		break;
	}
	case EnemyType::EnemySmallBoulder:
	{
		enemy = SmallBoulder();
		float a = (float)(rand()) / (float)(RAND_MAX);
		movement.posB = vec2(cos(a), sin(a)) * 1000.f;
		break;
	}
	case EnemyType::EnemyBigBoulder:
	{
		enemy = BigBoulder();
		float a = (float)(rand()) / (float)(RAND_MAX);
		movement.posB = vec2(cos(a), sin(a)) * 1000.f;
		break;
	}
	case EnemyType::EnemyEvilSkull:
	{
		enemy = EvilSkull();
		break;
	}
	case EnemyType::EnemySnail:
	{
		enemy = Snail();
		break;
	}
	case EnemyType::EnemyEvilSnail:
	{
		enemy = EvilSnail();
		break;
	}
	case EnemyType::EnemyHifiBoid:
	{
		enemy = HifiBoid();
		Boid &boid = registry.boids.emplace(entity);
		boid.position = pos;
		float randomX = getRandomFloat(-150.f, 150.f);
		float randomY = getRandomFloat(-150.f, 150.f);
		boid.velocity = vec2(randomX, randomY);
		boid.maxSpeed = 500.f;
		break;
	}
	case EnemyType::EnemyBioBoid:
	{
		enemy = BioBoid();
		Boid &boid = registry.boids.emplace(entity);
		boid.position = pos;
		float randomX = getRandomFloat(-150.f, 150.f);
		float randomY = getRandomFloat(-150.f, 150.f);
		boid.velocity = vec2(randomX, randomY);
		boid.maxSpeed = 500.f;
		break;
	}
	case EnemyType::EnemySword:
	{
		enemy = Sword();
		break;
	}
	case EnemyType::EnemyHealer:
	{
		enemy = HealerAngel();
		Healer &healer = registry.healers.emplace(entity);
		healer.coolDown = 0.f;
	}
	case EnemyType::EnemyFishBoid:
	{
		enemy = FishBoid();
		Boid &boid = registry.boids.emplace(entity);
		boid.position = pos;
		float randomX = getRandomFloat(-35.f, 35.f);
		float randomY = getRandomFloat(-35.f, 35.f);
		boid.velocity = vec2(randomX, randomY);
		boid.maxSpeed = 400.f;
		break;
	}
	case EnemyType::EnemyTwinLaserVertical1:
	{
		enemy = TwinLaserVertical1();
		break;
	}
	case EnemyType::EnemyTwinLaserVertical2:
	{
		enemy = TwinLaserVertical2();
		break;
	}
	case EnemyType::EnemyHifiTwinLaserHorizontal1:
	{
		enemy = TwinLaserHorizontal1();
		break;
	}
	case EnemyType::EnemyHifiTwinLaserHorizontal2:
	{
		enemy = TwinLaserHorizontal2();
		break;
	}
	case EnemyType::EnemySniper:
	{
		enemy = HifiSniper();
		break;
	}
	case EnemyType::EnemyLaserSniper:
		enemy = HifiLaserSniper();
		// registry.specialRotators.emplace(entity);
		break;
	case EnemyType::EnemyLaserSniperHard:
		enemy = HifiLaserSniperHard();
		// registry.specialRotators.emplace(entity);
		break;
	case EnemyType::EnemyHifiCharger:
		enemy = HifiCharger();
		break;
	case EnemyType::EnemyHifiTrail:
		enemy = HifiTrail();
		break;
	case EnemyType::EnemyHifiCannon:
		enemy = HifiCannon();
		break;
	case EnemyType::EnemyHifiChargerHard:
		enemy = HifiChargerHard();
		break;
	case EnemyType::EnemyHifiSniperHard:
		enemy = HifiSniperHard();
		break;
	case EnemyType::EnemyHifiTrailHard:
		enemy = HifiTrailHard();
		break;
	case EnemyType::EnemyHifiCannonHard:
		enemy = HifiCannonHard();
		break;
	case EnemyType::EnemyHifiTemporaryBoid:
	{
		enemy = HifiTemporaryBoid();
		Boid &boid = registry.boids.emplace(entity);
		boid.position = pos;
		float randomX = getRandomFloat(-150.f, 150.f);
		float randomY = getRandomFloat(-150.f, 150.f);
		boid.velocity = vec2(randomX, randomY);
		boid.maxSpeed = 500.f;
		break;
	}
	case EnemyType::EnemyHifiJellyFish: enemy = HifiJellyFish(); break;
	case EnemyType::EnemyHifiTackShooter: enemy = HifiTackShooter(); break;
	case EnemyType::EnemyHifiBallLauncher: enemy = HifiBallLauncher(); break;
	case EnemyType::EnemyHifiWhip: enemy = HifiWhip(); break;
	case EnemyType::ScientistlaserAttack:
	{
		enemy = InvisibleRotateLaserEnemy();
		InvisibleEnemy& inv = registry.invisibleEnemy.emplace(entity);
		break;
	}
	case EnemyType::ScientistradialAttack:
	{
		enemy = InvisibleTurretEnemy();
		InvisibleEnemy& inv = registry.invisibleEnemy.emplace(entity);
		break;
	}
	case EnemyType::ScientistexplosiveAttack:
	{
		enemy = InvisibleExplosiveEnemy();
		InvisibleEnemy& inv = registry.invisibleEnemy.emplace(entity);
		break;
	}
	case EnemyType::ScientistLaserGridAttack:
	{
		enemy = InvisibleLaserEnemy();
		InvisibleEnemy& inv = registry.invisibleEnemy.emplace(entity);
		break;
	}
	case EnemyType::ScientistLaserGridVerticalAttack:
	{
		enemy = InvisibleLaserEnemyVertical();
		InvisibleEnemy& inv = registry.invisibleEnemy.emplace(entity);
		break;
	}
	case EnemyType::ScientistHomingAttack:
	{
		enemy = InvisibleHomingEnemy();
		InvisibleEnemy& inv = registry.invisibleEnemy.emplace(entity);
		break;
	};
	case EnemyType::ScientistBoss:
	{
		enemy = ScientistBossEnemy();
		registry.bosses.insert(entity, { "The Purple Cyborg" });
		Scientist& scien = registry.scientist.emplace(entity);
		Buffer& buffer = registry.buffers.emplace(entity);
		buffer.range = 1500.f;
		buffer.maxCoolDown = 1000.f;
		buffer.duration = 2500.f;

		break;
	}
	case EnemyType::ScientistShield:
	{
		enemy = ScientistSheildEnemy();
		registry.shield.emplace(entity);
		auto& ep = registry.enemyParts.emplace(entity);
		ep.offset = {0,0};
		ep.alwaysFollow = true;
		break;
	}
	case EnemyType::ScientistHand:
	{
		enemy = ScientistHandEnemy();
		registry.hand.emplace(entity);
		break;
	}
	case BossCrab:
    {
        enemy = BossChimeraCrab();
        auto &boss = registry.bosses.emplace(entity);
        boss.name = "Chimeric Crab";
        break;
    }
	case BossCrabLaser :
	{
		enemy = chimeraCrabSniper();
		InvisibleEnemy& inv = registry.invisibleEnemy.emplace(entity);
		BossParts& bp = registry.bossParts.emplace(entity);
		break;
	}
	case EnemyMedicalBoid :
    {
        enemy = MedBoid();
        Boid &boid = registry.boids.emplace(entity);
        boid.position = pos;
        float randomX = getRandomFloat(-150.f, 150.f);
        float randomY = getRandomFloat(-150.f, 150.f);
        boid.velocity = vec2(randomX, randomY);
        boid.maxSpeed = 500.f;
        break;
    }

	case EnemyMiningBoulderSmall:
		{
			enemy = SmallBoulder();
			break;
		}
	case EnemyMiningBoulderBig:
		{
			enemy = BigBoulder();
			break;
		}
	case EnemyPileDriverTurret:
	{
		enemy = PileDriverTurret();
		break;
	}
	case BossMole: {
		enemy = MoleBoss();
		auto &boss = registry.bosses.emplace(entity);
		boss.name = "Bomber Mole";
		break;
	}
	case BossDrillWormHead:
	{
		enemy = DrillWormHead();
		auto& boss = registry.bosses.emplace(entity);
		boss.name = "\"Autumn Willow\"";
		break;
	}
	case EnemySmallMiningWormHead:
	{
		enemy = SmallMiningWormHead();
		break;
	}
	case EnemyMiningBoidWormHead:
	{
		enemy = MiningBoidWormHead();
		Boid& boid = registry.boids.emplace(entity);
		boid.position = pos;
		float randomX = getRandomFloat(-150.f, 150.f);
		float randomY = getRandomFloat(-150.f, 150.f);
		boid.velocity = vec2(randomX, randomY);
		boid.maxSpeed = 500.f;
		break;
	}
	case EnemySpinePatrolWormHead:
	{
		enemy = SpinePatrolWormHead();
		break;
	}
	case EnemySpineFollowWormHead:
	{
		enemy = SpineFollowWormHead();
		break;
	}
	case EnemyDynamite:
	{
		enemy = Dynamite();
		break;
	}
	case EnemyBulldozer:
	{
		enemy = Bulldozer();
		break;
	}
	case EnemyChainDogHead:
	{
		enemy = ChainDogHead();
		createProp(renderer, pos, "Anchor.png", vec2(120 / 2), vec2(1), false);
		break;
	}
	case EnemySpineChainedWormHead:
	{
		enemy = SpineChainedWormHead();
		createProp(renderer, pos, "Anchor.png", vec2(120 / 2), vec2(1), false);
		break;
	}
	case EnemyMedicalBMP:
		{
			enemy = BMP();
			Buffer& buffer = registry.buffers.emplace(entity);
			buffer.range = 500.f;
			buffer.maxCoolDown = 1000.f;
			buffer.duration = 1000.f;
			break;
	}
	case EnemyScissors:
    {
        enemy = Scissors();
        break;
    }
	case EnemyProstheticHand:
	{
		enemy = ProstheticHand();
		break;
	}
	case EnemyMedicalRodA:
        {
            enemy = RodOfA();
			Buffer& buffer = registry.buffers.emplace(entity);
			buffer.range = 500.f;
			buffer.maxCoolDown = 2000.f;
			// Healer& healer = registry.healers.emplace(entity);
			// healer.healPower = 15.f;
            break;
        }
	case EnemyMedicalRodC:
        {
            enemy = RodOfC();
            Buffer& buffer = registry.buffers.emplace(entity);
            buffer.range = 2500.f;
            buffer.maxCoolDown = 5000.f;
			// Healer& healer = registry.healers.emplace(entity);
			// healer.healPower = 15.f;
            break;
        }
	case EnemyMedicalPillBoid:
		{
			enemy = PillBoid();
			Boid &boid = registry.boids.emplace(entity);
			boid.position = pos;
			float randomX = getRandomFloat(-150.f, 150.f);
			float randomY = getRandomFloat(-150.f, 150.f);
			boid.velocity = vec2(randomX, randomY);
			boid.maxSpeed = 500.f;
			Buffer& buffer = registry.buffers.emplace(entity);
			buffer.range = 500.f;
			buffer.maxCoolDown = 1000.f;
			break;
		}
	case EnemyMedicalPillBoidSpawner:
		{
			enemy = PillBoidSpawner();
			break;
		}

	case EnemyMedicalSyringe :
        {
            enemy = Syringe();
            Buffer& buffer = registry.buffers.emplace(entity);
            buffer.range = 500.f;
            buffer.maxCoolDown = 1000.f;
            break;
        }
	case BossMultiCube:
	{
		enemy = MultiCube();
		auto& boss = registry.bosses.emplace(entity);
		boss.name = "Prismatic Construct";
		break;
	}
	case BossConstructYELLOW:
	{
		enemy = ConstructYELLOW();
		Buffer& buffer = registry.buffers.emplace(entity);
		buffer.range = 750.f;
		buffer.maxCoolDown = 1200.f;
		buffer.duration = 2000.f;
		auto& bp = registry.bossParts.emplace(entity);
		bp.diesWithBoss = true;
		bp.showHpBar = true;
		break;
	}
	case BossConstructPURPLE:
	{
		enemy = ConstructPURPLE();
		Buffer& buffer = registry.buffers.emplace(entity);
		buffer.range = 10000.f;
		buffer.maxCoolDown = 1000000000000.f;
		buffer.duration = 15000.f;
		auto& bp = registry.bossParts.emplace(entity);
		bp.diesWithBoss = true;
		bp.showHpBar = true;
		break;
	}
	case BossConstructGREEN:
	{
		enemy = ConstructGREEN();
		Buffer& buffer = registry.buffers.emplace(entity);
		buffer.range = 10000.f;
		buffer.maxCoolDown = 1000000000000.f;
		buffer.duration = 15000.f;
		auto& bp = registry.bossParts.emplace(entity);
		bp.diesWithBoss = true;
		bp.showHpBar = true;
		break;
	}
	case BossConstructRED:
	{
		enemy = ConstructRED();
		Buffer& buffer = registry.buffers.emplace(entity);
		buffer.range = 10000.f;
		buffer.maxCoolDown = 1000000000000.f;
		buffer.duration = 15000.f;
		auto& bp = registry.bossParts.emplace(entity);
		bp.diesWithBoss = true;
		bp.showHpBar = true;
		break;
	}
	case EnemyEyeCube:
		{
			enemy = EyeCube();
			break;
		}
	case EnemyPhantom: {
		enemy = Phantom();
		Buffer& buffer = registry.buffers.emplace(entity);
		buffer.range = 2000.f;
		buffer.maxCoolDown = 500.f;
		buffer.duration = 5000.f;
		break;
	}

		case InvincibleGranter: {
		enemy = InvincibleBuffGranter();
		Buffer& buffer = registry.buffers.emplace(entity);
		buffer.range = 10.f;
		buffer.duration = 3000.f;
		buffer.maxCoolDown = 6000.f;
		EnemyPart& ep = registry.enemyParts.emplace(entity);
		ep.offset = {0,0};
		ep.alwaysFollow = true;
		registry.invisibleEnemy.emplace(entity);
		break;
	}
		case InvisibleGranter : {
		enemy = InvisibleBuffGranter();
		Buffer& buffer = registry.buffers.emplace(entity);
		buffer.range = 10.f;
		buffer.duration = 4000.f;
		buffer.maxCoolDown = 5000.f;
		EnemyPart& ep = registry.enemyParts.emplace(entity);
		ep.offset = {0,0};
		ep.alwaysFollow = true;
		registry.invisibleEnemy.emplace(entity);
		break;
	}
		case CloakedGranter : {
		enemy = CloakedBuffGranter();
		Buffer& buffer = registry.buffers.emplace(entity);
		buffer.range = 10.f;
		buffer.duration = 5000.f;
		buffer.maxCoolDown = 5000.f;
		EnemyPart& ep = registry.enemyParts.emplace(entity);
		ep.offset = {0,0};
		ep.alwaysFollow = true;
		registry.invisibleEnemy.emplace(entity);
		break;
	}
		case ProtectedGranter : {
		enemy = ProtectedBuffGranter();
		Buffer& buffer = registry.buffers.emplace(entity);
		buffer.range = 10.f;
		buffer.duration = 2000.f;
		buffer.maxCoolDown = 1500.f;
		EnemyPart& ep = registry.enemyParts.emplace(entity);
		ep.offset = {0,0};
		ep.alwaysFollow = true;
		registry.invisibleEnemy.emplace(entity);
		break;
	}
		case VulnerableGranter: {
		enemy = VulnerableBuffGranter();
		Buffer& buffer = registry.buffers.emplace(entity);
		buffer.range = 10.f;
		buffer.duration = 2000.f;
		buffer.maxCoolDown = 1500.f;
		EnemyPart& ep = registry.enemyParts.emplace(entity);
		ep.offset = {0,0};
		ep.alwaysFollow = true;
		registry.invisibleEnemy.emplace(entity);
		break;
	}
		case UnderGroundGranter: {
		enemy = UnderGroundBuffGranter();
		Buffer& buffer = registry.buffers.emplace(entity);
		buffer.range = 10.f;
		buffer.duration = 3000.f;
		buffer.maxCoolDown = 5000.f;
		EnemyPart& ep = registry.enemyParts.emplace(entity);
		ep.offset = {0,0};
		ep.alwaysFollow = true;
		registry.invisibleEnemy.emplace(entity);
		break;
	}
	case InvincibleGranterRoomWide : {
		enemy = WholeRoomInvincibleGranter();
		Buffer& buffer = registry.buffers.emplace(entity);
		buffer.range = 999999999.f;
		buffer.duration = 3000.f;
		buffer.maxCoolDown = 6000.f;
		RoomWideBuffers& roomWide = registry.roomWideBuffers.emplace(entity);
		registry.invisibleEnemy.emplace(entity);
		break;
	}
	case InvisibleGranterRoomWide : {
		enemy = WholeRoomInvisibleGranter();
		Buffer& buffer = registry.buffers.emplace(entity);
		buffer.range = 999999999.f;
		buffer.duration = 3000.f;
		buffer.maxCoolDown = 5000.f;
		RoomWideBuffers& roomWide = registry.roomWideBuffers.emplace(entity);
		registry.invisibleEnemy.emplace(entity);
		break;
	}
	case CloakedGranterRoomWide : {
		enemy = WholeRoomCloakedGranter();
		Buffer& buffer = registry.buffers.emplace(entity);
		buffer.range = 999999999.f;
		buffer.duration = 7500.f;
		buffer.maxCoolDown = 5000.f;
		RoomWideBuffers& roomWide = registry.roomWideBuffers.emplace(entity);
		registry.invisibleEnemy.emplace(entity);
		break;
	}
	case ProtectedGranterRoomWide : {
		enemy = WholeRoomProtectedGranter();
		Buffer& buffer = registry.buffers.emplace(entity);
		buffer.range = 999999999.f;
		buffer.duration = 4000.f;
		buffer.maxCoolDown = 5000.f;
		RoomWideBuffers& roomWide = registry.roomWideBuffers.emplace(entity);
		registry.invisibleEnemy.emplace(entity);
		break;
	}
	case VulnerableGranterRoomWide : {
		enemy = WholeRoomVulnerableGranter();
		Buffer& buffer = registry.buffers.emplace(entity);
		buffer.range = 999999999.f;
		buffer.duration = 3000.f;
		buffer.maxCoolDown = 5000.f;
		RoomWideBuffers& roomWide = registry.roomWideBuffers.emplace(entity);
		registry.invisibleEnemy.emplace(entity);
		break;
	}
	case UnderGroundGranterRoomWide : {
		enemy = WholeRoomUnderGroundGranter();
		Buffer& buffer = registry.buffers.emplace(entity);
		buffer.range = 999999999.f;
		buffer.duration = 3000.f;
		buffer.maxCoolDown = 5000.f;
		RoomWideBuffers& roomWide = registry.roomWideBuffers.emplace(entity);
		registry.invisibleEnemy.emplace(entity);
		break;
	}
	case RegenerateGranterRoomWide : {
		enemy = WholeRoomRegenerateGranter();
		Buffer& buffer = registry.buffers.emplace(entity);
		buffer.range = 999999999.f;
		buffer.duration = 3000.f;
		buffer.maxCoolDown = 7500.f;
		RoomWideBuffers& roomWide = registry.roomWideBuffers.emplace(entity);
		registry.invisibleEnemy.emplace(entity);
		break;
	}
	case EnemyBubbleShield: {
		enemy = Bubble();
		auto& ep = registry.enemyParts.emplace(entity);
		ep.offset = {0, 0};
		ep.alwaysFollow = true;
		auto& instance = registry.instanceDamages.emplace(entity);
		break;
	}
	default:
		assert(false);
	};


	Motion &motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.position = pos;
	if (registry.enemyParts.has(entity)) {
		auto& ep = registry.enemyParts.get(entity);
		if (!registry.enemies.has(ep.parent)) {
			float mindistence = 1000000;
			for (Entity e  : registry.enemies.entities) {
				Motion &motion = registry.motions.get(e);
				if (glm::distance(motion.position, pos) < mindistence && !registry.enemyParts.has(e) && e != entity && !registry.bossParts.has(e)) {
					ep.parent = e;
					mindistence = glm::distance(motion.position, pos);
				}
			}
		}
		if (registry.motions.has(ep.parent)) {
			motion.position = registry.motions.get(ep.parent).position + ep.offset;
		} else {
			registry.deleteds.emplace(entity);
		}
	}
	motion.velocity = vec2(0, 0);
	motion.scale = enemy.scale;
	if ((type == EnemyType::EnemySmallCShield || type == EnemyBubbleShield)&& registry.motions.has(registry.enemyParts.get(entity).parent)) {
		Motion& parentMotion = registry.motions.get(registry.enemyParts.get(entity).parent);
		motion.scale.x = min(max(parentMotion.scale.x, parentMotion.scale.y) * 1.75f, min(parentMotion.scale.x, parentMotion.scale.y) + 120.f);
		motion.scale.y = type == EnemyType::EnemySmallCShield ? motion.scale.x * (1.998858f / 1.923352f) : motion.scale.x;
	}

	movement.posB = AISystem::getMove(enemy.currEnemyPattern().type, entity);
	movement.speed = 100.0f * enemy.speedMultiplier;
	movement.distanceTraveled = 0.0f;

	registry.bursts.emplace(entity);

	if (enemy.sprite.geometryId == GEOMETRY_BUFFER_ID::SPRITE)
	{
		// Check how rectangular
		// If ration between side lengths > 1:1.25
		// Use poly colliders instead
		if (abs(max(motion.scale.x, motion.scale.y) / min(motion.scale.x, motion.scale.y)) > 1.25) {
			PolyCollider& pc = registry.polyColliders.emplace(entity);
			pc.offsetVertices = {
				{motion.scale.x / 2.1, motion.scale.y / 2.1},
				{motion.scale.x / 2.1, -motion.scale.y / 2.1},
				{-motion.scale.x / 2.1, -motion.scale.y / 2.1},
				{-motion.scale.x / 2.1, motion.scale.y / 2.1} };
			pc.maxLength = glm::length(vec2(motion.scale.x / 2, motion.scale.y / 2));
			pc.minLength = min(motion.scale.x / 2, motion.scale.y / 2);
		}
		else {
			CircleCollider& cc = registry.circleColliders.emplace(entity);
			cc.radius = abs(min(motion.scale.x, motion.scale.y)) / 2.0;
		}
		Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
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
		animate.max_frames = enemy.sprite.max_Frames;
		animate.animation_countdown = enemy.sprite.countdown;
		animate.animation_countdown_base = animate.animation_countdown;
	}

	if (type == EnemyType::ScientistBoss) {
		auto& animate = registry.animations.emplace(entity);
		animate.max_frames = 24; 
		animate.animation_countdown = 36;
		animate.animation_countdown_base = 36;
	}

	if (type == EnemyType::BossBeehiveMain)
	{
		createEnemy(renderer, pos + vec2(97, -95), EnemyType::BossBeehiveGun);
		createEnemy(renderer, pos + vec2(-118, 72), EnemyType::BossBeehiveGun);
	}

	if (enemy.headData.size > 0) {

		WormHead& head = registry.wormHeads.emplace(entity);
		head.size = enemy.headData.size;
		head.constrainDistance = enemy.headData.constrainDistance;
		head.body = enemy.headData.body;
		head.anchor = enemy.headData.anchor;
		//head.anchorPoint = enemy.headData.anchorPoint;
		head.anchorPoint = pos;

		Map& map = registry.maps.components[0];
		vec2 mid = map.currRoom.roomStart + 0.5f * (map.currRoom.roomEnd - map.currRoom.roomStart);
		vec2 offset = head.constrainDistance * glm::normalize(pos - mid);
		head.points.push_back(pos);
		for (int i = 1; i < head.size + 1; i++) {
			vec2 posi = pos + (float)i * offset;
			head.points.push_back(posi);
			if (i < head.size) createWormBody(renderer, posi, head.body, entity, i);
		}
	}

	if (type == EnemyType::BossBigC)
    {
        createEnemy(renderer, pos + vec2(0, 0), EnemyType::BossBigCShield);
    }

	//HP and damage scaling
	Map& map = registry.maps.components[0];
	int dmgScale = registry.elites.has(entity) ? (int) map.currRegion - 1 + registry.elites.get(entity).eliteLevel : (int) map.currRegion - 1;
	enemy.maxHealth = enemy.maxHealth * pow(1.2, (max(dmgScale , 0)));
	enemy.currHealth = enemy.maxHealth;
	if (registry.instanceDamages.has(entity)) {
		auto& instance = registry.instanceDamages.get(entity);
		instance.instance = enemy.maxHealth;
	}

	// to make hp bar drawing easier
	if ((!registry.wormBodies.has(entity) && !registry.boids.has(entity) && !registry.invisibleEnemy.has(entity)) && (!registry.bossParts.has(entity) || !registry.bossParts.get(entity).showHpBar) && !registry.enemyParts.has(entity)) {
		HPBarUI& hpbar = registry.hpBarHavers.emplace(entity);
		if (registry.bosses.has(entity)) {
			hpbar.followCamera = false;
		}
	}

	return entity;
};

void createWormBody(RenderSystem* renderer, vec2 pos, EnemyType type, Entity head, int index)
{
	auto entity = Entity();

	EnemyMovement& movement = registry.enemyMovement.emplace(entity);
	movement.posA = pos;

	Enemy& enemy = registry.enemies.emplace(entity);
	switch (type)
	{
	case BossDrillWormBody:
		enemy = DrillWormBody();
		break;
	case EnemyChainDogBody:
		enemy = ChainDogBody();
		break;
	case EnemySpineChainedWormBody:
		enemy = SpineChainedWormBody();
		break;
	case EnemySpinePatrolWormBody:
		enemy = SpinePatrolWormBody();
		break;
	case EnemySpineFollowWormBody:
		enemy = SpineFollowWormBody();
		break;
	case EnemyMiningBoidWormBody:
		enemy = MiningBoidWormBody();
		break;
	case EnemySmallMiningWormBody:
		enemy = SmallMiningWormBody();
		break;
	default:
		assert(false);
	};

	WormBody& body = registry.wormBodies.emplace(entity);
	body.head = head;
	body.index = index;

	Motion& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.position = pos;
	motion.velocity = vec2(0, 0);
	motion.scale = enemy.scale;

	registry.bursts.emplace(entity);

	if (enemy.sprite.geometryId == GEOMETRY_BUFFER_ID::SPRITE)
	{
		// Check how rectangular
		// If ration between side lengths > 1:1.25
		// Use poly colliders instead
		if (abs(max(motion.scale.x, motion.scale.y) / min(motion.scale.x, motion.scale.y)) > 1.25) {
			PolyCollider& pc = registry.polyColliders.emplace(entity);
			pc.offsetVertices = {
				{motion.scale.x / 2, motion.scale.y / 2},
				{motion.scale.x / 2, -motion.scale.y / 2},
				{-motion.scale.x / 2, -motion.scale.y / 2},
				{-motion.scale.x / 2, motion.scale.y / 2} };
			pc.maxLength = glm::length(vec2(motion.scale.x / 2, motion.scale.y / 2));
			pc.minLength = min(motion.scale.x / 2, motion.scale.y / 2);
		}
		else {
			CircleCollider& cc = registry.circleColliders.emplace(entity);
			cc.radius = abs(min(motion.scale.x, motion.scale.y)) / 2.0;
		}
		Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
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
		auto& animate = registry.animations.emplace(entity);
		animate.animate = enemy.sprite.animationType;
		animate.max_frames = enemy.sprite.max_Frames;
		animate.animation_countdown = enemy.sprite.countdown;
		animate.animation_countdown_base = animate.animation_countdown;
	}
};

void createEnemyGroup(RenderSystem *renderer, vec2 pos, EnemyType type)
{
	std::vector<Entity> groupMembers;
	Map &map = registry.maps.components[0];
	WindowState &ws = registry.windowStates.components[0];
	if (type == EnemyType::EnemyTwinLaserVertical1)
	{
		// should spawn twin on the side perpendicular to patrol direction
		vec2 twinPos = pos; // normalized position
		twinPos.x = ws.width - pos.x;
		groupMembers.push_back(createEnemy(renderer, pos, type));
		groupMembers.push_back(createEnemy(renderer, twinPos, EnemyTwinLaserVertical2));
	}
	else if (type == EnemyType::EnemyHifiTwinLaserHorizontal1)
	{
		// should spawn twin on the side perpendicular to patrol direction
		vec2 twinPos = pos; // normalized position
		twinPos.y = ws.height - twinPos.y;
		groupMembers.push_back(createEnemy(renderer, pos, type));
		groupMembers.push_back(createEnemy(renderer, twinPos, EnemyHifiTwinLaserHorizontal2));
	}
	for (Entity gm : groupMembers)
	{
		EnemyGroup &eg = registry.enemyGroups.emplace(gm);
		for (Entity other : groupMembers)
		{
			if (other == gm)
				continue;
			eg.others.push_back(other);
		}
	}
}

Entity createPopBullet(RenderSystem* renderer, vec2 pos, vec2 velocity, vec2 veer, AttackData atkData)
{
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	EnemyBullet& bullet = registry.enemyBullets.emplace(entity);
	bullet.bulletSpeed = atkData.speed;
	bullet.bulletRange = atkData.bulletRange;
	bullet.bulletBounce = atkData.bulletBounce;
	bullet.bulletPierce = atkData.bulletPierce;
	bullet.bulletEffects = atkData.rareBulletEffects;
	bullet.shape = atkData.shape;
	if (atkData.onDeath != EnemyBulletDeath::NONE)
		bullet.onDeath = atkData.onDeath;

	Motion& motion = registry.motions.emplace(entity);
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
		bullet.bulletRange = atkData.bulletRange * 0.66f;

		PolyCollider& pc = registry.polyColliders.emplace(entity);
		pc.offsetVertices = {
			{motion.scale.x / 2, motion.scale.y / 2},
			{motion.scale.x / 2, -motion.scale.y / 2},
			{-motion.scale.x / 2, -motion.scale.y / 2},
			{-motion.scale.x / 2, motion.scale.y / 2} };
		pc.maxLength = glm::length(vec2(motion.scale.x / 2, motion.scale.y / 2));
		pc.minLength = min(motion.scale.x / 2, motion.scale.y / 2);

		registry.renderRequests.insert(
			entity,
			{ "enemy_bullet_key.png",
			 EFFECT_ASSET_ID::TEXTURED,
			 GEOMETRY_BUFFER_ID::SPRITE });

		ParticleProps props = enemyBullet;
		props.colorEffects.push_back({enemyBulletParticleColors.at(Key),0});
		props.position.variation = VecOp::rotate(motion.scale, motion.angle);
		EmitParticle& ep = registry.emitParticles.emplace(entity, PBulletTrail, props, bullet.bulletRange, Random::Int(3) + 5);

		return entity;
	}
	if (bullet.bulletEffects[0].type == PlayerStackSize) {
		bullet.bulletRange *= 0.33f;
		bullet.bulletSpeed *= 2.f;
 	}

	motion.angleOffset = M_PI / 2.f;
	motion.angle += M_PI / 2.f;

	std::string renderShape;
	PolyCollider& pc = registry.polyColliders.emplace(entity);
	pc.offsetVertices = {
		{motion.scale.x / 2, motion.scale.y / 2},
		{motion.scale.x / 2, -motion.scale.y / 2},
		{-motion.scale.x / 2, -motion.scale.y / 2},
		{-motion.scale.x / 2, motion.scale.y / 2} };
	pc.maxLength = glm::length(vec2(motion.scale.x / 2, motion.scale.y / 2));
	pc.minLength = min(motion.scale.x / 2, motion.scale.y / 2);
	auto& spriteComponent = registry.sprites.emplace(entity);
	spriteComponent.sprites[SPRITE_STATE::BASE] = "bullet_values";
	renderShape = "bullet_values";
	
	registry.renderRequests.insert(
		entity,
		{ renderShape,
		 EFFECT_ASSET_ID::BULLET,
		 GEOMETRY_BUFFER_ID::SPRITE });

	// bullet trail
	ParticleProps props = enemyBullet;
	for (const BulletStackEffect& effect : bullet.bulletEffects)
	{
		BulletEffectType type = effect.type;
		if (type == BulletEffectType::Inert)
			continue;
		if (enemyBulletParticleColors.count(type) > 0)
		{
			props.colorEffects.push_back({enemyBulletParticleColors.at(type),effect.value});
		}
		else
		{
			printf("Warning: enemy bullet color not defined\n");
		}
	}
	if (!props.colorEffects.empty())
	{
		props.position.variation = VecOp::rotate(motion.scale, motion.angle);
		EmitParticle& ep = registry.emitParticles.emplace(entity, PBulletTrail, props, bullet.bulletRange, Random::Int(3) + 5);
	}

	return entity;
}

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
	bullet.bulletEffects = getBulletEffects(atkData, bullet.isSpecial);
	bullet.shape = atkData.shape;
	if (atkData.onDeath != EnemyBulletDeath::NONE)
		bullet.onDeath = atkData.onDeath;

	Motion &motion = registry.motions.emplace(entity);
	motion.angle = atan2(velocity.y, velocity.x);
	motion.position = pos;
	motion.velocity = velocity * bullet.bulletSpeed;
	motion.scale = atkData.size; // Ensure scale is initialized
	motion.veer = veer;

	// Make special bullets easier to hit
	if (bullet.isSpecial)
	{
		bullet.bulletSpeed *= 0.8;
		bullet.bulletRange *= 1.5;
		motion.scale *= 1.2f;
	}

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
		props.colorEffects.push_back({enemyBulletParticleColors.at(Key),0});
		props.position.variation = VecOp::rotate(motion.scale, motion.angle);
		EmitParticle &ep = registry.emitParticles.emplace(entity, PBulletTrail, props, 100000, Random::Int(3) + 5);

		return entity;
	}

	if (atkData.homing > 0.0)
	{
		HomingBullet &homing = registry.homes.emplace(entity);
		homing.homingIntensity = atkData.homing;
		homing.target = registry.players.entities[0];
	}
	else if (checkTierThreshold(Homing)) {
		HomingBullet& homing = registry.homes.emplace(entity);
		homing.homingIntensity = 0.02;
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
	for (const BulletStackEffect& effect : bullet.bulletEffects)
	{
		BulletEffectType type = effect.type;
		if (type == BulletEffectType::Inert)
			continue;
		if (enemyBulletParticleColors.count(type) > 0)
		{
			props.colorEffects.push_back({ enemyBulletParticleColors.at(type),effect.value });
		}
		else
		{
			printf("Warning: enemy bullet color not defined\n");
		}
	}
	if (!props.colorEffects.empty())
	{
		props.position.variation = VecOp::rotate(vec2(0, motion.scale.y), motion.angle);
		EmitParticle& ep = registry.emitParticles.emplace(entity, PBulletTrail, props, 10000, 3 + 4 * bullet.isSpecial);
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

	std::string render = "";
	if (onDeath == EnemyBulletDeath::EXPLODE)
	{
		bullet.bulletSpeed = 0;
		bullet.bulletRange = 200;
		motion.velocity = velocity * bullet.bulletSpeed;
		motion.scale = {160, 160}; // Ensure scale is initialized
		render = "none.png";
	}
	else if (onDeath == EnemyBulletDeath::CLUSTER)
	{
		bullet.bulletSpeed = 200;
		bullet.bulletRange = 1000;
		motion.velocity = velocity * bullet.bulletSpeed;
		motion.scale = {30, 30}; // Ensure scale is initialized
		motion.veer = -motion.velocity * 0.8f;
		render = "enemy_bullet_circle.png";
	}
	else if (onDeath == EnemyBulletDeath::BOMBARD)
	{
		bullet.bulletSpeed = 200;
		bullet.bulletRange = 2000;
		motion.velocity = velocity * bullet.bulletSpeed;
		motion.scale = { 20, 20 }; // Ensure scale is initialized
		render = "enemy_bullet_triangle.png";
	}
	else if (onDeath == EnemyBulletDeath::BOMBARDBOMBING)
	{
		bullet.bulletSpeed = 200;
		bullet.bulletRange = 200;
		motion.velocity = velocity * bullet.bulletSpeed;
		motion.scale = { 0, 0 }; // Ensure scale is initialized
		bullet.onDeath = EnemyBulletDeath::EXPLODE;
		render = "none.png";
	}

	auto &spriteComponent = registry.sprites.emplace(entity);

	CircleCollider &cc = registry.circleColliders.emplace(entity);
	cc.radius = motion.scale.x / 2;

	registry.renderRequests.insert(
		entity,
		{// spriteComponent.sprites[SPRITE_STATE::BASE],
		 render,
		 EFFECT_ASSET_ID::BULLET,
		 GEOMETRY_BUFFER_ID::SPRITE});

	return entity;
}

Entity createEnemyLaser(RenderSystem* renderer, vec2 pos, float angle, Entity start, AttackData atkData)
{
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	EnemyBullet& bullet = registry.enemyBullets.emplace(entity);
	bullet.bulletSpeed = 0;
	bullet.bulletRange = atkData.bulletRange;
	bullet.initialRange = atkData.bulletRange;
	bullet.bulletBounce = 0;
	bullet.bulletPierce = 10000;

	// No more laser special effects, too easy to farm
	bullet.bulletEffects = {atkData.defaultEffect};
	//bullet.bulletEffects = getBulletEffects(atkData, bullet.isSpecial);

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
	if (registry.enemies.get(start).rotationBehaviour == EnemyRotationBehavior::LASER_CONTROL) {
		laser.rotation = angle;
	}

	ParticleProps props = enemyBulletDeathParticle;
	props.lifetime = 200.f;
	props.velocity.variation = {200, 200};
	props.colorEffects.push_back({{{1, 1, 1, 1}, {1, 1, 1, 1}},0});
	registry.emitParticles.emplace(entity, PLaser, props, 100000, 1);

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

	bool type = (rand() % 100 > 30);

	EnemyBullet &bullet = registry.enemyBullets.emplace(entity);
	bullet.bulletSpeed = 450;
	bullet.bulletRange = 5000;
	bullet.bulletBounce = 10;
	bullet.bulletPierce = 10000;
	(type) ? bullet.bulletEffects = {lightningRotate} : bullet.bulletEffects = {lightningShuffle};
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
		{(type) ? "lightning_bullet_1" : "lightning_bullet_2",
		 EFFECT_ASSET_ID::ANIMATE,
		 GEOMETRY_BUFFER_ID::SPRITE});

	return entity;
}

Entity createKeyBullet(RenderSystem *renderer, vec2 pos)
{
	auto entity = Entity();

	Mesh &mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	EnemyBullet &bullet = registry.enemyBullets.emplace(entity);
	bullet.bulletSpeed = 450;
	bullet.bulletRange = 5000;
	bullet.bulletBounce = 10;
	bullet.bulletPierce = 0;
	bullet.bulletEffects = {key};
	bullet.shape = RECTANGLE;

	Motion &motion = registry.motions.emplace(entity);
	float angle = (rand() % 100 / 100.f) * 2 * M_PI;
	motion.angle = angle;
	motion.position = pos;
	motion.velocity = 450.f * vec2(cos(angle), sin(angle));
	motion.scale = 20.f * vec2(2.8, 1); // Ensure scale is initialized
	motion.veer = {0, 0};

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
	props.colorEffects.push_back({enemyBulletParticleColors.at(Key),0});
	props.position.variation = VecOp::rotate(motion.scale, motion.angle);
	EmitParticle &ep = registry.emitParticles.emplace(entity, PBulletTrail, props, 100000, Random::Int(3) + 5);

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

	// exceeded bullet limit; delete an old bullet before making this one
	if (registry.playerBullets.size() > MAX_BULLETS) {
		if (!registry.deleteds.has(registry.playerBullets.entities[0]))
			registry.deleteds.emplace(registry.playerBullets.entities[0]);
		registry.playerBullets.remove(registry.playerBullets.entities[0]);
	}

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh &mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial values
	PlayerBullet &bullet = registry.playerBullets.emplace(entity);
	bullet.damage = getModifiedValue(BulletDamage, bullet.damage) / min(1.f, getModifiedValue(BulletNum, 1) - 0.5f);
	float x = getEffectValue(BulletRange);
	bullet.bulletSpeed = bullet.bulletSpeed + clamp(-400.f, (x > 0) ? (float)x * 120.f : (float)x * -80.f, 1400.f);
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
	//motion.angle = atan2(direction.y, direction.x);
	motion.angle = 0;
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

void createNGenericPlayerBullet(RenderSystem* renderer, int number, vec2 position, vec2 direction, float range) {
	float offset = atan2(direction.y, direction.x);
	for (uint i = 0; i < number; i++)
	{
		float a = offset + i * (2 * M_PI / number);
		createGenericPlayerBullet(renderer, position, vec2(cos(a), sin(a)), range);
	}
}

Entity createGenericPlayerBullet(RenderSystem* renderer, vec2 position, vec2 direction, float range)
{
	auto entity = Entity();

	// exceeded bullet limit; delete an old bullet before making this one
	if (registry.playerBullets.size() > MAX_BULLETS) {
		if (!registry.deleteds.has(registry.playerBullets.entities[0]))
			registry.deleteds.emplace(registry.playerBullets.entities[0]);
		registry.playerBullets.remove(registry.playerBullets.entities[0]);
	}

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial values
	PlayerBullet& bullet = registry.playerBullets.emplace(entity);
	bullet.damage = 0.2f * getModifiedValue(BulletDamage, bullet.damage) / min(1.f, getModifiedValue(BulletNum, 1) - 0.5f);
	bullet.bulletSpeed = 600;
	bullet.bulletRange = range;
	bullet.bulletSize = 12;
	bullet.bulletPierce = 5;
	bullet.bulletBounce = 0;
	bullet.generic = true;

	// Invisible &inv = registry.invisibles.emplace(entity);
	// inv.countdown = (75.0f / bullet.bulletSpeed) * 1000.0f;

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0;
	//motion.angle = atan2(direction.y, direction.x);
	motion.velocity = direction * bullet.bulletSpeed;
	motion.position = position;
	motion.scale = vec2(bullet.bulletSize, bullet.bulletSize); // Ensure scale is initialized

	CircleCollider& cc = registry.circleColliders.emplace(entity);
	cc.radius = motion.scale.x / 2;

	registry.ignores.emplace(entity);

	auto& spriteComponent = registry.sprites.emplace(entity);
	spriteComponent.sprites[SPRITE_STATE::BASE] = "player_bullet.png";

	registry.renderRequests.insert(
		entity,
		{ "player_bullet.png",
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	// add bullet trail
	ParticleProps props = playerBulletTrail;
	props.position.variation = VecOp::rotate(motion.scale, motion.angle);
	EmitParticle& ep = registry.emitParticles.emplace(entity, PBulletTrail, props, 10000, Random::Int(2) + 1);
	return entity;
}

Entity createBombard(RenderSystem* renderer, float wait, vec2 position, EnemyAttackPattern pattern)
{
	auto entity = Entity();

	auto& bombard = registry.bombards.emplace(entity);
	bombard.cdTillAppear = wait;
	bombard.effect = (pattern == EnemyAttackPattern::BOMBARD) ? EnemyBulletDeath::BOMBARD : EnemyBulletDeath::BOMBARDBOMBING;

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = M_PI * (float)(rand() % 100) / 100.f;
	motion.velocity = vec2(0);
	motion.position = position;
	motion.scale = vec2(288 / 4); // Ensure scale is initialized

	auto& spriteComponent = registry.sprites.emplace(entity);
	spriteComponent.sprites[SPRITE_STATE::BASE] = "Target.png";

	registry.renderRequests.insert(
		entity,
		{ "Target.png",
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

void createNTentaclePlayerBullet(RenderSystem* renderer, int number, vec2 position, vec2 direction, float range) {
	float offset = atan2(direction.y, direction.x);
	for (uint i = 0; i < number; i++)
	{
		float a = offset + i * (2 * M_PI / number);
		createGenericPlayerBullet(renderer, position, vec2(cos(a), sin(a)), range);
	}
}

Entity createTentaclePlayerBullet(RenderSystem* renderer, vec2 position, vec2 direction, float range)
{
	auto entity = Entity();

	// exceeded bullet limit; delete an old bullet before making this one
	if (registry.playerBullets.size() > MAX_BULLETS) {
		if (!registry.deleteds.has(registry.playerBullets.entities[0]))
			registry.deleteds.emplace(registry.playerBullets.entities[0]);
		registry.playerBullets.remove(registry.playerBullets.entities[0]);
	}

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial values
	PlayerBullet& bullet = registry.playerBullets.emplace(entity);
	bullet.damage = 0.1f;
	bullet.bulletSpeed = 400;
	bullet.bulletRange = range;
	bullet.bulletSize = 10;
	bullet.bulletPierce = 0;
	bullet.bulletBounce = 0;
	bullet.generic = true;

	// Invisible &inv = registry.invisibles.emplace(entity);
	// inv.countdown = (75.0f / bullet.bulletSpeed) * 1000.0f;

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	//motion.angle = atan2(direction.y, direction.x);
	motion.angle = 0;
	motion.velocity = direction * bullet.bulletSpeed;
	motion.position = position;
	motion.scale = vec2(bullet.bulletSize, bullet.bulletSize); // Ensure scale is initialized

	CircleCollider& cc = registry.circleColliders.emplace(entity);
	cc.radius = motion.scale.x / 2;

	registry.ignores.emplace(entity);

	auto& spriteComponent = registry.sprites.emplace(entity);
	spriteComponent.sprites[SPRITE_STATE::BASE] = "player_bullet.png";

	registry.renderRequests.insert(
		entity,
		{ "player_bullet.png",
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

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

std::vector<BulletStackEffect> getBulletEffects(AttackData atkData, bool &isSpecial)
{
	Map& map = registry.maps.components[0];

	// Fixed chance of special bullet
	float positiveProb = 0.2f;

	// TODO: is this needed?
	assert(!map.currRoom.preset.negativeEffects.empty() || map.currRoom.type == TutorialRoom2);

	// Non-room related effects
	if (atkData.positiveBulletEffects.size() > 0 && Random::Float() < positiveProb && map.currRoom.preset.numSpecialBulletsToSpawn > 0)
	{
		isSpecial = true;
		return atkData.positiveBulletEffects;
	}
	isSpecial = false;
	if (Random::Float() < 0.2f * log((float)map.currRegion) || map.currRoom.type == Testing || map.currRoom.type == TutorialRoom2) {
		return atkData.negativeBulletEffects;
	}
	return { blunt };

}

vec2 lerpToRoom(vec2 point) {
	Map& map = registry.maps.components[0];
	//return glm::lerp(map.currRoom.roomStart, map.currRoom.roomEnd, point);
	return map.currRoom.roomStart * (vec2(1) - point) + map.currRoom.roomEnd * point;
}
