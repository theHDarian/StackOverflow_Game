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
	"Inert",
	""};

const BulletStackEffect lightning1 = {
	Lightning,
	Additive,
	0,
	"Lightning",
	""};

const BulletStackEffect lightning2 = {
	Lightning,
	Multiplicative,
	0,
	"Lightning",
	""};

const BulletStackEffect dmgDownA = {
	BulletDamage,
	Additive,
	-2.0,
	"Damage Down (+)",
	""};

const BulletStackEffect dmgDownM = {
	BulletDamage,
	Multiplicative,
	-0.3,
	"Damage Down (%)",
	""};

const BulletStackEffect dmgUpA = {
	BulletDamage,
	Additive,
	5.0,
	"Damage Up (+)",
	""};

const BulletStackEffect dmgUpM = {
	BulletDamage,
	Multiplicative,
	0.5,
	"Damage Up (%)",
	""};

const BulletStackEffect numBulletsUpA = {
	BulletNum,
	Additive,
	1,
	"Bullet Count Up (+)",
	""};

const BulletStackEffect sizeUpA = {
	ProjectileSize,
	Additive,
	10,
	"Bullet Size Up (+)",
	""};
const BulletStackEffect sizeUpM = {
	ProjectileSize,
	Multiplicative,
	0.5,
	"Bullet Size Up (+)",
	""};

const BulletStackEffect spreadUpA = {
	BulletSpread,
	Additive,
	15,
	"Bullet Spread Up (+)",
	""};

const BulletStackEffect bulletSpeedUpA = {
	ProjectileSpeed,
	Additive,
	50,
	"Bullet Speed Up (+)",
	""};

const BulletStackEffect bulletSpeedUpM = {
	ProjectileSpeed,
	Multiplicative,
	0.5,
	"Bullet Speed Up (%)",
	""};

const BulletStackEffect bulletRangeUpA = {
	BulletRange,
	Additive,
	500,
	"Range Up (+)",
	""};

const BulletStackEffect bulletRangeUpM = {
	BulletRange,
	Multiplicative,
	0.5,
	"Range Up (%)",
	""};

const BulletStackEffect bulletBurstUpA = {
	BulletBurst,
	Additive,
	1,
	"Burst Up (+)",
	""};

const BulletStackEffect bulletBurstUpM = {
	BulletBurst,
	Multiplicative,
	.5,
	"Burst Up (%)",
	""};

const BulletStackEffect bulletPierceUpA = {
	Pierce,
	Additive,
	1,
	"Pierce Up (+)",
	""};
const BulletStackEffect bulletPierceUpAII = {
	Pierce,
	Additive,
	2,
	"Pierce Up II (+)",
	""};

const BulletStackEffect bulletPierceUpM = {
	Pierce,
	Multiplicative,
	0.5,
	"Pierce Up (%)",
	""};

const BulletStackEffect bulletBounceUpA = {
	Bounce,
	Additive,
	1,
	"Bounce Up (+)",
	""};

const BulletStackEffect bulletBounceUpM = {
	Bounce,
	Multiplicative,
	1,
	"Bounce Up (%)",
	""};

const BulletStackEffect dashUpA = {
	PlayerNumDash,
	Additive,
	1,
	"Dash Count Up (+)",
	""};

const BulletStackEffect dashUpM = {
	PlayerNumDash,
	Multiplicative,
	1,
	"Dash Count Up (%)",
	""};

const BulletStackEffect stackSizeUpA = {
	PlayerStackSize,
	Additive,
	4,
	"Stack Size Up (+)",
	""};

const BulletStackEffect stackSizeUpM = {
	PlayerStackSize,
	Multiplicative,
	.5,
	"Stack Size Up (%)",
	""};

const BulletStackEffect dashCDRUpA = {
	PlayerDashCDR,
	Additive,
	500,
	"Dash Cooldown Up (+)",
	""};

const BulletStackEffect dashCDRUpM = {
	PlayerDashCDR,
	Multiplicative,
	.25,
	"Dash Cooldown Up (%)",
	""};

const BulletStackEffect dashCDRDownA = {
	PlayerDashCDR,
	Additive,
	-150,
	"Dash Cooldown Down (+)",
	""};

const BulletStackEffect dashCDRDownM = {
	PlayerDashCDR,
	Multiplicative,
	-0.5,
	"Dash Cooldown Down (%)",
	""};

