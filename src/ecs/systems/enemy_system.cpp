#include "enemy_system.hpp"
#include <iostream>
#include <glm/trigonometric.hpp>

#include "tiny_ecs_registry.hpp"
#include "common.hpp"
#include "world_init.hpp"
#include "actor_components.hpp"
#include <thread>
#include <vector>
#include <chrono>

#include "ai_system.hpp"
#include "ai_system.hpp"
#include "ai_system.hpp"
#include "ai_system.hpp"
#include "ai_system.hpp"
#include "ai_system.hpp"
#include "ai_system.hpp"
#include "ai_system.hpp"
#include "ai_system.hpp"
#include "ai_system.hpp"
#include "ai_system.hpp"
#include "ai_system.hpp"

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
    std::vector<Entity> delete_queue;

    Entity& player = registry.players.entities[0];
    Motion& playerMotion = motion_registry.get(player);
    
    for (uint i = 0; i < enemy_registry.components.size(); i++) {
        Enemy &enemy = enemy_registry.components[i];
        Entity &entity = enemy_registry.entities[i];

        Motion &motion = motion_registry.get(entity);
        AttackData& atkData = registry.attackDatas.get(entity);
        vec2 pos = motion.position;
        float angle = motion.angle;

        /*
      * Sky's AI logic: firing cooldown is handled by the shoot function, so no need to worry about that here
      * The number of bursts and the interval between bursts is stored in the PlayerAttackData component
      * change them per enemy type in createEnemy function in world_init.cpp
      * pass render and elapsed_ms directly
      * instead of an angle, pass a normalized vector. in the example, it's a vector pointing towards the player
      * after that, pass the number of shots in a spread (think shotgun) and the spread angle (angle from the leftmost to the rightmost bullet)
      * !! careful with the enemy death logic, this may crash if the enemy is removed before the function runs.
      * the check for shoots component is there to prevent that, still best to put it before the damage handling
      */

        //if (registry.shoots.has(entity)) {
        //    vec2 playerPos = registry.motions.get(registry.players.entities[0]).position;
        //    vec2 playerDir = playerPos - pos;
        //    playerDir = glm::normalize(playerDir);
        //    shoot(entity, pos, playerDir, elapsed_ms, 3, 30.f);
        //}

        // HANDLING DAMGE FROM COLLISION
        for (int i = collision_registry.entities.size() - 1; i >= 0; i--)
        {
            Entity& entity = collision_registry.entities[i];
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
                    delete_queue.push_back(entity);
                }

                registry.deleteEntityAndRelatedEntities(other_entity);
            }
        }

        enemy.attackCooldown -= elapsed_ms;
        // std::cout << "enemy attack in:" << enemy.attackCooldown << std::endl;
        if (enemy.attackCooldown < 0.f)
        {
            if (atkData.attackType == EnemyAttackPattern::SHOTGUN)
            {
                shootShotgun(playerMotion.position - pos, pos, atkData);
                enemy.attackCooldown = COOLDOWN_SHOOT_MS;
            }
            else if (atkData.attackType == EnemyAttackPattern::ALL_DIRECTION)
            {
                shootAllDirection(pos, atkData);
                enemy.attackCooldown = COOLDOWN_SHOOT_MS;
            }
            else if (atkData.attackType == EnemyAttackPattern::BURST || atkData.attackType == EnemyAttackPattern::SPRAY)
            {
                Burst& burst = registry.bursts.get(entity);
                vec2 velocity = playerMotion.position - pos;
                shootBurst(playerMotion.position - pos, pos, atkData, elapsed_ms, burst, enemy);
                burst.burstDirection = atan2(velocity.y, velocity.x);
                if (burst.curBurst <= 0)
                {
                    enemy.attackCooldown = COOLDOWN_SHOOT_MS;
                    burst.curBurst = atkData.maxBurst;
                }
            }
        }
        
    }
    for (Entity entity: delete_queue) {
        registry.deleteEntityAndRelatedEntities(entity);
    }
}

void EnemySystem::shootShotgun(vec2 velocity, vec2 pos, AttackData atkData) {
    float angle = atan2(velocity.y, velocity.x);
    if (atkData.numBullets % 2 == 0) {
        for (uint i = 0; i < atkData.numBullets / 2; i++) {
            float a1 = angle + (i + 0.5) * atkData.angleOffset;
            float a2 = angle - (i + 0.5) * atkData.angleOffset;
            createEnemyBullet(render, pos, {cos(a1), sin(a1)}, atkData.veer.x * vec2(cos(a1 + atkData.veer.y), sin(a1 + atkData.veer.y)), atkData);
            createEnemyBullet(render, pos, {cos(a2), sin(a2)}, atkData.veer.x * vec2(cos(a2 - atkData.veer.y), sin(a2 - atkData.veer.y)), atkData);
        }
    }
    else {
        createEnemyBullet(render, pos, { cos(angle), sin(angle) }, atkData.veer.x * vec2(cos(angle), sin(angle)), atkData);
        for (uint i = 0; i < (atkData.numBullets - 1) / 2; i++) {
            float a1 = angle + (i + 1) * atkData.angleOffset;
            float a2 = angle - (i + 1) * atkData.angleOffset;
            createEnemyBullet(render, pos, { cos(a1), sin(a1) }, atkData.veer.x * vec2(cos(a1 + atkData.veer.y), sin(a1 + atkData.veer.y)), atkData);
            createEnemyBullet(render, pos, { cos(a2), sin(a2) }, atkData.veer.x * vec2(cos(a2 - atkData.veer.y), sin(a2 - atkData.veer.y)), atkData);
        }
    }
}

void EnemySystem::shootAllDirection(vec2 pos, AttackData atkData) {
    for (uint i = 0; i < atkData.numBullets; i++) {
        float a = atkData.angleOffset + i * (2 * M_PI / atkData.numBullets);
        createEnemyBullet(render, pos, { cos(a), sin(a) }, atkData.veer.x * vec2(cos(a + atkData.veer.y), sin(a + atkData.veer.y)), atkData);
    }
}

void EnemySystem::shootBurst(vec2 velocity, vec2 pos, AttackData atkData, float elapsed_ms, Burst& burst, Enemy& enemy) {
    if ((burst.curBurst <= 0) || (burst.burstCooldown -= elapsed_ms) > 0) {
        return;
    }

    if (atkData.attackType == EnemyAttackPattern::SPRAY)
    {
        double range = atkData.angleOffset;

        // Generate a random offset within the range
        double offset = (2 * (static_cast<double>(rand()) / RAND_MAX) - 1) * range;
        offset = burst.burstDirection + offset;
        createEnemyBullet(render, pos, {cos(offset), sin(offset)}, atkData.veer.x * vec2(cos(offset + atkData.veer.y)),atkData);
    } else if (burst.curBurst != atkData.maxBurst) {
        float currentAngle = atan2(velocity.y, velocity.x);
        float angleDifference = currentAngle - burst.burstDirection;

        float maxDifference = M_PI / 32;
        if (abs(angleDifference) > maxDifference) {
            if (angleDifference > 0) {
                currentAngle = burst.burstDirection + maxDifference;
            } else {
                currentAngle = burst.burstDirection - maxDifference;
            }
        }

        float angle = currentAngle;
        std::cout << angle << std::endl;
        createEnemyBullet(render, pos, {cos(angle), sin(angle)}, atkData.veer.x * vec2(cos(angle + atkData.veer.y)), atkData);
    }
    burst.curBurst--;
    burst.burstCooldown = 150;


}





