#pragma once
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <any>
#include "components.hpp"
#include <random>



enum BulletEffectType {
    BulletDamage,
    ProjectileSpeed,
    ProjectileSize,
    FireRate,
    BulletRange,
    BulletSpread,
    BulletNum, // Number of bullets fired in a single shot
    BulletBurst, // Number of bullets fired in a burst
    Bounce,
    Pierce,
    Homing,
    PlayerSpeed,
    PlayerNumDash,
    PlayerStackSize,
    PlayerDashCDR,
    Inert, // Bullet that does nothing but take up stack space
    Lightning,
    Key
};

enum EffectCalculation {
    Additive,
    Multiplicative
};

struct BulletStackEffect {
	BulletEffectType type = BulletEffectType::Inert;
    EffectCalculation effectCalc;
    float value;

    // For UIq
    std::string name;
    std::string tooltip;
    vec3 color;

};

struct EffectStack {
    std::vector<BulletStackEffect> stack;
};

// Player component
struct Player
{
    float baseSpeed = 250;
    float baseFiringInterval = 300.0f;
    int baseDashNum = 3;
    float baseDashCDR = 3000.0f;
    float baseDashSpeed = 2500.0f;

    float dashSpeed = baseDashSpeed;
    int currDashCharges = 3;

    int maxDashCharges = baseDashNum;

    float currDashCooldown = baseDashCDR;
    float dashCooldown = baseDashCDR;

    int bulletCluster = 1;
};

// Holds the actual data of currStack
// Referenced on Player and PlayerBullet steps
// ie (baseSpeed + PlayerSpeedAdditive ) * PlayerSpeedMultiplicative
// When adding/removing something to the stack, update relevant fields
// Must be easily accessible
struct StackCompile {
    int baseStackSize = 16;
    std::vector<BulletStackEffect> currStack;

    std::map<BulletEffectType, float> additives = {
        {BulletDamage,      0},
        {ProjectileSpeed,   0},
        {ProjectileSize,    0},
        {FireRate,          0},
        {BulletRange,       0},
        {BulletSpread,      0},
        {BulletNum,         0},
        {BulletBurst,       0},
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
        {BulletBurst,       1},
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
        {FireRate,          0.5},
        {BulletRange,       1},
        {BulletSpread,      1},
        {BulletNum,         1},
        {BulletBurst,       1},
        {Bounce,            0},
        {Pierce,            0},
        {Homing,            0},
        {PlayerSpeed,       50},
        {PlayerNumDash,     0},
        {PlayerStackSize,   1},
        {PlayerDashCDR,     1}
    };

    std::map<BulletEffectType, float> maximums = {
        {BulletDamage,      100000},
        {ProjectileSpeed,   2000},
        {ProjectileSize,    50},
        {FireRate,          100000},
        {BulletRange,       100000},
        {BulletSpread,      100000},
        {BulletNum,         50},
        {BulletBurst,       50},
        {Bounce,            100},
        {Pierce,            100},
        {Homing,            1.0f},
        {PlayerSpeed,       1000},
        {PlayerNumDash,     20},
        {PlayerStackSize,   100},
        {PlayerDashCDR,     100000}
    };

    bool add(BulletStackEffect effect) {
    	if (effect.type == Lightning) {
	    if (currStack.size() < 1) return true;
	    if (effect.effectCalc == Additive) {
		std::rotate(currStack.begin(), currStack.begin() + currStack.size() - 1, currStack.end());
	    }
	    else {
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(currStack.begin(), currStack.end(), g);
	    }
    	    return true;
        }
        int maxStackSize = (baseStackSize + additives[PlayerStackSize]) * multiplicatives[PlayerStackSize];
        if (currStack.size() >= maxStackSize) {
            return false;
        }
        if (effect.type == Inert || effect.type == Key) {
            currStack.push_back(effect);
            return true;
        }
        if (effect.effectCalc == Additive) {
            additives[effect.type] += effect.value;
        } else {
            multiplicatives[effect.type] += effect.value;
        }
        currStack.push_back(effect);
        return true;
    }