const BulletStackEffect playerSpeedUpA = {
	PlayerSpeed,
	Additive,
	100,
	"Movement Speed Up (+)",
	""};

const BulletStackEffect playerSpeedUpM = {
	PlayerSpeed,
	Multiplicative,
	.1,
	"Movement Speed Up (%)",
	""};

const BulletStackEffect playerSpeedDownA = {
	PlayerSpeed,
	Additive,
	-100,
	"Movement Speed Down (+)",
	""};

const BulletStackEffect playerSpeedDownM = {
	PlayerSpeed,
	Multiplicative,
	-0.2,
	"Movement Speed Down (%)",
	""};

const BulletStackEffect fireRateUpA = {
	FireRate,
	Additive,
	1.5,
	"Fire Rate Up (+)",
	""};

const BulletStackEffect fireRateUpM = {
	FireRate,
	Multiplicative,
	.2,
	"Fire Rate Up (%)",
	""};
const BulletStackEffect fireRateDownM = {
	FireRate,
	Multiplicative,
	-0.2,
	"Fire Rate Down (%)",
	""};

const BulletStackEffect ostrichWarrior = {
	PlayerSpeed,
	Multiplicative,
	2,
	"Ostrich Warrior",
	""};

const BulletStackEffect ostrichWarriorDownside = {
	PlayerDashCDR,
	Multiplicative,
	2,
	"Ostrich Warrior Downside",
	""};

const BulletStackEffect key = {
	Key,
	Additive,
	0,
	"Key",
	""};

const BulletStackEffect homingUpA = {
	Homing,
	Additive,
	0.1,
	"Homing Accuracy (+)",
	""};

const BulletStackEffect buzz = {
	PlayerSpeed,
	Additive,
	15,
	"Buzz",
	""};

const BulletStackEffect sluggish = {
	ProjectileSpeed,
	Additive,
	-20,
	"Sluggish Bullets",
	""};

const BulletStackEffect hardShell = {
	Bounce,
	Additive,
	1,
	"Hard Shell",
	""};

const BulletStackEffect APRounds = {
	Pierce,
	Additive,
	1,
	"AP Rounds",
	""};

const BulletStackEffect ConcentratedFire = {
	BulletNum,
	Additive,
	-1,
	"Concentrated Fire",
	""};

const BulletStackEffect SniperPower = {
	BulletDamage,
	Multiplicative,
	1.5,
	"Sniper's Prowess",
	""};

const BulletStackEffect SniperSpeed = {
	ProjectileSpeed,
	Additive,
	500,
	"Sniper's Speed",
	""};

const BulletStackEffect SniperLethargy = {
	FireRate,
	Additive,
	-750,
	"Sniper's Lethargy",
	""};

const BulletStackEffect SniperBurden = {
	PlayerSpeed,
	Multiplicative,
	-0.6,
	"Sniper's Burden",
	""};

const BulletStackEffect DataCompression = {
	PlayerStackSize,
	Multiplicative,
	.5,
	"Data Compression",
	""};

const BulletStackEffect WarMachine = {
	BulletBurst,
	Additive,
	2,
	"War Machine",
	""};

const BulletStackEffect WeaponOfWar = {
	Homing,
	Additive,
	1,
	"Weapon of War",
	""};

const BulletStackEffect theCurse = {
	Homing,
	Additive,
	-0.5,
	"\"The Curse\"",
	""};

const BulletStackEffect Freedom = {
	PlayerSpeed,
	Multiplicative,
	1.5,
	"\"Freedom\"",
	""};

const BulletStackEffect serum = {
	ProjectileSize,
	Additive,
	2,
	"Unknown Serum",
	""};

