#pragma once
#include "actor_components.hpp"

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
	""
};

const BulletStackEffect dmgDownA = {
	BulletDamage,
	Additive,
	-2.0,
	"dmgDownA",
	""
};

const BulletStackEffect dmgDownM = {
	BulletDamage,
	Multiplicative,
	0.7,
	"dmgDownM",
	""
};

const BulletStackEffect numBulletsUpA = {
	BulletNum,
	Additive,
	1,
	"numBulletsUpA",
	""
};

const BulletStackEffect sizeUpA = {
	ProjectileSize,
	Additive,
	10,
	"sizeUpA",
	""
};

const BulletStackEffect spreadUpA = {
	BulletSpread,
	Additive,
	15,
	"spreadUpA",
	""
};

const BulletStackEffect bulletSpeedUpA = {
    ProjectileSpeed,
    Additive,
    50,
    "speedUpA",
    ""
};

const BulletStackEffect bulletSpeedUpM = {
    ProjectileSpeed,
    Multiplicative,
    1.5,
    "speedUpM",
    ""
};

const BulletStackEffect bulletRangeUpA = {
    BulletRange,
    Additive,
    500,
    "rangeUpA",
    ""
};

const BulletStackEffect bulletRangeUpM = {
    BulletRange,
    Multiplicative,
    1.5,
    "rangeUpM",
    ""
};

const BulletStackEffect bulletBurstUpA = {
    BulletBurst,
    Additive,
    1,
    "burstUpA",
    ""
};

const BulletStackEffect bulletBurstUpM = {
    BulletBurst,
    Multiplicative,
    1.5,
    "burstUpM",
    ""
};

const BulletStackEffect bulletPierceUpA = {
    Pierce,
    Additive,
    1,
    "pierceUpA",
    ""
};

const BulletStackEffect bulletPierceUpM = {
    Pierce,
    Multiplicative,
    1.5,
    "pierceUpM",
    ""
};

const BulletStackEffect bulletBounceUpA = {
    Bounce,
    Additive,
    1,
    "bounceUpA",
    ""
};

const BulletStackEffect bulletBounceUpM = {
    Bounce,
    Multiplicative,
    1.5,
    "bounceUpM",
    ""
};

const BulletStackEffect dashUpA = {
    PlayerNumDash,
    Additive,
    1,
    "dashUpA",
    ""
};

const BulletStackEffect dashUpM = {
    PlayerNumDash,
    Multiplicative,
    2,
    "dashUpM",
    ""
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
	int bulletBounce = 0;
	float homing = 0;
};
*/

const AttackData  none{
	EnemyAttackPattern::NONE,
	CIRCLE,
	{},
	blunt,
	3,
	M_PI / 6.0,
	{20,20},
	200,
	3000,
	{0,0},
	0,
	1,
	0
};

const AttackData  trail{
	EnemyAttackPattern::TRAIL,
	CIRCLE,
	{},
	blunt,
	1,
	0,
	{20,20},
	0,
	10000,
	{0,0},
	0,
	0,
	0
};

const AttackData  wave{
	EnemyAttackPattern::WAVE,
	CIRCLE,
	{},
	blunt,
	5,
	0,
	{20,20},
	200,
	3000,
	{0,0},
	0,
	0,
	0
};

const AttackData  laserNoRotate{
	EnemyAttackPattern::LASER,
	CIRCLE,
	{},
	blunt,
	3,
	0,
	{20,20},
	0,
	30000,
	{0.5,0},
	0,
	0,
	0
};

const AttackData  laserRotate{
	EnemyAttackPattern::LASER,
	CIRCLE,
	{},
	blunt,
	3,
	0,
	{0,20},
	0,
	8000,
	{0.8,M_PI / 300},
	0,
	0,
	0
};

const AttackData  threeShot{
	EnemyAttackPattern::SHOTGUN,
	CIRCLE,
	{},
	blunt,
	3,
	M_PI/6.0,
	{20,20},
	200,
	3000,
	{0,0},
	0,
	1,
	0
};

const AttackData  sixShot{
	EnemyAttackPattern::SHOTGUN,
	RECTANGLE,
	{},
	blunt,
	6,
	M_PI / 8,
	{20,20},
	200,
	3000,
	{0,0},
	0,
	1,
	0
};

const AttackData  radial{
	EnemyAttackPattern::BURST_RADIAL,
	CIRCLE,
	{numBulletsUpA, sizeUpA},
	blunt,
	40,
	M_PI / 40,
	{20,20},
	150,
	3000,
	{4,200},
	0,
	0,
	0
};

