#pragma once
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <any>
#include "components.hpp"
#include <random>

constexpr float random_float = -9999;
constexpr vec2 random_vec2 = {random_float, random_float};

enum BulletEffectType {
    BulletDamage,
    ProjectileSpeed,
    ProjectileSize,
    FireRate,
    BulletRange,
    BulletAccuracy,
    BulletNum, // Number of bullets fired in a single shot
    Bounce,
    Pierce,
    Homing,
    PlayerSpeed,
    PlayerNumDash,
    PlayerStackSize,
    PlayerDashRecharge,
    Inert, // Bullet that does nothing but take up stack space
    Lightning,
    Key,
    Pop, // Bullet that pops the stack, should only be a contact effect
};

struct BulletStackEffect {
	BulletEffectType type = BulletEffectType::Inert;
    int value = 0;

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
    float baseSpeed = 300;
    int baseDashNum = 3;
    float baseDashCDR = 2000.0f;
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
    int baseStackSize = 20;
    std::vector<BulletStackEffect> currStack;
    std::vector<BulletStackEffect> recentRemoved;

    typedef float (StackCompile ::* FP)(int);
    // x<0 does nothing (except waste space on stack)
    float bulletDamageFunc(int x)       { return clamp(0.f, (float)x * 8.f, 90.f); };
    float projectileSpeedFunc(int x)    { return clamp(-400.f, (x > 0) ? (float)x * 120.f : (float)x * -80.f, 1400.f); };
    float projectileSizeFunc(int x)     { return clamp(-5.f, (x > 0) ? ((x < tierThresholds[ProjectileSize]) ? (float)x * 8.f : ((float)x - 5) * 5.f) : (float)x, 80.f); };
    float fireRateFunc(int x)           { return clamp(-400.f, (x > 0) ? ((x < tierThresholds[FireRate]) ? -500.f + 1000.f / ((float)x + 2.f) : -300.f + 1000.f / ((float)x + 2.f)) : -50.f * (float)x, 1000.f); };
    float bulletRangeFunc(int x)        { return clamp(-500.f, (x > 0) ? (float)x * 200.f : (float)x * 100.f, 1000000.f); };
    float bulletSpreadFunc(int x)       { return clamp(-15.f, (x > 0) ? -2.f * (float)x : -20.f * (float)x, 330.f); };
    float bulletNumFunc(int x)          { return clamp(0.f, (float)x, 50.f); };
    float bounceFunc(int x)             { return clamp(0.f, (float)x, 100.f); };
    float pierceFunc(int x)             { return clamp(0.f, (float)x, 100.f); };
    float homingFunc(int x)             { return clamp(0.f, (float)x / 40.f, 1.f); };
    float playerSpeedFunc(int x)        { return clamp(-150.f, (float)x * 20.f, 300.f); };
    float playerNumDashFunc(int x)      { return clamp(0.f, (float)x, 5.f); };
    float playerStackSizeFunc(int x)    { return clamp(0.f, (float)x * 2.f, 64.f); };
    float playerDashCDRFunc(int x)      { return clamp(-1500.f, (x > 0) ? (float)x * -150.f : (float)x * -200.f, 8000.f); };

    std::map<BulletEffectType, FP> functions = {
        {BulletDamage,      &StackCompile::bulletDamageFunc},
        {ProjectileSpeed,   &StackCompile::projectileSpeedFunc},
        {ProjectileSize,    &StackCompile::projectileSizeFunc},
        {FireRate,          &StackCompile::fireRateFunc},
        {BulletRange,       &StackCompile::bulletRangeFunc},
        {BulletAccuracy,    &StackCompile::bulletSpreadFunc},
        {BulletNum,         &StackCompile::bulletNumFunc},
        {Bounce,            &StackCompile::bounceFunc},
        {Pierce,            &StackCompile::pierceFunc},
        {Homing,            &StackCompile::homingFunc},
        {PlayerSpeed,       &StackCompile::playerSpeedFunc},
        {PlayerNumDash,     &StackCompile::playerNumDashFunc},
        {PlayerStackSize,   &StackCompile::playerStackSizeFunc},
        {PlayerDashRecharge,&StackCompile::playerDashCDRFunc}
    };