// note: adding the effect to list is not necessary
// but guarantees it will be tokenized on game load
const std::vector<BulletStackEffect> premadeBullets = {
	blunt,
	dmgDownA,
	dmgDownM,
	dmgUpA,
	dmgUpM,
	numBulletsUpA,
	sizeUpA,
	spreadUpA,
	bulletSpeedUpA,
	bulletSpeedUpM,
	bulletRangeUpA,
	bulletRangeUpM,
	bulletBurstUpA,
	bulletBurstUpM,
	bulletPierceUpA,
	bulletPierceUpM,
	bulletBounceUpA,
	bulletBounceUpM,
	dashUpA,
	dashUpM,
	stackSizeUpA,
	stackSizeUpM,
	dashCDRUpA,
	dashCDRUpM,
	dashCDRDownA,
	dashCDRDownM,
	playerSpeedUpA,
	playerSpeedUpM,
	ostrichWarrior,
	ostrichWarriorDownside,
	key,
	homingUpA,
	buzz,
	sluggish,
	hardShell,
	APRounds,
	ConcentratedFire,
	SniperPower,
	SniperSpeed,
	SniperLethargy,
	SniperBurden,
	DataCompression,
	SniperBurden,
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
	int numBullets = 1;
	int angleOffset = 0;
	vec2 size = { 20,20 };
	float speed = 200;
	float bulletRange = 3000;
	vec2 veer = { 0,0 }; // {magnitude, angle}, {growth, rotation} lasers, {#bullets per shot, burst CD} radial burst
	int bulletPierce = 0;
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
	{dmgUpA},
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
	{numBulletsUpA, dmgDownA},
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
	{dashUpA},
	dashCDRDownA,
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
	{APRounds},
	dashCDRDownA,
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
	{dashCDRDownM},
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
	{spreadUpA, sizeUpA},
	dmgDownA,
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
	{dmgUpA},
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
	{numBulletsUpA, spreadUpA},
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
	{dmgUpA},
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
	{APRounds, dmgUpM, ConcentratedFire},
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
	,{dashUpA, dashCDRUpA}
	,dashCDRDownM
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
		dashUpA,
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
		{hardShell},
		sluggish,
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
	};
};

struct EvilSnail : Enemy
{
	const AttackData snailTrail{
		EnemyAttackPattern::TRAIL,
		CIRCLE,
		{hardShell},
		sluggish,
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
	};
};

struct Crab : Enemy
{

	const AttackData twoPincerShot{
		EnemyAttackPattern::SHOTGUN,
		TRIANGLE,
		{hardShell},
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
	};
};

struct LaserCrab : Enemy
{

	const AttackData crabLaser{
		EnemyAttackPattern::LASER,
		CIRCLE,
		{APRounds},
		dashCDRDownA,
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
	};
};

struct EvilCrab : Enemy
{

	const AttackData radialSquare{
		EnemyAttackPattern::RADIAL_POLYGON,
		CIRCLE,
		{sizeUpA},
		sluggish,
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
	};
};

struct BossChimeraCrab : Enemy {
	const AttackData crabLaser{
		EnemyAttackPattern::LASER,
		CIRCLE,
		{APRounds},
		dashCDRDownA,
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
		{APRounds},
		dashCDRDownA,
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
		{sizeUpA},
		sluggish,
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
		{sizeUpA},
		spreadUpA,
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
		{hardShell},
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
		{numBulletsUpA},
		playerSpeedDownA,
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
		{numBulletsUpA},
		playerSpeedDownA,
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
		{APRounds},
		dashCDRDownA,
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
		{spreadUpA, sizeUpA},
		dmgDownA,
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
	};
};

struct chimeraCrabSniper : Enemy
{
	EnemyPattern randomState = {"Follow", EnemyBehavior::FOLLOW_PLAYER, {{0.99, 0.01}, {0.99, 0.99}, {0.01, 0.99}, {0.01, 0.01}, {0.99, 0.01}}, 0, 0.f, 150.f, {{ReactionType::DURATION, 0}}, 0, true, 0.f, 5000.f, FastLaser};
	EnemyPattern charging = {"Charge", EnemyBehavior::FOLLOWSCIENTIST, {}, 0, 0.f, 30000.f, {{ReactionType::DURATION, 0}}, 0, true, 0.f, 5000.f, NoAttack};

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
		rotatePower = 90.0f;
		speedMultiplier = 1.3;
		rotationBehaviour = EnemyRotationBehavior::FACE_PLAYER;
	};
};

struct BigC : Enemy
{

	BigC()
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
		rotatePower = 1.f;
		scale = vec2({700, 700 * (1.998858f / 1.923352f)});
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
		collisionBullet = buzz;
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
		{numBulletsUpA, dmgDownA},
		sluggish,
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
		{numBulletsUpA, dmgDownA},
		buzz,
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
		{numBulletsUpA, dmgDownA},
		sluggish,
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
	};
};

