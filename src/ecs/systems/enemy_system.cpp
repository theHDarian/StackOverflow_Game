#include "enemy_system.hpp"
#include <iostream>
#include "tiny_ecs_registry.hpp"
#include "common.hpp"
#include "world_init.hpp"
#include "actor_components.hpp"
#include <thread>
#include <chrono>

float COOLDOWN_SHOOT_MS = 2000;
float BASE_BULLET_SPEED = 1;
float PLACEHOLDER_FOR_ANGLE = 0.f;

EnemySystem::EnemySystem(RenderSystem *renderer)
{
    render = renderer;
};

EnemySystem::~EnemySystem() {
};

void EnemySystem::step(float elapsed_ms)
{
    auto &enemy_registry = registry.enemies;
    auto &motion_registry = registry.motions;
    auto &collision_registry = registry.collisions;
    for (uint i = 0; i < enemy_registry.components.size(); i++)
    {
        Enemy &enemy = enemy_registry.components[i];
        Entity &entity = enemy_registry.entities[i];

        Motion &motion = motion_registry.get(entity);
        vec2 pos = motion.position;
        float angle = motion.angle;
        // HANDLING DAMGE FROM COLLISION
        for (auto &entity : collision_registry.entities)
        {
            const Collision &collision = registry.collisions.get(entity);
            Entity other_entity = collision.other;

            if (enemy_registry.has(entity) && registry.playerBullets.has(other_entity))
            {
                Enemy &enemyStat = enemy_registry.get(entity);
                PlayerBullet &bulletStat = registry.playerBullets.get(other_entity);

                enemyStat.currHealth -= bulletStat.damage;
                std::cout << "current enemy health" << enemyStat.currHealth << std::endl;
                if (enemyStat.currHealth <= 0)
                {
                    registry.remove_all_components_of(entity);
                }

                registry.remove_all_components_of(other_entity);
            }
        }

        enemy.attackCooldown -= elapsed_ms;
        // std::cout << "enemy attack in:" << enemy.attackCooldown << std::endl;
        if (enemy.attackCooldown < 0.f)
        {
            if (enemy.attackPattern == EnemyAttackPattern::SINGLE_SHOT)
            {
                shoot(vec2(-100, 0), pos, angle);
                enemy.attackCooldown = COOLDOWN_SHOOT_MS;
            }
            else if (enemy.attackPattern == EnemyAttackPattern::DOUBLE_SHOT)
            {

                shoot(vec2(-100, -30), pos, angle);
                shoot(vec2(-100, 30), pos, angle);
                enemy.attackCooldown = COOLDOWN_SHOOT_MS;
            }
            else if (enemy.attackPattern == EnemyAttackPattern::ALL_DIRECTION)
            {
                const int numShots = 12;
                const float angleIncrement = 30.f;
                const float velocity = 100.f;
                for (uint j = 0; j < numShots; ++j)
                {
                    float currAngle = j * angleIncrement;
                    float radians = currAngle * M_PI / 180.f;
                    vec2 direction = vec2(cos(radians) * velocity, sin(radians) * velocity);
                    shoot(direction, pos, angle);
                }
                enemy.attackCooldown = COOLDOWN_SHOOT_MS;
            }
        }
    }
}


void EnemySystem::shoot(vec2 velocity, vec2 pos, float angle)
{
    // SHOOT STRAIGHT BASED ON ENEMIES DIRECTION
    createBulletEnemy(render, pos, velocity, angle + PLACEHOLDER_FOR_ANGLE);
}
