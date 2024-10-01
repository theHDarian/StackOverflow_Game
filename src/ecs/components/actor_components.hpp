#pragma once
#include <vector>


enum BulletEffectType {
    BulletDamage,
    ProjectileSpeed,
    ProjectileSize,
    FireRate,
    BulletRange,
    BulletSpread,
    BulletNum,
    Bounce,
    Pierce,
    Homing,
    PlayerSpeed,
    PlayerNumDash,
    PlayerStackSize,
    PlayerDashCDR,
    Inert // Bullet that does nothing but take up stack space
};

enum EffectCalculation {
    Additive,
    Multiplicative
};

struct BulletStackEffect {
	BulletEffectType type = BulletEffectType::Inert;
    EffectCalculation effectCalc;
    float value;
};

// Player component
struct Player
{
    int baseStackSize;
    std::vector<BulletStackEffect> currStack;

    float baseSpeed;
    float baseFireRate;
    float baseDashNum;
    float baseDashCDR;

    float dashDistance;
    int currDashCharges;
    float currDashCooldown;
    float currFireRateCooldown;
};

// Holds the actual data of currStack
// Referenced on Player and PlayerBullet steps
// ie (baseSpeed + PlayerSpeedAdditive ) * PlayerSpeedMultiplicative
// When adding/removing something to the stack, update relevant fields
// Must be easily accessible
struct StackCompile {
    // Could probably be changed to a hardcoded map<BulletEffectType, float>, one for additive, one for multiplicative
    float BulletDamageAdditive;
    float BulletDamageMultiplicative;

    float ProjectileSpeedAdditive;
    float ProjectileSpeedMultiplicative;

    float ProjectileSizeAdditive;
    float ProjectileSizeMultiplicative;

    float FireRateAdditive;
    float FireRateMultiplicative;

    float BulletRangeAdditive;
    float BulletRangeMultiplicative;

    float BulletSpreadAdditive;
    float BulletSpreadMultiplicative;

    int BulletNumAdditive;
    int BulletNumMultiplicative;

    int BounceAdditive;
    int PierceAdditive;
    // If homing >0, homing value represents search radius for enemies 
    float HomingAdditive;

    float PlayerSpeedAdditive;
    float PlayerSpeedMultiplicative;

    int PlayerNumDashAdditive;
    int PlayerNumDashMultiplicative;

    float PlayerDashCDRAdditive;
    float PlayerDashCDRMultiplicative;

    int PlayerStackSizeAdditive;
};

// anything that is deadly to the player
struct Enemy {
	int state; //TODO: can change to enum once state determined
    int maxHealth;
    int currHealth;
    float speed;
    // TODO add attack pattern data?
};

struct BossEnemy {
};

struct Invincible {
    // Deletes itself when countdown <0
    // Entity can't be hit while has Invincible component
    int countdown;
};

// TODO Add a way to use parametric equations for bullet path

struct PlayerBullet {
    float damage;
    float bulletSpeed;
    // Number than counts down every step, delete bullet when <0
    float bulletRange;
    // Player bullet only scale in all directions?
    float bulletSize;
    int bulletPierce;
    int bulletBounce;
};

struct EnemyBullet {
    float bulletSpeed;
    // Number than counts down every step, delete bullet when <0
    float bulletRange;
    // Enemy bullet can scale x,y independently?
    vec2 bulletSize;
    int bulletBounce;
    std::vector<BulletStackEffect> bulletEffects;
};

// All data relevant to the shape and motion of entities
struct Motion {
	vec2 position = { 0, 0 };
	float angle = 0;
	vec2 velocity = { 0, 0 };
	vec2 scale = { 10, 10 };
};