    // Use extra when you want to find what a higher/lower value would yield
    // Useful for UI info
    float Call(const BulletEffectType& s, int extra = 0) {
        FP fp = functions[s];
        return (this->*fp)(values[s] + extra);
    }

    std::map<BulletEffectType, float> values = {
        {BulletDamage,      0},
        {ProjectileSpeed,   0},
        {ProjectileSize,    0},
        {FireRate,          0},
        {BulletRange,       0},
        {BulletAccuracy,    0},
        {BulletNum,         0},
        {Bounce,            0},
        {Pierce,            0},
        {Homing,            0},
        {PlayerSpeed,       0},
        {PlayerNumDash,     0},
        {PlayerStackSize,   0},
        {PlayerDashRecharge,0}
    };

    std::map<BulletEffectType, float> tierThresholds = {
        {BulletDamage,      5},     // Inflict burning on hit. 15% default, +15% per additional point over threshold
        {ProjectileSpeed,   5},
        {ProjectileSize,    5},     // Bullet explodes into smaller bullets on deletion
        {FireRate,          5},
        {BulletRange,       5},     // Deal more damage the further away from the player the enemy is (up to 2x)
        {BulletAccuracy,    5},     // Inflict vulnerable for 4000
        {BulletNum,         5},     // Fires 4 * (1 + value-threshold) mini bullets
        {Bounce,            5},     // Bouncing towards random enemy
        {Pierce,            5},     // Deal more dmg to protected enemies, and vulnerable effect stronger
        {Homing,            3},     // Causes all enemy bullets to get subtle homing (negative >:)
        {PlayerSpeed,       5},
        {PlayerNumDash,     5},     // Clears non-special enemy bullets at end of dash, 50px * (1 + value-threshold) radius
        {PlayerStackSize,   5},
        {PlayerDashRecharge,5}      // 50% Chance to dodge inert effect from bullets or enemies
    };

    bool add(BulletStackEffect effect) {
    	if (effect.type == Lightning) {
	        if (currStack.size() < 1) return true;
	        if (effect.value == -1) {
		        std::rotate(currStack.begin(), currStack.begin() + currStack.size() - 1, currStack.end());
	        }
	        else {
		        std::random_device rd;
		        std::mt19937 g(rd());
		        std::shuffle(currStack.begin(), currStack.end(), g);
	        }
            stackMerge();
    	    return true;
        }
        int maxStackSize = baseStackSize + Call(PlayerStackSize);
        if (currStack.size() >= maxStackSize 
            && (currStack[currStack.size() - 1].type != effect.type || 
                (abs(currStack[currStack.size() - 1].value + effect.value) > 3 || currStack[currStack.size() - 1].value == 0))) {
            // value = 0 means unstackable, 3 is max stack value
            return false;
        }
        if (effect.type == Inert || effect.type == Key) {
            currStack.push_back(effect);
            return true;
        }
        values[effect.type] += effect.value;
        currStack.push_back(effect);
        stackMerge();
        return true;
    }

    void stackMerge() {
        if (currStack.size() > 1) {
            int i = 0;
            while (i < currStack.size() - 1) {
                if (currStack[i].type != currStack[i + 1].type || currStack[i].value == 0 || values.find(currStack[i].type) == values.end()) {
                    i++;
                    continue;
                }
                if (abs(currStack[i].value + currStack[i + 1].value) <= 3) {
                    currStack[i].value += currStack[i + 1].value;
                    currStack.erase(currStack.begin() + i + 1);
                    if (currStack[i].value == 0) currStack.erase(currStack.begin() + i);
                    i--;
                }
                else if (currStack[i].value + currStack[i + 1].value == 4) {
                    currStack[i].value = 3;
                    currStack[i + 1].value = 1;
                    i++;
                }
                else if (currStack[i].value + currStack[i + 1].value == -4) {
                    currStack[i].value = -3;
                    currStack[i + 1].value = -1;
                    i++;
                }
                else if (abs(currStack[i].value + currStack[i + 1].value) > 4 && abs(currStack[i].value) < abs(currStack[i + 1].value)) {
                    int swap = currStack[i].value;
                    currStack[i].value = currStack[i + 1].value;
                    currStack[i + 1].value = swap;
                    i--;
                }
                else {
                    i++;
                }
            }
        }
    }