const AttackData  fourAllAround{
	EnemyAttackPattern::RADIAL,
	TRIANGLE,
	{},
	blunt,
	4,
	M_PI/4,
	{20,20},
	200,
	3000,
	{0,0},
	0,
	1,
	0
};

const AttackData  twoPincerShot{
	EnemyAttackPattern::SHOTGUN,
	CIRCLE,
	{},
	blunt,
	2,
	M_PI / 1.0,
	{20,20},
	200,
	3000,
	{200,-2*M_PI/3.0},
	0,
	1,
	0
};

const AttackData  twelveSpiralShot{
	EnemyAttackPattern::RADIAL,
	TRIANGLE,
	{},dashUpA,
	12,
	0.0,
	{20,20},
	200,
	3000,
	{200,-2 * M_PI / 3.0},
	0,
	0,
	0
};

const AttackData  threeHomingShot{
	EnemyAttackPattern::SHOTGUN,
	TRIANGLE,
	{dmgDownA, dmgDownM},
	blunt,
	3,
	M_PI / 4.0,
	{20,20},
	200,
	6000,
	{0.0,0.0},
	0,
	0,
	0.01
};

const AttackData  threeBurst{
	EnemyAttackPattern::BURST,
	CIRCLE,
	{ numBulletsUpA},
	sizeUpA,
	4,
	M_PI/6,
	{20,20},
	200,
	3000,
	{0,0},
	0,
	1,
	0
};

const AttackData  fiveBurst{
	EnemyAttackPattern::BURST,
	CIRCLE,
	{},
	blunt,
	6,
	M_PI / 6,
	{20,20},
	200,
	3000,
	{0,0},
	0,
	1,
	0,
};

const AttackData  threeSpray{
	EnemyAttackPattern::SPRAY,
	CIRCLE,
	{},
	blunt,
	3,
	M_PI / 16,
	{20,20},
	200,
	3000,
	{0,0},
	0,
	1,
	0
};

const AttackData  SniperShot{
	EnemyAttackPattern::SHOTGUN,
	TRIANGLE,
	{},
	blunt,
	1,
	0,
	{20, 20},
	900,
	3000,
	{0,0},
	0,
	0,
	0
};

const AttackData  NoAttack {
	EnemyAttackPattern::NONE,
	CIRCLE,
	{},
	blunt,
	0,
	0,
	{0,0},
	0,
	0,
	{0,0},
	0,
	0,
	0
};

////////////////////////////////////
//////////// ENEMY TYPE ////////////
////////////////////////////////////


struct EnemyEasySentry : Enemy {
    EnemyEasySentry() {
        attackData = {twelveSpiralShot};
		maxHealth = 50;
        currHealth = maxHealth;
        behavior = EnemyBehavior::ROTATE_IN_PLACE;
        attackCooldown = 2000;
		currCooldown = 0;
        state = 10;
    };
};

struct EnemyMediumCharge : Enemy {
    EnemyMediumCharge() {
        attackData = {NoAttack};
        maxHealth = 200;
        currHealth = maxHealth;
        behavior = EnemyBehavior::FOLLOW_PLAYER;
        attackCooldown = 1000;
		currCooldown = 0;
        state = 10;
    };
};

struct EnemyMediumHoming : Enemy {
    EnemyMediumHoming() {
        attackData = {threeHomingShot};
        maxHealth = 60;
        currHealth = maxHealth;
        behavior = EnemyBehavior::PATROLLING;
        attackCooldown = 1500;
		currCooldown = 0;
        state = 10;
		patrolIndex = 0;
        patrolPath = std::vector<vec2>{
            {400, 100}, 
            {1000, 100}, 
            {1000, 700}, 
            {400, 700}
        };
    };
};

struct EnemyEasySniper : Enemy {
	EnemyEasySniper() {
		attackData = {SniperShot};
		maxHealth = 40;
		currHealth = maxHealth;
		behavior = EnemyBehavior::EVADEBULLET;
		attackCooldown = 5000;
		currCooldown = 0;
		state = 10;
	};
};

struct TestEnemy : Enemy {
	TestEnemy() {
		attackData = { radial };
		maxHealth = 40;
		currHealth = maxHealth;
		behavior = EnemyBehavior::ROTATE_IN_PLACE;
		attackCooldown = 3000;
		currCooldown = 0;
		state = 10;
	};
};