    BulletStackEffect remove(int index) {
        printf("remove %d\n",index);
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
        printf("modify %d\n",index);
        BulletStackEffect effect = remove(index);
        if (effect.effectCalc == EffectCalculation::Additive) {
            effect.value = -effect.value;
        } else {
            effect.value = 1 / effect.value;
        }
        add(effect);
        return effect;
    }
    bool useKey() {
        auto comp = [](BulletStackEffect a) {
            return a.type == BulletEffectType::Key;
        };

        // Finding the index of val
        auto it = std::find_if(currStack.rbegin(), currStack.rend(), comp);
        if (it == currStack.rend()) return false;

        // Interate backwards from end to index, remove each
        int end = (it + 1).base() - currStack.begin();
        for (int i = currStack.size() - 1; i >= end; i--) {
            remove(i);
        }

        return true;
    }
    void printStack() {
        for (auto& element : currStack) {
            std::cout << element.name << std::endl;
        }
    }
};

struct Invincible {
    // Deletes itself when countdown <0
    // Entity can't be hit while has Invincible component
    float max = 1000;
    float countdown = max;
};

struct Invisible {
    float countdown = 1000;
};

struct PlayerAttackData {
    float currFiringInterval = 0.0f;
    float maxFiringInterval = 300.0f;
    float bulletSpeed = 400;

    int maxBulletBurst = 1;
    int currBulletBurst = 1;
    float bulletBurstCooldown = 50;
};

struct PlayerBullet {
    float damage = 10;
    float bulletSpeed = 400;
    // Number than counts down every step, delete bullet when <0
    float bulletRange = 6000;
    // Player bullet only scale in all directions?
    float bulletSize = 20;
    int bulletPierce = 0;
    int bulletBounce = 0;
};

enum EnemyType {
    EasyEnemySentry,
    // //MediumEnemyClusterShot,
    MediumEnemyCharge,
    // MediumEnemyHoming,
    // EasyEnemySniper,
    // HardEnemyBehavior
    TestRevampedEnemy,
    BossBigC,
    BossBeehiveGun,
    BossBeehiveMain,
    Snail,
    EvilSnail,
    OneBee,
    TwoBee,
    ThreeBee,
    MediumEnemyTank,
    HardEnemyTank,
    LaserEnemyTank,
    BeeHive,
    HardEnemyAngel,
    EasyEnemySkull,
    HardEnemySkull,
    HardEnemyBoid,
    MediumEnemyBoar,
    MediumEnemyHealer,
    HardEnemyBoidBio,
    ScientistlaserAttack,
    HardEnemyBoidFish,
    HifiEnemyTwinLaserVertical1,
    HifiEnemyTwinLaserVertical2,
    HifiEnemyTwinLaserHorizontal1,
    HifiEnemyTwinLaserHorizontal2,
    HifiEnemySniper,
    HifiEnemyCharger,
    HifiEnemyTrail,
    HifiEnemyCannon,
    HifiEnemyChargerHard,
    HifiEnemySniperHard,
    HifiEnemyTrailHard,
    HifiEnemyCannonHard,
    HifiEnemyLaserSniper,
};

enum class EnemyAttackPattern {
    // this is the attack pattern
    SHOTGUN,
    RADIAL,
    RADIAL_POLYGON,
    BURST,
    BURST_RADIAL,
    SPRAY,
    WAVE,
    LASER,
    TWIN_LASER,
    TRAIL,
    SPAWNING,
    NONE
};

enum class EnemyBulletDeath {
    NONE,
    EXPLODE,
    CLUSTER
};


enum EnemyBulletShape {
    RECTANGLE   = 0,
    TRIANGLE    = 1,
    CIRCLE      = 2,
    KEY         = 3
};

struct AttackData {
    EnemyAttackPattern attackType;
    EnemyBulletShape shape = EnemyBulletShape::CIRCLE;
    std::vector<BulletStackEffect> rareBulletEffects;
    BulletStackEffect defaultEffect;
    int numBullets = 1;
    float angleOffset = 0;
    vec2 size = {20,20};
    float speed = 200;
    float bulletRange = 3000;
    vec2 veer = {0,0};
    int bulletPierce = 0;
    int bulletBounce = 0;
    float homing = 0;
    EnemyBulletDeath onDeath = EnemyBulletDeath::NONE;
    EnemyType spawn;
};

enum class EnemyBehavior {
    // this is the basic
    RANDOM,
    RANDOM_NEAR,
    RANDOM_FAR,
    FOLLOW_PLAYER,
    RETREAT,
    RECOIL,
    ANGRY,
    PATROLLING,
    EVADEBULLET,
    CIRCLINGPLAYER,
    ROTATE_IN_PLACE,
    TELEPORT,
    IDLE,
    MERGE_BEE,
    SPAWNING,
    BOIDS,
    BOIDSGROUP,
    BOIDSEXPLODE,
    CHARGING,
    HEALING,
    DEATHSTATE,
    BOIDSWARMPLAYER,
    BOIDSFISH
};