    BulletStackEffect remove(int index) {
        printf("remove %d\n",index);
        assert(index < currStack.size() && index >= 0);

        BulletStackEffect effect = currStack[index];
        if (effect.type != Key) {
            recentRemoved.push_back(effect);
        }
        currStack.erase(currStack.begin() + index);
        values[effect.type] -= effect.value;
        return effect;
    }

    bool useKey() {
        auto comp = [](BulletStackEffect a) {
            return a.type == BulletEffectType::Key;
        };

        // Finding the index of val
        auto it = std::find_if(currStack.rbegin(), currStack.rend(), comp);
        if (it == currStack.rend()) return false;

        recentRemoved.clear();

        // Interate backwards from end to index, remove each
        int end = (it + 1).base() - currStack.begin();
        for (int i = currStack.size() - 1; i >= end; i--) {
            remove(i);
        }

        return true;
    }
    void printStack() {
        std::cout << "STACK START" << std::endl;
        for (auto& element : currStack) {
            std::cout << element.name << " : " << element.value << std::endl;
        }
        std::cout << "STACK END" << std::endl;
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

struct Vulnerability {
    float countdown = 1000;
    float modifier = 1.5f;
};

struct Burning {
    // When countdown reaches 0, deal damage * stack dmg to the enemy and decrement stack
    // reset countdown if stack > 0
    float countdown = 3000;
    float maxCountdown = 3000;
    float damage = 10.f;
    int stack = 0;
};

struct PlayerAttackData {
    float currFiringInterval = 0.0f;
    float maxFiringInterval = 500.0f;

    int maxBulletBurst = 1;
    int currBulletBurst = 1;
    float bulletBurstCooldown = 50;
};

struct PlayerBullet {
    float damage = 10;
    float bulletSpeed = 600;
    // Number than counts down every step, delete bullet when <0
    float bulletRange = 1000;
    // Player bullet only scale in all directions?
    float bulletSize = 20;
    int bulletPierce = 0;
    int bulletBounce = 0;

    // Generic bullets can't create more bullets
    bool generic = false;
};

enum EnemyType {

    // Testing
    EnemyTestLightningRotate,
    EnemyTestLightningShuffle,
    EnemyTestEffectBlunt,
    EnemyTestEffectPlusOne,
    EnemyTestEffectPlusTwo,
    EnemyTestEffectPlusThree,
    EnemyTestEffectMinusOne,
    EnemyTestEffectMinusTwo,
    EnemyTestEffectMinusThree,

    // Generic
    EnemyMage,
    EnemySkull,
    EnemyEvilSkull,
    EnemySword,
    EnemyHealer,
    EnemyQuadshooter,
    EnemyQuadshooterElite,
    EnemyMagnet,
    EnemySmallCShield,
    TutorialEnemyQuadshooter,

    // Biology
    BossCrab,
    BossCrabLaser,
    BossBeehiveGun,
    BossBeehiveMain,
    EnemySnail,
    EnemyEvilSnail,
    EnemyOneBee,
    EnemyTwoBee,
    EnemyThreeBee,
    EnemyCrab,
    EnemyEvilCrab,
    EnemyLaserCrab,
    EnemyBioBoid,
    EnemyPufferfish,
    EnemyBeeHive,
    HardEnemyAngel,

    // Mining
    EnemyMiningBoulderSmall,
    EnemyMiningBoulderBig,
    EnemySmallMole,
    EnemySurfaceMole,
    EnemyDynamite,
    EnemyBulldozer,
    EnemySmallBoulder,
    EnemyBigBoulder,
    BossDrillWormHead,
    BossDrillWormBody,
    EnemyChainDogHead,
    EnemyChainDogBody,
    EnemyMiningBoidWormHead,
    EnemyMiningBoidWormBody,
    EnemyPileDriverTurret,
    EnemySmallMiningWormHead,
    EnemySmallMiningWormBody,
    BossMole,

    // Hifi
    EnemyHifiBoidFish,
    EnemyTwinLaserVertical1,
    EnemyTwinLaserVertical2,
    EnemyHifiTwinLaserHorizontal1,
    EnemyHifiTwinLaserHorizontal2,
    EnemySniper,
    EnemyHifiCharger,
    EnemyHifiTrail,
    EnemyHifiCannon,
    EnemyHifiChargerHard,
    EnemyHifiSniperHard,
    EnemyHifiTrailHard,
    EnemyHifiCannonHard,
    EnemyLaserSniper,
    EnemyLaserSniperHard,
    EnemyHifiTemporaryBoid,
    EnemyHifiJellyFish,
    EnemyHifiTackShooter,
    EnemyHifiBallLauncher,
    EnemyHifiWhip,
    BossBigC,
    BossBigCShield,
    EnemyHifiBoid,
    EnemyFishBoid, // Unsure if this is hifi lol
    BossMultiCube,
    BossConstructYELLOW,
    BossConstructPURPLE,
    BossConstructGREEN,
    BossConstructRED,


    // Medical
    EnemyProstheticHand,
    EnemyMedicalBoid,
    EnemySpinePatrolWormHead,
    EnemySpinePatrolWormBody,
    EnemySpineFollowWormHead,
    EnemySpineFollowWormBody,
    EnemySpineChainedWormHead,
    EnemySpineChainedWormBody,
    EnemyMedicalBMP,
    EnemyScissors,
    EnemyMedicalRodA,
    EnemyMedicalRodC,
    EnemyMedicalPillBoid,
    EnemyMedicalPillBoidSpawner,
    EnemyMedicalSyringe,

    ScientistradialAttack,
    ScientistexplosiveAttack,
    ScientistHomingAttack,
    ScientistLaserGridAttack,
    ScientistLaserGridVerticalAttack,
    ScientistShield,
    ScientistBoss,
    ScientistHand,
    ScientistlaserAttack,

    // Military
    EnemyEyeCube,
    EnemyPhantom,

    // event room enemies
    // single target buffs, place at the same position as target
    InvincibleGranter,
    InvisibleGranter,
    VulnerableGranter,
    ProtectedGranter,
    UnderGroundGranter,
    RegeneratingGranter,
    CloakedGranter,
    // room wide buffs, place at the center of the room
    InvincibleGranterRoomWide,
    InvisibleGranterRoomWide,
    VulnerableGranterRoomWide,
    ProtectedGranterRoomWide,
    UnderGroundGranterRoomWide,
    RegenerateGranterRoomWide,
    CloakedGranterRoomWide,
    // Enemyparts
    EnemyBubbleShield,
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
    ONE_WALL,
    TWO_WALL,
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
    
    // Pre Effect Rework effect system
    // Used for bosses, elites
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
    std::vector<vec2> spawnPosition = {};
    
    // Post Effect Rework effect system
    bool gottenRoomEffects = false;
    std::vector<BulletStackEffect> positiveBulletEffects;
    std::vector<BulletStackEffect> negativeBulletEffects;
};

enum class SpecialStates {
    // order corresponds to order drawn in ui
    INVINCIBLE,
    UNDERGROUND,
    VULNERABLE,
    PROTECTED,
    REGENERATING,
    ONFIRE,

    // these won't be shown in ui
    NORMAL,
    INVISIBLE,
    CLOAKED,
    INC_ANIM,
    CLEAR_ALL,
    CLEAR_INVISIBLE,
    CLEAR_INVINCIBLE,
    CLEAR_VULNERABLE,
    CLEAR_PROTECTED,
    CLEAR_UNDERGROUND,
    CLEAR_REGENERATING,
    CLEAR_CLOAKED,
};

enum class EnemyBehavior {
    // this is the basic
    RANDOM,
    ROLLING,
    ROOK_FOLLOW,
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
    BOIDSFISH,
    FOLLOWSCIENTIST,
    FOLLOWPARENT,
    WORM_FOLLOW,
    // Worm will Teleport to first position in spline if not there
    // Remedy using WORM_GOTO
    WORM_PATROL,
    WORM_RANDOM,
    WORM_GOTO,
    WORM_BODY,
    GRANTINGBUFFS, // for enemies that give effects to other enemies, using this makes specials states apply to the other entity rather than itself
    GRANTINGBUFFSAOE, // for enemies that give effects to all other enemies
};

enum class EnemyRotationBehavior {
    NONE,
    REGULAR,
    FACE_UP,
    FACE_CENTER,
    FACE_PLAYER,
    FACE_TWIN,
    SPIN,
    LASER_CONTROL,
    WORM
};


enum class ReactionType {
    DURATION,
    PLAYER_FAR,
    PLAYER_CLOSE,
    PLAYER_REALLY_CLOSE,
    PLAYER_BULLET_CLOSE,
    FINISH_PATROL,
    SEVENTYFIVE_HEALTH,
    FIFTY_HEALTH,
    TWENTYFIVE_HEALTH,
    TEN_HEALTH,
    TWENTY_HEALTH,
    THIRTY_HEALTH,
    FORTY_HEALTH,
    SIXTY_HEALTH,
    SEVENTY_HEALTH,
    EIGHTY_HEALTH,
    NINETY_HEALTH,
    BEE_CLOSE,
    TEAM_HURT,
    NO_BEES,
    SHIELDBREAK,
};

struct Reaction {
    ReactionType React;
    int index;
    //trigger effect when reaction is met
    SpecialStates specialState = SpecialStates::NORMAL;
};

struct InvisibleEnemy {

};

struct Spawning {
    float max = 660;
    float countdown = max;
};

struct Shield {

};

struct Scientist {
    Entity shield;
    Entity hand;
};

struct Hand {

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

    //special states, for if the enemy has some special attributes like being invisible or invincible
    //the state will last for the duration of the current pattern
    SpecialStates specialState = SpecialStates::NORMAL;

    //for enemies that can grant buffs to other enemies, this will be the effect that is granted
    SpecialStates buffEffect = SpecialStates::NORMAL;

};
struct Boid {
    vec2 velocity;
    vec2 position;
    float wanderAngle;
    float maxSpeed;
};

struct WormHead {
    std::vector<vec2> points = {};
    float constrainDistance = 100;
    int size = -1;
    EnemyType body = EnemyType::BossDrillWormBody;
    bool anchor = false;
    vec2 anchorPoint;
};

struct WormBody {
    Entity head;
    int index;
};

struct Healer {
    float coolDown;
    float maxCoolDown = 3000.f;
    int healPower = 35;
    Entity targetEntity;
};

struct Buffer {
    float cooldown = 0;
    float maxCoolDown = 5000;
    float duration = 2000.f;
    float range = 200.f;
    Entity targetEntity;
};


struct SpriteData
{
	std::string texturePath;
	EFFECT_ASSET_ID effectId = EFFECT_ASSET_ID::TEXTURED;
	GEOMETRY_BUFFER_ID geometryId = GEOMETRY_BUFFER_ID::SPRITE;
	vec2 offset = vec2(0);
    int animationType = 1;
    int max_Frames = 1;
    float countdown = 20;
};



// anything that is deadly to the player
struct Enemy {
    int maxHealth;
    int currHealth;
    vec2 velocity;
    std::vector<BulletStackEffect> collisionBullet = {{
        Inert,
        0,
        "Inert",
        "" 
    }};
    std::vector<EnemyPattern> enemyPatterns;
    int patternIndex;
    EnemyPattern& currEnemyPattern() {
        return enemyPatterns[patternIndex];
    };
    vec2 scale = vec2(1);
    SpriteData sprite;
    bool newPattern = false;
    float rotatePower = 1.f;
    EnemyRotationBehavior rotationBehaviour = EnemyRotationBehavior::REGULAR;
    float speedMultiplier = 1.0f;
    int armour = 1;
    WormHead headData;
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

    // Worm curves!
    std::vector<vec2> points = { vec2(0.8,0.2), vec2(0.8,0.8), vec2(0.2, 0.8), vec2(0.2,0.2), vec2(0.8,0.2)};
    float t = 0.f;
};

struct BossEnemy {
    std::string name;
};

struct BossParts {
    bool diesWithBoss = true;
    bool showHpBar = false;
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

struct Elite {
    int eliteLevel = 1;
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
	float angle = 0.f;
	vec2 velocity = { 0, 0 };
	vec2 scale = { 10, 10 };
    vec2 veer = { 0,0 };

    float angleOffset = 0.f;
};

struct Damaged {
    float max = 200;
    float countdown = max;
};

// should be separate from damaged so that damage from player takes precedence
struct BurnTick : Damaged {

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
    int cheese = 0;
    bool hasSeenPieRecipe = false;
};

enum InteractableRequestType {
    PopStack, // clears stack, spawns bullets with effects that were in stack
    ClearStack, // clears stack without spawning anything
    ExtendStack, // extends stack by n
    GrantWish, // grants wish based on choice, 0 is first choice, 1 is second, etc
    AddEffect, // adds effect to stack
    RemoveEffect, // removes effect from stack
    SpawnEnemy, // spawns enemy based on region, or can pass in specific enemy
    PopX, // creates x bullets with effects (used for key)
};

struct InteractableRequest {
    InteractableRequestType type;
    int choice = -1;
    std::vector<std::tuple<EnemyType,vec2>> enemies = {};
    std::vector<BulletStackEffect> effects = {};
};

struct UIRequest {
    UIRequestType type = UIRequestType::ResetUI;
    std::string text = "";
    std::vector<BulletStackEffect> effects = {};
    UIRequest(UIRequestType type = UIRequestType::ResetUI, std::string text = "", std::vector<BulletStackEffect> effects = {}) {
        this->type = type;
        this->text = text;
        this->effects = effects;
    }
};

struct specialRotators {

};

struct RoomWideBuffers {

};

struct InstanceDamage {
    int instance;
};

struct Mole {
    float max = 1000;
    float countdown = max;
};

struct AOEIndicator {
    SpecialStates type;
};

struct Regenerate {
    float max = 2000;
    float countdown = max;
    float healAmount = 1;
    float healInterval = 250;
    float currHealInterval = 0;
};

struct EnemyPart {
    Entity parent;
    vec2 offset;
    bool alwaysFollow = false;
};

struct Cloaked {
    float max = 1000;
    float countdown = max;
    float cloakingDistance = 400;
};

// simplest way for text system to know where hp bar is drawn in render system
// so that text can "wobble" along with hp bar
struct HPBarUI {
    vec2 position;
    vec2 scale;
    float alpha = 1.0;
    bool followCamera = true;
    std::vector<int> activeStatuses = std::vector<int>(static_cast<int>(SpecialStates::NORMAL));
    std::vector<vec2> statusPositions = std::vector<vec2>(static_cast<int>(SpecialStates::NORMAL));
    vec2 iconSize;
    float textSize;
};

// not sure how I feel about struct just for this
// but is easiest way for text and render system to talk
struct StackUI {
    vec2 bulletStartPos;
    vec2 bulletSize;
    float bulletOffset; // space between bullets
    vec2 stackSize;
    vec2 stackPos;

    std::vector<vec2> bulletPositions;
    std::map<BulletEffectType, vec2> activeTiers; // map of active tiers and their icon position

        void updateStackUISize(int bulletCapacity) {
        stackSize = vec2(bulletCapacity * bulletSize.x + bulletCapacity * bulletOffset + 2 * bulletOffset, bulletSize.y + 2 * bulletOffset);
        stackPos = vec2(bulletStartPos.x + stackSize.x / 2 - bulletSize.x - bulletOffset / 2, bulletStartPos.y);
    }
};