struct BeehiveGun : Enemy
{

	const AttackData beehivesentry{
		EnemyAttackPattern::SHOTGUN,
		TRIANGLE,
		{APRounds, bulletSpeedUpA},
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
		{dashUpA},
		dashCDRDownA,
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
		{dashUpA},
		dashCDRDownA,
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
		rotatePower = 1.0f;
		rotationBehaviour = EnemyRotationBehavior::FACE_PLAYER;
	}
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
		collisionBullet = buzz;
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
		collisionBullet = serum;
	}
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
	EnemyPattern randomState = {"PatrolBoundary", EnemyBehavior::PATROLLING, {{0.99, 0.01}, {0.99, 0.99}, {0.01, 0.99}, {0.01, 0.01}, {0.99, 0.01}}, 0, 0.f, 100.f, {{ReactionType::DURATION, 0}}, 0, true, 0.f, 5000.f, FastLaser};
	EnemyPattern charging = {"Charge", EnemyBehavior::CHARGING, {}, 0, 0.f, 30000.f, {{ReactionType::DURATION, 0}}, 0, true, 0.f, 5000.f, NoAttack};

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
		rotatePower = 90.0f;
		speedMultiplier = 1.3;
		rotationBehaviour = EnemyRotationBehavior::FACE_PLAYER;
	};
};

