#pragma once
#include "actor_components.hpp"
#include "render_system.hpp"

//////////////////////////////////////////
///////////  BulletEffects  //////////////
//////////////////////////////////////////
/*
struct BulletStackEffect {
	BulletEffectType type = BulletEffectType::Inert;
	EffectCalculation effectCalc;
	float value;

	// For UIq
	std::string name;
	std::string tooltip;
	vec3 color;

};
*/

const BulletStackEffect blunt = {
	Inert,
	Additive,
	0,
	"blunt",
	""};

const BulletStackEffect lightning = {
	Lightning,
	Additive,
	0,
	"lightning",
	""};

const BulletStackEffect dmgDownA = {
	BulletDamage,
	Additive,
	-2.0,
	"dmgDownA",
	""};

const BulletStackEffect dmgDownM = {
	BulletDamage,
	Multiplicative,
	0.7,
	"dmgDownM",
	""};

const BulletStackEffect numBulletsUpA = {
	BulletNum,
	Additive,
	1,
	"numBulletsUpA",
	""};

const BulletStackEffect sizeUpA = {
	ProjectileSize,
	Additive,
	10,
	"sizeUpA",
	""};

const BulletStackEffect spreadUpA = {
	BulletSpread,
	Additive,
	15,
	"spreadUpA",
	""};

const BulletStackEffect bulletSpeedUpA = {
	ProjectileSpeed,
	Additive,
	50,
	"speedUpA",
	""};

const BulletStackEffect bulletSpeedUpM = {
	ProjectileSpeed,
	Multiplicative,
	1.5,
	"speedUpM",
	""};

const BulletStackEffect bulletRangeUpA = {
	BulletRange,
	Additive,
	500,
	"rangeUpA",
	""};

const BulletStackEffect bulletRangeUpM = {
	BulletRange,
	Multiplicative,
	1.5,
	"rangeUpM",
	""};

const BulletStackEffect bulletBurstUpA = {
	BulletBurst,
	Additive,
	1,
	"burstUpA",
	""};

const BulletStackEffect bulletBurstUpM = {
	BulletBurst,
	Multiplicative,
	1.5,
	"burstUpM",
	""};

const BulletStackEffect bulletPierceUpA = {
	Pierce,
	Additive,
	1,
	"pierceUpA",
	""};

const BulletStackEffect bulletPierceUpM = {
	Pierce,
	Multiplicative,
	1.5,
	"pierceUpM",
	""};

const BulletStackEffect bulletBounceUpA = {
	Bounce,
	Additive,
	1,
	"bounceUpA",
	""};

const BulletStackEffect bulletBounceUpM = {
	Bounce,
	Multiplicative,
	1.5,
	"bounceUpM",
	""};

const BulletStackEffect dashUpA = {
	PlayerNumDash,
	Additive,
	1,
	"dashUpA",
	""};

const BulletStackEffect dashUpM = {
	PlayerNumDash,
	Multiplicative,
	2,
	"dashUpM",
	""};

const BulletStackEffect key = {
	Key,
	Additive,
	0,
	"Key",
	""};

//////////////////////////////////////////
///////////  AttackData  /////////////////
//////////////////////////////////////////
/*
struct AttackData {
	EnemyAttackPattern attackType;

	EnemyBulletShape shape = EnemyBulletShape::CIRCLE;
	std::vector<BulletStackEffect> rareBulletEffects;
	BulletStackEffect defaultEffect;
	int numBullets = 1;
	int angleOffset = 0;
	vec2 size = { 20,20 };
	float speed = 200;
	float bulletRange = 3000;
	vec2 veer = { 0,0 }; // {magnitude, angle}, {growth, rotation} lasers, {#bullets per shot, burst CD} radial burst
	int bulletPierce = 0;
	int bulletBounce = 0;
	float homing = 0;
};
*/

