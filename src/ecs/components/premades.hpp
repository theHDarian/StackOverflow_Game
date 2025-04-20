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
	0,
	"Inert",
	""};

const BulletStackEffect lightningRotate = {
	Lightning,
	-1,
	"Lightning",
	""};

const BulletStackEffect lightningShuffle = {
	Lightning,
	1,
	"Lightning",
	""};

const BulletStackEffect dmgDown = {
	BulletDamage,
	-1,
	"Damage Down",
	""};

const BulletStackEffect dmgDown2 = {
	BulletDamage,
	-2,
	"Damage Down",
	"" };

const BulletStackEffect dmgDown3 = {
	BulletDamage,
	-3,
	"Damage Down",
	"" };

const BulletStackEffect dmgUp = {
	BulletDamage,
	1,
	"Damage Up",
	""};

const BulletStackEffect dmgUp2 = {
	BulletDamage,
	2,
	"Damage Up",
	"" };

const BulletStackEffect dmgUp3 = {
	BulletDamage,
	3,
	"Damage Up",
	"" };

const BulletStackEffect numBulletsUp = {
	BulletNum,
	1,
	"Bullet Count Up",
	""};

const BulletStackEffect numBulletsDown = {
	BulletNum,
	-1,
	"Bullet Count Down",
	"" };

const BulletStackEffect sizeUp = {
	ProjectileSize,
	1,
	"Bullet Size Up",
	"" };

const BulletStackEffect sizeDown = {
	ProjectileSize,
	-1,
	"Bullet Size Up",
	"" };


const BulletStackEffect accuracyUp = {
	BulletAccuracy,
	1,
	"Bullet Accuracy Up",
	""};

const BulletStackEffect accuracyDown = {
	BulletAccuracy,
	-1,
	"Bullet Accuracy Down",
	"" };

const BulletStackEffect bulletSpeedUp = {
	ProjectileSpeed,
	1,
	"Bullet Speed Up",
	""};

const BulletStackEffect bulletSpeedDown = {
	ProjectileSpeed,
	-1,
	"Bullet Speed Up",
	""};

const BulletStackEffect bulletRangeUp = {
	BulletRange,
	1,
	"Range Up",
	""};

const BulletStackEffect bulletRangeDown = {
	BulletRange,
	-1,
	"Range Down",
	""};

const BulletStackEffect bulletBurstUp = {
	BulletBurst,
	1,
	"Burst Up",
	""};

const BulletStackEffect bulletBurstDown = {
	BulletBurst,
	-1,
	"Burst Down",
	""};

const BulletStackEffect bulletPierceUp = {
	Pierce,
	1,
	"Pierce Up",
	""};

const BulletStackEffect bulletPierceDown = {
	Pierce,
	-1,
	"Pierce Down",
	""};

const BulletStackEffect bulletBounceUp = {
	Bounce,
	1,
	"Bounce Up",
	""};

const BulletStackEffect bulletBounceDown = {
	Bounce,
	-1,
	"Bounce Down",
	""};

const BulletStackEffect dashUp = {
	PlayerNumDash,
	1,
	"Dash Count Up",
	""};

const BulletStackEffect dashDown = {
	PlayerNumDash,
	-1,
	"Dash Count Down",
	""};

const BulletStackEffect stackSizeUp = {
	PlayerStackSize,
	1,
	"Stack Size Up",
	""};

const BulletStackEffect stackSizeDown = {
	PlayerStackSize,
	-1,
	"Stack Size Down",
	""};

const BulletStackEffect dashRechargeUp = {
	PlayerDashRecharge,
	1,
	"Dash Recharge Rate Up",
	""};

const BulletStackEffect dashRechargeDown = {
	PlayerDashRecharge,
	-1,
	"Dash Recharge Rate Down",
	""};

const BulletStackEffect playerSpeedUp = {
	PlayerSpeed,
	1,
	"Movement Speed Up",
	""};

const BulletStackEffect playerSpeedDown = {
	PlayerSpeed,
	-1,
	"Movement Speed Down",
	""};

const BulletStackEffect fireRateUp = {
	FireRate,
	1,
	"Fire Rate Up",
	""};

const BulletStackEffect fireRateDown = {
	FireRate,
	-1,
	"Fire Rate Down",
	""};

const BulletStackEffect key = {
	Key,
	0,
	"Key",
	""};

const BulletStackEffect homingUp = {
	Homing,
	1,
	"Homing Accuracy Up",
	"" };

const BulletStackEffect homingDown = {
	Homing,
	-1,
	"Homing Accuracy Down",
	"" };

const BulletStackEffect popCurrentStack = {
	Pop,
	0,
	"Pop the stack",
	""
};

// note: adding the effect to list is not necessary
// but guarantees it will be tokenized on game load
const std::vector<BulletStackEffect> premadeBullets = {
	blunt,
	dmgDown,
	dmgUp,
	numBulletsUp,
	sizeUp,
	accuracyUp,
	bulletSpeedUp,
	bulletSpeedDown,
	bulletRangeUp,
	bulletRangeDown,
	bulletBurstUp,
	bulletBurstDown,
	bulletPierceUp,
	bulletBounceUp,
	bulletBounceDown,
	dashUp,
	dashDown,
	stackSizeUp,
	stackSizeDown,
	dashRechargeUp,
	dashRechargeDown,
	playerSpeedUp,
	playerSpeedDown,
	key,
	homingUp
};

