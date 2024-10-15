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
	int burstMax = 0; // always add 1 to the end
};
*/


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
	{ spreadUpA, sizeUpA },
	blunt,
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