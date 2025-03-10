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

const BulletStackEffect dmgUp = {
	BulletDamage,
	1,
	"Damage Up",
	""};

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
	,{5000, 0}
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
		headData.anchorPoint = { 0.5f, 0.5f };
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
		dashRechargeDown,
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
		dashRechargeDown,
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
		dashRechargeDown,
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
		SpecialStates::INVINCIBLE};
	Reaction repositioned = {
		ReactionType::FINISH_PATROL,
		5 };

	EnemyPattern randomState = {"RANDOM POSITION", EnemyBehavior::RANDOM_FAR, {}, 0, 3000.f, 3000.f, {duration,halfhp}, 1, false, 0.f, 0.f, crabSummon, SpecialStates::INVINCIBLE};
	EnemyPattern idleState = {"IDLE SHOOTING", EnemyBehavior::IDLE, {}, 0, 5000.f, 5000.f, {duration,halfhp}, 2, true, 0.f, 1000.f, radialSquare};
	EnemyPattern walkingRage = {"WALKING RAGE", EnemyBehavior::RANDOM_FAR, {}, 0, 1000.f, 1000.f, {duration,halfhp}, 3, false, 0.f, 0.f, twoPincerShot};
	EnemyPattern shootMisile = {"MISSILE", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {duration, halfhp}, 0, true, 0.f, 500.f, missile};

	EnemyPattern HalfHPRepostion = { "RANDOM POSITION", EnemyBehavior::PATROLLING, {{0.5, 0.5},{0.5, 0.5}}, 0, 1000.f, 1000.f, {repositioned}, 5, true, 0.f, 500.f, radialSquare, SpecialStates::INVINCIBLE };
	EnemyPattern HalfHPLaser = { "Laser Spin", EnemyBehavior::ROTATE_IN_PLACE, {}, 0, 25000.f, 25000.f, {duration}, 6, true, 0.f, 5000.f, broadsideLasers, SpecialStates::INVINCIBLE };

	EnemyPattern RetreatAndShoot = {"RetreatAndShoot", EnemyBehavior::RETREAT, {}, 0, 2000.f, 2000.f, {duration,}, 7, true, 0.f, 75.f,  wave};
	EnemyPattern randomStateHalfHP = {"RANDOM POSITION", EnemyBehavior::RANDOM_FAR, {}, 0, 3000.f, 3000.f, {duration}, 8, true, 0.f, 15000.f, crabSummon, SpecialStates::INVINCIBLE};
	EnemyPattern walkingRageHalfhp = {"WALKING RAGE", EnemyBehavior::RANDOM_FAR, {}, 0, 1000.f, 1000.f, {duration}, 9, true, 0.f, 100.f, twoPincerShot};
	EnemyPattern randomlaserState = {"PatrolBoundary", EnemyBehavior::PATROLLING, {{0.99, 0.01}, {0.99, 0.99}, {0.01, 0.99}, {0.01, 0.01}, {0.99, 0.01}}, 0, 2000.f, 2000.f, {duration}, 6, true, 0.f, 120.f, radialSquarephase2};

	BossChimeraCrab()
	{
		maxHealth = 650;
		currHealth = maxHealth;
		enemyPatterns = {
			randomState, idleState, walkingRage, shootMisile, HalfHPRepostion, HalfHPLaser, RetreatAndShoot, randomStateHalfHP, walkingRageHalfhp, randomlaserState};
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
		speedMultiplier = 4.5f;
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

struct BigC : Enemy
{

	const AttackData FastLaser{
		EnemyAttackPattern::LASER
		,CIRCLE
		,{dashUp, dashRechargeDown}
		,bulletPierceDown
		,1
		,0
		,{70, 70}
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
		,{dashUp, dashRechargeDown}
		,bulletPierceDown
		,1
		,0
		,{70, 70}
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
		maxHealth = 10000;

		currHealth = maxHealth;

		Reaction duration = {
            ReactionType::DURATION,
            1};

		EnemyPattern idleState = {
            "IDLE", EnemyBehavior::ROTATE_IN_PLACE, {}, 0, 15000.f, 15000.f, {duration}, 3, true, 2500, 7000.f, FastLaser};

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
		dashRechargeDown,
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
		6,
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
		dashRechargeDown,
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
		dashRechargeDown,
		8,
		M_PI / 8.f,
		{20, 20},
		0,
		1500,
		{500, 0},
		0,
		0,
		0};


	Reaction duration = {
		ReactionType::DURATION,
		1};
	Reaction halfhp = {
		ReactionType::SEVENTYFIVE_HEALTH,
		4,
		SpecialStates::INVINCIBLE};
	Reaction repositioned = {
		ReactionType::FINISH_PATROL,
		5 };
	Reaction playerClose = {
        ReactionType::PLAYER_CLOSE,
        1};
	Reaction playerCloseHalfHP = {
        ReactionType::PLAYER_CLOSE,
        2};

	EnemyPattern idleState = {
		"IDLE", EnemyBehavior::IDLE, {}, 0, 15000.f, 15000.f, {duration, halfhp, playerClose}, 0, false, 0.f, 1000.f,
		NoAttack
	};

	EnemyPattern PlayerClose = {
		"PlayerClose", EnemyBehavior::IDLE, {}, 0, 3000.f, 3000.f, {duration, halfhp, }, 0, true, 0.f, 3000.f,
		sixShot, SpecialStates::VULNERABLE
	};

	EnemyPattern PlayerCloseHalfHP = {
		"PlayerClose", EnemyBehavior::IDLE, {}, 0, 3000.f, 3000.f, {duration}, 3, false, 2000.f, 500.f, NoAttack, SpecialStates::VULNERABLE
	};

	EnemyPattern PlayerCloseHalfHPLaser = {"PlayerClose", EnemyBehavior::IDLE, {}, 0, 7000.f, 7000.f, {duration}, 5, true, 0.f, 100.f, laserRotate, SpecialStates::INVINCIBLE};

	EnemyPattern randomStateHalfHP = {
		"RANDOM POSITION", EnemyBehavior::IDLE, {}, 0, 5000.f, 5000.f, {duration, playerCloseHalfHP}, 5, true,
		0.f, 1500.f, spawning, SpecialStates::PROTECTED
	};
	EnemyPattern laserState1 = {"ATTACK LASER", EnemyBehavior::IDLE, {}, 0, 2000.f, 2000.f, {duration, playerCloseHalfHP}, 6, true, 0.f, 2000.f, laser1, SpecialStates::VULNERABLE};
	EnemyPattern laserState2 = {"ATTACK LASER", EnemyBehavior::IDLE, {}, 0, 2000.f, 2000.f, {duration, playerCloseHalfHP}, 4, true, 0.f, 2000.f, laser2, SpecialStates::VULNERABLE};

	BossBigCCore()
	{
		maxHealth =900;
		currHealth = maxHealth;
		enemyPatterns = {
			idleState, PlayerClose, PlayerCloseHalfHP, PlayerCloseHalfHPLaser, randomStateHalfHP, laserState1, laserState2};
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
		collisionBullet = playerSpeedUp;
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
		collisionBullet = playerSpeedUp;
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
		collisionBullet = playerSpeedUp;
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
		5};
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
		maxHealth = 800;
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
		EnemyAttackPattern::SHOTGUN,
		TRIANGLE,
		{bulletPierceUp, bulletSpeedUp},
		blunt,
		1,
		0.0,
		{30, 30},
		200,
		3000,
		{400, 0.0},
		0,
		0,
		0};

	// TODO Vincent add code to make rotate to face player, but not move
	EnemyPattern rotateState = {"FACE PLAYER", EnemyBehavior::FOLLOW_PLAYER, {}, 0, 10000.f, 10000.f, {}, 0, true, 0.f, 2000.f, beehivesentry};

	BeehiveGun()
	{
		maxHealth = 300;
		currHealth = maxHealth;

		enemyPatterns = {rotateState};
		speedMultiplier = 0.f;
		rotatePower = 0.5;

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
		dashRechargeDown,
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
		dashRechargeDown,
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
		collisionBullet = playerSpeedUp;
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

	Reaction boid{
		ReactionType::DURATION,
		0};
	EnemyPattern boidState = {"BOID", EnemyBehavior::BOIDSWARMPLAYER, {}, 0, 5000.f, 5000.f, {}, 0, false, 0.f, 0.f, NoAttack};
	MedBoid()
	{
		maxHealth = 1;
		currHealth = maxHealth;
		enemyPatterns = {boidState};
		sprite = {
			"Syringe.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		scale = vec2({25.f, 25.f});
		patternIndex = 0;
		collisionBullet = sizeUp;
	}
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
	}
};

struct BMP : Enemy {
	Reaction AttackLaser{
		ReactionType::DURATION,
		0};
	EnemyPattern heal_state = {"ATTACK LASER", EnemyBehavior::GRANTINGBUFFSAOE, {}, 0, 15000.f, 15000.f, {AttackLaser, }, 1, false, 0.f, 10000.f, NoAttack, SpecialStates::NORMAL, SpecialStates::INVINCIBLE};
	EnemyPattern telePortState = {"TELEPORT", EnemyBehavior::IDLE, {}, 0, 1500.f, 1500.f, {AttackLaser}, 0, true, 0.f, 700.f, twelveSpiralShot, SpecialStates::VULNERABLE, SpecialStates::NORMAL};
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
	};

};