//////////////////////////////////////////
///////////  AttackData  /////////////////
//////////////////////////////////////////
/*
struct AttackData {
	EnemyAttackPattern attackType;

	EnemyBulletShape shape = EnemyBulletShape::CIRCLE;
	std::vector<BulletStackEffect> rareBulletEffects;
	BulletStackEffect defaultEffect;
	int numBullets = 1; //For Wall attacks, refers to number of holes in the wall
	int angleOffset = 0;
	vec2 size = { 20,20 };
	float speed = 200;
	float bulletRange = 3000;
	vec2 veer = { 0,0 }; // {magnitude, angle}, {growth, rotation} lasers, {#bullets per shot, burst CD} radial burst
	int bulletPierce = 0; // Very low pierce implies ignore walls
	int bulletBounce = 0; // Very low bounce implies piledriver (bullet sticks into the wall until duration)
	float homing = 0;
	EnemyBulletDeath 
	EnemyType //for spawning
	vector<vec2> spawnPosition; spawning position have to have the same length as number of spawning
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
	{dmgUp},
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
	{numBulletsUp, dmgDown},
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

const AttackData HomingWave{
	EnemyAttackPattern::WAVE,
	CIRCLE,
	{numBulletsUp, dmgDown},
	blunt,
	5,
	0,
	{20, 20},
	200,
	3000,
	{0, 0},
	0,
	0,
	.02};

const AttackData laserNoRotate{
	EnemyAttackPattern::LASER,
	CIRCLE,
	{dashUp},
	dashRechargeDown,
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
	{bulletPierceUp},
	dashRechargeDown,
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
	{dashRechargeDown},
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
	{accuracyUp, sizeUp},
	dmgDown,
	1,
	0,
	{50, 30},
	400,
	8000,
	{0, 0},
	0,
	0,
	0.01,
	EnemyBulletDeath::EXPLODE};

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
	{numBulletsUp, sizeUp},
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
	{numBulletsUp, sizeUp},
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
	{dmgUp},
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
	{numBulletsUp, accuracyUp},
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
	{dmgDown, dmgDown},
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
	{numBulletsUp},
	sizeUp,
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
	{numBulletsUp, sizeUp},
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
	{dmgUp},
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
	{bulletPierceUp, dmgUp, numBulletsDown},
	blunt,
	1,
	0,
	{30, 20},
	100,
	3000,
	{600, 0},
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

const AttackData FastLaser{
	EnemyAttackPattern::LASER
	,CIRCLE
	,{dashUp, playerSpeedUp, playerSpeedUp}
	,dashRechargeUp
	,1
	,0
	,{20, 20}
	,0
	, 2000
	,{5000, 0.97}
	,0
	,0
	,0
	,EnemyBulletDeath::NONE
	};

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

struct TestLightningRotate : Enemy
{
	const AttackData testShot{
		EnemyAttackPattern::RADIAL,
		RECTANGLE,
		{},
		lightningRotate,
		1,
		0.0,
		{20, 20},
		100,
		2000,
		{0.0, 0.0},
		0,
		0,
		0 
	};

	EnemyPattern idleState = { "IDLE", EnemyBehavior::IDLE, {}, 0, 500000.f, 500000.f, {}, 0, true, 0, 1000, testShot };

	TestLightningRotate()
	{
		maxHealth = 500000;
		currHealth = maxHealth;
		enemyPatterns = { idleState };
		patternIndex = 0;
		sprite = {
			"enemy_QuadShooter.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0) 
		};
		scale = vec2({ 192.0f / 2, 192.f / 2 });
		rotatePower = 0.f;
	};
};

struct TestLightningShuffle : Enemy
{
	const AttackData testShot{
		EnemyAttackPattern::RADIAL,
		RECTANGLE,
		{},
		lightningShuffle,
		1,
		0.0,
		{20, 20},
		100,
		2000,
		{0.0, 0.0},
		0,
		0,
		0
	};

	EnemyPattern idleState = { "IDLE", EnemyBehavior::IDLE, {}, 0, 500000.f, 500000.f, {}, 0, true, 0, 1000, testShot };

	TestLightningShuffle()
	{
		maxHealth = 500000;
		currHealth = maxHealth;
		enemyPatterns = { idleState };
		patternIndex = 0;
		sprite = {
			"enemy_QuadShooter.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0)
		};
		scale = vec2({ 192.0f / 2, 192.f / 2 });
		rotatePower = 0.f;
	};
};

struct TestEffectPlusOne : Enemy
{
	const AttackData testShot{
		EnemyAttackPattern::RADIAL,
		CIRCLE,
		{},
		dmgUp,
		1,
		0.0,
		{20, 20},
		100,
		2000,
		{0.0, 0.0},
		0,
		0,
		0
	};

	EnemyPattern idleState = { "IDLE", EnemyBehavior::IDLE, {}, 0, 500000.f, 500000.f, {}, 0, true, 0, 1000, testShot };

	TestEffectPlusOne()
	{
		maxHealth = 500000;
		currHealth = maxHealth;
		enemyPatterns = { idleState };
		patternIndex = 0;
		sprite = {
			"enemy_QuadShooter.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0)
		};
		scale = vec2({ 192.0f / 2, 192.f / 2 });
		rotatePower = 0.f;
	};
};

struct TestEffectPlusTwo : Enemy
{
	const AttackData testShot{
		EnemyAttackPattern::RADIAL,
		CIRCLE,
		{},
		dmgUp2,
		1,
		0.0,
		{20, 20},
		100,
		2000,
		{0.0, 0.0},
		0,
		0,
		0
	};

	EnemyPattern idleState = { "IDLE", EnemyBehavior::IDLE, {}, 0, 500000.f, 500000.f, {}, 0, true, 0, 1000, testShot };

	TestEffectPlusTwo()
	{
		maxHealth = 500000;
		currHealth = maxHealth;
		enemyPatterns = { idleState };
		patternIndex = 0;
		sprite = {
			"enemy_QuadShooter.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0)
		};
		scale = vec2({ 192.0f / 2, 192.f / 2 });
		rotatePower = 0.f;
	};
};

struct TestEffectPlusThree : Enemy
{
	const AttackData testShot{
		EnemyAttackPattern::RADIAL,
		CIRCLE,
		{},
		dmgUp3,
		1,
		0.0,
		{20, 20},
		100,
		2000,
		{0.0, 0.0},
		0,
		0,
		0
	};

	EnemyPattern idleState = { "IDLE", EnemyBehavior::IDLE, {}, 0, 500000.f, 500000.f, {}, 0, true, 0, 1000, testShot };

	TestEffectPlusThree()
	{
		maxHealth = 500000;
		currHealth = maxHealth;
		enemyPatterns = { idleState };
		patternIndex = 0;
		sprite = {
			"enemy_QuadShooter.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0)
		};
		scale = vec2({ 192.0f / 2, 192.f / 2 });
		rotatePower = 0.f;
	};
};

struct TestEffectMinusOne : Enemy
{
	const AttackData testShot{
		EnemyAttackPattern::RADIAL,
		TRIANGLE,
		{},
		dmgDown,
		1,
		0.0,
		{20, 20},
		100,
		2000,
		{0.0, 0.0},
		0,
		0,
		0
	};

	EnemyPattern idleState = { "IDLE", EnemyBehavior::IDLE, {}, 0, 500000.f, 500000.f, {}, 0, true, 0, 1000, testShot };

	TestEffectMinusOne()
	{
		maxHealth = 500000;
		currHealth = maxHealth;
		enemyPatterns = { idleState };
		patternIndex = 0;
		sprite = {
			"enemy_QuadShooter.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0)
		};
		scale = vec2({ 192.0f / 2, 192.f / 2 });
		rotatePower = 0.f;
	};
};

struct TestEffectMinusTwo : Enemy
{
	const AttackData testShot{
		EnemyAttackPattern::RADIAL,
		TRIANGLE,
		{},
		dmgDown2,
		1,
		0.0,
		{20, 20},
		100,
		2000,
		{0.0, 0.0},
		0,
		0,
		0
	};

	EnemyPattern idleState = { "IDLE", EnemyBehavior::IDLE, {}, 0, 500000.f, 500000.f, {}, 0, true, 0, 1000, testShot };

	TestEffectMinusTwo()
	{
		maxHealth = 500000;
		currHealth = maxHealth;
		enemyPatterns = { idleState };
		patternIndex = 0;
		sprite = {
			"enemy_QuadShooter.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0)
		};
		scale = vec2({ 192.0f / 2, 192.f / 2 });
		rotatePower = 0.f;
	};
};

struct TestEffectMinusThree : Enemy
{
	const AttackData testShot{
		EnemyAttackPattern::RADIAL,
		TRIANGLE,
		{},
		dmgDown3,
		1,
		0.0,
		{20, 20},
		100,
		2000,
		{0.0, 0.0},
		0,
		0,
		0
	};

	EnemyPattern idleState = { "IDLE", EnemyBehavior::IDLE, {}, 0, 500000.f, 500000.f, {}, 0, true, 0, 1000, testShot };

	TestEffectMinusThree()
	{
		maxHealth = 500000;
		currHealth = maxHealth;
		enemyPatterns = { idleState };
		patternIndex = 0;
		sprite = {
			"enemy_QuadShooter.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0)
		};
		scale = vec2({ 192.0f / 2, 192.f / 2 });
		rotatePower = 0.f;
	};
};


struct Pufferfish : Enemy
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
	Pufferfish()
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

struct ChainDogHead : Enemy
{
	
	EnemyPattern followState = { "Follow", EnemyBehavior::WORM_FOLLOW, {}, 0, 3000.f, 3000.f, {}, 0, false, 0.f, 5000.f, quadShot };

	ChainDogHead()
	{
		maxHealth = 200;
		currHealth = maxHealth;

		enemyPatterns = { followState };

		patternIndex = 0;
		sprite = {
			"DogBall.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0)
		};
		scale = vec2({ 120.0f / 2, 120.f / 2 });
		rotationBehaviour = EnemyRotationBehavior::FACE_UP;

		speedMultiplier = 2.5f;

		headData.size = 10;
		headData.body = EnemyChainDogBody;
		headData.constrainDistance = 30.f;
		headData.anchor = true;
	};
};

struct ChainDogBody : Enemy
{

	EnemyPattern state = { "IDLE", EnemyBehavior::WORM_BODY, {}, 0, 1000000.f, 1000000.f, {}, 0, false, 0.f, 2500.f, none };

	ChainDogBody()
	{
		maxHealth = 100;
		currHealth = maxHealth;

		enemyPatterns = { state };

		patternIndex = 0;
		sprite = {
			"Chain.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0)
		};
		scale = vec2({ 96.0f / 2, 72.f / 2 });
		rotationBehaviour = EnemyRotationBehavior::WORM;
	};
};

struct Quadshooter : Enemy
{

	EnemyPattern rotateState = {"ROTATE IN PLACE", EnemyBehavior::ROTATE_IN_PLACE, {}, 0, 10000.f, 10000.f, {}, 0, true, 0.f, 5000.f, quadShot};

	Quadshooter()
	{
		maxHealth = 100;
		currHealth = maxHealth;

		enemyPatterns = {rotateState};

		patternIndex = 0;
		sprite = {
			"enemy_QuadShooter.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0)};
		scale = vec2({240.0f / 2, 240.f / 2});
		rotatePower = 1.f;
	};
};

struct EliteQuadshooter : Enemy
{
	const AttackData whip{
		EnemyAttackPattern::RADIAL,
		TRIANGLE,
		{dmgUp},
		 blunt,
		4,
		M_PI/16,
		{15, 15},
		400,
		2500,
		{200, M_PI / 1.5},
		0,
		0,
		0};

	const AttackData broadsideLasers{
		EnemyAttackPattern::LASER,
		CIRCLE,
		{bulletPierceUp},
		dashRechargeUp,
		4,
		0,
		{30, 30},
		0,
		4500,
		{10, 0},
		0,
		0,
		0 };

	Reaction LowHP = {
		ReactionType::FIFTY_HEALTH,
		3,
		SpecialStates::INVINCIBLE
	};

	Reaction Duration = {
		ReactionType::DURATION,
		1};

	EnemyPattern rotateState = {"ROTATE IN PLACE", EnemyBehavior::ROTATE_IN_PLACE, {}, 0, 3500.f, 3500.f, {Duration, LowHP}, 1, true, 0.f, 100.f, whip};
	EnemyPattern CoolDownState1 = {"ROTATE IN PLACE", EnemyBehavior::ROTATE_IN_PLACE, {}, 0, 6000.f, 6000.f, {Duration, LowHP}, 2, false, 0.f, 5000.f, NoAttack, SpecialStates::VULNERABLE};
	EnemyPattern TelePort = {"TELEPORT", EnemyBehavior::ROTATE_IN_PLACE, {}, 0, 1000.f, 1000.f, {Duration, LowHP}, 0, false, 0.f, 500.f, NoAttack,SpecialStates::VULNERABLE};
	EnemyPattern LaserRoatateState = {"ROTATE IN PLACE", EnemyBehavior::ROTATE_IN_PLACE, {}, 0, 5000.f, 5000.f, {Duration}, 4, true, 0.f, 5000.f, broadsideLasers,  };
	EnemyPattern rotateState2 = {"ROTATE IN PLACE", EnemyBehavior::ROTATE_IN_PLACE, {}, 0, 4500.f, 4500.f, {Duration}, 5, true, 0.f, 100.f, whip};
	EnemyPattern CoolDownState2 = {"ROTATE IN PLACE", EnemyBehavior::ROTATE_IN_PLACE, {}, 0, 5000.f, 6000.f, {Duration}, 6, false, 0.f, 5000.f, NoAttack, SpecialStates::VULNERABLE};
	EnemyPattern TelePort2 = {"TELEPORT", EnemyBehavior::ROTATE_IN_PLACE, {}, 0, 1000.f, 1000.f, {Duration}, 3, false, 0.f, 500.f, NoAttack,SpecialStates::VULNERABLE};
	EliteQuadshooter()
	{
		maxHealth = 300;
		currHealth = maxHealth;

		enemyPatterns = {rotateState, CoolDownState1, TelePort, LaserRoatateState,  rotateState2, CoolDownState2, TelePort2};

		patternIndex = 0;
		sprite = {
			"enemy_OverClockedQuadShooter.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0)};
		scale = vec2({240.0f / 1.5, 240.f / 1.5});
		rotatePower = 0.7f;
		rotationBehaviour = EnemyRotationBehavior::LASER_CONTROL;
	};
};


struct TutorialQuadshooter : Enemy
{
	const AttackData tutorialQuadShot{
		EnemyAttackPattern::RADIAL,
		TRIANGLE,
		{},
		dashUp,
		4,
		0.0,
		{20, 20},
		200,
		3000,
		{0.0, 0.0},
		0,
		0,
		0};

	Reaction duration = {
		ReactionType::DURATION,
		1};

	EnemyPattern rotateState = {"ROTATE IN PLACE", EnemyBehavior::ROTATE_IN_PLACE, {}, 0, 5000.f, 5000.f, {duration}, 1, true, 0.f, 5000.f, tutorialQuadShot};
	EnemyPattern rotateStatenorm = {"ROTATE IN PLACE", EnemyBehavior::ROTATE_IN_PLACE, {}, 0, 0.f, 10000.f, {duration}, 1, true, 0.f, 5000.f, quadShot};


	TutorialQuadshooter()
	{
		maxHealth = 50;
		currHealth = maxHealth;

		enemyPatterns = {rotateState, rotateStatenorm};

		patternIndex = 0;
		sprite = {
			"enemy_QuadShooter.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0)};
		scale = vec2({240.0f / 2, 240.f / 2});
		rotatePower = 1.f;
	};
};

struct Snail : Enemy
{
	const AttackData snailTrail{
		EnemyAttackPattern::TRAIL,
		CIRCLE,
		{bulletBounceUp},
		playerSpeedDown,
		1,
		0,
		{20, 20},
		0,
		20000,
		{0, 0},
		0,
		0,
		0};

	EnemyPattern rotateState = {"Follow Player", EnemyBehavior::FOLLOW_PLAYER, {}, 0, 10000.f, 10000.f, {}, 0, true, 0.f, 5000.f, snailTrail};

	Snail()
	{
		maxHealth = 150;
		currHealth = maxHealth;

		enemyPatterns = {rotateState};
		rotatePower = 0.5;
		rotationBehaviour = EnemyRotationBehavior::FACE_UP;

		patternIndex = 0;
		sprite = {
			"enemy_Snail.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0)};
		scale = vec2(336, 216) * 0.5f;
		armour = 2;
	};
};

struct EvilSnail : Enemy
{
	const AttackData snailTrail{
		EnemyAttackPattern::TRAIL,
		CIRCLE,
		{bulletBounceUp},
		playerSpeedDown,
		1,
		0,
		{20, 20},
		0,
		8000,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::CLUSTER};

	EnemyPattern rotateState = {"Follow Player", EnemyBehavior::FOLLOW_PLAYER, {}, 0, 10000.f, 10000.f, {}, 0, true, 0.f, 5000.f, snailTrail};

	EvilSnail()
	{
		maxHealth = 200;
		currHealth = maxHealth;

		enemyPatterns = {rotateState};
		rotationBehaviour = EnemyRotationBehavior::FACE_UP;

		patternIndex = 0;
		sprite = {
			"enemy_EvilSnail.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0)};
		scale = vec2(336, 216) * 0.5f;
		speedMultiplier = 1.15;
		rotatePower = 0.5;
		armour = 2;
	};
};

struct Crab : Enemy
{

	const AttackData twoPincerShot{
		EnemyAttackPattern::SHOTGUN,
		TRIANGLE,
		{bulletBounceUp},
		blunt,
		2,
		M_PI / 1.5,
		{40, 40},
		500,
		1700,
		{600, -2 * M_PI / 3.0},
		0,
		0,
		0};

	Reaction duration = {
		ReactionType::DURATION,
		1};

	EnemyPattern randomState = {"RANDOM POSITION", EnemyBehavior::RANDOM, {}, 0, 3000.f, 3000.f, {duration}, 1, false, 0.f, 0.f, NoAttack};
	EnemyPattern idleState = {"IDLE SHOOTING", EnemyBehavior::IDLE, {}, 0, 3000.f, 3000.f, {duration}, 2, true, 0.f, 1000.f, twoPincerShot};
	EnemyPattern walkingRage = {"WALKING RAGE", EnemyBehavior::RANDOM, {}, 0, 3000.f, 3000.f, {duration}, 3, false, 0.f, 0.f, NoAttack};
	EnemyPattern shootMisile = {"MISSILE", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {duration}, 0, true, 0.f, 500.f, missile};
	Crab()
	{
		maxHealth = 150;
		currHealth = maxHealth;
		enemyPatterns = {
			randomState, idleState, walkingRage, shootMisile};
		patternIndex = 0;
		sprite = {
			"enemy_Crab.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE};
		scale = vec2({336.0f / 2, 240.f / 2});
		rotatePower = 0.8f;
		armour = 2;
	};
};

struct LaserCrab : Enemy
{

	const AttackData crabLaser{
		EnemyAttackPattern::LASER,
		CIRCLE,
		{bulletPierceUp},
		dashRechargeUp,
		1,
		0,
		{20, 20},
		0,
		10000000,
		{4, 0},
		0,
		0,
		0};

	Reaction duration = {
		ReactionType::FINISH_PATROL,
		1};

	EnemyPattern randomState = {"PatrolBoundary", EnemyBehavior::PATROLLING, {{0.99, 0.01}, {0.99, 0.99}, {0.01, 0.99}, {0.01, 0.01}, {0.99, 0.01}}, 0, 3000.f, 3000.f, {duration}, 0, true, 0.f, 1000000000.f, crabLaser};
	LaserCrab()
	{
		maxHealth = 150;
		currHealth = maxHealth;
		enemyPatterns = {
			randomState};
		patternIndex = 0;
		sprite = {
			"enemy_LaserCrab.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE};
		scale = vec2({240.0f / 2, 336.f / 2});
		rotatePower = 1.0f;
		speedMultiplier = 2.6;
		rotationBehaviour = EnemyRotationBehavior::FACE_CENTER;
		armour = 2;
	};
};

struct EvilCrab : Enemy
{

	const AttackData radialSquare{
		EnemyAttackPattern::RADIAL_POLYGON,
		CIRCLE,
		{sizeUp},
		playerSpeedDown,
		5,
		M_PI / 4,
		{20, 20},
		150,
		3000,
		{0, 0},
		0,
		0,
		0};

	Reaction duration = {
		ReactionType::DURATION,
		1};

	EnemyPattern randomState = {"RANDOM POSITION", EnemyBehavior::RANDOM_FAR, {}, 0, 3000.f, 3000.f, {duration}, 1, false, 0.f, 0.f, NoAttack};
	EnemyPattern idleState = {"IDLE SHOOTING", EnemyBehavior::IDLE, {}, 0, 5000.f, 5000.f, {duration}, 2, true, 0.f, 1000.f, radialSquare};
	EnemyPattern walkingRage = {"WALKING RAGE", EnemyBehavior::RANDOM_FAR, {}, 0, 1000.f, 1000.f, {duration}, 3, false, 0.f, 0.f, NoAttack};
	EnemyPattern shootMisile = {"MISSILE", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {duration}, 0, true, 0.f, 300.f, missile};
	EvilCrab()
	{
		maxHealth = 200;
		currHealth = maxHealth;
		enemyPatterns = {
			randomState, idleState, walkingRage, shootMisile};
		patternIndex = 0;
		sprite = {
			"enemy_EvilCrab.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE};
		scale = vec2({336.0f / 2, 240.f / 2});
		rotatePower = 0.8f;
		speedMultiplier = 1.5f;
		armour = 2;
	};
};


struct BossChimeraCrab : Enemy {
	const AttackData crabLaser{
		EnemyAttackPattern::LASER,
		CIRCLE,
		{bulletPierceUp},
		dashRechargeUp,
		1,
		0,
		{20, 20},
		0,
		10000000,
		{4, 0},
		0,
		0,
		0};

	const AttackData broadsideLasers{
		EnemyAttackPattern::LASER,
		CIRCLE,
		{bulletPierceUp},
		dashRechargeUp,
		2,
		0,
		{60, 60},
		0,
		10000,
		{500, 0},
		0,
		0,
		0 };

	const AttackData radialSquare{
		EnemyAttackPattern::RADIAL_POLYGON,
		CIRCLE,
		{sizeUp},
		playerSpeedDown,
		5,
		M_PI / 4,
		{20, 20},
		150,
		30000,
		{0, 0},
		0,
		2,
		0};

	const AttackData radialSquarephase2{
		EnemyAttackPattern::RADIAL_POLYGON,
		CIRCLE,
		{sizeUp},
		accuracyUp,
		5,
		M_PI / 4,
		{20, 20},
		250,
		15000,
		{0, 0},
		0,
		12,
		0};

	const AttackData twoPincerShot{
		EnemyAttackPattern::SHOTGUN,
		TRIANGLE,
		{bulletBounceUp},
		blunt,
		2,
		M_PI / 1.5,
		{30, 30},
		500,
		4700,
		{600, -2 * M_PI / 3.0},
		0,
		1,
		0};

	std::vector<EnemyType> crabs = {EnemyType::EnemyCrab, EnemyType::EnemyEvilCrab, EnemyType::EnemyLaserCrab};

	const AttackData crabSummon{
		EnemyAttackPattern::SPAWNING,
		TRIANGLE,
		{numBulletsUp},
		playerSpeedDown,
		1,
		M_PI,
		{20, 20},
		600,
		3000,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		Random::ListItem(crabs),
		{}};
	const AttackData laserSummon{
		EnemyAttackPattern::SPAWNING,
		TRIANGLE,
		{numBulletsUp},
		playerSpeedDown,
		1,
		M_PI,
		{20, 20},
		600,
		3000,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		BossCrabLaser,
		{}};
	const AttackData laserRotate{
		EnemyAttackPattern::LASER,
		CIRCLE,
		{bulletPierceUp},
		dashRechargeUp,
		3,
		0,
		{0, 20},
		0,
		8000,
		{1.2, M_PI / 300},
		0,
		0,
		0};

	const AttackData missile{
		EnemyAttackPattern::SHOTGUN,
		TRIANGLE,
		{accuracyUp, sizeUp},
		dmgDown,
		2,
		 M_PI / 1.5,
		{50, 30},
		350,
		10000,
		{0, 0},
		0,
		0,
		0.015,
		EnemyBulletDeath::EXPLODE};

	std::vector<AttackData> phase2Normals = {twoPincerShot, radialSquare, crabLaser};

	Reaction duration = {
		ReactionType::DURATION,
		1};
	Reaction halfhp = {
		ReactionType::FIFTY_HEALTH,
		4,
		};
	Reaction repositioned = {
		ReactionType::FINISH_PATROL,
		5 };

	Reaction PlayerFar = {
		ReactionType::PLAYER_FAR,
		10};

	EnemyPattern randomState = {"RANDOM POSITION", EnemyBehavior::RANDOM_FAR, {}, 0, 3000.f, 3000.f, {duration,halfhp}, 1, false, 0.f, 0.f, crabSummon, SpecialStates::INVINCIBLE};
	EnemyPattern idleState = {"IDLE SHOOTING", EnemyBehavior::IDLE, {}, 0, 5000.f, 5000.f, {duration,halfhp}, 2, true, 0.f, 1000.f, radialSquare};
	EnemyPattern walkingRage = {"WALKING RAGE", EnemyBehavior::RANDOM_FAR, {}, 0, 1000.f, 1000.f, {duration,halfhp}, 3, false, 0.f, 0.f, twoPincerShot};
	EnemyPattern shootMisile = {"MISSILE", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {duration, halfhp}, 0, true, 0.f, 500.f, missile};

	EnemyPattern HalfHPRepostion = { "RANDOM POSITION", EnemyBehavior::PATROLLING, {{0.5, 0.5},{0.5, 0.5}}, 0, 1000.f, 1000.f, {repositioned}, 5, true, 0.f, 500.f, radialSquare, SpecialStates::INVINCIBLE };
	EnemyPattern HalfHPLaser = { "Laser Spin", EnemyBehavior::ROTATE_IN_PLACE, {}, 0, 25000.f, 25000.f, {duration}, 6, true, 0.f, 5000.f, broadsideLasers, SpecialStates::INVINCIBLE };

	EnemyPattern RetreatAndShoot = {"RetreatAndShoot", EnemyBehavior::RETREAT, {}, 0, 2000.f, 2000.f, {duration,PlayerFar}, 7, true, 0.f, 75.f,  wave};
	EnemyPattern randomStateHalfHP = {"RANDOM POSITION", EnemyBehavior::RANDOM_FAR, {}, 0, 3000.f, 3000.f, {duration}, 8, true, 0.f, 15000.f, crabSummon};
	EnemyPattern walkingRageHalfhp = {"WALKING RAGE", EnemyBehavior::RANDOM, {}, 0, 1000.f, 1000.f, {duration}, 9, true, 0.f, 100.f, twoPincerShot};
	EnemyPattern randomlaserState = {"PatrolBoundary", EnemyBehavior::PATROLLING, {{0.99, 0.01}, {0.99, 0.99}, {0.01, 0.99}, {0.01, 0.01}, {0.99, 0.01}}, 0, 2000.f, 2000.f, {duration}, 6, true, 0.f, 120.f, radialSquarephase2, SpecialStates::INVINCIBLE};
	EnemyPattern Phase2Laser = { "Laser Spin", EnemyBehavior::ROTATE_IN_PLACE, {}, 0, 4000.f, 4000.f, {duration}, 7, true, 0.f, 5000.f, broadsideLasers, };

	BossChimeraCrab()
	{
		maxHealth = 850;
		currHealth = maxHealth;
		enemyPatterns = {
			randomState, idleState, walkingRage, shootMisile, HalfHPRepostion, HalfHPLaser, RetreatAndShoot, randomStateHalfHP, walkingRageHalfhp, randomlaserState, Phase2Laser};
		patternIndex = 0;
		sprite = {
			"chimera_crab_boss",
			EFFECT_ASSET_ID::ANIMATE,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0),
			AnimationTypes::REGULAR,
			4,
			150};
		scale = vec2({336.0f , 240.f });
		rotatePower = 0.8f;
		rotationBehaviour = EnemyRotationBehavior::LASER_CONTROL;
		speedMultiplier = 1.5f;
		armour = 3;
	};
};

struct chimeraCrabSniper : Enemy
{
	EnemyPattern randomState = {"Follow", EnemyBehavior::FOLLOW_PLAYER, {{0.99, 0.01}, {0.99, 0.99}, {0.01, 0.99}, {0.01, 0.01}, {0.99, 0.01}}, 0, 0.f, 150.f, {{ReactionType::DURATION, 0}}, 0, true, 0.f, 5000.f, FastLaser};
	// EnemyPattern charging = {"Charge", EnemyBehavior::FOLLOWSCIENTIST, {}, 0, 0.f, 30000.f, {{ReactionType::DURATION, 0}}, 0, true, 0.f, 5000.f, NoAttack};

	chimeraCrabSniper()
	{
		maxHealth = 100;
		currHealth = maxHealth;
		enemyPatterns = {randomState};
		patternIndex = 0;
		sprite = {
			"enemy_hifi_007.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE};
		scale = vec2({160.0f / 2, 160.f / 2});
		rotatePower = 1.0f;
		speedMultiplier = 1.3;
		rotationBehaviour = EnemyRotationBehavior::FACE_PLAYER;
	};
};

struct SmallC : Enemy
{
	const AttackData shortFastLaser{
		EnemyAttackPattern::LASER
		,CIRCLE
		,{dashUp, dashRechargeUp}
		,bulletPierceUp
		,1
		,0
		,{50, 50}
		,0
		, 2000
		,{5000, 0}
		,0
		,0
		,0
		,EnemyBulletDeath::NONE
		};

	SmallC()
	{
		maxHealth = 5000;

		currHealth = maxHealth;

		Reaction duration = {
			ReactionType::DURATION,
			1};

		EnemyPattern idleState = {
			"IDLE", EnemyBehavior::ROTATE_IN_PLACE, {}, 0, 15000.f, 15000.f, {duration}, 2, false, 2500, 7000.f, NoAttack};

		EnemyPattern LaserSpin = {
			"FOLLOW", EnemyBehavior::ROTATE_IN_PLACE, {}, 0, 10000.f, 10000.f, {duration}, 3, true, 0.f, 10000.f, NoAttack};

		EnemyPattern LineUp = {
			"FOLLOW", EnemyBehavior::IDLE, {}, 0, 5000.f, 5000.f, {duration}, 3, false, 0.f, 5000.f, NoAttack, };

		EnemyPattern LaserAim = {
			"FOLLOW", EnemyBehavior::IDLE, {}, 0, 15000.f, 15000.f, {duration}, 0, true, 3000.f, 6000, shortFastLaser, SpecialStates::VULNERABLE};

		enemyPatterns = {
			idleState, LaserSpin, LineUp, LaserAim};
		patternIndex = 0;
		sprite = {// registry.sprites.get(entity).sprites[SPRITE_STATE::BASE],
			"none",
			EFFECT_ASSET_ID::MESH,
			GEOMETRY_BUFFER_ID::MESH_GB};
		rotatePower = 0.6f;
		scale = vec2({300, 300 * (1.998858f / 1.923352f)});
		rotationBehaviour = EnemyRotationBehavior::FACE_PLAYER;
		armour = 5;
	};
};


struct BigC : Enemy
{

	const AttackData FastLaser{
		EnemyAttackPattern::LASER
		,CIRCLE
		,{dashUp, dashRechargeUp}
		,bulletPierceUp
		,1
		,0
		,{120, 120}
		,0
		, 5000
		,{5000, 0}
		,0
		,0
		,0
		,EnemyBulletDeath::NONE
		};

	const AttackData shortFastLaser{
		EnemyAttackPattern::LASER
		,CIRCLE
		,{dashUp, dashRechargeUp}
		,bulletPierceUp
		,1
		,0
		,{120, 120}
		,0
		, 2000
		,{5000, 0}
		,0
		,0
		,0
		,EnemyBulletDeath::NONE
		};

	BigC()
	{
		maxHealth = 250000;

		currHealth = maxHealth;

		Reaction duration = {
            ReactionType::DURATION,
            1};

		EnemyPattern idleState = {
            "IDLE", EnemyBehavior::ROTATE_IN_PLACE, {}, 0, 15000.f, 15000.f, {duration}, 2, true, 2500, 7000.f, FastLaser};

		EnemyPattern LaserSpin = {
            "FOLLOW", EnemyBehavior::ROTATE_IN_PLACE, {}, 0, 10000.f, 10000.f, {duration}, 3, true, 0.f, 10000.f, FastLaser};

		EnemyPattern LineUp = {
            "FOLLOW", EnemyBehavior::IDLE, {}, 0, 5000.f, 5000.f, {duration}, 3, false, 0.f, 5000.f, NoAttack, };

		EnemyPattern LaserAim = {
			"FOLLOW", EnemyBehavior::IDLE, {}, 0, 15000.f, 15000.f, {duration}, 0, true, 3000.f, 6000, shortFastLaser, SpecialStates::VULNERABLE};

		enemyPatterns = {
			idleState, LaserSpin, LineUp, LaserAim};
		patternIndex = 0;
		sprite = {// registry.sprites.get(entity).sprites[SPRITE_STATE::BASE],
				  "none",
				  EFFECT_ASSET_ID::MESH,
				  GEOMETRY_BUFFER_ID::MESH_GB};
		rotatePower = 0.6f;
		scale = vec2({800, 800 * (1.998858f / 1.923352f)});
		rotationBehaviour = EnemyRotationBehavior::FACE_PLAYER;
		armour = 10;
		collisionBullet = {blunt,blunt, blunt,};
	};
};

struct BossBigCCore : Enemy{

	const AttackData sixShot{
	EnemyAttackPattern::RADIAL,
	TRIANGLE,
	{numBulletsUp, accuracyUp},
	blunt,
	6,
	0.0,
	{20, 20},
	400,
	1000,
	{300, M_PI},
	0,
	0,
	0 };

	const AttackData laserRotate{
		EnemyAttackPattern::LASER,
		CIRCLE,
		{bulletPierceUp},
		dashRechargeUp,
		6,
		M_PI / 6,
		{0, 20},
		0,
		2000,
		{2.5, M_PI / 300},
		0,
		0,
		0};


	const AttackData spawning{
		EnemyAttackPattern::SPAWNING,
		CIRCLE,
		{},
		blunt,
		18,
		0,
		{60, 60},
		600,
		1000,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::EnemyHifiTemporaryBoid};

	const AttackData laser2{
		EnemyAttackPattern::LASER,
		CIRCLE,
		{dashUp},
		dashRechargeUp,
		8,
		0,
		{20, 20},
		0,
		1500,
		{500, 0},
		0,
		0,
		0};

	const AttackData laser1{
		EnemyAttackPattern::LASER,
		CIRCLE,
		{dashUp},
		dashRechargeUp,
		8,
		M_PI / 8.f,
		{20, 20},
		0,
		1500,
		{500, 0},
		0,
		0,
		0};

	const AttackData laser4{
		EnemyAttackPattern::LASER,
		CIRCLE,
		{dashUp},
		dashRechargeUp,
		8,
		0,
		{60, 60},
		0,
		1500,
		{500, 0},
		0,
		0,
		0};

	const AttackData laser3{
		EnemyAttackPattern::LASER,
		CIRCLE,
		{dashUp},
		dashRechargeUp,
		8,
		M_PI / 8.f,
		{60, 60},
		0,
		1500,
		{500,0},
		0,
		0,
		0};

	const AttackData spawningLaserSniper1{
		EnemyAttackPattern::SPAWNING,
		CIRCLE,
		{},
		blunt,
		3,
		0,
		{60, 60},
		600,
		1000,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::EnemyLaserSniper,
			{{0.5f, 1-0.63f}, {0.38, 1-0.42}, {1-0.38, 1-0.42} }
		};

	const AttackData spawningState2{
		EnemyAttackPattern::SPAWNING,
		CIRCLE,
		{},
		blunt,
		25,
		0,
		{60, 60},
		600,
		1000,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::EnemyHifiBoid};

	const AttackData spawningState3{
		EnemyAttackPattern::SPAWNING,
		CIRCLE,
		{},
		blunt,
		4,
		0,
		{60, 60},
		600,
		1000,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyHifiJellyFish,
		{{0.25, 0.25}, {0.75, 0.75}, {0.25, 0.75}, {0.75, 0.25}}
	};

	const AttackData spawningState4{
		EnemyAttackPattern::SPAWNING,
		CIRCLE,
		{},
		blunt,
		150,
		0,
		{60, 60},
		600,
		1000,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::EnemyFishBoid};


	Reaction duration = {
		ReactionType::DURATION,
		1};
	Reaction seven_five_hp = {
		ReactionType::SEVENTYFIVE_HEALTH,
		7,
		SpecialStates::INVINCIBLE};

	Reaction halfhp = {
		ReactionType::FIFTY_HEALTH,
				8
	};

	Reaction twenty_five_HP = {
		ReactionType::TWENTYFIVE_HEALTH,
		9,
		SpecialStates::INVINCIBLE};

	Reaction playerClose = {
        ReactionType::PLAYER_CLOSE,
        1};
	Reaction playerCloseHalfHP = {
        ReactionType::PLAYER_CLOSE,
        2};
	Reaction playerCloseTwentyFiveHP = {
		ReactionType::PLAYER_CLOSE,
			10
	};

	EnemyPattern idleState = {
		"IDLE", EnemyBehavior::IDLE, {}, 0, 15000.f, 15000.f, {duration, seven_five_hp, playerClose, twenty_five_HP}, 0, false, 0.f, 1000.f,
		NoAttack
	};

	EnemyPattern PlayerClose = {
		"PlayerClose", EnemyBehavior::IDLE, {}, 0, 3000.f, 3000.f, {duration, seven_five_hp,twenty_five_HP }, 0, true, 0.f, 3000.f,
		sixShot, SpecialStates::VULNERABLE
	};

	EnemyPattern PlayerCloseHalfHP = {
		"PlayerClose", EnemyBehavior::IDLE, {}, 0, 3000.f, 3000.f, {duration, twenty_five_HP}, 3, false, 2000.f, 500.f, NoAttack, SpecialStates::VULNERABLE
	};

	EnemyPattern PlayerCloseHalfHPLaser = {
		"PlayerClose", EnemyBehavior::IDLE, {}, 0, 7000.f, 7000.f, {duration, twenty_five_HP}, 5, true, 0.f, 100.f, laserRotate,
		SpecialStates::INVINCIBLE
	};

	EnemyPattern randomStateSevenFiveHP = {
		"RANDOM POSITION", EnemyBehavior::IDLE, {}, 0, 5000.f, 5000.f, {duration, playerCloseHalfHP, halfhp, twenty_five_HP}, 5, true,
		100.f, 1500.f, spawning, SpecialStates::PROTECTED
	};
	EnemyPattern laserState1 = {"ATTACK LASER", EnemyBehavior::IDLE, {}, 0, 2000.f, 2000.f, {duration, playerCloseHalfHP, twenty_five_HP }, 6, true, 0.f, 2000.f, laser1, SpecialStates::VULNERABLE};
	EnemyPattern laserState2 = {"ATTACK LASER", EnemyBehavior::IDLE, {}, 0, 2000.f, 2000.f, {duration, playerCloseHalfHP, twenty_five_HP}, 4, true, 0.f, 2000.f, laser2, SpecialStates::VULNERABLE};

	EnemyPattern spawningLaserSniper = {"SPAWNING", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {duration}, 4, true, 0.f, 2000.f, spawningLaserSniper1, };

	EnemyPattern randomStateHalfHP = {
		"RANDOM POSITION", EnemyBehavior::IDLE, {}, 0, 5000.f, 5000.f, {duration, playerCloseHalfHP, twenty_five_HP}, 5, true,
		0.f, 1500.f, spawningState2, SpecialStates::PROTECTED
	};

	EnemyPattern spawningState = {"SPAWNING", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {duration}, 11, true, 0.f, 1000.f, spawningState3, SpecialStates::PROTECTED};

	EnemyPattern PlayerCloseTwentyFiveHP = {
		"PlayerClose", EnemyBehavior::IDLE, {}, 0, 3000.f, 3000.f, {duration}, 11, false, 0.f, 100.f, laserRotate,
	};

	EnemyPattern PlayerCloseTwentyFiveHPLaser = {
		"PlayerClose", EnemyBehavior::IDLE, {}, 0, 10000.f, 10000.f, {duration}, 12, true, 0.f, 100.f, laserRotate,
		SpecialStates::INVINCIBLE
	};

	EnemyPattern randomStateTwentyFiveHP = {
		"RANDOM POSITION", EnemyBehavior::IDLE, {}, 0, 5000.f, 5000.f, {duration, playerCloseTwentyFiveHP}, 13, true,
		0.f, 1500.f, spawningState4, SpecialStates::PROTECTED
	};

	EnemyPattern laserState1TwentyFiveHP = {"ATTACK LASER", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {duration, playerCloseTwentyFiveHP}, 14, true, 0.f, 2000.f, laser1, };
	EnemyPattern laserState2TwentyFiveHP = {"ATTACK LASER", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {duration, playerCloseTwentyFiveHP}, 15, true, 0.f, 2000.f, laser2, };
	EnemyPattern laserState3TwentyFiveHP = {"ATTACK LASER", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {duration, playerCloseTwentyFiveHP}, 16, true, 0.f, 2000.f, laser3, };
	EnemyPattern laserState4TwentyFiveHP = {"ATTACK LASER", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {duration, playerCloseTwentyFiveHP}, 12, true, 0.f, 2000.f, laser4, };

	BossBigCCore()
	{
		maxHealth = 4000;
		currHealth = maxHealth;
		enemyPatterns = {
			idleState, PlayerClose, PlayerCloseHalfHP, PlayerCloseHalfHPLaser, randomStateSevenFiveHP, laserState1,
			laserState2, spawningLaserSniper, randomStateHalfHP, spawningState,
			PlayerCloseTwentyFiveHP, PlayerCloseTwentyFiveHPLaser, randomStateTwentyFiveHP, laserState1TwentyFiveHP, laserState2TwentyFiveHP, laserState3TwentyFiveHP, laserState4TwentyFiveHP,
		};
		patternIndex = 0;
		sprite = {
			"BigCBoss.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		scale = vec2({180.0f , 180.f });
		rotatePower = 0.8f;
		rotationBehaviour = EnemyRotationBehavior::NONE,
		speedMultiplier = 4.5f;
		armour = 2;
	};
};

struct Magnet : Enemy
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
	EnemyPattern followState = {"FOLLOW ENEMY", EnemyBehavior::FOLLOW_PLAYER, {}, 0, 10000.f, 10000.f, {lowHealth, durationFollow}, 1, false, 0.f, 0.f, NoAttack};
	Magnet()
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
		scale = vec2({144.f, 120.f});
		rotatePower = 0.8f;
	};
};

struct OneBee : Enemy
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
	EnemyPattern randomBee = {"RANDOM", EnemyBehavior::RANDOM_NEAR, {}, 0, 1000.f, 1000.f, {reactionBeeClose, reactionIdle}, 0, false, 0, 2000.f, NoAttack};
	EnemyPattern mergeBee = {"MERGE BEE", EnemyBehavior::MERGE_BEE, {}, 0, 0.f, 0.f, {reactionBeeClose, reactionNoBees}, 0, false, 0.f, 5000.f, NoAttack};
	OneBee()
	{
		maxHealth = 20;
		currHealth = maxHealth;
		enemyPatterns = {idleBee, randomBee, mergeBee};
		patternIndex = 0;
		sprite = {
			"bee_fly_1",
			EFFECT_ASSET_ID::ANIMATE,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0),
			AnimationTypes::REGULAR,
			5};
		scale = vec2({864 / 8.f, 480 / 8.f});
		rotatePower = 1.f;
		speedMultiplier = 3.0f;
		collisionBullet = {playerSpeedUp};
	};
};

struct TwoBee : Enemy
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

	TwoBee()
	{
		maxHealth = 60;
		currHealth = maxHealth;
		enemyPatterns = {idleBee, randomBee, mergeBee};
		patternIndex = 0;
		sprite = {
			"bee_fly_2",
			EFFECT_ASSET_ID::ANIMATE,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0),
			AnimationTypes::REGULAR,
			5};
		scale = vec2({864 / 8.f, 480 / 8.f});
		rotatePower = 1.f;
		collisionBullet = {playerSpeedUp};
	};
};

struct ThreeBee : Enemy
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
		{numBulletsUp, dmgDown},
		playerSpeedDown,
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
	ThreeBee()
	{
		maxHealth = 100;
		currHealth = maxHealth;
		enemyPatterns = {idleBee, randomBee};
		patternIndex = 0;
		sprite = {
			"bee_fly_3",
			EFFECT_ASSET_ID::ANIMATE,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0),
			AnimationTypes::REGULAR,
			8};
		scale = vec2({864 / 8.f, 720 / 8.f});
		rotatePower = 1.f;
		collisionBullet = {playerSpeedUp};
	};
};

struct BeeHive : Enemy
{

	const AttackData OneBee{
		EnemyAttackPattern::SPAWNING,
		TRIANGLE,
		{},
		blunt,
		1,
		0,
		{20, 20},
		100,
		3000,
		{600, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::EnemyOneBee};

	const AttackData ManyBoidBees{
		EnemyAttackPattern::SPAWNING,
		TRIANGLE,
		{},
		blunt,
		12,
		0,
		{20, 20},
		100,
		3000,
		{600, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::EnemyBioBoid};

	Reaction Spawning{
		ReactionType::DURATION,
		1};
	Reaction idling{
		ReactionType::DURATION,

		0};
	Reaction halfHP{
		ReactionType::TWENTYFIVE_HEALTH,
		2,
		};
	Reaction finalIdle{

		ReactionType::DURATION,
		3};
	EnemyPattern idlingState = {"IDLING", EnemyBehavior::IDLE, {}, 0, 1500.f, 1500.f, {Spawning, halfHP}, 1, false, 0.f, 0.f, NoAttack};
	EnemyPattern spawningState = {"SPAWNING", EnemyBehavior::IDLE, {}, 0, 3000.f, 3000.f, {idling, halfHP}, 0, true, 0.f, 3000.f, OneBee};
	EnemyPattern halfHPState = {"SPAWN LOT BEES", EnemyBehavior::IDLE, {}, 0, 3000.f, 3000.f, {finalIdle}, 3, true, 0.f, 3000.f, ManyBoidBees};
	EnemyPattern deadHiveState = {"DEAD HIVE", EnemyBehavior::IDLE, {}, 0, 10000.f, 10000.f, {finalIdle}, 3, false, 0.f, 0.f, NoAttack};
	BeeHive()
	{
		maxHealth = 200;
		currHealth = maxHealth;
		enemyPatterns = {idlingState, spawningState, halfHPState, deadHiveState};
		sprite = {
			"bee_hive",
			EFFECT_ASSET_ID::ANIMATE,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0),
			AnimationTypes::ONCE,
			2,
			800};
		patternIndex = 0;
		scale = vec2({240.0f / 1.7, 312.f / 1.7});
		rotatePower = 0.f;
	};
};

struct BossBeeHive : Enemy
{

	const AttackData OneBee{
		EnemyAttackPattern::SPAWNING,
		TRIANGLE,
		{},
		blunt,
		1,
		0,
		{20, 20},
		100,
		3000,
		{600, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::EnemyOneBee};

	const AttackData twoBee{
		EnemyAttackPattern::SPAWNING,
		TRIANGLE,
		{},
		blunt,
		4,
		0,
		{20, 20},
		100,
		3000,
		{600, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::EnemyOneBee};

	const AttackData threeBoidBees{
		EnemyAttackPattern::SPAWNING,
		TRIANGLE,
		{},
		blunt,
		3,
		0,
		{20, 20},
		100,
		3000,
		{600, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::EnemyBioBoid};

	const AttackData sixBoidBees{
		EnemyAttackPattern::SPAWNING,
		TRIANGLE,
		{},
		blunt,
		6,
		0,
		{20, 20},
		100,
		3000,
		{600, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::EnemyBioBoid};

	const AttackData twelveBoidBees{
		EnemyAttackPattern::SPAWNING,
		TRIANGLE,
		{},
		blunt,
		12,
		0,
		{20, 20},
		100,
		3000,
		{600, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::EnemyBioBoid};

	const AttackData radialHexagon{
		EnemyAttackPattern::RADIAL_POLYGON,
		CIRCLE,
		{},
		blunt,
		6,
		0,
		{40, 40},
		400,
		3000,
		{0, 0},
		0,
		0,
		0};

	const AttackData radialBeehiveBurst1{
		EnemyAttackPattern::BURST_RADIAL,
		TRIANGLE,
		{numBulletsUp, dmgDown},
		playerSpeedUp,
		128,
		M_PI / 20,
		{20, 20},
		150,
		9000,
		{4, 200},
		0,
		0,
		0};

	const AttackData radialBeehiveBurst2{
		EnemyAttackPattern::BURST_RADIAL,
		TRIANGLE,
		{numBulletsUp, dmgDown},
		playerSpeedDown,
		192,
		-M_PI / 20,
		{20, 20},
		150,
		9000,
		{5, 200},
		0,
		0,
		0};

	Reaction duration{
		ReactionType::DURATION,
		0};
	Reaction halfHP{
		ReactionType::FIFTY_HEALTH,
		5,
		SpecialStates::INVINCIBLE
	};
	Reaction threeQuartersHP{
		ReactionType::SEVENTYFIVE_HEALTH,
		2};
	Reaction quarterHP{
		ReactionType::TWENTYFIVE_HEALTH,
		8};
	EnemyPattern startingState1 = {"IDLING", EnemyBehavior::IDLE, {}, 0, 1500.f, 1500.f, {duration, threeQuartersHP}, 1, true, 0.f, 1000.f, radialHexagon};
	EnemyPattern startingState2 = {"IDLING", EnemyBehavior::IDLE, {}, 0, 1500.f, 1500.f, {duration, threeQuartersHP}, 0, true, 0.f, 1000.f, threeBoidBees};
	EnemyPattern halfHpState1 = {"SHOOT RadialBurst", EnemyBehavior::IDLE, {}, 0, 25600.f, 25600.f, {duration, halfHP}, 3, true, 0.f, 100.f, radialBeehiveBurst1};
	EnemyPattern halfHpState2 = {"SPAWN 1Bee", EnemyBehavior::IDLE, {}, 0, 3000.f, 3000.f, {duration, halfHP}, 4, true, 0.f, 3000.f, OneBee};
	EnemyPattern halfHpState3 = {"SPAWN 6Boids", EnemyBehavior::IDLE, {}, 0, 7000.f, 7000.f, {duration, halfHP}, 2, true, 0.f, 7000.f, sixBoidBees};
	EnemyPattern quarterHpState1 = {"SHOOT RadialBurst", EnemyBehavior::IDLE, {}, 0, 25600.f * 1.5f, 25600.f * 1.5f, {duration, quarterHP}, 6, true, 0.f, 100.f, radialBeehiveBurst2};
	EnemyPattern quarterHpState2 = {"SPAWN 12Boids", EnemyBehavior::IDLE, {}, 0, 3000.f, 3000.f, {duration, quarterHP}, 7, true, 0.f, 3000.f, sixBoidBees};
	EnemyPattern quarterHpState3 = {"SPAWN 2Bee", EnemyBehavior::IDLE, {}, 0, 3000.f, 3000.f, {duration, quarterHP}, 5, true, 0.f, 1000.f, twoBee};
	EnemyPattern endingState = {"25% HP HIVE", EnemyBehavior::IDLE, {}, 0, 10000.f, 10000.f, {}, 8, true, 0.f, 5000.f, twelveBoidBees};
	BossBeeHive()
	{
		maxHealth = 1200;
		currHealth = maxHealth;
		enemyPatterns = {startingState1, startingState2, halfHpState1, halfHpState2, halfHpState3, quarterHpState1, quarterHpState2, quarterHpState3, endingState};
		sprite = {
			"bee_hive_boss",
			EFFECT_ASSET_ID::ANIMATE,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0),
			AnimationTypes::ONCE,
			2,
			800};
		patternIndex = 0;
		scale = vec2(384, 480);
		armour = 2;
	};
};

struct BeehiveGun : Enemy
{

	const AttackData beehivesentry{
		EnemyAttackPattern::BURST,
		TRIANGLE,
		{bulletPierceUp, bulletSpeedUp},
		blunt,
		3,
		0.0,
		{30, 30},
		400,
		3000,
		{400, 0.0},
		0,
		0,
		0};

	EnemyPattern rotateState = {"FACE PLAYER", EnemyBehavior::IDLE, {}, 0, 10000.f, 10000.f, {}, 0, true, 0.f, 2000.f, beehivesentry};

	BeehiveGun()
	{
		maxHealth = 500;
		currHealth = maxHealth;
		rotationBehaviour = EnemyRotationBehavior::FACE_PLAYER;

		enemyPatterns = {rotateState};
		speedMultiplier = 0.f;
		rotatePower = 4.5f;

		patternIndex = 0;
		sprite = {
			"bossBeehiveGun.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0)};
		scale = vec2({200.0f, 200.f});
	};
};

struct Skull : Enemy
{
	Reaction AttackLaser{
		ReactionType::DURATION,
		0};
	EnemyPattern laserState = {"ATTACK LASER", EnemyBehavior::IDLE, {}, 0, 5000.f, 5000.f, {AttackLaser}, 0, true, 0.f, 10000.f, laserRotate};
	Skull()
	{
		maxHealth = 100;
		currHealth = maxHealth;
		enemyPatterns = {laserState};
		sprite = {
			"skull",
			EFFECT_ASSET_ID::ANIMATE,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0),
			AnimationTypes::REGULAR,
			4,
			500};
		patternIndex = 0;
		scale = vec2(168.0f, 216.f) / 1.5f;
		rotatePower = 0.f;
	};
};

struct EvilSkull : Enemy
{
	const AttackData laser2{
		EnemyAttackPattern::LASER,
		CIRCLE,
		{dashUp},
		dashRechargeUp,
		8,
		0,
		{20, 20},
		0,
		1500,
		{500, 0},
		0,
		0,
		0};

	const AttackData laser1{
		EnemyAttackPattern::LASER,
		CIRCLE,
		{dashUp},
		dashRechargeUp,
		8,
		M_PI / 8.f,
		{20, 20},
		0,
		1500,
		{500, 0},
		0,
		0,
		0};

	Reaction AttackLaser{
		ReactionType::DURATION,
		0};

	EnemyPattern laserState1 = {"ATTACK LASER", EnemyBehavior::IDLE, {}, 0, 2000.f, 2000.f, {AttackLaser}, 1, true, 0.f, 2000.f, laser1};
	EnemyPattern laserState2 = {"ATTACK LASER", EnemyBehavior::IDLE, {}, 0, 2000.f, 2000.f, {AttackLaser}, 2, true, 0.f, 2000.f, laser2};
	EnemyPattern idleState = {"Idle", EnemyBehavior::IDLE, {}, 0, 2000.f, 2000.f, {AttackLaser}, 0, false, 0.f, 10000.f, laserRotate};
	EvilSkull()
	{
		maxHealth = 150;
		currHealth = maxHealth;
		enemyPatterns = {laserState1, laserState2, idleState};
		sprite = {
			"skull_evil",
			EFFECT_ASSET_ID::ANIMATE,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0),
			AnimationTypes::REGULAR,
			4,
			500};
		patternIndex = 0;
		scale = vec2(168.0f, 216.f) / 1.5f;
		rotatePower = 0.f;
	};
};

struct HifiBoid : Enemy
{
	Reaction normalboid{
		ReactionType::DURATION,
		0};

	Reaction singularityTransition1{
		ReactionType::DURATION,
		1};

	Reaction supernovaTransition1{
		ReactionType::DURATION,
		2};
	Reaction singularityTransition2{
		ReactionType::DURATION,
		3};

	Reaction supernovaTransition2{
		ReactionType::DURATION,
		4};
	EnemyPattern boidState = {"BOID", EnemyBehavior::BOIDS, {}, 0, 20000.f, 20000.f, {singularityTransition1}, 1, false, 0.f, 0.f, NoAttack};
	EnemyPattern singularityState1 = {"GROUPING", EnemyBehavior::BOIDSGROUP, {}, 0, 1000.f, 1000.f, {supernovaTransition1}, 2, false, 0.f, 0.f, NoAttack};
	EnemyPattern supernovaState1 = {"EXPLODE", EnemyBehavior::BOIDSEXPLODE, {}, 0, 3000.f, 3000.f, {singularityTransition2}, 3, false, 0.f, 0.f, NoAttack};
	EnemyPattern singularityState2 = {"GROUPING", EnemyBehavior::BOIDSGROUP, {}, 0, 1000.f, 1000.f, {supernovaTransition2}, 4, false, 0.f, 0.f, NoAttack};
	EnemyPattern supernovaState2 = {"EXPLODE", EnemyBehavior::BOIDSEXPLODE, {}, 0, 5000.f, 5000.f, {normalboid}, 0, false, 0.f, 0.f, NoAttack};
	HifiBoid()
	{
		maxHealth = 1;
		currHealth = maxHealth;
		enemyPatterns = {boidState, singularityState1, supernovaState1, singularityState2, supernovaState2};
		sprite = {
			"hifi_boid.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		scale = vec2({20.f, 20.f});
		patternIndex = 0;
		rotatePower = 1.f;
	}
};

struct HifiTemporaryBoid : Enemy
{
	Reaction singularityTransition1{
		ReactionType::DURATION,
		1
	};
	EnemyPattern boidState = { "BOID", EnemyBehavior::BOIDS, {}, 0, 20000.f, 20000.f, {singularityTransition1}, 1, false, 0.f, 0.f, NoAttack };
	EnemyPattern die = { "DIE", EnemyBehavior::DEATHSTATE, {}, 0, 1000.f, 1000.f, {{ReactionType::DURATION,1}}, 1, false, 0.f, 0.f, NoAttack };;
	HifiTemporaryBoid()
	{
		maxHealth = 1;
		currHealth = maxHealth;
		enemyPatterns = {boidState, die};
		sprite = {
			"enemy_tempBoid.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		scale = vec2({20.f, 20.f});
		patternIndex = 0;
		rotatePower = 1.f;
	}
};

struct Sword : Enemy
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
	EnemyPattern randomPos = {"RANDOM", EnemyBehavior::RANDOM, {}, 0, 2000.f, 2000.f, {noticePlayer}, 0, true, 0.f, 1000.f, NoAttack};
	EnemyPattern idleState = {"IDLE", EnemyBehavior::IDLE, {}, 0, 3000.f, 3000.f, {reactionToRandom}, 3, false, 0.f, 0.f, NoAttack};
	EnemyPattern randomPosNoCharge = {"RANDOM", EnemyBehavior::RANDOM, {}, 0, 1000.f, 1000.f, {reactionToRandom2}, 0, true, 0.f, 1000.f, NoAttack};

	Sword()
	{
		maxHealth = 200;
		currHealth = maxHealth;
		enemyPatterns = {randomPos, chargingState, idleState, randomPosNoCharge};
		sprite = {
			"enemy_Sword.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		scale = vec2({864 / 8.f, 480 / 8.f}) * 1.5f;
		patternIndex = 0;
		rotatePower = 2.0f;
		rotationBehaviour = EnemyRotationBehavior::FACE_PLAYER;
	}
};

struct Mage : Enemy
{

	const AttackData iceWall{
		EnemyAttackPattern::SHOTGUN,
		RECTANGLE,
		{},
		bulletPierceUp,
		4,
		M_PI / 2.f,
		{20, 400},
		200,
		10000,
		{0, 0},
		0,
		-100,
		0
	};

	const AttackData magicMissile{
		EnemyAttackPattern::RADIAL,
		TRIANGLE,
		{dmgUp, bulletRangeDown},
		bulletPierceUp,
		10,
		0,
		{20, 40},
		600,
		1500,
		{100, 0},
		0,
		0,
		0.02
	};

	const AttackData fireball{
		EnemyAttackPattern::SHOTGUN,
		CIRCLE,
		{accuracyUp},
		accuracyDown,
		1,
		0,
		{80, 80},
		300,
		4000,
		{0, 0},
		0,
		0,
		0.07,
		EnemyBulletDeath::EXPLODE 
	};

	const AttackData radialSquare{
	EnemyAttackPattern::RADIAL_POLYGON,
	RECTANGLE,
	{fireRateUp},
	bulletSpeedUp,
	4,
	0,
	{30, 30},
	400,
	2000,
	{0, 0},
	0,
	0,
	0 
	};

	const AttackData radialTriangle{
	EnemyAttackPattern::RADIAL_POLYGON,
	TRIANGLE,
	{fireRateUp},
	bulletSpeedUp,
	3,
	0,
	{30, 30},
	400,
	2000,
	{0, 0},
	0,
	0,
	0 
	};

	const Reaction PlayerBullet = {
		ReactionType::PLAYER_BULLET_CLOSE,
		3,
	};

	const Reaction HalfHP = {
		ReactionType::FIFTY_HEALTH,
		3,
	};

	const Reaction PlayerClose = {
		ReactionType::PLAYER_CLOSE,
		1
	};

	const Reaction PlayerClose2 = {
		ReactionType::PLAYER_CLOSE,
		4
	};


	EnemyPattern IdleState = { "Follow Player", EnemyBehavior::RANDOM_NEAR, {}, 0, 4000.f, 4000.f, {{ReactionType::DURATION,  2}, PlayerBullet}, 2, true, 0.f, 2500.f, magicMissile };

	EnemyPattern teleport = { "Follow Player", EnemyBehavior::TELEPORT, {}, 0, 500.f, 500.f, {{ReactionType::DURATION, 0}, HalfHP}, 0, true, 0.f, 500.f, radialTriangle};
	
	EnemyPattern shootingState = { "ROTATE", EnemyBehavior::RANDOM, {}, 0, 2000.f, 2000.f, {{ReactionType::DURATION, 0}, PlayerClose, PlayerBullet}, 0, true, 0.f, 1000.f, iceWall};

	EnemyPattern RetreatAndShoot = {"RetreatAndShoot", EnemyBehavior::RANDOM_FAR, {}, 0, 3000.f, 3000.f, {{ReactionType::DURATION, 4}, PlayerClose2}, 4, true, 0.f, 3000.f,  fireball};

	EnemyPattern teleport2 = { "Follow Player", EnemyBehavior::TELEPORT, {}, 0, 500.f, 500.f, {{ReactionType::DURATION, 0}, HalfHP}, 0, true, 0.f, 500.f, radialSquare};

	Mage()
	{
		maxHealth = 150;
		currHealth = maxHealth;
		enemyPatterns = {IdleState, teleport, shootingState,  RetreatAndShoot, teleport2};
		sprite = {
			"mage",
			EFFECT_ASSET_ID::ANIMATE,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0),
			AnimationTypes::REGULAR,
			4,
			200};
		patternIndex = 0;
		scale = vec2(216.f, 264.f) * 0.4f;
		rotatePower = 0.f;
		speedMultiplier = 2.f;

	};
};

struct HealerAngel : Enemy
{

	Reaction teamHurt{
		ReactionType::TEAM_HURT,
		1};
	Reaction reactionToIdle{
		ReactionType::DURATION,
		0};
	EnemyPattern idleState = {"IDLE", EnemyBehavior::IDLE, {}, 0, 2000.f, 2000.f, {teamHurt}, 0, false, 0.f, 0.f, NoAttack};
	EnemyPattern healState = {"HEAL", EnemyBehavior::HEALING, {}, 0, 10000.f, 10000.f, {reactionToIdle, teamHurt}, 0, false, 0.f, 0.f, NoAttack};
	HealerAngel()
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

struct BioBoid : Enemy
{

	Reaction boid{
		ReactionType::DURATION,
		0};
	EnemyPattern boidState = {"BOID", EnemyBehavior::BOIDSWARMPLAYER, {}, 0, 5000.f, 5000.f, {}, 0, false, 0.f, 0.f, NoAttack};
	BioBoid()
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
		collisionBullet = {playerSpeedUp};
	}
};

struct FishBoid : Enemy
{

	Reaction boid{
		ReactionType::DURATION,
		0};
	EnemyPattern boidState = {"BOID", EnemyBehavior::BOIDSFISH, {}, 0, 5000.f, 5000.f, {}, 0, false, 0.f, 0.f, NoAttack};
	FishBoid()
	{
		maxHealth = 1;
		currHealth = maxHealth;
		enemyPatterns = {boidState};
		sprite = {
			"hifi_boid.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		scale = vec2({20.f, 20.f});
		patternIndex = 0;
	}
};

struct EnemyTestPatrol : Enemy
{

	EnemyPattern boidState = {"BOID", EnemyBehavior::PATROLLING, {{0.f, 0.f}, {0.5f, 0.5f}, {1.f, 0.f}}, 0, 5000.f, 5000.f, {}, 0, false, 0.f, 0.f, NoAttack};
	EnemyTestPatrol()
	{
		maxHealth = 100;
		currHealth = maxHealth;
		enemyPatterns = {boidState};
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



//----------------------------------------- MEDICAL REGION ENEMIES ---------------------------------
struct MedBoid : Enemy
{
	Reaction boid1{
		ReactionType::DURATION,
		1};
	Reaction boid0{
		ReactionType::DURATION,
		0 };

	EnemyPattern boidState1 = {"BOID", EnemyBehavior::BOIDSWARMPLAYER, {}, 0, 5000.f, 5000.f, {boid1}, 1, false, 0.f, 0.f, NoAttack, SpecialStates::CLOAKED};
	EnemyPattern boidState2 = { "BOID", EnemyBehavior::BOIDSFISH, {}, 0, 5000.f, 5000.f, {boid0}, 0, false, 0.f, 0.f, NoAttack, SpecialStates::CLOAKED };

	MedBoid()
	{
		maxHealth = 9;
		currHealth = maxHealth;
		enemyPatterns = { boidState1, boidState2 };
		sprite = {
			"SyringeBoid.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		scale = vec2({72.f / 1.25, 48.f / 1.25});
		patternIndex = 0;
		collisionBullet = {bulletPierceDown};
	}
};

struct PillBoid : Enemy
{
	Reaction normalboid{
		ReactionType::DURATION,
		0};

	Reaction singularityTransition1{
		ReactionType::DURATION,
		1};

	Reaction supernovaTransition1{
		ReactionType::DURATION,
		2};
	Reaction singularityTransition2{
		ReactionType::DURATION,
		3};

	Reaction supernovaTransition2{
		ReactionType::DURATION,
		4};

	Reaction allyHurt{
		ReactionType::TEAM_HURT,
		1};
	EnemyPattern boidState = {"BOID", EnemyBehavior::BOIDSFISH, {}, 0, 5000.f, 5000.f, {singularityTransition1}, 1, true, 0.f, 250.f, trail};
	EnemyPattern singularityState1 = {"GROUPING", EnemyBehavior::BOIDSGROUP, {}, 0, 1000.f, 1000.f, {supernovaTransition1}, 2, false, 0.f, 0.f, NoAttack};
	EnemyPattern supernovaState1 = {"EXPLODE", EnemyBehavior::BOIDSEXPLODE, {}, 0, 3000.f, 3000.f, {singularityTransition2}, 3, false, 0.f, 0.f, NoAttack};
	EnemyPattern singularityState2 = {"GROUPING", EnemyBehavior::BOIDSGROUP, {}, 0, 1000.f, 1000.f, {supernovaTransition2}, 4, false, 0.f, 0.f, NoAttack};
	EnemyPattern supernovaState2 = {"EXPLODE", EnemyBehavior::BOIDSEXPLODE, {}, 0, 5000.f, 5000.f, {normalboid}, 0, true, 0.f, 200.f, trail};
	// EnemyPattern charge = {"CHARGE", EnemyBehavior::CHARGING, {}, 0, 500.f, 500.f, {singularityTransition1}, 0, true, 0.f, 0.f, NoAttack};
	// EnemyPattern FishBoidState = {"BOID", EnemyBehavior::BOIDSFISH, {}, 0, 5000.f, 5000.f, {}, 0, false, 0.f, 0.f, NoAttack};
	// EnemyPattern giveRegen = {"GIVE REGEN", EnemyBehavior::GRANTINGBUFFSAOE, {}, 0, 500.f, 500.f, {singularityTransition1}, 0, false, 0.f, 0.f, NoAttack};
	PillBoid()
	{
		maxHealth = 1;
		currHealth = maxHealth;
		enemyPatterns = {boidState, singularityState1, supernovaState1, singularityState2, supernovaState2};
		sprite = {
			"PillBoid.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		scale = vec2({48.f, 24.f});
		patternIndex = 0;
		rotatePower = 1.f;
		collisionBullet = {bulletRangeDown, bulletRangeDown};
	}
};

struct PillBoidSpawner : Enemy
{

	const AttackData cannonShot{
		EnemyAttackPattern::BURST,
		CIRCLE,
		{dmgUp, bulletSpeedUp},
		{playerSpeedDown},
		3,
		M_PI / 2,
		{120, 70},
		100,
		10000,
		{400, -M_PI / 4},
		0,
		0,
		0.01,
		EnemyBulletDeath::CLUSTER};
	const AttackData broadsideLasers{
		EnemyAttackPattern::LASER,
		CIRCLE,
		{dmgUp, blunt},
		bulletRangeUp,
		2,
		0,
		{60, 60},
		0,
		4000,
		{5000, 0},
		0,
		0,
		0 };
	const AttackData spawning{
		EnemyAttackPattern::SPAWNING,
		CIRCLE,
		{},
		blunt,
		1,
		0,
		{60, 60},
		600,
		1000,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::EnemyMedicalPillBoid};
	const AttackData iceWall{
		EnemyAttackPattern::SHOTGUN,
		RECTANGLE,
		{},
		bulletRangeUp,
		2,
		M_PI / 2.f,
		{20, 400},
		200,
		10000,
		{0, 0},
		0,
		-100,
		0
	};

	EnemyPattern random1 = {"MOVE", EnemyBehavior::RANDOM, {}, 0, 2000.f, 2000.f, {{ReactionType::DURATION, 1}, {ReactionType::PLAYER_CLOSE, 4}}, 1, false, 0.f, 0.f, NoAttack};
	EnemyPattern chargingState = {"CHARGING", EnemyBehavior::IDLE, {}, 0, 3000.f, 3000.f, {{ReactionType::DURATION, 2}}, 2, false, 0.f, 0.f, NoAttack, SpecialStates::PROTECTED};
	EnemyPattern backUp = {"RECOIL", EnemyBehavior::RECOIL, {}, 0, 100.f, 100.f, {{ReactionType::DURATION, 3}}, 3, false, 0.f, 0.f, NoAttack, SpecialStates::VULNERABLE};
	EnemyPattern shootCannon = {"SHOOT1", EnemyBehavior::ROTATE_IN_PLACE, {}, 0, 4000.f, 4000.f, {{ReactionType::DURATION, 0}}, 4, true, 0.f, 2000.f, broadsideLasers};
	EnemyPattern spawn = {"SHOOT2", EnemyBehavior::IDLE, {}, 0, 2000.f, 2000.f, {{ReactionType::DURATION, 5}}, 5, true, 0.f, 200.f, spawning};
	EnemyPattern rest = {"SHOOT1", EnemyBehavior::IDLE, {}, 0, 800.f, 800.f, {{ReactionType::DURATION, 0}}, 6, false, 0.f, 200.f, NoAttack};
	EnemyPattern laser = {"LASER", EnemyBehavior::IDLE, {}, 0, 0.f, 2000.f, {{ReactionType::DURATION, 0}}, 0, true, 0.f, 2000.f, iceWall};

	PillBoidSpawner()
	{
		maxHealth = 400;
		currHealth = maxHealth;
		enemyPatterns = {random1, chargingState, backUp, shootCannon, spawn, rest, laser};
		patternIndex = 0;
		sprite = {
			"PillCarrier.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		scale = vec2({128 * 1.5, 48 * 1.5});
		rotatePower = 0.6f;
		rotationBehaviour = EnemyRotationBehavior::LASER_CONTROL;
		speedMultiplier = 0.3;
		armour = 3;
		collisionBullet = {bulletRangeDown, bulletRangeDown, blunt};
	};
};


struct Scissors : Enemy
{
	const AttackData explode{
		EnemyAttackPattern::TRAIL,
		CIRCLE,
		{},
		playerSpeedUp,
		1,
		0,
		{20, 20},
		0,
		8000,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::CLUSTER};

	const AttackData Cluster{
		EnemyAttackPattern::TRAIL,
		CIRCLE,
		{},
		{blunt},
		3,
		M_PI / 16,
		{70, 70},
		60,
		0,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::CLUSTER};



	EnemyPattern randomPos = {"RANDOM", EnemyBehavior::RANDOM, {}, 0, 2000.f, 2000.f, {{ReactionType::PLAYER_CLOSE, 1}, {ReactionType::DURATION, 1}}, 1, true, 0.f, 1000.f, NoAttack};
	EnemyPattern chargingState = {"CHARGE", EnemyBehavior::CHARGING, {}, 0, 500.f, 500.f, {{ReactionType::DURATION, 2}}, 2, true, 0.f, 5.f, explode, SpecialStates::VULNERABLE};
	EnemyPattern idleStateCD1 = {"IDLE", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {{ReactionType::DURATION, 3}}, 3, false, 0.f, 0.f, NoAttack, SpecialStates::PROTECTED};
	EnemyPattern chargingMidState = {"CHARGE", EnemyBehavior::CHARGING, {}, 0, 500.f, 500.f, {{ReactionType::DURATION, 2}}, 0, true, 50.f, 50.f, fourAllAround, SpecialStates::VULNERABLE};

	EnemyPattern idleStateCD2 = {"IDLE", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {{ReactionType::DURATION, 5}}, 5, false, 0.f, 0.f, NoAttack, SpecialStates::PROTECTED};
	EnemyPattern chargingEndState = {"CHARGE", EnemyBehavior::CHARGING, {}, 0, 1500.f, 1500.f, {{ReactionType::DURATION, 6}}, 6, true, 0.f, 50.f, fourAllAround, SpecialStates::VULNERABLE};
	EnemyPattern idleState = {"IDLE", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {{ReactionType::DURATION, 7}}, 7, false, 0.f, 0.f, NoAttack, SpecialStates::PROTECTED};
	EnemyPattern explodingCharge = {"CHARGE", EnemyBehavior::CHARGING, {}, 0, 500.f, 500.f, {{ReactionType::DURATION, 8}}, 8, true, 0.f, 5.f, explode, SpecialStates::VULNERABLE};
	EnemyPattern idleState3 = {"IDLE", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {{ReactionType::DURATION, 9}}, 0, false, 0.f, 0.f, NoAttack ,SpecialStates::PROTECTED};
	// EnemyPattern randomPosNoCharge = {"RANDOM", EnemyBehavior::RANDOM, {}, 0, 4000.f, 4000.f, {{ReactionType::DURATION, 0}}, 0, true, 0.f, 1000.f, NoAttack};

	Scissors()
	{
		maxHealth = 120;
		currHealth = maxHealth;
		enemyPatterns = {randomPos, chargingState, idleStateCD1, chargingMidState, idleStateCD2, chargingEndState, idleState, explodingCharge, idleState3};
		sprite = {
			"scissors",
			EFFECT_ASSET_ID::ANIMATE,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0),
			AnimationTypes::REGULAR,
			2,
			300};
		scale = vec2({140, 140});
		patternIndex = 0;
		rotatePower = 1.8f;
		rotationBehaviour = EnemyRotationBehavior::FACE_PLAYER;
		speedMultiplier = 2.6f;
		collisionBullet = {dmgDown};
	}
};

struct BMP : Enemy {
	Reaction AttackLaser{
		ReactionType::DURATION,
		0};
	const AttackData spiral{
		EnemyAttackPattern::RADIAL,
		TRIANGLE,
		{dmgUp, numBulletsDown},
		blunt,
		6,
		0.0,
		{20, 20},
		300,
		5000,
		{200, -2 * M_PI / 2.0},
		0,
		0,
		0};
	EnemyPattern heal_state = {"ATTACK LASER", EnemyBehavior::GRANTINGBUFFSAOE, {}, 0, 15000.f, 15000.f, {AttackLaser, }, 1, false, 0.f, 10000.f, NoAttack, SpecialStates::NORMAL, SpecialStates::INVINCIBLE};
	EnemyPattern telePortState = {"TELEPORT", EnemyBehavior::IDLE, {}, 0, 1500.f, 1500.f, {AttackLaser}, 0, true, 0.f, 700.f, spiral, SpecialStates::VULNERABLE, SpecialStates::NORMAL};
	BMP()
	{
		maxHealth = 300;
		currHealth = maxHealth;
		enemyPatterns = {heal_state, telePortState};
		sprite = {
			"heart_monitor",
			EFFECT_ASSET_ID::ANIMATE,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0),
			AnimationTypes::REGULAR,
			5,
			500};
		patternIndex = 0;
		scale = vec2({216 / 1.5f, 216 / 1.5f});
		rotatePower = 0.f;
		collisionBullet = {dmgDown};
	};

};

struct Syringe : Enemy
{
	/**
	 * Based on hifisniperHard - can regenerate health of other enemies when far from player
	 */
	const AttackData sniperShot{
		EnemyAttackPattern::BURST,
		CIRCLE,
		{sizeUp, dmgUp, bulletRangeDown},
		sizeDown,
		4,
		0,
		{30, 20},
		700,
		10000,
		{100, 0},
		0,
		1,
		0.01,
		EnemyBulletDeath::CLUSTER};
	const AttackData spray{
		EnemyAttackPattern::SHOTGUN,
		CIRCLE,
		{numBulletsUp, sizeUp},
		sizeDown,
		3,
		M_PI / 12,
		{30, 20},
		200,
		2000,
		{400, -M_PI / 2},
		0,
		0,
		0.01,
		EnemyBulletDeath::CLUSTER};

	const AttackData Spawn{
		EnemyAttackPattern::SPAWNING,
		TRIANGLE,
		{},
		blunt,
		1,
		0,
		{20, 20},
		100,
		3000,
		{600, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::EnemyMedicalBoid};

	Reaction playerFar{
        ReactionType::PLAYER_FAR,
        4};

	EnemyPattern random = {"STATIONARY", EnemyBehavior::RANDOM, {}, 0, 1500.f, 1500.f, {{ReactionType::PLAYER_CLOSE, 1}, playerFar,{ReactionType::DURATION, 0}}, 0, true, 0, 3000.f, sniperShot};
	EnemyPattern random2 = {"STATIONARY", EnemyBehavior::CHARGING, {}, 0, 100.f, 100.f, {{ReactionType::DURATION, 2}}, 2, true, 0, 0.f, NoAttack, SpecialStates::VULNERABLE};
	EnemyPattern random3 = {"STATIONARY", EnemyBehavior::RECOIL, {}, 0, 100.f, 100.f, {{ReactionType::DURATION, 3}}, 3, true, 0, 0.f, NoAttack, SpecialStates::VULNERABLE};
	EnemyPattern random4 = {"STATIONARY", EnemyBehavior::IDLE, {}, 0, 2000.f, 2000.f, {{ReactionType::DURATION, 0}}, 0, true, 0, 1000.f, spray};

	EnemyPattern healing = {"HEAL", EnemyBehavior::GRANTINGBUFFSAOE, {}, 0, 2000.f, 2000.f, {{ReactionType::DURATION, 0}, {ReactionType::PLAYER_CLOSE, 1}}, 5, false, 0, 0.f, NoAttack, SpecialStates::VULNERABLE, SpecialStates::REGENERATING};
	EnemyPattern spawn = {"GRANT BUFF", EnemyBehavior::GRANTINGBUFFSAOE, {}, 0, 1000.f, 1000.f, {{ReactionType::DURATION, 0},{ReactionType::PLAYER_CLOSE, 1}}, 6, true, 0, 500.f, Spawn, SpecialStates::VULNERABLE};
	EnemyPattern charge = {"STATIONARY", EnemyBehavior::CHARGING, {}, 0, 100.f, 100.f, {{ReactionType::DURATION, 2}}, 7, true, 0, 0.f, NoAttack, SpecialStates::PROTECTED};
	EnemyPattern recoil = {"STATIONARY", EnemyBehavior::RECOIL, {}, 0, 100.f, 100.f, {{ReactionType::DURATION, 3}}, 8, true, 0, 0.f, NoAttack, SpecialStates::VULNERABLE};
	EnemyPattern farRandom = {"STATIONARY", EnemyBehavior::RANDOM_FAR, {}, 0, 1500.f, 1500.f, {{ReactionType::PLAYER_CLOSE, 1}, {ReactionType::DURATION, 0}}, 4, true, 0, 1500.f, sniperShot, SpecialStates::VULNERABLE};


	Syringe()
	{
		maxHealth = 150;
		currHealth = maxHealth;
		enemyPatterns = {random, random2, random3, random4, healing, spawn,  charge, recoil, farRandom};
		patternIndex = 0;
		sprite = {
			"Syringe.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		scale = vec2({240.f / 1.75f, 72.f / 1.75f});
		rotatePower = 1.5f;
		rotationBehaviour = EnemyRotationBehavior::FACE_PLAYER;
		speedMultiplier = 1.25f;
	};
};



struct RodOfC : Enemy {
	const AttackData whip{
		EnemyAttackPattern::RADIAL,
		RECTANGLE,
		{dmgUp},
		bulletRangeDown,
		8,
		-M_PI / 4.f,
		{15, 15},
		400,
		5000,
		{200, M_PI / 1.5},
		0,
		2,
		0};

	const AttackData Spawn{
		EnemyAttackPattern::SPAWNING,
		TRIANGLE,
		{},
		blunt,
		1,
		0,
		{20, 20},
		100,
		3000,
		{600, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::EnemyMedicalRodA};

	const AttackData halo{
		EnemyAttackPattern::SHOTGUN,
		RECTANGLE,
		{},
		bulletRangeUp,
		12,
		M_PI / 6.f,
		{500, 15},
		250,
		10000,
		{125, 0},
		0,
		0,
		0
	};

	const AttackData magicMissile{
		EnemyAttackPattern::RADIAL,
		TRIANGLE,
		{dmgUp, bulletRangeDown},
		bulletPierceUp,
		5,
		0,
		{20, 40},
		500,
		2000,
		{120, 30},
		0,
		0,
		0.02
	};

	const AttackData fireball{
		EnemyAttackPattern::SHOTGUN,
		CIRCLE,
		{accuracyUp},
		accuracyDown,
		1,
		0,
		{80, 80},
		300,
		4000,
		{0, 0},
		0,
		0,
		0.07,
		EnemyBulletDeath::EXPLODE
	};

	const AttackData spiral{
		EnemyAttackPattern::RADIAL,
		RECTANGLE,
		{dmgUp},
		playerSpeedDown,
		10,
		0.0,
		{20, 20},
		400,
		3000,
		{200, -2 * M_PI / 2.0},
		0,
		0,
		0};

	const AttackData HomingShot{
		EnemyAttackPattern::SHOTGUN,
		TRIANGLE,
		{dmgDown, dmgDown},
		blunt,
		3,
		M_PI / 4.0,
		{20, 20},
		500,
		3000,
		{0.0, 0.0},
		0,
		0,
		0.01};

	Reaction duration{
		ReactionType::DURATION,
		0};

	Reaction lowHP{
		ReactionType::TWENTYFIVE_HEALTH,
		8};

	EnemyPattern heal_state = {"ATTACK LASER", EnemyBehavior::GRANTINGBUFFSAOE, {}, 0, 1000.f, 1000.f, {duration, lowHP}, 1, false, 0.f, 10000.f, NoAttack, SpecialStates::VULNERABLE, SpecialStates::REGENERATING};
	EnemyPattern random_pos = {"TELEPORT", EnemyBehavior::RANDOM, {}, 0, 1500.f, 1500.f, {duration, lowHP}, 2, true, 0.f, 700.f, magicMissile, };
	EnemyPattern coolDown = {"IDLE", EnemyBehavior::RANDOM, {}, 0, 3000.f, 3000.f, {duration, lowHP}, 6, false, 0.f, 0.f, NoAttack, SpecialStates::PROTECTED};
	EnemyPattern whipState = {"WHIP", EnemyBehavior::IDLE, {}, 0, 2500.f, 2500.f, {duration, lowHP}, 7, true, 0.f, 1000.f, halo};
	EnemyPattern coolDown2 = {"IDLE", EnemyBehavior::RANDOM, {}, 0, 3000.f, 3000.f, {duration, lowHP}, 5, false, 0.f, 0.f, NoAttack};
	EnemyPattern protection_pulse = {"ATTACK LASER", EnemyBehavior::GRANTINGBUFFSAOE, {}, 0, 1000.f, 1000.f, {duration, lowHP}, 3, false, 0.f, 10000.f, NoAttack, SpecialStates::VULNERABLE, SpecialStates::INVISIBLE};
	EnemyPattern wall = {"ATTACK LASER", EnemyBehavior::RANDOM_NEAR, {}, 0, 1000.f, 1000.f, {duration, lowHP}, 4, true, 0.f, 200.f, spiral, SpecialStates::VULNERABLE, SpecialStates::NORMAL};
	EnemyPattern coolDown3 = {"IDLE", EnemyBehavior::RANDOM, {}, 0, 4000.f, 4000.f, {duration, lowHP}, 0, false, 100.f, 750.f, HomingShot};
	EnemyPattern Spawning = {"TELEPORT", EnemyBehavior::RANDOM_NEAR, {}, 0, 5000.f, 5000.f, {duration}, 2, true, 0.f, 2500.f, Spawn, SpecialStates::VULNERABLE, SpecialStates::NORMAL};
	RodOfC()
	{
		maxHealth = 800;
		currHealth = maxHealth;
		enemyPatterns = {heal_state, random_pos, coolDown, whipState, coolDown2, protection_pulse, wall, coolDown3, Spawning};
		sprite = {
			"RodOfCaduceus.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		patternIndex = 0;
		scale = vec2(912, 240) / 1.5f;
		rotatePower = 0.f;
		armour = 3;
		collisionBullet = {playerSpeedDown, dmgDown};
	};

};

struct RodOfA : Enemy {
	Reaction AttackLaser{
		ReactionType::DURATION,
		0};
	const AttackData spray{
		EnemyAttackPattern::SHOTGUN,
		TRIANGLE,
		{numBulletsUp, playerSpeedDown},
		blunt,
		2,
		M_PI / 12,
		{30, 10},
		1000,
		750,
		{400, -M_PI / 2},
		0,
		0,
		0};
	EnemyPattern heal_state = {"ATTACK LASER", EnemyBehavior::GRANTINGBUFFSAOE, {}, 0, 1000.f, 1000.f, {AttackLaser, }, 1, false, 0.f, 10000.f, NoAttack, SpecialStates::NORMAL, SpecialStates::REGENERATING};
	EnemyPattern telePortState = {"TELEPORT", EnemyBehavior::IDLE, {}, 0, 1500.f, 1500.f, {AttackLaser}, 2, true, 0.f, 700.f, fourAllAround, SpecialStates::NORMAL, SpecialStates::NORMAL};
	EnemyPattern chargingState = {"CHARGE", EnemyBehavior::CHARGING, {}, 0, 500.f, 500.f, {{ReactionType::DURATION, 2}}, 0, true, 50.f, 250.f, spray, SpecialStates::VULNERABLE};

	RodOfA()
	{
		maxHealth = 200;
		currHealth = maxHealth;
		enemyPatterns = {heal_state, telePortState, chargingState};
		sprite = {
			"RodOfAsclepius.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		patternIndex = 0;
		scale = vec2(768, 288) / 4.f;
		rotatePower = 0.2f;
		rotationBehaviour = EnemyRotationBehavior::FACE_PLAYER;
		armour = 1;
		collisionBullet = {playerSpeedDown, playerSpeedDown};
	};

};

struct ProstheticHand : Enemy
{

	const AttackData bloodTrail{
		EnemyAttackPattern::TRAIL,
		TRIANGLE,
		{dmgUp},
		dmgDown,
		1,
		0,
		{15, 15},
		0,
		4000,
		{0, 0},
		0,
		0,
		0
	};

	Reaction duration0 = {
		ReactionType::DURATION,
		0 
	};


	Reaction duration1 = {
		ReactionType::DURATION,
		1 
	};

	EnemyPattern restingState = { "ROLLING", EnemyBehavior::IDLE, {}, 0, 400.f, 400.f, {duration1}, 1, false, 0.f, 5000.f, quadShot };
	EnemyPattern movingState = { "ROLLING", EnemyBehavior::FOLLOW_PLAYER, {}, 0, 800.f, 800.f, {duration0}, 0, true, 0.f, 50.f, bloodTrail };

	ProstheticHand()
	{
		maxHealth = 300;
		currHealth = maxHealth;

		enemyPatterns = { restingState, movingState };

		patternIndex = 0;
		sprite = {
			"hand_prosthetic",
			EFFECT_ASSET_ID::ANIMATE,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0),
			1,
			3,
			400
		};
		scale = vec2({ 240.0f / 2, 120.f / 2 });
		rotatePower = 10.0;
		speedMultiplier = 2.0f;
	};
};

struct SpineChainedWormHead : Enemy
{
	const AttackData shotgun{
		EnemyAttackPattern::SHOTGUN,
		TRIANGLE,
		{},
		blunt,
		6,
		M_PI / 16,
		{30, 20},
		400,
		10000,
		{0, 0},
		0,
		0,
		0 };

	EnemyPattern startState = { "LOOP", EnemyBehavior::WORM_FOLLOW, {}, 0, 1000000.f, 1000000.f, {}, 0, true, 0.f, 5000.f, shotgun };

	SpineChainedWormHead()
	{
		maxHealth = 700;
		currHealth = maxHealth;

		enemyPatterns = { startState };

		patternIndex = 0;
		sprite = {
			"spine_variant_2.png",
			EFFECT_ASSET_ID::ANIMATE,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0)
		};
		scale = vec2({ 72.0f, 120.f });
		rotationBehaviour = EnemyRotationBehavior::WORM;

		headData.size = 12;
		headData.body = EnemySpineChainedWormBody;
		headData.constrainDistance = 48.f;
		headData.anchor = true;
	};
};

struct SpineChainedWormBody : Enemy
{
	EnemyPattern state = { "IDLE", EnemyBehavior::WORM_BODY, {}, 0, 1000000.f, 1000000.f, {}, 0, false, 0.f, 2500.f, none, SpecialStates::PROTECTED };

	SpineChainedWormBody()
	{
		maxHealth = 300;
		currHealth = maxHealth;

		enemyPatterns = { state };

		patternIndex = 0;
		sprite = {
			"spine_variant_7.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0)
		};
		scale = vec2({ 72.0f, 120.f });
		rotationBehaviour = EnemyRotationBehavior::WORM;
	};
};

struct SpinePatrolWormHead : Enemy
{
	Reaction gotTo = {
		ReactionType::FINISH_PATROL,
		1 };

	Reaction gotTo2 = {
		ReactionType::FINISH_PATROL,
		2 };


	EnemyPattern startState = { "LOOP", EnemyBehavior::WORM_GOTO, { vec2(0.8,0.2) }, 0, 1000000.f, 1000000.f, {gotTo}, 0, false, 0.f, 5000.f, quadShot };
	EnemyPattern loopState = { "LOOP", EnemyBehavior::WORM_PATROL, { vec2(0.8,0.2), vec2(0.9, 0.5), vec2(0.8,0.8), vec2(0.5, 0.9), vec2(0.2, 0.8), vec2(0.1, 0.5), vec2(0.2,0.2), vec2(0.5, 0.1), vec2(0.9,0.2)}, 0, 1000000.f, 1000000.f, {}, 1, false, 0.f, 5000.f, quadShot};


	SpinePatrolWormHead()
	{
		maxHealth = 700;
		currHealth = maxHealth;

		enemyPatterns = { startState, loopState};

		patternIndex = 0;
		sprite = {
			"spine_variant_0.png",
			EFFECT_ASSET_ID::ANIMATE,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0)
		};
		scale = vec2({ 72.0f, 120.f });
		rotationBehaviour = EnemyRotationBehavior::WORM;

		headData.size = 20;
		headData.body = EnemySpinePatrolWormBody;
		headData.constrainDistance = 50.f;
	};
};

struct SpinePatrolWormBody : Enemy
{
	const AttackData dualShot{
	EnemyAttackPattern::RADIAL,
	TRIANGLE,
	{},
	sizeUp,
	2,
	M_PI * 0.5f,
	{20, 20},
	200,
	2000,
	{0.0, 0.0},
	0,
	0,
	0 };

	EnemyPattern state = { "IDLE", EnemyBehavior::WORM_BODY, {}, 0, 1000000.f, 1000000.f, {}, 0, true, 0.f, 2500.f, dualShot, SpecialStates::PROTECTED };

	SpinePatrolWormBody()
	{
		maxHealth = 300;
		currHealth = maxHealth;

		enemyPatterns = { state };

		patternIndex = 0;
		sprite = {
			"spine_variant_0.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0)
		};
		scale = vec2({ 72.0f, 120.f });
		rotationBehaviour = EnemyRotationBehavior::WORM;
	};
};

struct SpineFollowWormHead : Enemy
{
	EnemyPattern startState = { "LOOP", EnemyBehavior::WORM_FOLLOW, {}, 0, 1000000.f, 1000000.f, {}, 0, false, 0.f, 5000.f, quadShot };

	SpineFollowWormHead()
	{
		maxHealth = 300;
		currHealth = maxHealth;

		enemyPatterns = { startState};

		collisionBullet = {playerSpeedUp};

		patternIndex = 0;
		sprite = {
			"spine_variant_8.png",
			EFFECT_ASSET_ID::ANIMATE,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0)
		};
		scale = vec2({ 72.0f, 120.f });
		rotationBehaviour = EnemyRotationBehavior::WORM;

		headData.size = 10;
		headData.body = EnemySpineFollowWormBody;
		headData.constrainDistance = 50.f;
	};
};

