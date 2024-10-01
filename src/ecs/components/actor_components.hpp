#pragma once
#include <vector>

// Player component
struct Player
{
};

// anything that is deadly to the player
struct Enemy {
	int state; //TODO: can change to enum once state determined
};

struct BossEnemy {

};

struct EnemyHealth {
	int maxHealth;
	int currHealth;
};
struct PlayerStack {
	int stackSize;
	std::vector<BulletStackEffect> currStack;
};


//entities that can shoot Bullets
struct Shooter {
    float fireRate;
    float bulletSpeed;

    float m_timeUntilNextBullet;
};

enum BulletEffectType {
    BulletDamage,
    ProjectileSpeed,
    FireRate,
    BulletRange,
    Bounce,
    Pierce,
    PlayerSpeed,
    PlayerNumDash,
    PlayerStackSize,
    PlayerDashCDR,
    Regular //damaging bullet
};

struct BulletStackEffect {
	BulletEffectType type = BulletEffectType::Regular;
	int tier;
};

struct Bullet {
	bool isFriendly;
    int damage;
    float range;
    float bulletSpeed;
    int type; //for damaging bullets ex. lightning bullets 
    //TODO can add bounce and piercing later
	BulletStackEffect stackEffect;

    //for destroying bullet when reaching max range, this method allows calculating range for bullets without fixed trajectory
    vec2 m_lastPosition; //update each frame
    float m_distTravelled; 
};

//entities that can move
struct Movement {
	float speed;
	int maxDashCharge;
	float dashCooldown; //in seconds
	float dashDistance;

    float m_timeUntilNextDash;
    int m_currDashCharge;
};


// All data relevant to the shape and motion of entities
struct Motion {
	vec2 position = { 0, 0 };
	float angle = 0;
	vec2 velocity = { 0, 0 };
	vec2 scale = { 10, 10 };
};