const AttackData none{
	EnemyAttackPattern::NONE,
	CIRCLE,
	{},
	blunt,
	3,
	M_PI / 6.0,
	{20, 20},
	200,
	3000,
	{0, 0},
	0,
	1,
	0};

const AttackData trail{
	EnemyAttackPattern::TRAIL,
	CIRCLE,
	{},
	blunt,
	1,
	0,
	{20, 20},
	0,
	10000,
	{0, 0},
	0,
	0,
	0};

const AttackData wave{
	EnemyAttackPattern::WAVE,
	CIRCLE,
	{},
	blunt,
	5,
	0,
	{20, 20},
	200,
	3000,
	{0, 0},
	0,
	0,
	0};

const AttackData laserNoRotate{
	EnemyAttackPattern::LASER,
	CIRCLE,
	{bulletBounceUpA},
	blunt,
	3,
	0,
	{20, 20},
	0,
	30000,
	{0.5, 0},
	0,
	0,
	0};

const AttackData laserRotate{
	EnemyAttackPattern::LASER,
	CIRCLE,
	{},
	blunt,
	3,
	0,
	{0, 20},
	0,
	8000,
	{1.2, M_PI / 300},
	0,
	0,
	0};

const AttackData threeShot{
	EnemyAttackPattern::SHOTGUN,
	CIRCLE,
	{},
	blunt,
	3,
	M_PI / 6.0,
	{20, 20},
	200,
	3000,
	{0, 0},
	0,
	1,
	0};

const AttackData missile{
	EnemyAttackPattern::SHOTGUN,
	TRIANGLE,
	{},
	blunt,
	1,
	0,
	{50, 30},
	400,
	8000,
	{0, 0},
	0,
	0,
	0.01,
	EnemyBulletDeath::CLUSTER};

const AttackData sixShot{
	EnemyAttackPattern::SHOTGUN,
	RECTANGLE,
	{},
	blunt,
	6,
	M_PI / 8,
	{20, 20},
	200,
	3000,
	{0, 0},
	0,
	1,
	0};

const AttackData radialBurst{
	EnemyAttackPattern::BURST_RADIAL,
	CIRCLE,
	{numBulletsUpA, sizeUpA},
	blunt,
	40,
	M_PI / 40,
	{20, 20},
	150,
	3000,
	{4, 200},
	0,
	0,
	0};

const AttackData radialPolygon{
	EnemyAttackPattern::RADIAL_POLYGON,
	CIRCLE,
	{numBulletsUpA, sizeUpA},
	blunt,
	5,
	M_PI / 40,
	{20, 20},
	150,
	3000,
	{4, 200},
	0,
	0,
	0};

const AttackData fourAllAround{
	EnemyAttackPattern::RADIAL,
	TRIANGLE,
	{},
	blunt,
	4,
	M_PI / 4,
	{20, 20},
	200,
	3000,
	{0, 0},
	0,
	1,
	0};

const AttackData twoPincerShot{
	EnemyAttackPattern::SHOTGUN,
	CIRCLE,
	{},
	blunt,
	2,
	M_PI / 1.0,
	{20, 20},
	200,
	3000,
	{200, -2 * M_PI / 3.0},
	0,
	1,
	0};

const AttackData twelveSpiralShot{
	EnemyAttackPattern::RADIAL,
	TRIANGLE,
	{key},
	blunt,
	12,
	0.0,
	{20, 20},
	200,
	3000,
	{200, -2 * M_PI / 3.0},
	0,
	0,
	0};

const AttackData threeHomingShot{
	EnemyAttackPattern::SHOTGUN,
	TRIANGLE,
	{dmgDownA, dmgDownM},
	blunt,
	3,
	M_PI / 4.0,
	{20, 20},
	200,
	6000,
	{0.0, 0.0},
	0,
	0,
	0.01};

const AttackData threeBurst{
	EnemyAttackPattern::BURST,
	CIRCLE,
	{numBulletsUpA},
	sizeUpA,
	4,
	M_PI / 6,
	{20, 20},
	200,
	3000,
	{0, 0},
	0,
	1,
	0};