struct SpineFollowWormBody : Enemy
{

	EnemyPattern state = { "IDLE", EnemyBehavior::WORM_BODY, {}, 0, 1000000.f, 1000000.f, {}, 0, false, 0.f, 2500.f, none };

	SpineFollowWormBody()
	{
		maxHealth = 200;
		currHealth = maxHealth;

		collisionBullet = {playerSpeedUp};

		enemyPatterns = { state };

		patternIndex = 0;
		sprite = {
			"spine_variant_8.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0)
		};
		scale = vec2({ 72.0f, 120.f });
		rotationBehaviour = EnemyRotationBehavior::WORM;
	};
};


//----------------------------------------- MINING REGION ENEMIES ---------------------------------
struct SmallMole : Enemy
{
	const AttackData dirtBlast{
		EnemyAttackPattern::BURST,
		CIRCLE,
		{dashRechargeUp},
		dashRechargeUp,
		5,
		0,
		{30, 30},
		400,
		6000,
		{0, 0},
		0,
		10,
		0
	};

	Reaction duration0 = {
		ReactionType::DURATION,
		0 };


	Reaction duration1 = {
		ReactionType::DURATION,
		1 };

	EnemyPattern shootingState = { "Shooting", EnemyBehavior::IDLE, {}, 0, 4000.f, 4000.f, {duration1}, 1, true, 0.f, 1000.f, dirtBlast };
	EnemyPattern diggingState = { "Digging", EnemyBehavior::RANDOM_FAR, {}, 0, 1500.f, 1500.f, {duration0}, 0, false, 0.f, 5000.f, quadShot, SpecialStates::UNDERGROUND };