struct HifiSniper : Enemy
{
	const AttackData HifiSniperShot{
		EnemyAttackPattern::BURST,
		TRIANGLE,
		{bulletPierceUpAII, bulletSpeedUpM},
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
		{numBulletsUpA, playerSpeedDownM},
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
		rotatePower = 90.f;
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
		{APRounds, dmgUpM, bulletRangeUpM},
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
		{numBulletsUpA, spreadUpA},
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
		rotatePower = 90.f;
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
		rotatePower = 90.f;
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
		rotatePower = 90.f;
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
		buzz,
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
		{numBulletsUpA, dmgDownM},
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
		buzz,
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
		{numBulletsUpA, dmgDownM},
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
	const BulletStackEffect playerSpeedDownASmall = {
		PlayerSpeed,
		Additive,
		-30,
		"Movement Speed Down Small (+)",
		""};

	const AttackData spiral{
		EnemyAttackPattern::RADIAL,
		TRIANGLE,
		{numBulletsUpA, playerSpeedDownASmall},
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
		{numBulletsUpA, dashCDRDownA},
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
	const BulletStackEffect playerSpeedDownASmall = {
		PlayerSpeed,
		Additive,
		-30,
		"Movement Speed Down Small (+)",
		""};

	const AttackData spiral{
		EnemyAttackPattern::RADIAL,
		RECTANGLE,
		{APRounds},
		sluggish,
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
	const BulletStackEffect playerSpeedDownASmall = {
		PlayerSpeed,
		Additive,
		-30,
		"Movement Speed Down Small (+)",
		""};

	const AttackData launch{
		EnemyAttackPattern::RADIAL,
		CIRCLE,
		{bulletBounceUpM, bulletBounceUpA},
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
		{dmgUpA},
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
		{SniperPower, SniperSpeed, sizeUpM},
		{fireRateDownM},
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
		{numBulletsUpA, bulletSpeedUpM},
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
		rotatePower = 0.5f;
		rotationBehaviour = EnemyRotationBehavior::FACE_PLAYER;
		speedMultiplier = 0.3;
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
		{SniperPower, SniperSpeed},
		{playerSpeedDownM},
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
		rotatePower = 0.5f;
		rotationBehaviour = EnemyRotationBehavior::FACE_PLAYER;
		speedMultiplier = 0.3;
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
		buzz,
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
		{numBulletsUpA, dmgDownM},
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
		{sizeUpA},
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
		{sizeUpA},
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
		{sizeUpA},
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
		{sizeUpA},
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
		{sizeUpA},
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
		{dashCDRUpA},
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
		{dashCDRUpA},
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
		{dmgDownM},
		dmgDownA,
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
		{APRounds},
		dashCDRDownA,
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
		{numBulletsUpA},
		playerSpeedDownA,
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
	}
};

struct InvisibleTurretEnemy : Enemy
{

	const AttackData radialScientistBurst{
		EnemyAttackPattern::SHOTGUN,
		CIRCLE,
		{numBulletsUpA, dmgDownA},
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
		{APRounds},
		dashCDRDownA,
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
		{dashUpA},
		dashCDRDownA,
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
		{dashUpA},
		dashCDRDownA,
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
		{bulletBounceUpM},
		{bulletPierceUpA},
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
		{numBulletsUpA, dmgDownA},
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
		{numBulletsUpA, dmgDownA},
		dmgDownA,
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
		{numBulletsUpA},
		playerSpeedDownA,
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
		{dmgDownM},
		playerSpeedUpA,
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
		{numBulletsUpA},
		dmgDownA,
		1,
		M_PI / 2,
		{75, 75},
		150,
		2000,
		{99, M_PI / 200},
		0,
		0,
		0};

	EnemyPattern idling = {"IDLE", EnemyBehavior::FOLLOWSCIENTIST, {}, 0, 7000.f, 7000.f, {duration}, 1, false, 0.f, 0.f, NoAttack};
	EnemyPattern chargePlayer = {"CHARGE", EnemyBehavior::CHARGING, {}, 0, 3000.f, 3000.f, {duration}, 2, false, 0.f, 0.f, NoAttack};
	EnemyPattern idling2 = {"IDLE", EnemyBehavior::FOLLOWSCIENTIST, {}, 0, 7000.f, 7000.f, {duration}, 3, false, 0.f, 0.f, NoAttack};
	EnemyPattern BombingState = {"PATROL", EnemyBehavior::IDLE, {{0.5, 0.5}}, 0, 6000.f, 6000.f, {duration}, 4, true, 1000.f, 1000.f, HandBomb};
	EnemyPattern idling3 = {"IDLE", EnemyBehavior::FOLLOWSCIENTIST, {}, 0, 7000.f, 7000.f, {duration}, 5, false, 0.f, 0.f, NoAttack};
	EnemyPattern teleportToScientist2 = {"IDLE", EnemyBehavior::IDLE, {}, 4, 1000.f, 1000.f, {duration}, 6, true, 500.f, 1000.f, quakeShot};
	EnemyPattern laserAttackPrepareState = {"PATROL", EnemyBehavior::TELEPORT, {{1, 0.1}}, 0, 2000.f, 2000.f, {duration}, 7, false, 1000.f, 1000.f, NoAttack};
	EnemyPattern patrolLaserState = {"PATROLLING", EnemyBehavior::PATROLLING, {{1, 0.1}, {1, 0.95}}, 0, 15000.f, 15000.f, {duration}, 8, true, 0.f, 15000.f, laserOne};
	EnemyPattern teleportToScientist3 = {"IDLE", EnemyBehavior::IDLE, {}, 4, 1000.f, 1000.f, {duration}, 9, true, 500.f, 1000.f, quakeShot};
	EnemyPattern idling4 = {"IDLE", EnemyBehavior::FOLLOWSCIENTIST, {}, 0, 7000.f, 7000.f, {duration}, 10, false, 0.f, 0.f, NoAttack};
	EnemyPattern SprayingState = {"PATROL", EnemyBehavior::PATROLLING, {{0.5, 0.5}}, 4, 6000.f, 6000.f, {duration}, 11, true, 1000.f, 1000.f, sprayingShot};
	EnemyPattern teleportToScientist4 = {"IDLE", EnemyBehavior::IDLE, {}, 4, 1000.f, 1000.f, {duration}, 12, true, 500.f, 1000.f, quakeShot};
	EnemyPattern idling5 = {"IDLE", EnemyBehavior::FOLLOWSCIENTIST, {}, 0, 7000.f, 7000.f, {duration}, 13, false, 0.f, 0.f, NoAttack};
	EnemyPattern sprayHomingState = {"SHOOT", EnemyBehavior::IDLE, {}, 0, 6000.f, 6000.f, {duration}, 14, true, 0.f, 3000.f, sprayHoming};
	EnemyPattern idling6 = {"IDLE", EnemyBehavior::FOLLOWSCIENTIST, {}, 0, 7000.f, 7000.f, {duration}, 15, false, 0.f, 0.f, NoAttack};
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
			Additive,
			0,
			"Pop the stack",
			""
		};
	};
};

// struct EvilSkull : {

// 	EvilSkull() {

// 	};
// };