const AttackData fiveBurst{
	EnemyAttackPattern::BURST,
	TRIANGLE,
	{numBulletsUpA, sizeUpA},
	blunt,
	6,
	M_PI / 6,
	{30, 30},
	200,
	3000,
	{0, 0},
	0,
	1,
	0,
};

const AttackData threeSpray{
	EnemyAttackPattern::SPRAY,
	CIRCLE,
	{},
	blunt,
	3,
	M_PI / 16,
	{20, 20},
	200,
	3000,
	{0, 0},
	0,
	1,
	0};

const AttackData SniperShot{
	EnemyAttackPattern::SHOTGUN,
	TRIANGLE,
	{},
	blunt,
	1,
	0,
	{20, 20},
	900,
	3000,
	{0, 0},
	0,
	0,
	0};

const AttackData NoAttack{
	EnemyAttackPattern::NONE,
	CIRCLE,
	{},
	blunt,
	0,
	0,
	{0, 0},
	0,
	0,
	{0, 0},
	0,
	0,
	0};

const AttackData quadShot{
	EnemyAttackPattern::RADIAL,
	TRIANGLE,
	{},
	blunt,
	4,
	0.0,
	{20, 20},
	200,
	3000,
	{0.0, 0.0},
	0,
	0,
	0};

////////////////////////////////////
//////////// ENEMY TYPE ////////////
////////////////////////////////////

//////////////////////////////////
//
// struct EnemyPattern {
//     // type is just like state
//	   std::string name;
//     EnemyBehavior type;
//     std::vector<vec2> path;
//     int pathIndex;
//     float curDuration;
//     float maxDuration;
//     // all possible reactions in current behavior state
//     std::vector<Reaction> reactions;
//     int next;
//     bool canAttack;
//     float currAtkCD;
//     float maxAtkCD;
//     AttackData atkData;
// };
/////////////////////////////////

struct TestEnemy : Enemy
{
	Reaction reactionPatrol = {
		ReactionType::DURATION,
		1};

	Reaction reactionIdle = {
		ReactionType::DURATION,
		0};
	Reaction reactionPlayerClose = {
		ReactionType::PLAYER_CLOSE,
		2};
	EnemyPattern idleState = {"IDLE", EnemyBehavior::IDLE, {}, 0, 1500.f, 1500.f, {reactionPatrol}, 1, false, 0, 0, NoAttack};
	EnemyPattern patrolState = {"RANDOM", EnemyBehavior::RANDOM, {}, 0, 10000.f, 10000.f, {reactionPlayerClose, reactionIdle}, 0, true, 0, 2000.f, twelveSpiralShot};
	EnemyPattern followState = {"FOLLOW", EnemyBehavior::FOLLOW_PLAYER, {}, 0, 0.f, 0.f, {reactionPlayerClose, reactionIdle}, 0, true, 0.f, 5000.f, SniperShot};
	TestEnemy()
	{
		maxHealth = 50;
		currHealth = maxHealth;
		enemyPatterns = {
			idleState,
			patrolState,
			followState};
		patternIndex = 0;
		sprite = {
			"enemy_Pufferfish.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(-12, 0)};
		scale = vec2({288.0f / 2, 240.f / 2});
		rotatePower = 0.5f;
	};
};

struct EnemyEasySentry : Enemy
{

	EnemyPattern rotateState = {"ROTATE IN PLACE", EnemyBehavior::ROTATE_IN_PLACE, {}, 0, 10000.f, 10000.f, {}, 0, true, 0.f, 5000.f, quadShot};

	EnemyEasySentry()
	{
		maxHealth = 100;
		currHealth = maxHealth;

		enemyPatterns = {rotateState};

		patternIndex = 0;
		sprite = {
			"enemy_QuadShooter.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(-12, 0)};
		scale = vec2({240.0f / 2, 240.f / 2});
		rotatePower = 0.f;
	};
};

struct EnemyMediumTank : Enemy
{