	SmallMole()
	{
		maxHealth = 200;
		currHealth = maxHealth;

		enemyPatterns = { shootingState, diggingState };

		patternIndex = 0;
		sprite = {
			"Mole.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0) };
		scale = vec2({ 264.0f / 2, 288.f / 2 });
		rotatePower = 1.0;
		speedMultiplier = 5.0f;
		rotationBehaviour = EnemyRotationBehavior::NONE;
	};
};

struct MoleBoss : Enemy
{
	const AttackData dirtBlast{
		EnemyAttackPattern::SHOTGUN,
		CIRCLE,
		{dashRechargeUp},
		dashRechargeUp,
		6,
		M_PI / 12.f,
		{30, 30},
		400,
		4000,
		{0, 0},
		0,
		1,
		0
	};

	const AttackData weakDirtBlast{
		EnemyAttackPattern::SHOTGUN,
		CIRCLE,
		{dashRechargeUp},
		dashRechargeUp,
		5,
		M_PI / 12.f,
		{30, 30},
		250,
		1000,
		{0, 0},
		0,
		0,
		0
	};

	const AttackData plantMine{
		EnemyAttackPattern::SPAWNING,
		CIRCLE,
		{},
		blunt,
		1,
		0,
		{60, 60},
		600,
		1000,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::EnemyDynamite };

	const AttackData halo{
		EnemyAttackPattern::SHOTGUN,
		CIRCLE,
		{bulletRangeUp, bulletRangeUp },
		dashRechargeDown,
		12,
		M_PI / 6.f,
		{30, 30},
		120,
		1000,
		{125, 0},
		0,
		0,
		0,
		EnemyBulletDeath::CLUSTER

	};

