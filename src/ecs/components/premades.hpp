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
	{0.8, M_PI / 300},
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
	{},
	dashUpA,
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
	RECTANGLE,
	{numBulletsUpA, sizeUpA, dashUpA},
	blunt,
	6,
	M_PI / 6,
	{20, 20},
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
	EnemyPattern patrolState = {"PATROL", EnemyBehavior::PATROLLING, {{100, 400}, {400, 400}, {400, 900}, {100, 900}}, 0, 10000.f, 10000.f, {reactionPlayerClose, reactionIdle}, 0, true, 0, 2000.f, twelveSpiralShot};
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
	};
};


struct EnemyEasySentry : Enemy
{
	EnemyPattern rotateState = {"ROTATE IN PLACE", EnemyBehavior::ROTATE_IN_PLACE, {}, 0, 10000.f, 10000.f, {}, 0, true, 0.f, 5000.f, twelveSpiralShot};
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
		scale = vec2({288.0f / 2, 240.f / 2});
	};
};

struct EnemyBigC : Enemy
{

	EnemyBigC()
	{
		maxHealth = 1000;

		currHealth = maxHealth;

		enemyPatterns = {
			{"ROTATE IN PLACE", EnemyBehavior::ROTATE_IN_PLACE, {}, 0, 10000.f, 10000.f, {}, 0, true, 0.f, 5000.f, twelveSpiralShot}};
		patternIndex = 0;
		sprite = {// registry.sprites.get(entity).sprites[SPRITE_STATE::BASE],
				  "none",
				  EFFECT_ASSET_ID::MESH,
				  GEOMETRY_BUFFER_ID::MESH_GB};
		scale = vec2({700 * (1.923352 / 2.f), 700});
	};
};

struct EnemyMediumCharge : Enemy
{
	Reaction lowHealth = {
		ReactionType::TWENTYFIVE_HEALTH,
		0};

	Reaction durationFollow = {
		ReactionType::DURATION,
		1};

	Reaction durationIdle = {
		ReactionType::DURATION,
		0};
	EnemyPattern idleState = {"IDLE ATTACKING", EnemyBehavior::IDLE, {}, 0, 10000.f, 10000.f, {durationIdle}, 0, true, 0.f, 2000.f, fiveBurst};
	EnemyPattern followState = {"FOLLOW ENEMY", EnemyBehavior::FOLLOW_PLAYER, {}, 0, 10000.f, 10000.f, {lowHealth, durationFollow}, 1, false, 0.f, 0.f, NoAttack};
	EnemyMediumCharge()
	{
		maxHealth = 500;
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
	};
};