	Reaction durationWalking = {
		ReactionType::DURATION,
		0};
	Reaction durationShoot = {
		ReactionType::DURATION,
		1};
	Reaction halfHP = {
		ReactionType::FIFTY_HEALTH,
		2};

	Reaction durationWalkingRage = {
		ReactionType::DURATION,
		3};

	Reaction durationAttackMissile = {
		ReactionType::DURATION,
		4};

	Reaction durationAttackSniper = {
		ReactionType::DURATION,
		5};

	Reaction durationSpiralShot = {
		ReactionType::DURATION,
		2};

	EnemyPattern randomState = {"RANDOM POSITION", EnemyBehavior::RANDOM, {}, 0, 3000.f, 3000.f, {durationShoot, halfHP}, 1, false, 0.f, 0.f, NoAttack};
	EnemyPattern idleState = {"IDLE SHOOTING", EnemyBehavior::IDLE, {}, 0, 5000.f, 5000.f, {durationWalking, halfHP}, 0, true, 0.f, 800.f, radialBurst};
	EnemyPattern rageState = {"HALF HP", EnemyBehavior::IDLE, {}, 0, 3000.f, 3000.f, {durationWalkingRage}, 3, true, 0.f, 500.f, twelveSpiralShot};
	EnemyPattern walkingRage = {"WALKING RAGE", EnemyBehavior::RANDOM, {}, 0, 3000.f, 3000.f, {durationAttackMissile}, 4, false, 0.f, 0.f, NoAttack};
	EnemyPattern shootMisile = {"MISSILE", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {durationAttackSniper}, 5, true, 0.f, 300.f, missile};
	EnemyPattern shootSniper = {"sniper", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {durationSpiralShot}, 2, true, 0.f, 300.f, SniperShot};
	EnemyMediumTank()
	{
		maxHealth = 500;
		currHealth = maxHealth;
		enemyPatterns = {
			randomState, idleState, rageState, walkingRage, shootMisile, shootSniper};
		patternIndex = 0;
		sprite = {
			"enemy_Crab.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE};
		scale = vec2({288.0f / 2, 240.f / 2});
		rotatePower = 0.8f;
	};
};

struct EnemyBigC : Enemy
{

	EnemyBigC()
	{
		maxHealth = 1000;

		currHealth = maxHealth;

		enemyPatterns = {
			{"ROTATE IN PLACE", EnemyBehavior::ROTATE_IN_PLACE, {}, 0, 10000.f, 10000.f, {}, 0, false, 0.f, 5000.f, twelveSpiralShot}};
		patternIndex = 0;
		sprite = {// registry.sprites.get(entity).sprites[SPRITE_STATE::BASE],
				  "none",
				  EFFECT_ASSET_ID::MESH,
				  GEOMETRY_BUFFER_ID::MESH_GB};
		scale = vec2({700 * (1.923352 / 2.f), 700});
		rotatePower = 1.f;
	};
};

struct EnemyMediumCharge : Enemy
{
	Reaction lowHealth = {
		ReactionType::FIFTY_HEALTH,
		0};

	Reaction durationFollow = {
		ReactionType::DURATION,
		1};

	Reaction durationIdle = {
		ReactionType::DURATION,
		0};
	EnemyPattern idleState = {"IDLE ATTACKING", EnemyBehavior::IDLE, {}, 0, 10000.f, 10000.f, {durationIdle}, 0, true, 0.f, 2000.f, missile};
	EnemyPattern followState = {"FOLLOW ENEMY", EnemyBehavior::FOLLOW_PLAYER, {}, 0, 0.f, 0.f, {lowHealth, durationFollow}, 1, false, 0.f, 0.f, NoAttack};
	EnemyMediumCharge()
	{
		maxHealth = 50;
		currHealth = maxHealth;
		enemyPatterns = {
			idleState, followState};
		patternIndex = 1;
		sprite = {
			"enemy_Magnet.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		scale = vec2({288.0f / 2, 240.f / 2});
		rotatePower = 0.8f;
	};
};

struct Bee1 : Enemy
{
	Reaction reactionPatrol = {
		ReactionType::DURATION,
		1};