	const AttackData summonMineField{
		EnemyAttackPattern::SPAWNING,
		CIRCLE,
		{},
		blunt,
		25,
		0,
		{60, 60},
		600,
		1000,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::EnemyDynamite,
		{
			Random::Vec2({1,1}),Random::Vec2({1,1}),Random::Vec2({1,1}),Random::Vec2({1,1}),Random::Vec2({1,1}),
			Random::Vec2({1,1}),Random::Vec2({1,1}),Random::Vec2({1,1}),Random::Vec2({1,1}),Random::Vec2({1,1}),
			Random::Vec2({1,1}),Random::Vec2({1,1}),Random::Vec2({1,1}),Random::Vec2({1,1}),Random::Vec2({1,1}),
			Random::Vec2({1,1}),Random::Vec2({1,1}),Random::Vec2({1,1}),Random::Vec2({1,1}),Random::Vec2({1,1}),

			Random::Vec2({1,1}),Random::Vec2({1,1}),Random::Vec2({1,1}),Random::Vec2({1,1}),Random::Vec2({1,1}),

		}

	};
	const AttackData summonMoles {
		EnemyAttackPattern::SPAWNING,
		CIRCLE,
		{},
		blunt,
		1,
		0,
		{60, 60},
		600,
		1000,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::EnemySmallMole };

	const AttackData summonBoulders {
		EnemyAttackPattern::SPAWNING,
		CIRCLE,
		{},
		blunt,
		8,
		0,
		{60, 60},
		600,
		1000,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::EnemySmallBoulder,
		{{0.25, 0.25}, {0.5, 0.25}, {0.75, 0.25}, {0.25, 0.5}, {0.75, 0.5}, {0.25, 0.75}, {0.5, 0.75}, {0.75, 0.75}}
	};


	Reaction duration = {
		ReactionType::DURATION,
		0 };

	Reaction halfHP = {
		ReactionType::FIFTY_HEALTH,
		9 };

	Reaction repositioned = {
		ReactionType::FINISH_PATROL,
		10 };

	Reaction playerClose = {
		ReactionType::PLAYER_CLOSE,
		5 };


	EnemyPattern diggingState = { "Digging", EnemyBehavior::RANDOM_FAR, {}, 0, 1500.f, 1500.f, {duration, halfHP}, 1, true, 0.f, 300.f, plantMine, SpecialStates::UNDERGROUND };
	EnemyPattern emergeState = { "Emerge", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {duration, halfHP}, 2, true, 200.f, 700.f, halo, SpecialStates::VULNERABLE };

	EnemyPattern shootingState = {
		"Shooting", EnemyBehavior::IDLE, {}, 0, 4000.f, 4000.f, {duration, halfHP}, 0, true, 0.f, 1000.f, dirtBlast
	};
	EnemyPattern mineFieldState = { "MineField", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {duration}, 4, true, 0.f, 1000.f, summonMineField, SpecialStates::UNDERGROUND };
	EnemyPattern diggingStatePhase2 = { "Digging", EnemyBehavior::RANDOM, {}, 0, 3600.f, 3600.f, {duration, playerClose}, 5, true, 0.f, 200.f, plantMine, SpecialStates::UNDERGROUND };

	EnemyPattern emergeStatePhase2 = { "Emerge", EnemyBehavior::IDLE, {}, 0, 3500.f, 3500.f, {duration}, 6, true, 1500.f, 1000.f, halo, SpecialStates::VULNERABLE };
	EnemyPattern shootingStatePhas2 = {
		"Shooting", EnemyBehavior::IDLE, {}, 0, 4000.f, 4000.f, {duration}, 7, true, 0.f, 700.f, dirtBlast, SpecialStates::VULNERABLE
	};

	EnemyPattern chasePlayer = { "CHASE", EnemyBehavior::FOLLOW_PLAYER, {}, 0, 3000.f, 3000.f, {duration}, 8, true, 0.f, 700.f, weakDirtBlast, SpecialStates::UNDERGROUND };
	EnemyPattern spawnMoles = { "Spawn Moles", EnemyBehavior::RANDOM, {}, 0, 1000.f, 1000.f, {duration}, 4, true, 0.f, 500.f, summonMoles, SpecialStates::UNDERGROUND };

	EnemyPattern phase2Reposition = { "RANDOM POSITION", EnemyBehavior::PATROLLING, {{0.5, 0.5}, {0.5, 0.5}}, 0, 1000.f, 1000.f, {repositioned}, 10, true, 0.f, 200.f, plantMine, SpecialStates::UNDERGROUND };
	EnemyPattern spawnBoulders = { "Summon Boulders", EnemyBehavior::RANDOM, {}, 0, 1000.f, 1000.f, {duration}, 3, true, 500.f, 5000.f, summonBoulders, SpecialStates::UNDERGROUND };

	MoleBoss()
	{
		maxHealth = 2000;
		currHealth = maxHealth;

		enemyPatterns = {  diggingState, emergeState,  shootingState, mineFieldState, diggingStatePhase2, emergeStatePhase2, shootingStatePhas2, chasePlayer, spawnMoles, phase2Reposition, spawnBoulders };

		patternIndex = 0;
		sprite = {
			"mole_boss",
			EFFECT_ASSET_ID::ANIMATE,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0),
			1,
			2,
			50
		};
		scale = vec2({ 336.f / 1.75, 408.f / 1.75 });
		rotatePower = 1.0;
		speedMultiplier = 5.0f;
		rotationBehaviour = EnemyRotationBehavior::NONE;
		collisionBullet = {playerSpeedDown, playerSpeedDown, playerSpeedDown, blunt, blunt};
	};
};

struct Bulldozer : Enemy
{

	const AttackData dirtBlast{
		EnemyAttackPattern::SPRAY,
		TRIANGLE,
		{dashRechargeUp},
		dashRechargeUp,
		16,
		1.5f * M_PI,
		{20, 20},
		300,
		1000,
		{0, 0},
		0,
		10,
		0
	};

	EnemyPattern rollingState = { "ROLLING", EnemyBehavior::PATROLLING, {{0.1, 0.1},{0.9, 0.9},{0.1, 0.9},{0.9,0.1}}, 0, 1000000.f, 1000000.f, {}, 0, true, 0.f, 2000.f, dirtBlast };

	Bulldozer()
	{
		maxHealth = 300;
		currHealth = maxHealth;

		enemyPatterns = { rollingState };

		patternIndex = 0;
		sprite = {
			"Bulldozer.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0) };
		scale = vec2({ 168.0f, 96.f });
		speedMultiplier = 2.0f;
		rotationBehaviour = EnemyRotationBehavior::FACE_UP;
	};
};

struct PileDriverTurret : Enemy
{
	const AttackData pileDrive{
		EnemyAttackPattern::SHOTGUN,
		RECTANGLE,
		{},
		blunt,
		2,
		M_PI/30.f,
		{300, 20},
		400,
		12000,
		{0, 0},
		100,
		-100,
		0
	};

	EnemyPattern shootingState = { "Shooting", EnemyBehavior::IDLE, {}, 0, 4000.f, 4000.f, {}, 0, true, 0.f, 1000.f, pileDrive };

	PileDriverTurret()
	{
		maxHealth = 200;
		currHealth = maxHealth;

		enemyPatterns = { shootingState };

		patternIndex = 0;
		sprite = {
			"SwivelPiledriver.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0) };
		scale = vec2({ 168.f, 96.f });
		rotatePower = 1.0;
		rotationBehaviour = EnemyRotationBehavior::FACE_PLAYER;
	};
};

struct SmallBoulder : Enemy
{

	EnemyPattern rollingState = { "ROLLING", EnemyBehavior::ROLLING, {}, 0, 1000000.f, 1000000.f, {}, 0, false, 0.f, 5000.f, quadShot };

	SmallBoulder()
	{
		maxHealth = 100;
		currHealth = maxHealth;

		enemyPatterns = { rollingState };

		patternIndex = 0;
		sprite = {
			"SmallBoulder.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0) };
		scale = vec2({ 144.0f / 2, 144.f / 2 });
		rotatePower = 1.0;
		speedMultiplier = 4.0f;
	};
};

struct Dynamite : Enemy
{
	const AttackData blowup{
		EnemyAttackPattern::RADIAL,
		TRIANGLE,
		{},
		blunt,
		10,
		0.0,
		{20, 20},
		300,
		200,
		{0.0, 0.0},
		0,
		0,
		0,
		EnemyBulletDeath::EXPLODE
	};

	const AttackData bomberManExplosion{
		EnemyAttackPattern::LASER,
		TRIANGLE,
		{},
		bulletRangeUp,
		4,
		0.0,
		{30, 30},
		300,
		400,
		{20, 0.0},
		0,
		0,
		0,
		EnemyBulletDeath::EXPLODE
	};

	Reaction playerClose = {
		ReactionType::PLAYER_REALLY_CLOSE,
		1,
		SpecialStates::INVINCIBLE
	};

	Reaction bulletClose = {
		ReactionType::PLAYER_BULLET_CLOSE,
		2,
		SpecialStates::INVINCIBLE
	};

	Reaction duration = {
		ReactionType::DURATION,
		0 };

	EnemyPattern waitingState = { "ROLLING", EnemyBehavior::IDLE, {}, 0, 1000000.f, 1000000.f, {playerClose, bulletClose}, 0, false, 0.f, 5000.f, quadShot, SpecialStates::CLOAKED };
	EnemyPattern explodingState = { "ROLLING", EnemyBehavior::DEATHSTATE, {}, 0, 1000000.f, 1000000.f, {}, 0, true, 0.f, 5000.f, blowup,  };
	EnemyPattern laserExplode = { "ROLLING", EnemyBehavior::IDLE, {}, 0, 400.f, 400.f, {duration}, 1, true, 0.f, 5000.f, bomberManExplosion };
	Dynamite()
	{
		maxHealth = 40;
		currHealth = maxHealth;

		enemyPatterns = { waitingState, explodingState, laserExplode };

		patternIndex = 0;
		sprite = {
			"dynamite",
			EFFECT_ASSET_ID::ANIMATE,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0),
			1,
			4,
			200
		};
		scale = vec2({ 96.f / 2, 48.f / 2 });
		rotatePower = 0.0;
		speedMultiplier = 0.0f;
	};
};

struct BigBoulder : Enemy
{
	const AttackData boulderTrail{
		EnemyAttackPattern::TRAIL,
		TRIANGLE,
		{},
		playerSpeedUp,
		1,
		0,
		{80, 80},
		0,
		6000,
		{0, 0},
		0,
		10,
		0
	};

	EnemyPattern rollingState = { "ROLLING", EnemyBehavior::ROLLING, {}, 0, 1000000.f, 1000000.f, {}, 0, true, 0.f, 1.f, boulderTrail };

	BigBoulder()
	{
		maxHealth = 100;
		currHealth = maxHealth;

		enemyPatterns = { rollingState };

		patternIndex = 0;
		sprite = {
			"BigBoulder.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0) };
		scale = vec2({ 240.0f / 2, 240.f / 2 });
		rotatePower = 0.5;
		speedMultiplier = 2.0f;
	};
};

struct DrillWormHead : Enemy
{

	const AttackData twoWalls{
	EnemyAttackPattern::TWO_WALL,
	CIRCLE,
	{dashRechargeUp},
	bulletBounceDown,
	4,
	M_PI / 4.f,
	{40, 40},
	150,
	14000.f,
	{0,0},
	-100,
	0,
	0 };

	const AttackData spawning{
		EnemyAttackPattern::SPAWNING,
		CIRCLE,
		{},
		blunt,
		2,
		0,
		{60, 60},
		600,
		1000,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::EnemyMiningBoidWormHead };

	Reaction seventyFiveHp = {
		ReactionType::SEVENTYFIVE_HEALTH,
		1,
		SpecialStates::INVINCIBLE
	};

	Reaction gotTo = {
		ReactionType::FINISH_PATROL,
		2,
		SpecialStates::PROTECTED
	};

	Reaction gotTo2 = {
		ReactionType::DURATION,
		3,
	};

	Reaction halfHp = {
		ReactionType::FIFTY_HEALTH,
		4,
		SpecialStates::INVINCIBLE
	};

	Reaction gotTo3 = {
		ReactionType::DURATION,
		5 };

	Reaction twentyFiveHp = {
		ReactionType::TWENTYFIVE_HEALTH,
		6,
		SpecialStates::INVINCIBLE
	};

	Reaction gotTo4 = {
		ReactionType::FINISH_PATROL,
		7,
		SpecialStates::PROTECTED
	};

	Reaction gotTo5 = {
		ReactionType::DURATION,
		8 };

	EnemyPattern spikyState1 = { "LOOP", EnemyBehavior::WORM_PATROL, { vec2(2,0.0), vec2(-1,0.5), vec2(2,1), vec2(1,2), vec2(0.5,-1), vec2(0.0,2), vec2(-1,1), vec2(2,0.5), vec2(-1,0), vec2(0,-1), vec2(0.5,2), vec2(2,-1) }, 0, 1000000.f, 1000000.f, {seventyFiveHp}, 0, false, 0.f, 5000.f, quadShot };

	float length = 0.7f;
	vec2 offset = vec2(0.5);
	vec2 loopStart = offset + length * vec2(cos(M_PI / 4.f), sin(M_PI / 4.f));
	std::vector<vec2> loopPatrol = { offset + length * vec2(cos(M_PI / 4.f), sin(M_PI / 4.f)), offset + length * vec2(cos(M_PI / 2.f), sin(M_PI / 2.f)), offset + length * vec2(cos(M_PI / 1.f), sin(M_PI / 1.f)), offset + length * vec2(cos(5.f * M_PI / 4.f), sin(5.f * M_PI / 4.f)), offset + length * vec2(cos(3.f * M_PI / 2.f), sin(3.f * M_PI / 2.f)), offset + length * vec2(cos(7.f * M_PI / 4.f), sin(7.f * M_PI / 4.f)), offset + length * vec2(cos(2.f * M_PI / 1.f), sin(2.f * M_PI / 1.f)), offset + length * vec2(cos(M_PI / 4.f), sin(M_PI / 4.f)) };

	// Lasers
	EnemyPattern startState1 = { "LOOP", EnemyBehavior::WORM_GOTO, { loopStart }, 0, 1000000.f, 1000000.f, {gotTo}, 0, false, 0.f, 5000.f, quadShot,  };
	EnemyPattern loopState1 = { "LOOP", EnemyBehavior::WORM_PATROL, loopPatrol, 0, 8000.f, 8000.f, {gotTo2}, 3, true, 0.f, 1000.f, twoWalls,  };
	
	// Boids
	EnemyPattern spikyState2 = { "LOOP", EnemyBehavior::WORM_PATROL, { vec2(2,0.0), vec2(-1,0.5), vec2(2,1), vec2(1,2), vec2(0.5,-1), vec2(0.0,2), vec2(-1,1), vec2(2,0.5), vec2(-1,0), vec2(0,-1), vec2(0.5,2), vec2(2,-1) }, 0, 1000000.f, 1000000.f, {halfHp}, 0, true, 0.f, 12000.f, spawning };

	EnemyPattern followState = { "LOOP", EnemyBehavior::WORM_FOLLOW, {}, 0, 8000.f, 8000.f, {gotTo3}, 5, false, 0.f, 5000.f, quadShot };

	// Boids
	EnemyPattern spikyState3 = { "LOOP", EnemyBehavior::WORM_PATROL, { vec2(2,0.0), vec2(-1,0.5), vec2(2,1), vec2(1,2), vec2(0.5,-1), vec2(0.0,2), vec2(-1,1), vec2(2,0.5), vec2(-1,0), vec2(0,-1), vec2(0.5,2), vec2(2,-1) }, 0, 1000000.f, 1000000.f, {twentyFiveHp}, 0, true, 0.f, 10000.f, spawning };

	// 2 Wall
	EnemyPattern startState2 = { "LOOP", EnemyBehavior::WORM_GOTO, { loopStart }, 0, 1000000.f, 1000000.f, {gotTo4}, 0, false, 0.f, 5000.f, quadShot,  };
	EnemyPattern loopState2 = { "LOOP", EnemyBehavior::WORM_PATROL, loopPatrol, 0, 8000.f, 8000.f, {gotTo5}, 8, true, 0.f, 1000.f, twoWalls,  };

	EnemyPattern spikyState4 = { "LOOP", EnemyBehavior::WORM_PATROL, { vec2(2,0.0), vec2(-1,0.5), vec2(2,1), vec2(1,2), vec2(0.5,-1), vec2(0.0,2), vec2(-1,1), vec2(2,0.5), vec2(-1,0), vec2(0,-1), vec2(0.5,2), vec2(2,-1) }, 0, 1000000.f, 1000000.f, {}, 0, true, 0.f, 8000.f, spawning };

	DrillWormHead()
	{
		maxHealth = 3500;
		currHealth = maxHealth;

		enemyPatterns = { spikyState1, startState1, loopState1, spikyState2, followState, spikyState3, startState2, loopState2, spikyState4 };

		patternIndex = 0;
		sprite = {
			"drill_worm_head",
			EFFECT_ASSET_ID::ANIMATE,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0),
			AnimationTypes::REGULAR,
			2,
			60};
		scale = vec2({ 288.0f / 2, 192.f / 2 });
		rotationBehaviour = EnemyRotationBehavior::LASER_CONTROL;
		speedMultiplier = 3.f;
		collisionBullet = { bulletPierceDown };

		headData.size = 20;
		headData.body = BossDrillWormBody;
		headData.constrainDistance = 126.f;
	};
};

struct DrillWormBody : Enemy
{

	const AttackData dualShot{
	EnemyAttackPattern::RADIAL,
	TRIANGLE,
	{},
	blunt,
	2,
	M_PI * 0.5f,
	{20, 20},
	200,
	1000,
	{0.0, 0.0},
	0,
	0,
	0 };

	const AttackData squareShot{
	EnemyAttackPattern::RADIAL_POLYGON,
	TRIANGLE,
	{},
	blunt,
	4,
	0,
	{20, 20},
	200,
	1000,
	{0.0, 0.0},
	0,
	0,
	0 };

	Reaction gotTo = {
		ReactionType::DURATION,
		1 };

	EnemyPattern state1 = { "IDLE", EnemyBehavior::WORM_BODY, {}, 0, 50000.f, 50000.f, {gotTo}, 1, true, 0.f, 2500.f, dualShot };
	EnemyPattern state2 = { "IDLE", EnemyBehavior::WORM_BODY, {}, 0, 50000.f, 50000.f, {}, 1, true, 0.f, 2500.f, squareShot };

	DrillWormBody()
	{
		maxHealth = 100;
		currHealth = maxHealth;

		enemyPatterns = { state1, state2 };

		patternIndex = 0;
		sprite = {
			"drill_worm_body",
			EFFECT_ASSET_ID::ANIMATE,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0),
			AnimationTypes::REGULAR,
			4,
			50
		};
		scale = vec2({ 288.0f / 2, 192.f / 2 });
		rotationBehaviour = EnemyRotationBehavior::WORM;
	};
};

struct SmallMiningWormHead : Enemy
{

	const AttackData miningLaser{
	EnemyAttackPattern::LASER,
	RECTANGLE,
	{dashUp},
	dashRechargeUp,
	1,
	0,
	{20, 20},
	100,
	500,
	{4, 0},
	0,
	0,
	0 };

	EnemyPattern followState = { "LOOP", EnemyBehavior::WORM_FOLLOW, {}, 0, 1000000.f, 1000000.f, {}, 0, true, 0.f, 200.f, miningLaser };

	SmallMiningWormHead()
	{
		maxHealth = 200;
		currHealth = maxHealth;

		enemyPatterns = { followState };

		patternIndex = 0;
		sprite = {
			"SmallWormHead.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0)
		};
		scale = vec2({ 96.0f / 2, 72.f / 2 });
		rotationBehaviour = EnemyRotationBehavior::LASER_CONTROL;
		speedMultiplier = 0.8f;

		headData.size = 15;
		headData.body = EnemySmallMiningWormBody;
		headData.constrainDistance = 45.f;
	};
};

struct SmallMiningWormBody : Enemy
{

	EnemyPattern state = { "IDLE", EnemyBehavior::WORM_BODY, {}, 0, 1000000.f, 1000000.f, {}, 0, false, 0.f, 2500.f, none };

	SmallMiningWormBody()
	{
		maxHealth = 100;
		currHealth = maxHealth;

		enemyPatterns = { state };

		patternIndex = 0;
		sprite = {
			"SmallWormBody.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0)
		};
		scale = vec2({ 96.0f / 2, 72.f / 2 });
		rotationBehaviour = EnemyRotationBehavior::WORM;
	};
};

struct MiningBoidWormHead : Enemy
{


	EnemyPattern startState = { "LOOP", EnemyBehavior::BOIDSWARMPLAYER, { vec2(0.8,0.2) }, 0, 1000000.f, 1000000.f, {}, 0, false, 0.f, 5000.f, quadShot };

	MiningBoidWormHead()
	{
		maxHealth = 30;
		currHealth = maxHealth;

		enemyPatterns = { startState };

		patternIndex = 0;
		sprite = {
			"MiningBoidWormHead.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0)
		};
		scale = vec2({ 48.0f / 2, 48.f / 2 });
		rotationBehaviour = EnemyRotationBehavior::WORM;

		headData.size = 15;
		headData.body = EnemyMiningBoidWormBody;
		headData.constrainDistance = 24.f;
	};
};

struct MiningBoidWormBody : Enemy
{

	EnemyPattern state = { "IDLE", EnemyBehavior::WORM_BODY, {}, 0, 1000000.f, 1000000.f, {}, 0, false, 0.f, 2500.f, none };

	MiningBoidWormBody()
	{
		maxHealth = 100;
		currHealth = maxHealth;

		enemyPatterns = { state };

		patternIndex = 0;
		sprite = {
			"MiningBoidWormBody.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0)
		};
		scale = vec2({ 48.0f / 2, 48.f / 2 });
		rotationBehaviour = EnemyRotationBehavior::WORM;
	};
};


//----------------------------------------- PHYSICS REGION ENEMIES ---------------------------------

struct ConstructYELLOW : Enemy
{
	const AttackData spawning{
		EnemyAttackPattern::SPAWNING,
		CIRCLE,
		{},
		blunt,
		3,
		0,
		{60, 60},
		600,
		1000,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::EnemyHifiTemporaryBoid };

	EnemyPattern init = {"GiveInvincibility", EnemyBehavior::GRANTINGBUFFSAOE, {{0.5, 0.5}}, 0, 100.f, 100.f, {{ReactionType::DURATION,1}}, 1, false, 0.f, 10.f, NoAttack, SpecialStates::INVINCIBLE, SpecialStates::INVINCIBLE};
	EnemyPattern randomState = { "PatrolSide", EnemyBehavior::RANDOM_FAR, {}, 0, 12000.f, 12000.f, {}, 1, true, 0.f, 6000.f, spawning };
	
	ConstructYELLOW()
	{
		maxHealth = 500;
		currHealth = maxHealth;
		enemyPatterns = { init,
			randomState };
		patternIndex = 0;
		sprite = {
			"HifiBoss2Yellow.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE };
		scale = vec2({ 135.f, 135.f });
		rotatePower = 90.0f;
	};
};
struct ConstructPURPLE : Enemy
{
	const AttackData bigBullet{
		EnemyAttackPattern::SHOTGUN,
		CIRCLE,
		{},
		blunt,
		1,
		0,
		{100, 100},
		400,
		4000,
		{50, 0},
		10,
		10,
		0 
	};
	EnemyPattern init = {"GiveInvincibility", EnemyBehavior::GRANTINGBUFFSAOE, {{0.5, 0.5}}, 0, 100.f, 100.f, {{ReactionType::DURATION,1}}, 1, true, 0.f, 10.f, NoAttack, SpecialStates::INVINCIBLE, SpecialStates::INVINCIBLE};
	EnemyPattern startState = { "PatrolSide", EnemyBehavior::RANDOM_FAR, {}, 0, 8000.f, 8000.f, {{ReactionType::PLAYER_CLOSE, 1}}, 2, true, 0.f, 4000.f, bigBullet };
	EnemyPattern teleState = { "PatrolSide", EnemyBehavior::TELEPORT, {}, 0, 100.f, 100.f, {{ReactionType::DURATION, 2}}, 3, false, 0.f, 1000.f, none };
	EnemyPattern cooldownState = { "PatrolSide", EnemyBehavior::RANDOM_NEAR, {}, 0, 8000.f, 8000.f, {{ReactionType::DURATION, 0}}, 1, true, 0.f, 4000.f, bigBullet };

	ConstructPURPLE()
	{
		maxHealth = 500;
		currHealth = maxHealth;
		enemyPatterns = { init, startState, teleState, cooldownState };
		patternIndex = 0;
		sprite = {
			"HifiBoss2Purple.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE };
		scale = vec2({ 135.f, 135.f });
		rotatePower = 90.0f;
		speedMultiplier = 1.3;
	};
};
struct ConstructGREEN : Enemy
{
	const AttackData crabLaser{
		EnemyAttackPattern::LASER,
		CIRCLE,
		{},
		blunt,
		1,
		0,
		{20, 20},
		0,
		10000000,
		{4, 0},
		0,
		0,
		0 };
	EnemyPattern init = {"GiveInvincibility", EnemyBehavior::GRANTINGBUFFSAOE, {{0.5, 0.5}}, 0, 100.f, 100.f, {{ReactionType::DURATION,1}}, 1, true, 0.f, 10.f, NoAttack, SpecialStates::INVINCIBLE, SpecialStates::INVINCIBLE};
	EnemyPattern startState = { "PatrolBoundary", EnemyBehavior::PATROLLING, {{0.9, 0.1}}, 0, 3000.f, 3000.f, {{ReactionType::FINISH_PATROL, 2}}, 2, false, 0.f, 1000000000.f, none };
	EnemyPattern randomState = { "PatrolBoundary", EnemyBehavior::PATROLLING, {{0.99, 0.01}, {0.99, 0.99}, {0.01, 0.99}, {0.01, 0.01}, {0.99, 0.01}}, 0, 3000.f, 3000.f, {}, 2, true, 0.f, 1000000000.f, crabLaser };
	
	ConstructGREEN()
	{
		maxHealth = 500;
		currHealth = maxHealth;
		enemyPatterns = { init, startState, randomState };
		patternIndex = 0;
		sprite = {
			"HifiBoss2Green.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE };
		scale = vec2({ 135.f, 135.f });
		rotatePower = 1.0f;
		speedMultiplier = 4.F;
		rotationBehaviour = EnemyRotationBehavior::FACE_CENTER;
	};
};
struct ConstructRED : Enemy
{

