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


BulletStackEffect blunt = {
	Inert,
	Additive,
	0,
	"blunt",
	""
};

BulletStackEffect dmgDownA = {
	BulletDamage,
	Additive,
	-2.0,
	"dmgDownA",
	""
};

BulletStackEffect dmgDownM = {
	BulletDamage,
	Multiplicative,
	0.7,
	"dmgDownM",
	""
};

BulletStackEffect numBulletsUpA = {
	BulletNum,
	Additive,
	1,
	"numBulletsUpA",
	""
};

BulletStackEffect sizeUpA = {
	ProjectileSize,
	Additive,
	10,
	"sizeUpA",
	""
};

BulletStackEffect spreadUpA = {
	BulletSpread,
	Additive,
	15,
	"spreadUpA",
	""
};

BulletStackEffect bulletSpeedUpA = {
    ProjectileSpeed,
    Additive,
    50,
    "speedUpA",
    ""
};

BulletStackEffect bulletSpeedUpM = {
    ProjectileSpeed,
    Multiplicative,
    1.5,
    "speedUpM",
    ""
};

BulletStackEffect bulletRangeUpA = {
    BulletRange,
    Additive,
    500,
    "rangeUpA",
    ""
};

BulletStackEffect bulletRangeUpM = {
    BulletRange,
    Multiplicative,
    1.5,
    "rangeUpM",
    ""
};

BulletStackEffect bulletBurstUpA = {
    BulletBurst,
    Additive,
    1,
    "burstUpA",
    ""
};

BulletStackEffect bulletBurstUpM = {
    BulletBurst,
    Multiplicative,
    1.5,
    "burstUpM",
    ""
};

BulletStackEffect bulletPierceUpA = {
    Pierce,
    Additive,
    1,
    "pierceUpA",
    ""
};

BulletStackEffect bulletPierceUpM = {
    Pierce,
    Multiplicative,
    1.5,
    "pierceUpM",
    ""
};

BulletStackEffect bulletBounceUpA = {
    Bounce,
    Additive,
    1,
    "bounceUpA",
    ""
};

BulletStackEffect bulletBounceUpM = {
    Bounce,
    Multiplicative,
    1.5,
    "bounceUpM",
    ""
};

BulletStackEffect dashUpA = {
    PlayerNumDash,
    Additive,
    1,
    "dashUpA",
    ""
};

BulletStackEffect dashUpM = {
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
	vec2 veer = { 0,0 }; // {magnitude, angle}
	int bulletPierce = 0;
	int bulletBounce = 0;
	float homing = 0;
};
*/

AttackData none{
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

AttackData wave{
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

AttackData laserNoRotate{
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

AttackData laserRotate{
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

AttackData threeShot{
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

AttackData sixShot{
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

AttackData fourAllAround{
	EnemyAttackPattern::ALL_DIRECTION,
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

AttackData twoPincerShot{
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

AttackData twelveSpiralShot{
	EnemyAttackPattern::ALL_DIRECTION,
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

AttackData threeHomingShot{
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

AttackData threeBurst{
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

AttackData fiveBurst{
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

AttackData threeSpray{
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

AttackData SniperShot{
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

AttackData NoAttack {
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
		currCooldown = attackCooldown;
		currCooldown = attackCooldown;
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
		currCooldown = attackCooldown;
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
		currCooldown = attackCooldown;
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
		currCooldown = attackCooldown;
		state = 10;
	};
};

struct TestEnemy : Enemy {
	TestEnemy() {
		attackData = { laserRotate };
		maxHealth = 40;
		currHealth = maxHealth;
		behavior = EnemyBehavior::ROTATE_IN_PLACE;
		attackCooldown = 5000;
		currCooldown = attackCooldown;
		state = 10;
	};
};