	Reaction reactionIdle = {
		ReactionType::DURATION,
		0};
	Reaction reactionBeeClose = {
		ReactionType::BEE_CLOSE,
		2};
	Reaction reactionNoBees = {
		ReactionType::NO_BEES,
		0};

	EnemyPattern idleBee = {"IDLE", EnemyBehavior::IDLE, {}, 0, 1500.f, 1500.f, {reactionPatrol, reactionBeeClose}, 1, false, 0, 0, NoAttack};
	EnemyPattern randomBee = {"RANDOM", EnemyBehavior::RANDOM, {}, 0, 10000.f, 10000.f, {reactionBeeClose, reactionIdle}, 0, true, 0, 2000.f, NoAttack};
	EnemyPattern mergeBee = {"MERGE BEE", EnemyBehavior::MERGE_BEE, {}, 0, 0.f, 0.f, {reactionBeeClose, reactionNoBees}, 0, true, 0.f, 5000.f, NoAttack};
	Bee1()
	{
		maxHealth = 20;
		currHealth = maxHealth;
		enemyPatterns = {idleBee, randomBee, mergeBee};
		patternIndex = 0;
		sprite = {
			"bee_fly_1",
			EFFECT_ASSET_ID::ANIMATE,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0)};
		scale = vec2({864 / 8.f, 480 / 8.f});
		rotatePower = 1.f;
	};
};

struct Bee2 : Enemy
{
	Reaction reactionPatrol = {
		ReactionType::DURATION,
		1};

	Reaction reactionIdle = {
		ReactionType::DURATION,
		0};

	Reaction reactionBeeClose = {
		ReactionType::BEE_CLOSE,
		2};
	Reaction reactionNoBees = {
		ReactionType::NO_BEES,
		0};

	EnemyPattern idleBee = {"IDLE", EnemyBehavior::IDLE, {}, 0, 1500.f, 1500.f, {reactionPatrol, reactionBeeClose}, 1, false, 0, 0, NoAttack};
	EnemyPattern randomBee = {"RANDOM", EnemyBehavior::RANDOM, {}, 0, 10000.f, 10000.f, {reactionBeeClose, reactionIdle}, 0, true, 0, 2000.f, SniperShot};
	EnemyPattern mergeBee = {"MERGE BEE", EnemyBehavior::MERGE_BEE, {}, 0, 0.f, 0.f, {reactionBeeClose, reactionNoBees}, 0, false, 0.f, 5000.f, NoAttack};

	Bee2()
	{
		maxHealth = 60;
		currHealth = maxHealth;
		enemyPatterns = {idleBee, randomBee, mergeBee};
		patternIndex = 0;
		sprite = {
			"bee_fly_2",
			EFFECT_ASSET_ID::ANIMATE,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		scale = vec2({864 / 8.f, 480 / 8.f});
		rotatePower = 1.f;
	};
};

struct Bee3 : Enemy
{
	Reaction reactionFollow = {
		ReactionType::DURATION,
		1};

	Reaction reactionIdle = {
		ReactionType::DURATION,
		0};

	Reaction playerClose = {
		ReactionType::PLAYER_CLOSE,
		1};

	const AttackData beeSpray{
		EnemyAttackPattern::SPRAY,
		TRIANGLE,
		{},
		blunt,
		15,
		M_PI,
		{20, 20},
		600,
		3000,
		{0, 0},
		0,
		0,
		0};