	const AttackData trail{
		EnemyAttackPattern::TRAIL,
		TRIANGLE,
		{},
		{blunt},
		1,
		0,
		{20, 20},
		0,
		5000,
		{0, 0},
		0,
		0,
		0 
	};

	EnemyPattern init = {"GiveInvincibility", EnemyBehavior::GRANTINGBUFFSAOE, {{0.5, 0.5}}, 0, 100.f, 100.f, {{ReactionType::DURATION,1}}, 1, true, 0.f, 10.f, NoAttack, SpecialStates::INVINCIBLE, SpecialStates::INVINCIBLE};
	EnemyPattern randomState = { "PatrolSide", EnemyBehavior::FOLLOW_PLAYER, {}, 0, 3000.f, 3000.f, {}, 1, true, 0.f, 1000.f, trail };

	ConstructRED()
	{
		maxHealth = 500;
		currHealth = maxHealth;
		enemyPatterns = { init,
			randomState };
		patternIndex = 0;
		sprite = {
			"HifiBoss2Red.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE };
		scale = vec2({ 135.f, 135.f });
		rotatePower = 90.0f;
		speedMultiplier = 1.9;
	};
};

struct MultiCube : Enemy 
{
	// Spawn YELLOW
	Reaction threeQuarterHP = {
		ReactionType::SEVENTYFIVE_HEALTH,
		2,
		SpecialStates::INC_ANIM
	};
	const AttackData spawnYELLOW{
		EnemyAttackPattern::SPAWNING,
		CIRCLE,
		{},
		blunt,
		1,
		0,
		{0, 0},
		0,
		0,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::BossConstructYELLOW 
	};

	// Spawn PURPLE
	Reaction halfHP = {
		ReactionType::FIFTY_HEALTH,
		5,
		SpecialStates::INC_ANIM 
	};
	const AttackData spawnPURPLE{
		EnemyAttackPattern::SPAWNING,
		CIRCLE,
		{},
		blunt,
		1,
		0,
		{0, 0},
		0,
		0,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::BossConstructPURPLE
	};

	// Spawn GREEN and RED
	Reaction quarterHP = {
		ReactionType::TWENTYFIVE_HEALTH,
		8,
		SpecialStates::INC_ANIM
	};
	const AttackData spawnGREEN{
		EnemyAttackPattern::SPAWNING,
		CIRCLE,
		{},
		blunt,
		1,
		0,
		{0, 0},
		0,
		0,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::BossConstructGREEN
	};
	const AttackData spawnRED{
		EnemyAttackPattern::SPAWNING,
		CIRCLE,
		{},
		blunt,
		1,
		0,
		{0, 0},
		0,
		0,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::BossConstructRED
	};

	const AttackData spiral{
		EnemyAttackPattern::RADIAL,
		TRIANGLE,
		{numBulletsUp, dmgDown},
		bulletPierceDown,
		12,
		0.0,
		{40, 40},
		800,
		6000,
		{400, -2 * M_PI / 3.0},
		0,
		1,
		0};

	const AttackData halo{
		EnemyAttackPattern::SHOTGUN,
		RECTANGLE,
		{bulletPierceUp, bulletPierceUp },
		bulletPierceDown,
		12,
		M_PI / 6.f,
		{500, 15},
		250,
		10000,
		{125, 0},
		0,
		0,
		0
	};

	const AttackData oneLaser{
		EnemyAttackPattern::LASER,
		CIRCLE,
		{},
		blunt,
		1,
		0,
		{20, 20},
		0,
		5000,
		{5, 0.03},
		0,
		0,
		0 };
	const AttackData twoLaser{
		EnemyAttackPattern::LASER,
		CIRCLE,
		{},
		blunt,
		2,
		0,
		{20, 20},
		0,
		5000,
		{5, 0.02},
		0,
		0,
		0 };
	const AttackData threeLaser{
		EnemyAttackPattern::LASER,
		CIRCLE,
		{},
		blunt,
		3,
		0,
		{20, 20},
		0,
		5000,
		{5, 0.01},
		0,
		0,
		0 };

	const AttackData blowup{
		EnemyAttackPattern::SPRAY,
		TRIANGLE,
		{},
		blunt,
		10,
		2.f * M_PI,
		{20, 20},
		300,
		200,
		{0.0, 0.0},
		0,
		0,
		0,
		EnemyBulletDeath::EXPLODE
	};

	const AttackData radialSquare{
	EnemyAttackPattern::RADIAL_POLYGON,
	RECTANGLE,
	{},
	blunt,
	4,
	M_PI / 4.f,
	{30, 30},
	400,
	2500,
	{0, 0},
	0,
	0,
	0
	};

	EnemyPattern zeroState1 = { "RookFollow", EnemyBehavior::ROOK_FOLLOW, {}, 0, 8000.f, 8000.f, {threeQuarterHP, {ReactionType::DURATION, 1}}, 1, false, 0.f, 1000.f, none };
	EnemyPattern zeroState2 = { "IDLE1", EnemyBehavior::IDLE, {}, 0, 5000.f, 5000.f, {threeQuarterHP, {ReactionType::DURATION, 0}}, 0, true, 0.f, 5000.f, oneLaser };

	EnemyPattern spawnYellow = { "spwnY", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {{ReactionType::DURATION, 3}}, 3, true, 0.f, 1000.f, spawnYELLOW , SpecialStates::INVINCIBLE};

	EnemyPattern oneState1 = { "center", EnemyBehavior::PATROLLING, {{0.5,0.5}}, 0, 8000.f, 8000.f, {halfHP, {ReactionType::DURATION, 4}}, 4, true, 0.f, 2000.f, radialSquare };
	EnemyPattern oneState2 = { "IDLE2", EnemyBehavior::IDLE, {}, 0, 5000.f, 5000.f, {halfHP, {ReactionType::DURATION, 3}}, 3, true, 0.f, 5000.f, twoLaser };

	EnemyPattern spawnPurple = { "spwnP", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {{ReactionType::DURATION, 6}}, 6, true, 0.f, 1000.f, spawnPURPLE , SpecialStates::INVINCIBLE};

	EnemyPattern twoState1 = { "IDLE3", EnemyBehavior::IDLE, {}, 0, 3000.f, 3000.f, {quarterHP, {ReactionType::DURATION, 7}}, 7, true, 0.f, 1000.f, radialSquare };
	EnemyPattern twoState2 = { "IDLE4", EnemyBehavior::IDLE, {}, 0, 5000.f, 5000.f, {quarterHP, {ReactionType::DURATION, 6}}, 6, true, 0.f, 5000.f, threeLaser };

	EnemyPattern spawnGreen = { "spwnG", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {{ReactionType::DURATION, 9, SpecialStates::INC_ANIM}}, 9, true, 0.f, 1000.f, spawnGREEN , SpecialStates::INVINCIBLE};
	EnemyPattern spawnRed =   { "spwnR", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {{ReactionType::DURATION, 10}}, 10, true, 0.f, 1000.f, spawnRED };

	EnemyPattern PredeathSuper = { "IDLE5", EnemyBehavior::IDLE, {}, 0, 5000.f, 5000.f, {{ReactionType::DURATION, 10}}, 11, true, 0.f, 500.f, spiral, SpecialStates::INVINCIBLE };
	EnemyPattern PredeathSuper2 = { "IDLE5", EnemyBehavior::IDLE, {}, 0, 5000.f, 5000.f, {{ReactionType::DURATION, 9}}, 12, true, 0.f, 1000.f, halo, SpecialStates::INVINCIBLE };

	EnemyPattern finalState1 = { "IDLE5", EnemyBehavior::IDLE, {}, 0, 3000.f, 3000.f, {{ReactionType::DURATION, 11}}, 13, true, 0.f, 500.f, blowup }; //blow up attack
	EnemyPattern finalState2 = { "die", EnemyBehavior::DEATHSTATE, {}, 0, 3000.f, 3000.f, {}, 11, false, 0.f, 1000.f, none };
	
	MultiCube()
	{
		maxHealth = 3000;
		currHealth = maxHealth;
		enemyPatterns = { zeroState1, zeroState2, spawnYellow, oneState1, oneState2, spawnPurple, twoState1, twoState2, spawnGreen, spawnRed, PredeathSuper, PredeathSuper2, finalState1, finalState2 };
		patternIndex = 0;
		sprite = {
			"hifi_boss_2_phases",
			EFFECT_ASSET_ID::ANIMATE,
			GEOMETRY_BUFFER_ID::SPRITE, 
			vec2(0),
			AnimationTypes::NONE,
			4,
			-5
		};
		scale = vec2({ 240.f, 240.f });
		rotatePower = 90.0f;
		speedMultiplier = 2.5f;
		rotationBehaviour = EnemyRotationBehavior::NONE;
	};
};

struct TwinLaserVertical1 : Enemy
{
	const AttackData crabLaser{
		EnemyAttackPattern::TWIN_LASER,
		CIRCLE,
		{},
		blunt,
		1,
		0,
		{20, 20},
		0,
		10000000,
		{100, 0},
		0,
		0,
		0};

	Reaction duration = {
		ReactionType::FINISH_PATROL,
		1};

	EnemyPattern randomState = {"PatrolSide", EnemyBehavior::PATROLLING, {{0.01, 0.01}, {0.01, 0.99}, {0.01, 0.01}}, 0, 3000.f, 3000.f, {duration}, 0, true, 0.f, 1000000000.f, crabLaser};
	TwinLaserVertical1()
	{
		maxHealth = 100;
		currHealth = maxHealth;
		enemyPatterns = {
			randomState};
		patternIndex = 0;
		sprite = {
			"enemy_hifi_004.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE};
		scale = vec2({160.0f / 2, 160.f / 2});
		rotatePower = 90.0f;
		speedMultiplier = 1.3;
		rotationBehaviour = EnemyRotationBehavior::FACE_TWIN;
	};
};

struct TwinLaserVertical2 : TwinLaserVertical1
{
	EnemyPattern randomState = {"PatrolSide", EnemyBehavior::PATROLLING, {{0.99, 0.01}, {0.99, 0.99}, {0.99, 0.01}}, 0, 3000.f, 3000.f, {duration}, 0, true, 0.f, 1000000000.f, crabLaser};
	TwinLaserVertical2() : TwinLaserVertical1()
	{
		enemyPatterns = {randomState};
	}
};

struct TwinLaserHorizontal1 : TwinLaserVertical1
{
	EnemyPattern randomState = {"PatrolSide", EnemyBehavior::PATROLLING, {{0.99, 0.01}, {0.01, 0.01}, {0.99, 0.01}}, 0, 3000.f, 3000.f, {duration}, 0, true, 0.f, 1000000000.f, crabLaser};
	TwinLaserHorizontal1() : TwinLaserVertical1()
	{
		enemyPatterns = {randomState};
	}
};
struct TwinLaserHorizontal2 : TwinLaserHorizontal1
{
	EnemyPattern randomState = {"PatrolSide", EnemyBehavior::PATROLLING, {{0.99, 0.99}, {0.01, 0.99}, {0.99, 0.99}}, 0, 3000.f, 3000.f, {duration}, 0, true, 0.f, 1000000000.f, crabLaser};
	TwinLaserHorizontal2() : TwinLaserHorizontal1()
	{
		enemyPatterns = {randomState};
	}
};

struct HifiLaserSniper : Enemy
{
	EnemyPattern randomState = {"Laser active", EnemyBehavior::IDLE, {{0.99, 0.01}, {0.99, 0.99}, {0.01, 0.99}, {0.01, 0.01}, {0.99, 0.01}}, 0, 0.f, 100.f, {{ReactionType::DURATION, 0}}, 0, true, 0.f, 5000.f, FastLaser};

	HifiLaserSniper()
	{
		maxHealth = 100;
		currHealth = maxHealth;
		enemyPatterns = {randomState};
		patternIndex = 0;
		sprite = {
			"enemy_hifi_007.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE};
		scale = vec2({160.0f / 2, 160.f / 2});
		rotatePower = 0.55f;
		speedMultiplier = 1.3;
		rotationBehaviour = EnemyRotationBehavior::FACE_PLAYER;
	};
};

struct HifiLaserSniperHard : Enemy
{
	const AttackData FastLaser{
		EnemyAttackPattern::LASER
		,CIRCLE
		,{dashUp, playerSpeedUp, playerSpeedUp}
		,dashRechargeUp
		,1
		,0
		,{20, 20}
		,0
		, 2000
		,{5000, 0}
		,0
		,0
		,0
		,EnemyBulletDeath::NONE
		};
	EnemyPattern laser = {"Laser active", EnemyBehavior::IDLE, {}, 0, 0.f, 700.f, {{ReactionType::DURATION, 0}}, 2, true, 0.f, 700.f, FastLaser};
	EnemyPattern Teleport = {"Charge", EnemyBehavior::IDLE, {}, 0, 300.f, 300.f, {{ReactionType::DURATION, 0}}, 0, false, 0.f, 350.f, twelveSpiralShot};
	EnemyPattern CoolDown = {"CoolDown", EnemyBehavior::IDLE, {}, 0, 0.f, 4000.f, {{ReactionType::DURATION, 0}}, 1, false, 0.f, 5000.f, NoAttack};

	HifiLaserSniperHard()
	{
		maxHealth = 150;
		currHealth = maxHealth;
		enemyPatterns = {CoolDown, laser, Teleport};
		patternIndex = 0;
		sprite = {
			"enemy_hifi_007_alt1.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE};
		scale = vec2({160.0f / 2, 160.f / 2});
		rotatePower = 0.7f;
		speedMultiplier = 1.3;
		rotationBehaviour = EnemyRotationBehavior::FACE_PLAYER;
	};
};

struct HifiSniper : Enemy
{
	const AttackData HifiSniperShot{
		EnemyAttackPattern::BURST,
		TRIANGLE,
		{bulletPierceDown, bulletSpeedDown},
		blunt,
		4,
		0,
		{30, 20},
		600,
		10000,
		{100, 0},
		0,
		1,
		0};
	const AttackData spray{
		EnemyAttackPattern::SHOTGUN,
		TRIANGLE,
		{numBulletsUp, playerSpeedDown},
		blunt,
		3,
		M_PI / 12,
		{30, 20},
		200,
		1000,
		{400, -M_PI / 2},
		0,
		0,
		0};

	EnemyPattern random = {"STATIONARY", EnemyBehavior::RANDOM, {}, 0, 3000.f, 3000.f, {{ReactionType::PLAYER_CLOSE, 1}, {ReactionType::DURATION, 0}}, 0, true, 0, 6000.f, HifiSniperShot};
	EnemyPattern random2 = {"STATIONARY", EnemyBehavior::CHARGING, {}, 0, 100.f, 100.f, {{ReactionType::DURATION, 2}}, 2, true, 0, 0.f, NoAttack};
	EnemyPattern random3 = {"STATIONARY", EnemyBehavior::RECOIL, {}, 0, 100.f, 100.f, {{ReactionType::DURATION, 3}}, 3, true, 0, 0.f, NoAttack};
	EnemyPattern random4 = {"STATIONARY", EnemyBehavior::IDLE, {}, 0, 3000.f, 3000.f, {{ReactionType::DURATION, 0}}, 0, true, 0, 3000.f, spray};

	HifiSniper()
	{
		maxHealth = 90;
		currHealth = maxHealth;
		enemyPatterns = {random, random2, random3, random4};
		patternIndex = 0;
		sprite = {
			"enemy_hifi_000.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		scale = vec2({160.f / 2.f, 160.f / 2.f});
		rotatePower = 1.5f;
		rotationBehaviour = EnemyRotationBehavior::FACE_PLAYER;
		speedMultiplier = 0.4;
	};
};

struct HifiSniperHard : Enemy
{
	/**
	 * Lighter Variant of Sniper - more shots, less bullets, more speed
	 */
	const AttackData sniperShot{
		EnemyAttackPattern::BURST,
		TRIANGLE,
		{bulletPierceUp, dmgUp, bulletRangeDown},
		blunt,
		3,
		0,
		{30, 20},
		600,
		10000,
		{100, 0},
		0,
		2,
		0};
	const AttackData spray{
		EnemyAttackPattern::SHOTGUN,
		TRIANGLE,
		{numBulletsUp, accuracyUp},
		blunt,
		3,
		M_PI / 12,
		{30, 20},
		200,
		2000,
		{400, -M_PI / 2},
		0,
		0,
		0};

	EnemyPattern random = {"STATIONARY", EnemyBehavior::RANDOM, {}, 0, 1500.f, 1500.f, {{ReactionType::PLAYER_CLOSE, 1}, {ReactionType::DURATION, 0}}, 0, true, 0, 3000.f, sniperShot};
	EnemyPattern random2 = {"STATIONARY", EnemyBehavior::CHARGING, {}, 0, 100.f, 100.f, {{ReactionType::DURATION, 2}}, 2, true, 0, 0.f, NoAttack};
	EnemyPattern random3 = {"STATIONARY", EnemyBehavior::RECOIL, {}, 0, 100.f, 100.f, {{ReactionType::DURATION, 3}}, 3, true, 0, 0.f, NoAttack};
	EnemyPattern random4 = {"STATIONARY", EnemyBehavior::IDLE, {}, 0, 2000.f, 2000.f, {{ReactionType::DURATION, 0}}, 0, true, 0, 1000.f, spray};

	HifiSniperHard()
	{
		maxHealth = 60;
		currHealth = maxHealth;
		enemyPatterns = {random, random2, random3, random4};
		patternIndex = 0;
		sprite = {
			"enemy_hifi_000_hard.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		scale = vec2({160.f / 2.f, 160.f / 2.f});
		rotatePower = 1.5f;
		rotationBehaviour = EnemyRotationBehavior::FACE_PLAYER;
		speedMultiplier = 1.5;
	};
};

struct HifiCharger : Enemy
{
	EnemyPattern randomPos = {"RANDOM", EnemyBehavior::RANDOM, {}, 0, 2000.f, 2000.f, {{ReactionType::PLAYER_CLOSE, 1}}, 0, true, 0.f, 1000.f, NoAttack};
	EnemyPattern chargingState = {"CHARGE", EnemyBehavior::CHARGING, {}, 0, 0.f, 0.f, {{ReactionType::DURATION, 2}}, 2, true, 0.f, 0.f, NoAttack};
	EnemyPattern idleStateCD1 = {"IDLE", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {{ReactionType::DURATION, 3}}, 3, false, 0.f, 0.f, NoAttack};
	EnemyPattern chargingMidState = {"CHARGE", EnemyBehavior::CHARGING, {}, 0, 0.f, 1000.f, {{ReactionType::DURATION, 4}}, 4, true, 0.f, 0.f, NoAttack};
	EnemyPattern idleStateCD2 = {"IDLE", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {{ReactionType::DURATION, 5}}, 5, false, 0.f, 0.f, NoAttack};
	EnemyPattern chargingEndState = {"CHARGE", EnemyBehavior::CHARGING, {}, 0, 0.f, 0.f, {{ReactionType::DURATION, 6}}, 6, true, 0.f, 0.f, NoAttack};
	EnemyPattern idleState = {"IDLE", EnemyBehavior::IDLE, {}, 0, 3000.f, 3000.f, {{ReactionType::DURATION, 7}}, 7, false, 0.f, 0.f, NoAttack};
	EnemyPattern randomPosNoCharge = {"RANDOM", EnemyBehavior::RANDOM, {}, 0, 1000.f, 1000.f, {{ReactionType::DURATION, 0}}, 0, true, 0.f, 1000.f, NoAttack};

	HifiCharger()
	{
		maxHealth = 70;
		currHealth = maxHealth;
		enemyPatterns = {randomPos, chargingState, idleStateCD1, chargingMidState, idleStateCD2, chargingEndState, idleState, randomPosNoCharge};
		sprite = {
			"enemy_hifi_002.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		scale = vec2({480 / 8.f, 480 / 8.f});
		patternIndex = 0;
		rotatePower = 1.8f;
		rotationBehaviour = EnemyRotationBehavior::FACE_PLAYER;
		speedMultiplier = 1.6f;
	}
};

struct HifiChargerHard : Enemy
{
	const AttackData explode{
		EnemyAttackPattern::TRAIL,
		CIRCLE,
		{},
		{blunt},
		3,
		M_PI / 16,
		{70, 70},
		60,
		0,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::EXPLODE};

	EnemyPattern randomPos = {"RANDOM", EnemyBehavior::RANDOM, {}, 0, 2000.f, 2000.f, {{ReactionType::PLAYER_CLOSE, 1}, {ReactionType::DURATION, 1}}, 1, true, 0.f, 1000.f, NoAttack};
	EnemyPattern chargingState = {"CHARGE", EnemyBehavior::CHARGING, {}, 0, 500.f, 500.f, {{ReactionType::DURATION, 2}}, 2, true, 50.f, 50.f, explode};
	EnemyPattern idleStateCD1 = {"IDLE", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {{ReactionType::DURATION, 3}}, 3, false, 0.f, 0.f, NoAttack};
	EnemyPattern chargingMidState = {"CHARGE", EnemyBehavior::CHARGING, {}, 0, 500.f, 500.f, {{ReactionType::DURATION, 4}}, 4, true, 50.f, 50.f, explode};
	EnemyPattern idleStateCD2 = {"IDLE", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {{ReactionType::DURATION, 5}}, 5, false, 0.f, 0.f, NoAttack};
	EnemyPattern chargingEndState = {"CHARGE", EnemyBehavior::CHARGING, {}, 0, 500.f, 500.f, {{ReactionType::DURATION, 6}}, 6, true, 50.f, 50.f, explode};
	EnemyPattern idleState = {"IDLE", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {{ReactionType::DURATION, 7}}, 7, false, 0.f, 0.f, NoAttack};
	EnemyPattern explodingCharge = {"CHARGE", EnemyBehavior::CHARGING, {}, 0, 500.f, 500.f, {{ReactionType::DURATION, 8}}, 8, true, 50.f, 50.f, explode};
	EnemyPattern idleState3 = {"IDLE", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {{ReactionType::DURATION, 9}}, 9, false, 0.f, 0.f, NoAttack};
	EnemyPattern randomPosNoCharge = {"RANDOM", EnemyBehavior::RANDOM, {}, 0, 4000.f, 4000.f, {{ReactionType::DURATION, 0}}, 0, true, 0.f, 1000.f, NoAttack};

	HifiChargerHard()
	{
		maxHealth = 90;
		currHealth = maxHealth;
		enemyPatterns = {randomPos, chargingState, idleStateCD1, chargingMidState, idleStateCD2, chargingEndState, idleState, explodingCharge, idleState3, randomPosNoCharge};
		sprite = {
			"enemy_hifi_002_hard.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		scale = vec2({480 / 8.f, 480 / 8.f});
		patternIndex = 0;
		rotatePower = 1.8f;
		rotationBehaviour = EnemyRotationBehavior::FACE_PLAYER;
		speedMultiplier = 2.6f;
	}
};

struct HifiTrail : Enemy
{
	const AttackData snailTrail{
		EnemyAttackPattern::TRAIL,
		CIRCLE,
		{},
		playerSpeedUp,
		1,
		0,
		{20, 20},
		0,
		8000,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::CLUSTER};

	const AttackData spiral{
		EnemyAttackPattern::RADIAL,
		TRIANGLE,
		{numBulletsUp, dmgDown},
		blunt,
		12,
		0.0,
		{20, 20},
		200,
		1000,
		{200, -2 * M_PI / 3.0},
		0,
		0,
		0};

	EnemyPattern rotateState = {
		"Follow Player",
		EnemyBehavior::RANDOM,
		{},
		0,
		4000.f,
		4000.f,
		{
			{ReactionType::PLAYER_CLOSE, 1},
		},
		0,
		true,
		0.f,
		5000.f,
		snailTrail};

	EnemyPattern chargingState = {
		"CHARGE",
		EnemyBehavior::CHARGING,
		{},
		0,
		1000.f,
		1000.f,
		{{ReactionType::DURATION, 2}},
		2,
		true,
		0.f,
		0.f,
		snailTrail};

	EnemyPattern chargingState2 = {
		"CHARGE",
		EnemyBehavior::CHARGING,
		{},
		0,
		2000.f,
		2000.f,
		{{ReactionType::PLAYER_CLOSE, 3}, {ReactionType::DURATION, 0}},
		0,
		true,
		0.f,
		0.f,
		snailTrail};

	EnemyPattern shootingState = {
		"ROTATE",
		EnemyBehavior::ROTATE_IN_PLACE,
		{},
		0,
		1000.f,
		1000.f,
		{{ReactionType::DURATION, 0}},
		0,
		true,
		250.f,
		250.f,
		spiral};

	HifiTrail()
	{
		maxHealth = 250;
		currHealth = maxHealth;

		enemyPatterns = {rotateState, chargingState, chargingState2, shootingState};
		rotationBehaviour = EnemyRotationBehavior::SPIN;

		patternIndex = 0;
		sprite = {
			"enemy_hifi_010.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0)};
		scale = vec2(240, 240) * 0.5f;
		speedMultiplier = 2.0;
		rotatePower = 0.5;
	};
};

struct HifiTrailHard : Enemy
{
	/**
	 * Lower health variant focused on spreading traps on the ground with its high speed and longer bullet range, dodging attacks
	 */
	const AttackData snailTrail{
		EnemyAttackPattern::TRAIL,
		CIRCLE,
		{},
		playerSpeedUp,
		1,
		0,
		{20, 20},
		0,
		8000,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::CLUSTER};

	const AttackData spiral{
		EnemyAttackPattern::RADIAL,
		TRIANGLE,
		{numBulletsUp, dmgDown},
		blunt,
		12,
		0.0,
		{20, 20},
		400,
		2000,
		{400, -2 * M_PI / 3.0},
		0,
		0,
		0};