struct RodOfC : Enemy {
	const AttackData whip{
		EnemyAttackPattern::RADIAL,
		RECTANGLE,
		{dmgUp},
		blunt,
		4,
		-M_PI / 4.f,
		{15, 15},
		400,
		2000,
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

	Reaction AttackLaser{
		ReactionType::DURATION,
		0};
	EnemyPattern heal_state = {"ATTACK LASER", EnemyBehavior::GRANTINGBUFFSAOE, {}, 0, 1000.f, 1000.f, {AttackLaser, }, 1, false, 0.f, 10000.f, NoAttack, SpecialStates::VULNERABLE, SpecialStates::REGENERATING};
	EnemyPattern telePortState = {"TELEPORT", EnemyBehavior::RANDOM_NEAR, {}, 0, 1500.f, 1500.f, {AttackLaser}, 2, true, 0.f, 700.f, twelveSpiralShot, };
	EnemyPattern whipState = {"WHIP", EnemyBehavior::IDLE, {}, 0, 2000.f, 2000.f, {AttackLaser}, 3, true, 0.f, 100.f, whip};
	EnemyPattern protection_pulse = {"ATTACK LASER", EnemyBehavior::GRANTINGBUFFSAOE, {}, 0, 1000.f, 1000.f, {AttackLaser, }, 4, false, 0.f, 10000.f, NoAttack, SpecialStates::VULNERABLE, SpecialStates::PROTECTED};
	EnemyPattern Spawning = {"TELEPORT", EnemyBehavior::RANDOM_NEAR, {}, 0, 1500.f, 1500.f, {AttackLaser}, 0, true, 0.f, 700.f, Spawn, SpecialStates::PROTECTED, SpecialStates::NORMAL};
	RodOfC()
	{
		maxHealth = 800;
		currHealth = maxHealth;
		enemyPatterns = {heal_state, telePortState, whipState, protection_pulse, Spawning};
		sprite = {
			"RodOfCaduceus.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		patternIndex = 0;
		scale = vec2(912, 240) / 1.5f;
		rotatePower = 0.f;
		armour = 3;
	};

};

struct RodOfA : Enemy {
	Reaction AttackLaser{
		ReactionType::DURATION,
		0};
	EnemyPattern heal_state = {"ATTACK LASER", EnemyBehavior::GRANTINGBUFFSAOE, {}, 0, 1000.f, 1000.f, {AttackLaser, }, 1, false, 0.f, 10000.f, NoAttack, SpecialStates::NORMAL, SpecialStates::REGENERATING};
	EnemyPattern telePortState = {"TELEPORT", EnemyBehavior::IDLE, {}, 0, 1500.f, 1500.f, {AttackLaser}, 2, true, 0.f, 700.f, twelveSpiralShot, SpecialStates::UNDERGROUND, SpecialStates::NORMAL};
	EnemyPattern chargingState = {"CHARGE", EnemyBehavior::CHARGING, {}, 0, 500.f, 500.f, {{ReactionType::DURATION, 2}}, 0, true, 50.f, 50.f, fourAllAround};

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


//----------------------------------------- MINING REGION ENEMIES ---------------------------------
struct SmallMole : Enemy
{
	const AttackData dirtBlast{
		EnemyAttackPattern::BURST,
		CIRCLE,
		{dmgUp},
		playerSpeedDown,
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

	Reaction playerClose = {
		ReactionType::PLAYER_REALLY_CLOSE,
		1 };

	Reaction bulletClose = {
		ReactionType::PLAYER_BULLET_CLOSE,
		1 };

	EnemyPattern waitingState = { "ROLLING", EnemyBehavior::IDLE, {}, 0, 1000000.f, 1000000.f, {playerClose, bulletClose}, 0, false, 0.f, 5000.f, quadShot };
	EnemyPattern explodingState = { "ROLLING", EnemyBehavior::DEATHSTATE, {}, 0, 1000000.f, 1000000.f, {}, 0, true, 0.f, 5000.f, blowup };

	Dynamite()
	{
		maxHealth = 40;
		currHealth = maxHealth;

		enemyPatterns = { waitingState, explodingState };

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
	Reaction gotTo = {
		ReactionType::FINISH_PATROL,
		1 };

	Reaction gotTo2 = {
		ReactionType::FINISH_PATROL,
		2 };


	EnemyPattern startState = { "LOOP", EnemyBehavior::WORM_GOTO, { vec2(0.8,0.2) }, 0, 1000000.f, 1000000.f, {gotTo}, 0, false, 0.f, 5000.f, quadShot };
	EnemyPattern loopState = { "LOOP", EnemyBehavior::WORM_PATROL, { vec2(0.8,0.2), vec2(0.8,0.8), vec2(0.2, 0.8), vec2(0.2,0.2), vec2(0.8,0.2) }, 0, 1000000.f, 1000000.f, {gotTo2}, 0, false, 0.f, 5000.f, quadShot };
	EnemyPattern followState = { "LOOP", EnemyBehavior::WORM_FOLLOW, {}, 0, 1000000.f, 1000000.f, {gotTo2}, 0, false, 0.f, 5000.f, quadShot };

	DrillWormHead()
	{
		maxHealth = 5000;
		currHealth = maxHealth;

		enemyPatterns = { startState, loopState, followState };

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
		rotationBehaviour = EnemyRotationBehavior::WORM;

		headData.size = 10;
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

	EnemyPattern state = { "IDLE", EnemyBehavior::WORM_BODY, {}, 0, 1000000.f, 1000000.f, {}, 0, true, 0.f, 2500.f, dualShot };

	DrillWormBody()
	{
		maxHealth = 100;
		currHealth = maxHealth;

		enemyPatterns = { state };

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


//----------------------------------------- PHYSICS REGION ENEMIES ---------------------------------
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
	EnemyPattern randomState = {"Laser active", EnemyBehavior::IDLE, {}, 0, 0.f, 700.f, {{ReactionType::DURATION, 0}}, 1, true, 0.f, 700.f, FastLaser};
	EnemyPattern Teleport = {"Charge", EnemyBehavior::TELEPORT, {}, 0, 300.f, 300.f, {{ReactionType::DURATION, 0}}, 2, true, 0.f, 350.f, twelveSpiralShot};
	EnemyPattern CoolDown = {"CoolDown", EnemyBehavior::IDLE, {}, 0, 0.f, 4000.f, {{ReactionType::DURATION, 0}}, 0, false, 0.f, 5000.f, NoAttack};

	HifiLaserSniperHard()
	{
		maxHealth = 150;
		currHealth = maxHealth;
		enemyPatterns = {randomState, Teleport, CoolDown};
		patternIndex = 0;
		sprite = {
			"enemy_hifi_007_alt1.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE};
		scale = vec2({160.0f / 2, 160.f / 2});
		rotatePower = 0.6f;
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
		{numBulletsUp, dashRechargeDown},
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
		{bulletBounceDown, bulletBounceUp},
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
		6};

	Reaction shieldBreak{
		ReactionType::SHIELDBREAK,
		7};

	Reaction secondPhase{
		ReactionType::FIFTY_HEALTH,
		9,
	SpecialStates::INVINCIBLE};

	Reaction secondPhasePlayerClose{
		ReactionType::PLAYER_CLOSE,
		13};

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

	const AttackData spawnBeeBoids{
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
		EnemyType::EnemyBioBoid,
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
		EnemyAttackPattern::SHOTGUN,
		TRIANGLE,
		{dmgDown},
		dmgDown,
		1,
		M_PI,
		{20, 20},
		800,
		3000,
		{0, 0},
		0,
		0,
		0};

	const AttackData laserBoss{
		EnemyAttackPattern::LASER,
		CIRCLE,
		{bulletPierceUp},
		dashRechargeDown,
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
		30,
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

	EnemyPattern spawnLaserHorizontalState = {"SHOOT HORIZONTAL", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {duration, spawningBeeBoid, shieldBreak, secondPhase}, 1, true, 0.f, 1000.f, spawnLaserHorizontal};
	EnemyPattern spawnLaserVerticalState = {"SHOOT VERTICAL", EnemyBehavior::IDLE, {}, 0, 4000.f, 4000.f, {duration, spawningBeeBoid, shieldBreak, secondPhase}, 2, true, 0.f, 4000.f, spawnLaserVertical};
	EnemyPattern spawnHomingState = {"SHOOT HOMING", EnemyBehavior::IDLE, {}, 0, 5000.f, 5000.f, {duration, spawningBeeBoid, shieldBreak, secondPhase}, 3, true, 0.f, 500.f, ScientistHomingShot};
	EnemyPattern spawnBoidState = {"SHOOT BOID", EnemyBehavior::IDLE, {}, 0, 5000.f, 5000.f, {duration, spawningBeeBoid, shieldBreak, secondPhase}, 4, true, 0.f, 2500.f, spawnBoids};
	EnemyPattern spawnLaserRotateState = {"SHOOT LASER", EnemyBehavior::IDLE, {}, 0, 7000.f, 7000.f, {duration, spawningBeeBoid, shieldBreak, secondPhase}, 5, true, 0.f, 7000.f, spawnLaserRotating};
	EnemyPattern spawnRadialState = {"SHOOT RADIAL", EnemyBehavior::IDLE, {}, 0, 4000.f, 4000.f, {duration, spawningBeeBoid, shieldBreak, secondPhase}, 0, true, 0.f, 4000.f, spawnTurret};
	EnemyPattern spawnBeeBoidState = {"YOU ARE CLOSE", EnemyBehavior::IDLE, {}, 0, 1000.f, 1000.f, {duration, shieldBreak, secondPhase}, 0, true, 0.f, 1000.f, spawnBeeBoids};
	EnemyPattern vunerableState = {"VURNERABLE", EnemyBehavior::IDLE, {}, 0, 10000.f, 10000.f, {duration, secondPhase}, 8, false, 0.f, 0.f, NoAttack};
	EnemyPattern spawnShieldState = {"Shield", EnemyBehavior::IDLE, {}, 0, 2000.f, 2000.f, {duration, shieldBreak, secondPhase}, 0, true, 0.f, 100.f, spawnShield};
	EnemyPattern secondPhaseState = {"SECOND PHASE", EnemyBehavior::TELEPORT, {{0.5f, 0.5f}}, 0, 2000.f, 2000.f, {duration}, 10, false, 0.f, 0.f, NoAttack};
	EnemyPattern spawnHandState = {"SUMMON HAND", EnemyBehavior::IDLE, {}, 0, 500.f, 500.f, {duration}, 11, true, 0.f, 5000.f, spawnHand};
	EnemyPattern patrollingState1 = {"RANDOM", EnemyBehavior::RANDOM, {{0.2, 0.8}, {0.2, 0.2}}, 0, 10000.f, 10000.f, {duration}, 12, true, 0.f, 5000.f, lethalShot};
	EnemyPattern patrollingState2 = {"RANDOM", EnemyBehavior::RANDOM, {{0.5, 0.5}, {0.7, 0.7}}, 0, 5000.f, 5000.f, {duration}, 13, true, 0.f, 5000.f, spawnExplosive};
	EnemyPattern teleportState = {"TELEPORT", EnemyBehavior::TELEPORT, {{0.5, 0.5}}, 0, 3000.f, 3000.f, {duration}, 11, true, 0.f, 3000.f, boidFishSummon};

	ScientistBossEnemy()
	{
		maxHealth = 2000;
		currHealth = 2000;
		enemyPatterns = {spawnLaserHorizontalState, spawnLaserVerticalState, spawnHomingState,
						 spawnBoidState, spawnLaserRotateState,
						 spawnRadialState, spawnBeeBoidState,
						 vunerableState, spawnShieldState, secondPhaseState,
						 spawnHandState,
						 patrollingState1,
						 patrollingState2, teleportState};
		sprite = {
			"scientist.png",
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE,
		};
		patternIndex = 8;
		scale = vec2(250, 250);
		rotatePower = 0.f;
		armour = 10;
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
		maxHealth = 200;
		currHealth = 200;
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

	Reaction deathTransition{
		ReactionType::DURATION,
		1};
	EnemyPattern RotateState = {"SHOOT", EnemyBehavior::IDLE, {}, 0, 5000.f, 5000.f, {deathTransition}, 1, true, 0.f, 2500.f, radialScientistBurst};
	EnemyPattern DeathState = {"REMOVE ME", EnemyBehavior::DEATHSTATE, {}, 0, 1000.f, 1000.f, {deathTransition}, 1, false, 0.f, 0.f, NoAttack};
	InvisibleTurretEnemy()
	{
		maxHealth = 1;
		currHealth = 1;
		enemyPatterns = {RotateState, DeathState};
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
		dashRechargeDown,
		8,
		0,
		{0, 20},
		0,
		8000,
		{10, M_PI / 600},
		0,
		0,
		0};
	Reaction deathTransition{
		ReactionType::DURATION,
		1};
	EnemyPattern laserRotateState = {"LASER", EnemyBehavior::IDLE, {}, 0, 7000.f, 7000.f, {deathTransition}, 1, true, 0.f, 7000.f, laserRotateBoss};
	EnemyPattern DeathState = {"REMOVE ME", EnemyBehavior::DEATHSTATE, {}, 0, 1000.f, 1000.f, {deathTransition}, 1, false, 0.f, 0.f, NoAttack};
	InvisibleRotateLaserEnemy()
	{
		maxHealth = 1;
		currHealth = 1;
		enemyPatterns = {laserRotateState, DeathState};
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
		dashRechargeDown,
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
		dashRechargeDown,
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

	Reaction deathTransition{
		ReactionType::DURATION,
		1};
	EnemyPattern ShootState = {"LASER", EnemyBehavior::IDLE, {}, 0, 5000.f, 5000.f, {deathTransition}, 1, true, 0.f, 2500.f, ScientistCannonShot};
	EnemyPattern DeathState = {"REMOVE ME", EnemyBehavior::DEATHSTATE, {}, 0, 1000.f, 1000.f, {deathTransition}, 1, false, 0.f, 0.f, NoAttack};
	InvisibleExplosiveEnemy()
	{
		maxHealth = 1;
		currHealth = 1;
		enemyPatterns = {ShootState, DeathState};
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

	Reaction deathTransition{
		ReactionType::DURATION,
		1};
	EnemyPattern ShootState = {"LASER", EnemyBehavior::IDLE, {}, 0, 5000.f, 5000.f, {deathTransition}, 1, true, 0.f, 600.f, ScientistHomingPiercingShot};
	EnemyPattern DeathState = {"REMOVE ME", EnemyBehavior::DEATHSTATE, {}, 0, 1000.f, 1000.f, {deathTransition}, 1, false, 0.f, 0.f, NoAttack};
	InvisibleHomingEnemy()
	{
		maxHealth = 1;
		currHealth = 1;
		enemyPatterns = {ShootState, DeathState};
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
	EnemyPattern chargePlayer = {"CHARGE", EnemyBehavior::CHARGING, {}, 0, 3000.f, 3000.f, {duration}, 2, false, 0.f, 0.f, NoAttack};
	EnemyPattern idling2 = {"IDLE", EnemyBehavior::FOLLOWSCIENTIST, {}, 0, 7000.f, 7000.f, {duration}, 3, false, 0.f, 0.f, NoAttack, SpecialStates::INVINCIBLE};
	EnemyPattern BombingState = {"PATROL", EnemyBehavior::IDLE, {{0.5, 0.5}}, 0, 6000.f, 6000.f, {duration}, 4, true, 1000.f, 1000.f, HandBomb};
	EnemyPattern idling3 = {"IDLE", EnemyBehavior::FOLLOWSCIENTIST, {}, 0, 7000.f, 7000.f, {duration}, 5, false, 0.f, 0.f, NoAttack, SpecialStates::INVINCIBLE};
	EnemyPattern teleportToScientist2 = {"IDLE", EnemyBehavior::IDLE, {}, 4, 1000.f, 1000.f, {duration}, 6, true, 500.f, 1000.f, quakeShot};
	EnemyPattern laserAttackPrepareState = {"PATROL", EnemyBehavior::TELEPORT, {{1, 0.1}}, 0, 2000.f, 2000.f, {duration}, 7, false, 1000.f, 1000.f, NoAttack};
	EnemyPattern patrolLaserState = {"PATROLLING", EnemyBehavior::PATROLLING, {{1, 0.1}, {1, 0.95}}, 0, 15000.f, 15000.f, {duration}, 8, true, 0.f, 15000.f, laserOne};
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
		maxHealth = 800;
		currHealth = 800;
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
		collisionBullet = {
			Pop,
			0,
			"Pop the stack",
			""
		};
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
		maxHealth = 500;
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