	EnemyPattern idleBee = {"IDLE", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {reactionFollow}, 1, true, 0, 600.f, beeSpray};
	EnemyPattern randomBee = {"FOLLOW", EnemyBehavior::FOLLOW_PLAYER, {}, 0, 2000.f, 2000.f, {reactionFollow}, 0, false, 0, 1000.f, none};
	Bee3()
	{
		maxHealth = 100;
		currHealth = maxHealth;
		enemyPatterns = {idleBee, randomBee};
		patternIndex = 0;
		sprite = {
			"bee_fly_3",
			EFFECT_ASSET_ID::ANIMATE,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		scale = vec2({864 / 8.f, 720 / 8.f});
		rotatePower = 1.f;
	};
};

struct EnemyMediumBeeHive : Enemy
{
	Reaction Spawning{
		ReactionType::DURATION,
		1};
	Reaction idling{
		ReactionType::DURATION,
		0};
	Reaction halfHP{
		ReactionType::FIFTY_HEALTH,
		2};
	Reaction finalIdle{
		ReactionType::DURATION,
		3};
	EnemyPattern idlingState = {"IDLING", EnemyBehavior::IDLE, {}, 0, 1500.f, 1500.f, {Spawning, halfHP}, 1, false, 0.f, 0.f, NoAttack};
	EnemyPattern spawningState = {"SPAWNING", EnemyBehavior::SPAWNING, {}, 0, 3000.f, 3000.f, {idling, halfHP}, 0, false, 0.f, 0.f, NoAttack};
	EnemyPattern halfHPState = {"SPAWN LOT BEES", EnemyBehavior::SPAWNING, {}, 0, 3000.f, 3000.f, {finalIdle}, 3, false, 0.f, 0.f, NoAttack};
	EnemyPattern deadHiveState = {"DEAD HIVE", EnemyBehavior::IDLE, {}, 0, 10000.f, 10000.f, {finalIdle}, 3, false, 0.f, 0.f, NoAttack};
	EnemyMediumBeeHive()
	{
		maxHealth = 200;
		currHealth = maxHealth;
		enemyPatterns = {idlingState, spawningState, halfHPState, deadHiveState};
		sprite = {
			"beehive_close.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		patternIndex = 0;
		scale = vec2({240.0f / 2, 312.f / 2});
		rotatePower = 0.f;
	};
};

struct EnemyHardAngel : Enemy
{
	Reaction Attack2{
		ReactionType::DURATION,
		3};
	Reaction Attack0{
		ReactionType::DURATION,
		1};
	Reaction Attack1{
		ReactionType::DURATION,
		2,
	};