	EnemyPattern rotateState = {
		"Follow Player",
		EnemyBehavior::RANDOM,
		{},
		0,
		4000.f,
		4000.f,
		{
			{ReactionType::PLAYER_CLOSE, 1},
		},
		0,
		true,
		0.f,
		500.f,
		snailTrail};

	EnemyPattern chargingState = {
		"CHARGE",
		EnemyBehavior::CHARGING,
		{},
		0,
		500.f,
		500.f,
		{{ReactionType::DURATION, 2}},
		2,
		true,
		0.f,
		0.f,
		snailTrail};

	EnemyPattern chargingState2 = {
		"CHARGE",
		EnemyBehavior::CHARGING,
		{},
		0,
		1000.f,
		1000.f,
		{{ReactionType::PLAYER_CLOSE, 3}, {ReactionType::DURATION, 0}},
		0,
		true,
		0.f,
		0.f,
		snailTrail};

	EnemyPattern shootingState = {
		"ROTATE",
		EnemyBehavior::ROTATE_IN_PLACE,
		{},
		0,
		1000.f,
		1000.f,
		{{ReactionType::DURATION, 0}},
		0,
		true,
		250.f,
		250.f,
		spiral};

	HifiTrailHard()
	{
		maxHealth = 170;
		currHealth = maxHealth;

		enemyPatterns = {rotateState, chargingState, chargingState2, shootingState};
		rotationBehaviour = EnemyRotationBehavior::SPIN;

		patternIndex = 0;
		sprite = {
			"enemy_hifi_010_hard.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0)};
		scale = vec2(240, 240) * 0.5f;
		speedMultiplier = 4.0;
		rotatePower = 0.5;
	};
};

struct HifiJellyFish : Enemy
{
	/**
	 * Jellyfish pattern attacks
	 */

	const AttackData spiral{
		EnemyAttackPattern::RADIAL,
		TRIANGLE,
		{numBulletsUp, playerSpeedDown},
		blunt,
		10,
		0.0,
		{20, 20},
		400,
		5000,
		{200, -2 * M_PI / 2.0},
		0,
		0,
		0};
	const AttackData spiky{
		EnemyAttackPattern::RADIAL,
		RECTANGLE,
		{numBulletsUp, dashRechargeUp},
		blunt,
		6,
		0.0,
		{20, 20},
		300,
		2000,
		{200, 0},
		0,
		0,
		0};

	EnemyPattern rotateState = {
		"Follow Player",
		EnemyBehavior::RANDOM,
		{},
		0,
		4000.f,
		4000.f,
		{
			{ReactionType::DURATION, 1},
		},
		1,
		true,
		0.f,
		500.f,
		NoAttack};

	EnemyPattern chargingState = {
		"CHARGE",
		EnemyBehavior::RANDOM,
		{},
		0,
		500.f,
		500.f,
		{{ReactionType::DURATION, 2}},
		2,
		true,
		0.f,
		0.f,
		NoAttack};

	EnemyPattern chargingState2 = {
		"CHARGE",
		EnemyBehavior::CHARGING,
		{},
		0,
		1000.f,
		1000.f,
		{{ReactionType::DURATION, 3}},
		3,
		true,
		0.f,
		0.f,
		NoAttack};

	EnemyPattern shootingState = {
		"ROTATE",
		EnemyBehavior::ROTATE_IN_PLACE,
		{},
		0,
		1000.f,
		1000.f,
		{{ReactionType::DURATION, 4}},
		4,
		true,
		250.f,
		250.f,
		spiral};

	EnemyPattern chargingState3 = {
		"CHARGE",
		EnemyBehavior::RECOIL,
		{},
		0,
		500.f,
		500.f,
		{{ReactionType::DURATION, 5}},
		5,
		true,
		0.f,
		0.f,
		NoAttack};
	EnemyPattern shootingState2 = {
		"ROTATE",
		EnemyBehavior::ROTATE_IN_PLACE,
		{},
		0,
		1000.f,
		1000.f,
		{{ReactionType::DURATION, 0}},
		0,
		true,
		250.f,
		250.f,
		spiky};

	HifiJellyFish()
	{
		maxHealth = 150;
		currHealth = maxHealth;

		enemyPatterns = {rotateState, chargingState, chargingState2, shootingState, chargingState3, shootingState2};
		rotationBehaviour = EnemyRotationBehavior::SPIN;

		patternIndex = 0;
		sprite = {
			"enemy_hifi_008.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0)};
		scale = vec2(240, 240) * 0.5f;
		speedMultiplier = 2.0;
		rotatePower = 2;
	};
};

struct HifiTackShooter : Enemy
{
	/**
	 * Stationary turret that attacks within a range around it
	 */

	const AttackData spiral{
		EnemyAttackPattern::RADIAL,
		RECTANGLE,
		{bulletPierceUp},
		playerSpeedDown,
		10,
		0.0,
		{20, 20},
		400,
		2000,
		{200, -2 * M_PI / 2.0},
		0,
		0,
		0};

	EnemyPattern rotateState = {
		"Follow Player",
		EnemyBehavior::IDLE,
		{},
		0,
		2000.f,
		2000.f,
		{{ReactionType::DURATION, 0},
		 {ReactionType::PLAYER_CLOSE, 1}},
		0,
		true,
		0.f,
		0.f,
		NoAttack};

	EnemyPattern shootingState = {
		"ROTATE",
		EnemyBehavior::ROTATE_IN_PLACE,
		{},
		0,
		1000.f,
		1000.f,
		{{ReactionType::DURATION, 2}},
		2,
		true,
		500.f,
		500.f,
		spiral};
	EnemyPattern rotateState2 = {
		"Follow Player",
		EnemyBehavior::ROTATE_IN_PLACE,
		{},
		0,
		2000.f,
		2000.f,
		{
			{ReactionType::DURATION, 0},
		},
		0,
		true,
		0.f,
		0.f,
		NoAttack};

	HifiTackShooter()
	{
		maxHealth = 250;
		currHealth = maxHealth;

		enemyPatterns = {rotateState, shootingState, rotateState2};
		rotationBehaviour = EnemyRotationBehavior::SPIN;

		patternIndex = 0;
		sprite = {
			"enemy_hifi_009.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0)};
		scale = vec2(240, 240) * 0.5f;
		speedMultiplier = 0;
		rotatePower = 3;
	};
};

struct HifiBallLauncher : Enemy
{
	/**
	 *
	 */

	const AttackData launch{
		EnemyAttackPattern::RADIAL,
		CIRCLE,
		{bulletBounceUp},
		blunt,
		4,
		M_PI / 4.f,
		{30, 30},
		600,
		10000,
		{0, 0},
		0,
		2,
		0};

	EnemyPattern rotateState = {
		"Follow Player",
		EnemyBehavior::ROTATE_IN_PLACE,
		{},
		0,
		500.f,
		500.f,
		{
			{ReactionType::DURATION, 1},
		},
		1,
		true,
		0.f,
		0.f,
		NoAttack};

	EnemyPattern shootingState = {
		"ROTATE",
		EnemyBehavior::IDLE,
		{},
		0,
		1000.f,
		1000.f,
		{{ReactionType::DURATION, 0}},
		0,
		true,
		0.f,
		500.f,
		launch};

	HifiBallLauncher()
	{
		maxHealth = 90;
		currHealth = maxHealth;

		enemyPatterns = {rotateState, shootingState};
		rotationBehaviour = EnemyRotationBehavior::REGULAR;

		patternIndex = 0;
		sprite = {
			"enemy_hifi_009.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0)};
		scale = vec2(240, 240) * 0.5f;
		speedMultiplier = 0;
		rotatePower = 2;
	};
};

struct HifiWhip : Enemy
{
	/**
	 * Expanding whip
	 */
	const AttackData whip{
		EnemyAttackPattern::RADIAL,
		RECTANGLE,
		{dmgUp},
		blunt,
		2,
		-M_PI / 4.f,
		{15, 15},
		400,
		2000,
		{200, M_PI / 1.5},
		0,
		2,
		0};

	EnemyPattern rotateState = {
		"Follow Player",
		EnemyBehavior::ROTATE_IN_PLACE,
		{},
		0,
		500.f,
		500.f,
		{
			{ReactionType::DURATION, 1},
		},
		1,
		true,
		0.f,
		0.f,
		whip};

	EnemyPattern restState = {
		"ROTATE",
		EnemyBehavior::IDLE,
		{},
		0,
		1000.f,
		1000.f,
		{{ReactionType::DURATION, 0}},
		0,
		true,
		0.f,
		500.f,
		NoAttack};

	HifiWhip()
	{
		maxHealth = 150;
		currHealth = maxHealth;

		enemyPatterns = {rotateState, restState};
		rotationBehaviour = EnemyRotationBehavior::REGULAR;

		patternIndex = 0;
		sprite = {
			"enemy_hifi_006.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0)};
		scale = vec2(240, 240) * 0.5f;
		speedMultiplier = 0;
		rotatePower = 2;
	};
};

struct HifiCannon : Enemy
{
	const AttackData cannonShot{
		EnemyAttackPattern::SHOTGUN,
		CIRCLE,
		{dmgUp, bulletSpeedUp, sizeUp},
		{fireRateDown},
		2,
		M_PI / 2,
		{60, 60},
		60,
		10000,
		{600, -M_PI / 4},
		0,
		0,
		0,
		EnemyBulletDeath::EXPLODE};
	const AttackData cluster{
		EnemyAttackPattern::BURST,
		CIRCLE,
		{numBulletsUp, bulletSpeedDown},
		blunt,
		4,
		0,
		{30, 30},
		600,
		1000,
		{-400, 0},
		0,
		0,
		0,
		EnemyBulletDeath::CLUSTER,
		EnemyHifiCharger};

	EnemyPattern random1 = {"MOVE", EnemyBehavior::RANDOM, {}, 0, 2000.f, 2000.f, {{ReactionType::DURATION, 1}, {ReactionType::PLAYER_CLOSE, 4}}, 1, false, 0.f, 0.f, NoAttack};
	EnemyPattern chargingState = {"CHARGING", EnemyBehavior::IDLE, {}, 0, 3000.f, 3000.f, {{ReactionType::DURATION, 2}}, 2, false, 0.f, 0.f, NoAttack};
	EnemyPattern backUp = {"RECOIL", EnemyBehavior::RECOIL, {}, 0, 100.f, 100.f, {{ReactionType::DURATION, 3}}, 3, false, 0.f, 0.f, NoAttack};
	EnemyPattern shootCannon = {"SHOOT1", EnemyBehavior::IDLE, {}, 0, 2000.f, 2000.f, {{ReactionType::DURATION, 0}}, 0, true, 0.f, 2000.f, cannonShot};
	EnemyPattern shootCluster = {"SHOOT2", EnemyBehavior::IDLE, {}, 0, 2000.f, 2000.f, {{ReactionType::DURATION, 0}}, 0, true, 0.f, 2000.f, cluster};

	HifiCannon()
	{
		maxHealth = 150;
		currHealth = maxHealth;
		enemyPatterns = {random1, chargingState, backUp, shootCannon, shootCluster};
		patternIndex = 0;
		sprite = {
			"enemy_hifi_003.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		scale = vec2({240.f / 2.f, 240.f / 2.f});
		rotatePower = 1.0f;
		rotationBehaviour = EnemyRotationBehavior::FACE_PLAYER;
		speedMultiplier = 0.3;
		armour = 2;
	};
};

struct HifiCannonHard : Enemy
{
	/**
	 * More glass cannon, destructive variant that fills up the player stack on hit, spawning minions when player gets close
	 */
	const AttackData cannonShot{
		EnemyAttackPattern::SHOTGUN,
		CIRCLE,
		{dmgUp, bulletSpeedUp},
		{playerSpeedDown},
		2,
		M_PI / 2,
		{120, 120},
		60,
		10000,
		{400, -M_PI / 4},
		0,
		0,
		0,
		EnemyBulletDeath::CLUSTER};
	const AttackData spawning{
		EnemyAttackPattern::SPAWNING,
		CIRCLE,
		{},
		blunt,
		2,
		0,
		{60, 60},
		600,
		1000,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::EnemyHifiTemporaryBoid};

	EnemyPattern random1 = {"MOVE", EnemyBehavior::RANDOM, {}, 0, 2000.f, 2000.f, {{ReactionType::DURATION, 1}, {ReactionType::PLAYER_CLOSE, 4}}, 1, false, 0.f, 0.f, NoAttack};
	EnemyPattern chargingState = {"CHARGING", EnemyBehavior::IDLE, {}, 0, 3000.f, 3000.f, {{ReactionType::DURATION, 2}}, 2, false, 0.f, 0.f, NoAttack};
	EnemyPattern backUp = {"RECOIL", EnemyBehavior::RECOIL, {}, 0, 100.f, 100.f, {{ReactionType::DURATION, 3}}, 3, false, 0.f, 0.f, NoAttack};
	EnemyPattern shootCannon = {"SHOOT1", EnemyBehavior::IDLE, {}, 0, 2000.f, 2000.f, {{ReactionType::DURATION, 0}}, 0, true, 0.f, 2000.f, cannonShot};
	EnemyPattern spawn = {"SHOOT2", EnemyBehavior::IDLE, {}, 0, 2000.f, 2000.f, {{ReactionType::DURATION, 5}}, 5, true, 0.f, 200.f, spawning};
	EnemyPattern rest = {"SHOOT1", EnemyBehavior::IDLE, {}, 0, 4000.f, 4000.f, {{ReactionType::DURATION, 0}}, 0, false, 0.f, 2000.f, NoAttack};
	EnemyPattern laser = {"LASER", EnemyBehavior::IDLE, {}, 0, 0.f, 2000.f, {{ReactionType::DURATION, 0}}, 0, true, 0.f, 2000.f, FastLaser};

	HifiCannonHard()
	{
		maxHealth = 400;
		currHealth = maxHealth;
		enemyPatterns = {random1, chargingState, backUp, shootCannon, spawn, rest, laser};
		patternIndex = 0;
		sprite = {
			"enemy_hifi_003_hard.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		scale = vec2({300.f / 2.f, 300.f / 2.f});
		rotatePower = 1.f;
		rotationBehaviour = EnemyRotationBehavior::FACE_PLAYER;
		speedMultiplier = 0.3;
		armour = 3;
	};
};

struct HifiBossEnemy : Enemy
{
	/**
	 * Boss enemy that has a variety of attacks and patterns
	 */
	const AttackData snailTrail{
		EnemyAttackPattern::TRAIL,
		CIRCLE,
		{},
		playerSpeedUp,
		1,
		0,
		{20, 20},
		0,
		8000,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::CLUSTER};

	const AttackData spiral{
		EnemyAttackPattern::RADIAL,
		TRIANGLE,
		{numBulletsUp, dmgDown},
		blunt,
		12,
		0.0,
		{20, 20},
		200,
		1000,
		{200, -2 * M_PI / 3.0},
		0,
		0,
		0};

	EnemyPattern rotateState = {
		"Follow Player",
		EnemyBehavior::RANDOM,
		{},
		0,
		4000.f,
		4000.f,
		{
			{ReactionType::PLAYER_CLOSE, 1},
		},
		0,
		true,
		0.f,
		5000.f,
		snailTrail};

	EnemyPattern chargingState = {
		"CHARGE",
		EnemyBehavior::CHARGING,
		{},
		0,
		1000.f,
		1000.f,
		{{ReactionType::DURATION, 2}},
		2,
		true,
		0.f,
		0.f,
		snailTrail};

	EnemyPattern chargingState2 = {
		"CHARGE",
		EnemyBehavior::CHARGING,
		{},
		0,
		2000.f,
		2000.f,
		{{ReactionType::PLAYER_CLOSE, 3}, {ReactionType::DURATION, 0}},
		0,
		true,
		0.f,
		0.f,
		snailTrail};

	EnemyPattern shootingState = {
		"ROTATE",
		EnemyBehavior::ROTATE_IN_PLACE,
		{},
		0,
		1000.f,
		1000.f,
		{{ReactionType::DURATION, 0}},
		0,
		true,
		250.f,
		250.f,
		spiral};

	EnemyPattern fourWayLaser = {
		"LASER",
		EnemyBehavior::IDLE,
		{},
		0,
		0.f,
		2000.f,
		{{ReactionType::DURATION, 0}},
		0,
		true,
		0.f,
		2000.f,
		FastLaser};
};

struct ScientistBossEnemy : Enemy
{
	Reaction duration{
		ReactionType::DURATION,
		0};

	Reaction spawningBeeBoid{
		ReactionType::PLAYER_CLOSE,
		6,
		};

	Reaction shieldBreak{
		ReactionType::SHIELDBREAK,
		7};

	Reaction secondPhase{
		ReactionType::FIFTY_HEALTH,
		9,
	};

	Reaction twentyFivePercent{
		ReactionType::TWENTYFIVE_HEALTH,
		20,
	};

	Reaction secondPhasePlayerClose{
		ReactionType::PLAYER_CLOSE,
		13,
		};

	Reaction playerFar {
		ReactionType::PLAYER_FAR,
		21,
	};

	const AttackData spawnLaserRotating{
		EnemyAttackPattern::SPAWNING,
		TRIANGLE,
		{sizeUp},
		blunt,
		1,
		0,
		{20, 20},
		100,
		3000,
		{0.5, 0.5},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::ScientistlaserAttack,
		{{0.5, 0.5}}};

	const AttackData spawnBoids{
		EnemyAttackPattern::SPAWNING,
		TRIANGLE,
		{},
		blunt,
		10,
		0,
		{20, 20},
		100,
		3000,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::EnemyHifiBoid,
		{}};

	const AttackData spawnMedicalBoids{
		EnemyAttackPattern::SPAWNING,
		TRIANGLE,
		{},
		blunt,
		10,
		0,
		{20, 20},
		100,
		3000,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::EnemyMedicalBoid,
		{}};

	const AttackData spawnLaserHorizontal{
		EnemyAttackPattern::SPAWNING,
		TRIANGLE,
		{sizeUp},
		blunt,
		5,
		0,
		{20, 20},
		100,
		3000,
		{0.5, 0.5},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::ScientistLaserGridAttack,
		{{0.03f, 0.2f}, {0.03f, 0.4f}, {0.03, 0.6}, {0.03, 0.8}, {0.03, 1}}};

	const AttackData spawnLaserVertical{
		EnemyAttackPattern::SPAWNING,
		TRIANGLE,
		{sizeUp},
		blunt,
		5,
		0,
		{20, 20},
		100,
		3000,
		{0.5, 0.5},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::ScientistLaserGridVerticalAttack,
		{{0.1f, 0.05f}, {0.3f, 0.05f}, {0.5, 0.05}, {0.7, 0.05}, {0.9, 0.05}}};

	const AttackData spawnTurret{
		EnemyAttackPattern::SPAWNING,
		TRIANGLE,
		{sizeUp},
		blunt,
		2,
		0,
		{20, 20},
		100,
		3000,
		{0.5, 0.5},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::ScientistradialAttack,
		{{0.5, 0.3}, {0.5, 0.7}}};

	const AttackData spawnExplosive{
		EnemyAttackPattern::SPAWNING,
		TRIANGLE,
		{sizeUp},
		blunt,
		2,
		0,
		{20, 20},
		100,
		3000,
		{0.5, 0.5},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::ScientistexplosiveAttack,
		{{0.5, 0.3}, {0.5, 0.7}}};

	const AttackData spawnShield{
		EnemyAttackPattern::SPAWNING,
		TRIANGLE,
		{dashRechargeUp},
		blunt,
		1,
		0,
		{20, 20},
		100,
		3000,
		{0.5, 0.5},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::ScientistShield,
		{}};

	const AttackData spawnHand{
		EnemyAttackPattern::SPAWNING,
		TRIANGLE,
		{dashRechargeUp},
		blunt,
		1,
		0,
		{20, 20},
		100,
		3000,
		{0.5, 0.5},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::ScientistHand,
		{}};

	const AttackData lethalShot{
		EnemyAttackPattern::BURST,
		TRIANGLE,
		{dmgUp,dmgUp,dmgUp,},
		dmgDown,
		3,
		M_PI,
		{35, 35},
		1000,
		3000,
		{0, 0},
		0,
		0,
		0.025};

	const AttackData laserBoss{
		EnemyAttackPattern::LASER,
		CIRCLE,
		{bulletPierceUp},
		dashRechargeUp,
		8,
		0,
		{0, 20},
		0,
		8000,
		{3, M_PI / 600},
		0,
		0,
		0};

	const AttackData ScientistHomingShot{
		EnemyAttackPattern::SHOTGUN,
		TRIANGLE,
		{},
		blunt,
		1,
		M_PI / 16,
		{20, 20},
		450,
		10000,
		{600, 0},
		1,
		0,
		0.02};

	const AttackData boidFishSummon{
		EnemyAttackPattern::SPAWNING,
		TRIANGLE,
		{numBulletsUp},
		playerSpeedDown,
		150,
		M_PI,
		{20, 20},
		600,
		3000,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::EnemyFishBoid,
		{}};

	const AttackData rodCSummon {
		EnemyAttackPattern::SPAWNING,
		TRIANGLE,
		{numBulletsUp},
		playerSpeedDown,
		1,
		M_PI,
		{20, 20},
		600,
		3000,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::EnemyMedicalRodC,
		{}};

	EnemyPattern spawnLaserHorizontalState = {
		"SHOOT HORIZONTAL", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f,
		{duration, shieldBreak, secondPhase, twentyFivePercent}, 1, true, 0.f, 1000.f, spawnLaserHorizontal
	};
	EnemyPattern spawnLaserVerticalState = {
		"SHOOT VERTICAL", EnemyBehavior::IDLE, {}, 0, 3000.f, 3000.f,
		{duration, shieldBreak, secondPhase, twentyFivePercent}, 2, true, 0.f, 4000.f, spawnLaserVertical
	};
	EnemyPattern spawnHomingState = {
		"SHOOT HOMING", EnemyBehavior::IDLE, {}, 0, 2000.f, 2000.f,
		{duration, shieldBreak, secondPhase, twentyFivePercent}, 3, true, 0.f, 500.f, ScientistHomingShot
	};
	EnemyPattern spawnBoidState = {
		"SHOOT BOID", EnemyBehavior::IDLE, {}, 0, 3000.f, 3000.f, {duration, spawningBeeBoid, shieldBreak, secondPhase, twentyFivePercent},
		4, true, 0.f, 2500.f, spawnBoids
	};
	EnemyPattern spawnLaserRotateState = {
		"SHOOT LASER", EnemyBehavior::IDLE, {}, 0, 3000.f, 3000.f,
		{duration, spawningBeeBoid, shieldBreak, secondPhase, twentyFivePercent}, 5, true, 0.f, 7000.f, spawnLaserRotating
	};
	EnemyPattern spawnRadialState = {
		"SHOOT RADIAL", EnemyBehavior::IDLE, {}, 0, 3000.f, 3000.f,
		{duration, spawningBeeBoid, shieldBreak, secondPhase, twentyFivePercent}, 0, true, 0.f, 4000.f, spawnTurret
	};
	EnemyPattern spawnBeeBoidState = {
		"YOU ARE CLOSE", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {duration, shieldBreak, secondPhase, twentyFivePercent}, 0, true, 0.f,
		1000.f, spawnMedicalBoids, SpecialStates::PROTECTED
	};
	EnemyPattern vunerableState = {
		"VURNERABLE", EnemyBehavior::IDLE, {}, 0, 5000.f, 5000.f, {duration, spawningBeeBoid, secondPhase, twentyFivePercent}, 8, false, 0.f, 0.f,
		NoAttack,
	};
	EnemyPattern spawnShieldState = {
		"Shield", EnemyBehavior::IDLE, {}, 0, 2000.f, 2000.f, {duration, shieldBreak, secondPhase, twentyFivePercent}, 0, true, 0.f, 100.f,
		spawnShield
	};
	EnemyPattern secondPhaseState = {
		"SECOND PHASE", EnemyBehavior::TELEPORT, {{0.5f, 0.5f}}, 0, 2000.f, 2000.f, {duration}, 10, false, 0.f, 0.f,
		NoAttack, SpecialStates::INVINCIBLE
	};
	EnemyPattern spawnHandState = {
		"SUMMON HAND", EnemyBehavior::IDLE, {}, 0, 500.f, 500.f, {duration}, 11, true, 0.f, 5000.f, spawnHand,
		SpecialStates::INVINCIBLE
	};
	EnemyPattern patrollingState1 = {
		"RANDOM", EnemyBehavior::RANDOM, {{0.2, 0.8}, {0.2, 0.2}}, 0, 5000.f, 5000.f, {duration, twentyFivePercent}, 12, true, 0.f,
		2000.f, lethalShot,
	};

	EnemyPattern patrollingState2 = {
		"RANDOM", EnemyBehavior::GRANTINGBUFFSAOE, {{0.2, 0.8}, {0.2, 0.2}}, 0, 2000.f, 2000.f, {duration, playerFar}, 13, true, 0.f,
		2000.f, spawnLaserHorizontal, SpecialStates::PROTECTED, SpecialStates::INVINCIBLE
	};

	EnemyPattern patrollingState3 = {
		"RANDOM", EnemyBehavior::RANDOM, {{0.5, 0.5}, {0.7, 0.7}}, 0, 5000.f, 5000.f, {duration, playerFar}, 14, true, 0.f, 5000.f,
		spawnExplosive, SpecialStates::PROTECTED
	};
	EnemyPattern teleportState = {
		"TELEPORT", EnemyBehavior::TELEPORT, {{0.5, 0.5}}, 0, 3000.f, 3000.f, {duration, playerFar}, 15, true, 0.f, 3000.f,
		boidFishSummon, SpecialStates::VULNERABLE
	};
	EnemyPattern spawnLaserRotateState2 = {
		"SHOOT LASER", EnemyBehavior::RANDOM, {}, 0, 1000.f, 1000.f,
		{duration, playerFar}, 16, true, 0.f, 7000.f, spawnLaserRotating
	};
	EnemyPattern spawnTurretState = {
		"SHOOT RADIAL", EnemyBehavior::RANDOM, {}, 0, 1000.f, 1000.f,
		{duration, playerFar}, 18, true, 0.f, 4000.f, spawnTurret
	};