enum class EnemyRotationBehavior {
    NONE,
    REGULAR,
    FACE_UP,
    FACE_CENTER,
    FACE_PLAYER,
    FACE_TWIN,
    SPIN
};


enum class ReactionType {
    DURATION,
    PLAYER_CLOSE,
    PLAYER_BULLET_CLOSE,
    FINISH_PATROL,
    SEVENTYFIVE_HEALTH,
    FIFTY_HEALTH,
    TWENTYFIVE_HEALTH,
    BEE_CLOSE,
    TEAM_HURT,
    NO_BEES
};

struct Reaction {
    ReactionType React;
    int index;
};

struct InvisibleEnemy {

};

// act like a state that can move depending on enemies reactions
struct EnemyPattern {
    // name is just like state (for debugging)
    std::string name;
    // behv
    EnemyBehavior type;
    std::vector<vec2> path;
    int pathIndex;
    float curDuration;
    float maxDuration;
    // all possible reactions in current behavior state
    std::vector<Reaction> reactions;
    int next;
    bool canAttack;
    float currAtkCD;
    float maxAtkCD;
    AttackData atkData;
};
struct Boid {
    vec2 velocity;
    vec2 position;
    float wanderAngle;
    float maxSpeed;
};

struct Healer {
    float coolDown;
    float maxCoolDown = 3000.f;
    int healPower = 35;
    Entity targetEntity;
};


struct SpriteData
{
	std::string texturePath;
	EFFECT_ASSET_ID effectId;
	GEOMETRY_BUFFER_ID geometryId;
	vec2 offset;
    int animationType = 1;
    int max_Frames = 1;
    float countdown = 20;
};



// anything that is deadly to the player
struct Enemy {
    int maxHealth;
    int currHealth;
    vec2 velocity;
    BulletStackEffect collisionBullet = {
        Inert,
        Additive,
        0,
        "Inert",
        "" 
    };
    std::vector<EnemyPattern> enemyPatterns;
    int patternIndex;
    EnemyPattern& currEnemyPattern() {
        return enemyPatterns[patternIndex];
    };
    vec2 scale;
    SpriteData sprite;
    bool newPattern = false;
    float rotatePower;
    EnemyRotationBehavior rotationBehaviour = EnemyRotationBehavior::REGULAR;
    float speedMultiplier = 1.0f;
};

struct EnemyGroup {
    std::vector<Entity> others;
};

struct EnemyMovement {
    vec2 posA;
    vec2 posB;
    float distanceTraveled;
    float speed = 20000;
    float angularSpeed = 90.0f;
};

struct BossEnemy {
    std::string name;
};

struct BossParts {
};

struct EnemyBullet {
    float bulletSpeed;
    // Number than counts down every step, delete bullet when <0
    float bulletRange = 1000;
    float initialRange = 0;
    // Enemy bullet can scale x,y independently?
    int bulletBounce;
    int bulletPierce;
    std::vector<BulletStackEffect> bulletEffects;
    EnemyBulletShape shape;
    EnemyBulletDeath onDeath = EnemyBulletDeath::NONE;

    bool isSpecial;
};

struct Burst {
    int curBurst = 0;
    float burstCooldown = 0;
    float burstDirection = 0;
    vec2 start;
};

struct HomingBullet {
    Entity target;
    float homingIntensity; // How quickly it can turn towards the target
};

struct Laser {
    Entity start;
    float length;
    float maxLength;
    float growth;
    float rotation;
};

struct Dash {
    float endTimer = 120.0f; //ms, duration of the dash
    vec2 dashDirection;
};

// All data relevant to the shape and motion of entities
struct Motion {
	vec2 position = { 0, 0 };
	float angle = 0;
	vec2 velocity = { 0, 0 };
	vec2 scale = { 10, 10 };
    vec2 veer = { 0,0 };
};

struct Damaged {
    float max = 200;
    float countdown = max;
};

struct BeeEnemy {
    std::set<Entity> nearbyBees;
    int mergeCount = 1;
    int maxMerge = 3;
    bool canMerge = true;
    bool merge = false;
};

struct Critter {
    float radius = 100;
    bool startled = false;
    vec2 flee = vec2(0);
    float life = 10000;
};

struct KeyItems {
    int fruits = 0;
    int honey = 0;

};
