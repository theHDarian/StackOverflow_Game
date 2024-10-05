#pragma once
#include <vector>
#include <string>
#include <map>


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

    // For UI
    std::string name;
    std::string tooltip;


};

// Player component
struct Player
{
    float baseSpeed;
    float baseFiringInterval = 100.0f;
    int baseDashNum = 3;
    float baseDashCDR = 3000.0f;
    float baseDashSpeed = 2500.0f;

    float dashSpeed = baseDashSpeed;
    int currDashCharges = 3;

    int maxDashCharges = baseDashNum;

    float currDashCooldown = 0.0f;
    float dashCooldown = baseDashCDR;

    float currFiringInterval = 0.0f;
    float maxFiringInterval = baseFiringInterval;
};

// Holds the actual data of currStack
// Referenced on Player and PlayerBullet steps
// ie (baseSpeed + PlayerSpeedAdditive ) * PlayerSpeedMultiplicative
// When adding/removing something to the stack, update relevant fields
// Must be easily accessible
struct StackCompile {
    int baseStackSize;
    std::vector<BulletStackEffect> currStack;

    std::map<BulletEffectType, float> additives = {
        {BulletDamage,      0},
        {ProjectileSpeed,   0},
        {ProjectileSize,    0},
        {FireRate,          0},
        {BulletRange,       0},
        {BulletSpread,      0},
        {BulletNum,         0},
        {Bounce,            0},
        {Pierce,            0},
        {Homing,            0},
        {PlayerSpeed,       0},
        {PlayerNumDash,     0},
        {PlayerStackSize,   0},
        {PlayerDashCDR,     0}
    };
    std::map<BulletEffectType, float> multiplicatives = {
        {BulletDamage,      1},
        {ProjectileSpeed,   1},
        {ProjectileSize,    1},
        {FireRate,          1},
        {BulletRange,       1},
        {BulletSpread,      1},
        {BulletNum,         1},
        {Bounce,            1},
        {Pierce,            1},
        {Homing,            1},
        {PlayerSpeed,       1},
        {PlayerNumDash,     1},
        {PlayerStackSize,   1},
        {PlayerDashCDR,     1}
    };

    // TODO choose reasonable minimums
    std::map<BulletEffectType, float> minimums = {
        {BulletDamage,      1},
        {ProjectileSpeed,   1},
        {ProjectileSize,    1},
        {FireRate,          1},
        {BulletRange,       1},
        {BulletSpread,      1},
        {BulletNum,         1},
        {Bounce,            0},
        {Pierce,            0},
        {Homing,            0},
        {PlayerSpeed,       1},
        {PlayerNumDash,     0},
        {PlayerStackSize,   1},
        {PlayerDashCDR,     1}
    };

    bool add(BulletStackEffect effect) {
        int maxStackSize = (baseStackSize + additives[PlayerStackSize]) * multiplicatives[PlayerStackSize];
        if (currStack.size() >= maxStackSize) {
            return false;
        }
        if (effect.effectCalc == Additive) {
            additives[effect.type] += effect.value;
        } else {
            multiplicatives[effect.type] *= effect.value;
        }
        currStack.push_back(effect);
        return true;
    }
    BulletStackEffect remove(int index) {
        assert(index < currStack.size() && index >= 0);

        BulletStackEffect effect = currStack[index];
        currStack.erase(currStack.begin() + index);
        if (effect.effectCalc == EffectCalculation::Additive) {
            additives[effect.type] -= effect.value;
        } else {
            multiplicatives[effect.type] /= effect.value;
        }
        return effect;
    }
    BulletStackEffect modify(int index) {
        BulletStackEffect effect = remove(index);
        if (effect.effectCalc == EffectCalculation::Additive) {
            effect.value = -effect.value;
        } else {
            effect.value = 1 / effect.value;
        }
        add(effect);
        return effect;
    }
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
    float countdown;
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

    vec2 bulletDirection;
};

struct EnemyBullet {
    float bulletSpeed;
    // Number than counts down every step, delete bullet when <0
    float bulletRange;
    // Enemy bullet can scale x,y independently?
    vec2 bulletSize;
    int bulletBounce;

    vec2 bulletDirection;

    std::vector<BulletStackEffect> bulletEffects;
};

struct Homing {
    Entity target;
    float homingIntensity; // How quickly it can turn towards the target
};

// All data relevant to the shape and motion of entities
struct Motion {
	vec2 position = { 0, 0 };
	float angle = 0;
	vec2 velocity = { 0, 0 };
	vec2 scale = { 10, 10 };
};