	EnemyPattern grantInvisibleState = {
		"GRANT INVISIBLE", EnemyBehavior::GRANTINGBUFFSAOE, {}, 0, 1000.f, 1000.f,
		{duration, playerFar}, 18, true, 0.f, 4000.f, NoAttack, SpecialStates::VULNERABLE, SpecialStates::INVISIBLE
	};

	EnemyPattern spawnLaserHorizontalStatePhase2 = {
		"SHOOT HORIZONTAL", EnemyBehavior::RANDOM, {}, 0, 1000.f, 1000.f,
		{duration, playerFar }, 19, true, 0.f, 1000.f, spawnLaserHorizontal, SpecialStates::VULNERABLE
	};
	EnemyPattern spawnLaserVerticalStatePhase2 = {
		"SHOOT VERTICAL", EnemyBehavior::RANDOM, {}, 0, 1000.f, 1000.f,
		{duration, playerFar}, 21, true, 0.f, 4000.f, spawnLaserVertical, SpecialStates::VULNERABLE
	};

	EnemyPattern spawnShieldStatePhase2 = {
		"Shield", EnemyBehavior::RANDOM, {}, 0, 1000.f, 1000.f, {duration}, 12, true, 0.f, 1000.f,
		spawnShield
	};

	EnemyPattern spawnRodC = {
		"SHOOT ROD C", EnemyBehavior::RANDOM, {}, 0, 3000.f, 3000.f,
		{duration}, 21, true, 0.f, 4000.f, rodCSummon, SpecialStates::INVINCIBLE
	};

	ScientistBossEnemy()
	{
		maxHealth = 5000;
		currHealth = 5000;
		enemyPatterns = {spawnLaserHorizontalState, spawnLaserVerticalState, spawnHomingState,
						 spawnBoidState, spawnLaserRotateState,
						 spawnRadialState, spawnBeeBoidState,
						 vunerableState, spawnShieldState, secondPhaseState,spawnHandState, patrollingState1,
						 patrollingState2, patrollingState3, teleportState, spawnLaserRotateState, spawnTurretState,
		grantInvisibleState, spawnLaserHorizontalStatePhase2, spawnLaserVerticalStatePhase2, spawnRodC, spawnShieldStatePhase2,
		};
		sprite = {
			"scientist.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		patternIndex = 8;
		scale = vec2(250, 250);
		rotatePower = 0.f;
		armour = 10;
		speedMultiplier = 1.5f;
		collisionBullet = {blunt, blunt, blunt, blunt};
	}
};

struct ScientistSheildEnemy : Enemy
{
	Reaction IdleTransition{
		ReactionType::DURATION,
		0};

	EnemyPattern IdleState = {"SHOOT", EnemyBehavior::IDLE, {}, 0, 5000.f, 5000.f, {IdleTransition}, 0, false, 0.f, 600.f, NoAttack};
	ScientistSheildEnemy()
	{
		maxHealth = 500;
		currHealth = 500;
		enemyPatterns = {IdleState};
		sprite = {
			"enemy_bullet_square.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		patternIndex = 0;
		scale = vec2(300, 300);
		rotatePower = 0.f;
		armour = 10;
	}
};

struct InvisibleTurretEnemy : Enemy
{

	const AttackData radialScientistBurst{
		EnemyAttackPattern::SHOTGUN,
		CIRCLE,
		{numBulletsUp, dmgDown},
		blunt,
		100,
		-M_PI / 20,
		{20, 20},
		150,
		9000,
		{5, 0},
		0,
		0,
		0};

	const AttackData deathSummon{
		EnemyAttackPattern::SPAWNING,
		TRIANGLE,
		{numBulletsUp},
		playerSpeedDown,
		1,
		M_PI,
		{20, 20},
		600,
		3000,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::EnemyScissors,
		};

	Reaction deathTransition{
		ReactionType::DURATION,
		1};
	EnemyPattern RotateState = {"SHOOT", EnemyBehavior::IDLE, {}, 0, 5000.f, 5000.f, {deathTransition}, 1, true, 0.f, 2500.f, radialScientistBurst};
	EnemyPattern spawnBoidState = {"SHOOT BOID", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {deathTransition}, 2, true, 0.f, 2500.f, deathSummon};
	EnemyPattern DeathState = {"REMOVE ME", EnemyBehavior::DEATHSTATE, {}, 0, 1000.f, 1000.f, {deathTransition}, 1, false, 0.f, 0.f, NoAttack};
	InvisibleTurretEnemy()
	{
		maxHealth = 1;
		currHealth = 1;
		enemyPatterns = {RotateState,spawnBoidState, DeathState};
		sprite = {
			"enemy_Angel.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		patternIndex = 0;
		scale = vec2(0, 0);
		rotatePower = 0.f;
	}
};

struct InvisibleRotateLaserEnemy : Enemy
{
	const AttackData laserRotateBoss{
		EnemyAttackPattern::LASER,
		CIRCLE,
		{bulletPierceUp},
		dashRechargeUp,
		8,
		0,
		{0, 20},
		0,
		8000,
		{10, M_PI / 600},
		0,
		0,
		0};

	const AttackData deathSummon{
		EnemyAttackPattern::SPAWNING,
		TRIANGLE,
		{numBulletsUp},
		playerSpeedDown,
		1,
		M_PI,
		{20, 20},
		600,
		3000,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::EnemyMedicalBMP,
		};

	Reaction deathTransition{
		ReactionType::DURATION,
		1};
	EnemyPattern laserRotateState = {"LASER", EnemyBehavior::IDLE, {}, 0, 7000.f, 7000.f, {deathTransition}, 1, true, 0.f, 7000.f, laserRotateBoss};
	EnemyPattern spawnBoidState = {"SHOOT BOID", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {deathTransition}, 2, true, 0.f, 2500.f, deathSummon};
	EnemyPattern DeathState = {"REMOVE ME", EnemyBehavior::DEATHSTATE, {}, 0, 1000.f, 1000.f, {deathTransition}, 1, false, 0.f, 0.f, NoAttack};
	InvisibleRotateLaserEnemy()
	{
		maxHealth = 1;
		currHealth = 1;
		enemyPatterns = {laserRotateState,spawnBoidState, DeathState};
		sprite = {
			"enemy_Angel.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		patternIndex = 0;
		scale = vec2(0, 0);
		rotatePower = 0.f;
	};
};

struct InvisibleLaserEnemy : Enemy
{
	Reaction deathTransition{
		ReactionType::DURATION,
		1};

	const AttackData laserGrid{
		EnemyAttackPattern::LASER,
		CIRCLE,
		{dashUp},
		dashRechargeUp,
		1,
		0,
		{20, 20},
		0,
		30000,
		{500, 0},
		0,
		0,
		0};

	EnemyPattern laserState = {"LASER", EnemyBehavior::IDLE, {}, 0, 5000.f, 5000.f, {deathTransition}, 1, true, 0.f, 5000.f, laserGrid};
	EnemyPattern DeathState = {"REMOVE ME", EnemyBehavior::DEATHSTATE, {}, 0, 1000.f, 1000.f, {deathTransition}, 1, false, 0.f, 0.f, NoAttack};
	InvisibleLaserEnemy()
	{
		maxHealth = 1;
		currHealth = 1;
		enemyPatterns = {laserState, DeathState};
		sprite = {
			"enemy_Angel.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		patternIndex = 0;
		scale = vec2(0, 0);
		rotatePower = 0.f;
	};
};

struct InvisibleLaserEnemyVertical : Enemy
{
	Reaction deathTransition{
		ReactionType::DURATION,
		1};

	const AttackData laserGrid{
		EnemyAttackPattern::LASER,
		CIRCLE,
		{dashUp},
		dashRechargeUp,
		1,
		M_PI / 2.f,
		{20, 20},
		0,
		30000,
		{500, 0},
		0,
		0,
		0};

	EnemyPattern laserState = {"LASER", EnemyBehavior::IDLE, {}, 0, 4000.f, 4000.f, {deathTransition}, 1, true, 0.f, 4000.f, laserGrid};
	EnemyPattern DeathState = {"REMOVE ME", EnemyBehavior::DEATHSTATE, {}, 0, 1000.f, 1000.f, {deathTransition}, 1, false, 0.f, 0.f, NoAttack};
	InvisibleLaserEnemyVertical()
	{
		maxHealth = 1;
		currHealth = 1;
		enemyPatterns = {laserState, DeathState};
		sprite = {
			"enemy_Angel.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		patternIndex = 0;
		scale = vec2(0, 0);
		rotatePower = 0.f;
	};
};

struct InvisibleExplosiveEnemy : Enemy
{

	const AttackData ScientistCannonShot{
		EnemyAttackPattern::RADIAL_POLYGON,
		CIRCLE,
		{bulletBounceDown},
		{bulletPierceUp},
		5,
		M_PI / 16,
		{50, 50},
		60,
		10000,
		{60, 0},
		0,
		0,
		0,
		EnemyBulletDeath::CLUSTER};

	const AttackData deathSummon{
		EnemyAttackPattern::SPAWNING,
		TRIANGLE,
		{numBulletsUp},
		playerSpeedDown,
		1,
		M_PI,
		{20, 20},
		600,
		3000,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::EnemyMedicalSyringe,
		};

	Reaction deathTransition{
		ReactionType::DURATION,
		1};
	EnemyPattern ShootState = {"LASER", EnemyBehavior::IDLE, {}, 0, 5000.f, 5000.f, {deathTransition}, 1, true, 0.f, 2500.f, ScientistCannonShot};
	EnemyPattern spawnBoidState = {"SHOOT BOID", EnemyBehavior::IDLE, {}, 0, 500.f, 500.f, {deathTransition}, 2, true, 0.f, 2500.f, deathSummon};
	EnemyPattern DeathState = {"REMOVE ME", EnemyBehavior::DEATHSTATE, {}, 0, 1000.f, 1000.f, {deathTransition}, 1, false, 0.f, 0.f, NoAttack};
	InvisibleExplosiveEnemy()
	{
		maxHealth = 1;
		currHealth = 1;
		enemyPatterns = {ShootState,spawnBoidState, DeathState};
		sprite = {
			"enemy_Angel.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		patternIndex = 0;
		scale = vec2(0, 0);
		rotatePower = 0.f;
	};
};

struct InvisibleHomingEnemy : Enemy
{

	const AttackData ScientistHomingPiercingShot{
		EnemyAttackPattern::SHOTGUN,
		TRIANGLE,
		{},
		blunt,
		1,
		M_PI / 16,
		{20, 20},
		350,
		10000,
		{600, 0},
		1,
		2,
		0.02};

	const AttackData deathSummon{
		EnemyAttackPattern::SPAWNING,
		TRIANGLE,
		{numBulletsUp},
		playerSpeedDown,
		1,
		M_PI,
		{20, 20},
		600,
		3000,
		{0, 0},
		0,
		0,
		0,
		EnemyBulletDeath::NONE,
		EnemyType::EnemyMedicalBMP,
		};

	Reaction deathTransition{
		ReactionType::DURATION,
		1};
	EnemyPattern ShootState = {"LASER", EnemyBehavior::IDLE, {}, 0, 5000.f, 5000.f, {deathTransition}, 1, true, 0.f, 600.f, ScientistHomingPiercingShot};
	EnemyPattern spawnBoidState = {"SHOOT BOID", EnemyBehavior::IDLE, {}, 0, 500.f, 500.f, {deathTransition}, 2, true, 0.f, 2500.f, deathSummon};
	EnemyPattern DeathState = {"REMOVE ME", EnemyBehavior::DEATHSTATE, {}, 0, 1000.f, 1000.f, {deathTransition}, 1, false, 0.f, 0.f, NoAttack};
	InvisibleHomingEnemy()
	{
		maxHealth = 1;
		currHealth = 1;
		enemyPatterns = {ShootState,spawnBoidState, DeathState};
		sprite = {
			"enemy_Angel.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		patternIndex = 0;
		scale = vec2(0, 0);
		rotatePower = 0.f;
	};
};

struct ScientistHandEnemy : Enemy
{

	Reaction duration{
		ReactionType::DURATION,
		0};

	const AttackData HandBomb{
		EnemyAttackPattern::SHOTGUN,
		CIRCLE,
		{numBulletsUp, dmgDown},
		blunt,
		7,
		-M_PI / 20,
		{50, 50},
		150,
		9000,
		{40, 0},
		0,
		0,
		0,
		EnemyBulletDeath::EXPLODE};

	const AttackData laserOne{
		EnemyAttackPattern::LASER,
		CIRCLE,
		{numBulletsUp, dmgDown},
		dmgDown,
		1,
		M_PI,
		{100, 100},
		150,
		15000,
		{99, 0},
		0,
		0,
		0,
		EnemyBulletDeath::EXPLODE};

	const AttackData quakeShot{
		EnemyAttackPattern::RADIAL,
		CIRCLE,
		{},
		blunt,
		16,
		-M_PI / 20,
		{20, 20},
		150,
		9000,
		{20, 0},
		0,
		0,
		0};

	const AttackData sprayingShot{
		EnemyAttackPattern::SPRAY,
		CIRCLE,
		{numBulletsUp},
		playerSpeedDown,
		50,
		-M_PI / 20,
		{20, 20},
		150,
		9000,
		{90, 0},
		0,
		0,
		0};

	const AttackData sprayHoming{
		EnemyAttackPattern::RADIAL,
		TRIANGLE,
		{dmgDown},
		playerSpeedUp,
		20,
		-M_PI / 20,
		{20, 20},
		400,
		3000,
		{0, 0},
		0,
		0,
		0.02};

	const AttackData laserArea{
		EnemyAttackPattern::LASER,
		CIRCLE,
		{numBulletsUp},
		dmgDown,
		1,
		M_PI / 2,
		{75, 75},
		150,
		2000,
		{99, M_PI / 200},
		0,
		0,
		0};

	EnemyPattern idling = {"IDLE", EnemyBehavior::FOLLOWSCIENTIST, {}, 0, 7000.f, 7000.f, {duration}, 1, false, 0.f, 0.f, NoAttack, SpecialStates::INVINCIBLE};
	EnemyPattern chargePlayer = {"CHARGE", EnemyBehavior::CHARGING, {}, 0, 3000.f, 3000.f, {duration}, 2, false, 0.f, 0.f, NoAttack, SpecialStates::VULNERABLE};
	EnemyPattern idling2 = {"IDLE", EnemyBehavior::FOLLOWSCIENTIST, {}, 0, 7000.f, 7000.f, {duration}, 3, false, 0.f, 0.f, NoAttack, SpecialStates::INVINCIBLE };
	EnemyPattern BombingState = {"PATROL", EnemyBehavior::IDLE, {{0.5, 0.5}}, 0, 6000.f, 6000.f, {duration}, 4, true, 1000.f, 1000.f, HandBomb};
	EnemyPattern idling3 = {"IDLE", EnemyBehavior::FOLLOWSCIENTIST, {}, 0, 7000.f, 7000.f, {duration}, 5, false, 0.f, 0.f, NoAttack, SpecialStates::INVINCIBLE};
	EnemyPattern teleportToScientist2 = {"IDLE", EnemyBehavior::IDLE, {}, 4, 1000.f, 1000.f, {duration}, 6, true, 500.f, 1000.f, quakeShot};
	EnemyPattern laserAttackPrepareState = {"PATROL", EnemyBehavior::TELEPORT, {{1, 0.1}}, 0, 2000.f, 2000.f, {duration}, 7, false, 1000.f, 1000.f, NoAttack, SpecialStates::VULNERABLE};
	EnemyPattern patrolLaserState = {"PATROLLING", EnemyBehavior::PATROLLING, {{1, 0.1}, {1, 0.95}}, 0, 15000.f, 15000.f, {duration}, 8, true, 0.f, 15000.f, laserOne, SpecialStates::VULNERABLE};
	EnemyPattern teleportToScientist3 = {"IDLE", EnemyBehavior::IDLE, {}, 4, 1000.f, 1000.f, {duration}, 9, true, 500.f, 1000.f, quakeShot};
	EnemyPattern idling4 = {"IDLE", EnemyBehavior::FOLLOWSCIENTIST, {}, 0, 7000.f, 7000.f, {duration}, 10, false, 0.f, 0.f, NoAttack, SpecialStates::INVINCIBLE};
	EnemyPattern SprayingState = {"PATROL", EnemyBehavior::PATROLLING, {{0.5, 0.5}}, 4, 6000.f, 6000.f, {duration}, 11, true, 1000.f, 1000.f, sprayingShot};
	EnemyPattern teleportToScientist4 = {"IDLE", EnemyBehavior::IDLE, {}, 4, 1000.f, 1000.f, {duration}, 12, true, 500.f, 1000.f, quakeShot};
	EnemyPattern idling5 = {"IDLE", EnemyBehavior::FOLLOWSCIENTIST, {}, 0, 7000.f, 7000.f, {duration}, 13, false, 0.f, 0.f, NoAttack, SpecialStates::INVINCIBLE};
	EnemyPattern sprayHomingState = {"SHOOT", EnemyBehavior::IDLE, {}, 0, 6000.f, 6000.f, {duration}, 14, true, 0.f, 3000.f, sprayHoming};
	EnemyPattern idling6 = {"IDLE", EnemyBehavior::FOLLOWSCIENTIST, {}, 0, 7000.f, 7000.f, {duration}, 15, false, 0.f, 0.f, NoAttack, SpecialStates::INVINCIBLE};
	EnemyPattern teleportToScientist5 = {"IDLE", EnemyBehavior::IDLE, {}, 4, 1000.f, 1000.f, {duration}, 16, true, 500.f, 1000.f, quakeShot};
	EnemyPattern teleportToWall = {"TELEPORT", EnemyBehavior::TELEPORT, {{1, 0.5}}, 0, 2000.f, 2000.f, {duration}, 17, false, 1000.f, 1000.f, NoAttack};
	EnemyPattern laserRotateState = {"SHOOT", EnemyBehavior::IDLE, {}, 0, 3000.f, 3000.f, {duration}, 0, true, 0.f, 3000.f, laserArea};


	ScientistHandEnemy()
	{
		maxHealth = 1500;
		currHealth = maxHealth;
		enemyPatterns = {idling, chargePlayer, idling2,
						 BombingState, idling3,teleportToScientist2,
						 laserAttackPrepareState, patrolLaserState, teleportToScientist3,
						 idling4, SprayingState, teleportToScientist4, idling5, sprayHomingState, idling6,teleportToScientist5, teleportToWall
						 , laserRotateState};
		sprite = {
			"hand_idletocharge",
			EFFECT_ASSET_ID::ANIMATE,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0),
			AnimationTypes::REGULAR,
			9};
		patternIndex = 0;
		scale = vec2(200, 200);
		rotatePower = 0.f;
		speedMultiplier = 4.f;
		collisionBullet = {popCurrentStack, blunt, blunt, blunt};
		armour =2;
	};
};


// ---------- MILITARY REGION ENEMIES -----------------------------------------------------------------------------------------------------------------------------

struct EyeCube : Enemy {
	const AttackData laserSweep{
	EnemyAttackPattern::LASER,
	CIRCLE,
	{},
	dmgDown,
	1,
	0,
	{75, 75},
	150,
	3000.f,
	{400, M_PI / 100},
	0,
	0,
	0 };


	const AttackData wallTest{
	EnemyAttackPattern::TWO_WALL,
	CIRCLE,
	{},
	dmgDown,
	0,
	0,
	{40, 40},
	150,
	14000.f,
	{0,0},
	-100,
	0,
	0 };


	Reaction duration0 = {
	ReactionType::DURATION,
	0 };


	Reaction duration1 = {
		ReactionType::DURATION,
		1 };


	EnemyPattern followState = { "ROLLING", EnemyBehavior::ROOK_FOLLOW, {}, 0, 7000.f, 7000.f, {duration1}, 1, false, 0.f, 5000.f, quadShot };
	EnemyPattern attackState = { "ROLLING", EnemyBehavior::IDLE, {}, 0, 3000.f, 3000.f, {duration0}, 0, true, 0.f, 3000.f, laserSweep };
	//EnemyPattern attackState = { "ROLLING", EnemyBehavior::IDLE, {}, 0, 4000.f, 4000.f, {duration0}, 0, true, 0.f, 2000.f, wallTest };


	EyeCube()
	{
		maxHealth = 1500;
		currHealth = maxHealth;

		enemyPatterns = { followState, attackState };

		patternIndex = 0;
		sprite = {
			"EyeCube.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
			vec2(0, 0) };
		scale = vec2({ 336.0f / 2, 336.f / 2 });
		rotatePower = 1.0;
		speedMultiplier = 4.0f;
		rotationBehaviour = EnemyRotationBehavior::NONE;
	};
};


// ---------- EVENTROOM ENEMIES -----------------------------------------------------------------------------------------------------------------------------

struct InvincibleBuffGranter : Enemy
{
	Reaction duration{
		ReactionType::DURATION,
		0, SpecialStates::INVISIBLE};

	EnemyPattern buff = {"INVINCIBLE", EnemyBehavior::GRANTINGBUFFSAOE, {}, 0, 5000.f, 5000.f, {duration}, 0, false, 0.f, 3000.f, NoAttack, SpecialStates::NORMAL, SpecialStates::INVINCIBLE};

	InvincibleBuffGranter()
	{
		maxHealth = 1;
		currHealth = maxHealth;
		enemyPatterns = {buff};
		sprite = {
			"enemy_Angel.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		patternIndex = 0;
		scale = vec2(0, 0);
		rotatePower = 0.f;
	};
};

struct InvisibleBuffGranter : Enemy
{
	Reaction duration{
		ReactionType::DURATION,
		0, SpecialStates::INVISIBLE};

	EnemyPattern buff = {"INVISIBLE", EnemyBehavior::GRANTINGBUFFSAOE, {}, 0, 5000.f, 5000.f, {duration}, 0, false, 0.f, 3000.f, NoAttack, SpecialStates::NORMAL, SpecialStates::INVISIBLE};

	InvisibleBuffGranter()
	{
		maxHealth = 10;
		currHealth = maxHealth;
		enemyPatterns = {buff};
		sprite = {
			"enemy_Angel.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		patternIndex = 0;
		scale = vec2(0, 0);
		rotatePower = 0.f;
	};
};

struct CloakedBuffGranter : Enemy
{
	Reaction duration{
		ReactionType::DURATION,
		0, SpecialStates::INVISIBLE};

	EnemyPattern buff = {"CLOAKED", EnemyBehavior::GRANTINGBUFFSAOE, {}, 0, 100000.f, 100000.f, {duration}, 0, false, 0.f, 3000.f, NoAttack, SpecialStates::NORMAL, SpecialStates::CLOAKED};

	CloakedBuffGranter()
	{
		maxHealth = 1;
		currHealth = maxHealth;
		enemyPatterns = {buff};
		sprite = {
			"enemy_Angel.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		patternIndex = 0;
		scale = vec2(0, 0);
		rotatePower = 0.f;
	};
};

struct RegenerateBuffGranter : Enemy
{
	Reaction duration{
		ReactionType::DURATION,
		0, SpecialStates::INVISIBLE};

	EnemyPattern buff = {"REGENERATE", EnemyBehavior::GRANTINGBUFFSAOE, {}, 0, 5000.f, 5000.f, {duration}, 0, false, 0.f, 3000.f, NoAttack, SpecialStates::NORMAL, SpecialStates::REGENERATING};

	RegenerateBuffGranter()
	{
		maxHealth = 1;
		currHealth = maxHealth;
		enemyPatterns = {buff};
		sprite = {
			"enemy_Angel.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		patternIndex = 0;
		scale = vec2(0, 0);
		rotatePower = 0.f;
	};
};

struct ProtectedBuffGranter : Enemy
{
	Reaction duration{
		ReactionType::DURATION,
		0, SpecialStates::INVISIBLE};

	EnemyPattern buff = {"PROTECTED", EnemyBehavior::GRANTINGBUFFSAOE, {}, 0, 5000.f, 5000.f, {duration}, 0, false, 0.f, 3000.f, NoAttack, SpecialStates::NORMAL, SpecialStates::PROTECTED};

	ProtectedBuffGranter()
	{
		maxHealth = 1;
		currHealth = maxHealth;
		enemyPatterns = {buff};
		sprite = {
			"enemy_Angel.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		patternIndex = 0;
		scale = vec2(0, 0);
		rotatePower = 0.f;
	};
};

struct VulnerableBuffGranter : Enemy
{
	Reaction duration{
		ReactionType::DURATION,
		0, SpecialStates::INVISIBLE};

	EnemyPattern buff = {"VULNERABLE", EnemyBehavior::GRANTINGBUFFSAOE, {}, 0, 5000.f, 5000.f, {duration}, 0, false, 0.f, 3000.f, NoAttack, SpecialStates::NORMAL, SpecialStates::VULNERABLE};

	VulnerableBuffGranter()
	{
		maxHealth = 1;
		currHealth = maxHealth;
		enemyPatterns = {buff};
		sprite = {
			"enemy_Angel.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		patternIndex = 0;
		scale = vec2(0, 0);
		rotatePower = 0.f;
	};
};

struct UnderGroundBuffGranter : Enemy
{
	Reaction duration{
		ReactionType::DURATION,
		0, SpecialStates::INVISIBLE};

	EnemyPattern buff = {"UNDERGROUND", EnemyBehavior::GRANTINGBUFFSAOE, {}, 0, 5000.f, 5000.f, {duration}, 0, false, 0.f, 3000.f, NoAttack, SpecialStates::NORMAL, SpecialStates::UNDERGROUND};

	UnderGroundBuffGranter()
	{
		maxHealth = 1;
		currHealth = maxHealth;
		enemyPatterns = {buff};
		sprite = {
			"enemy_Angel.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		patternIndex = 0;
		scale = vec2(0, 0);
		rotatePower = 0.f;
	};
};