	Reaction Attack3{
		ReactionType::DURATION,
		0};
	EnemyPattern attack1State = {"ATTACK 1", EnemyBehavior::FOLLOW_PLAYER, {}, 0, 3000.f, 3000.f, {Attack0}, 1, true, 500.f, 500.f, SniperShot};
	EnemyPattern attack2State = {"ATTACK 2", EnemyBehavior::FOLLOW_PLAYER, {}, 0, 2000.f, 2000.f, {Attack1}, 2, true, 0.f, 500.f, wave};
	EnemyPattern attack3State = {"ATTACK 3", EnemyBehavior::FOLLOW_PLAYER, {}, 0, 2000.f, 2000.f, {Attack2}, 0, true, 0.f, 500.f, fourAllAround};
	EnemyPattern followPlayerState = {"ATTACK4", EnemyBehavior::FOLLOW_PLAYER, {}, 0, 3000.f, 3000.f, {Attack3}, 0, true, 0.f, 600.f, SniperShot};
	EnemyHardAngel()
	{
		maxHealth = 200;
		currHealth = maxHealth;
		enemyPatterns = {attack1State, attack2State, attack3State, followPlayerState};
		sprite = {
			"enemy_Angel.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		patternIndex = 0;
		scale = vec2({192 / 2.f, 216 / 2.f});
		rotatePower = 0.f;
	};
};

struct EnemyEasySkull : Enemy
{
	Reaction AttackLaser{
		ReactionType::DURATION,
		0};
	EnemyPattern laserState = {"ATTACK LASER", EnemyBehavior::IDLE, {}, 0, 5000.f, 5000.f, {AttackLaser}, 0, true, 0.f, 10000.f, laserRotate};
	EnemyEasySkull()
	{
		maxHealth = 100;
		currHealth = maxHealth;
		enemyPatterns = {laserState};
		sprite = {
			"skull.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE};
		patternIndex = 0;
		scale = vec2({230.0f / 2, 240.f / 2});
		rotatePower = 0.f;
	};
};

struct EnemyHardBoid : Enemy
{
	Reaction boid{
		ReactionType::DURATION,
		0};
	EnemyPattern boidState = {"BOID", EnemyBehavior::BOIDS, {}, 0, 5000.f, 5000.f, {}, 0, false, 0.f, 0.f, NoAttack};
	EnemyHardBoid()
	{
		maxHealth = 1;
		currHealth = maxHealth;
		enemyPatterns = {boidState};
		sprite = {
			"bio_boid.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		scale = vec2({20.f, 20.f});
		patternIndex = 0;
		rotatePower = 1.f;
	}
};

struct EnemyMediumBoar : Enemy
{
	Reaction noticePlayer{
		ReactionType::PLAYER_CLOSE,
		1};
	Reaction reactionToIdle{
		ReactionType::DURATION,
		2};

	Reaction reactionToRandom{
		ReactionType::DURATION,
		3};

	Reaction reactionToRandom2{
		ReactionType::DURATION,
		0};

	EnemyPattern chargingState = {"CHARGE", EnemyBehavior::CHARGING, {}, 0, 2000.f, 2000.f, {reactionToIdle}, 2, true, 0.f, 0.f, NoAttack};
	EnemyPattern randomPos = {"RANDOM", EnemyBehavior::RANDOM, {}, 0, 2000.f, 2000.f, {noticePlayer}, 0, true, 0.f, 1000.f, wave};
	EnemyPattern idleState = {"IDLE", EnemyBehavior::IDLE, {}, 0, 3000.f, 3000.f, {reactionToRandom}, 3, false, 0.f, 0.f, NoAttack};
	EnemyPattern randomPosNoCharge = {"RANDOM", EnemyBehavior::RANDOM, {}, 0, 1000.f, 1000.f, {reactionToRandom2}, 0, true, 0.f, 1000.f, wave};

	EnemyMediumBoar()
	{
		maxHealth = 300;
		currHealth = maxHealth;
		enemyPatterns = {randomPos, chargingState, idleState, randomPosNoCharge};
		sprite = {
			"enemy_Snail.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		scale = vec2({864 / 8.f, 480 / 8.f});
		patternIndex = 0;
		rotatePower = 0.5f;
	}
};

struct EnemyMediumHeal : Enemy
{

	Reaction teamHurt{
		ReactionType::TEAM_HURT,
		1};
	Reaction reactionToIdle{
		ReactionType::DURATION,
		0};
	EnemyPattern idleState = {"IDLE", EnemyBehavior::IDLE, {}, 0, 2000.f, 2000.f, {teamHurt}, 0, false, 0.f, 0.f, NoAttack};
	EnemyPattern healState = {"HEAL", EnemyBehavior::HEALING, {}, 0, 10000.f, 10000.f, {reactionToIdle, teamHurt}, 0, false, 0.f, 0.f, NoAttack};
	EnemyMediumHeal()
	{
		maxHealth = 200;
		currHealth = maxHealth;
		enemyPatterns = {idleState, healState};
		sprite = {
			"enemy_Angel.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		patternIndex = 0;
		scale = vec2({192 / 2.f, 216 / 2.f});
		rotatePower = 0.f;
	}
};

// struct EnemyHardSkull : {

// 	EnemyHardSkull() {

// 	};
// };