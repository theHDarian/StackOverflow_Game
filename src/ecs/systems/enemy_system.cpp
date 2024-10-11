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
    
    for (uint i = 0; i < enemy_registry.components.size(); i++) {
        Enemy &enemy = enemy_registry.components[i];
        Entity &entity = enemy_registry.entities[i];

        Motion &motion = motion_registry.get(entity);
        vec2 pos = motion.position;
        float angle = motion.angle;

        /*
      * Sky's AI logic: firing cooldown is handled by the shoot function, so no need to worry about that here
      * The number of bursts and the interval between bursts is stored in the Shoots component
      * change them per enemy type in createEnemy function in world_init.cpp
      * pass render and elapsed_ms directly
      * instead of an angle, pass a normalized vector. in the example, it's a vector pointing towards the player
      * after that, pass the number of shots in a spread (think shotgun) and the spread angle (angle from the leftmost to the rightmost bullet)
      * !! careful with the enemy death logic, this may crash if the enemy is removed before the function runs.
      * the check for shoots component is there to prevent that, still best to put it before the damage handling
      */

        if (registry.shoots.has(entity)) {
            vec2 playerPos = registry.motions.get(registry.players.entities[0]).position;
            vec2 playerDir = playerPos - pos;
            playerDir = glm::normalize(playerDir);
            shoot(entity, pos, playerDir, elapsed_ms, 3, 30.f);
        }

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
    for (Entity entity: delete_queue) {
        registry.remove_all_components_of(entity);
    }
}


void EnemySystem::shoot(vec2 velocity, vec2 pos, float angle) {
    // SHOOT STRAIGHT BASED ON ENEMIES DIRECTION
    createBulletEnemy(render, pos, velocity, angle + PLACEHOLDER_FOR_ANGLE);
}

void EnemySystem::shoot(Entity& enemy, vec2 pos, vec2 bulletDir, float elapsed_ms_since_last_update, int cluster, float BulletSpread) {
    //SHOOT STRAIGHT BASED ON ENEMIES DIRECTION
    // createBulletEnemy(render, pos, vec2(-100, 0), angle + PLACEHOLDER_FOR_ANGLE);
    Shoots& pl = registry.shoots.get(enemy);
    if (pl.currFiringInterval > 0) {
        pl.currFiringInterval -= elapsed_ms_since_last_update;
    }
    if (pl.bulletBurstCooldown > 0) {
        pl.bulletBurstCooldown -= elapsed_ms_since_last_update;
    }
    if (pl.currFiringInterval <= 0) {
        pl.currBulletBurst = pl.maxBulletBurst;
        pl.currFiringInterval = pl.maxFiringInterval;
    }
    if (pl.bulletBurstCooldown <= 0 && pl.currBulletBurst > 0) {
        //convert interval from ms to rounds per second for getModifiedValue, then back to ms
        pl.currBulletBurst--;
        pl.bulletBurstCooldown = min(
            50.0f,
            (pl.maxFiringInterval / pl.maxBulletBurst)
        );
        // create bullet
        vec2 bulletPos = pos;

        if (cluster == 1) {
            createEnemyBullet(render, bulletPos, bulletDir, pl.bulletSpeed);
            return;
        }

        // Calculate the offset between bullets for cluster shots

        float offSet = radians(BulletSpread) / cluster;
        // Create a rotation matrix
        glm::mat2 rotationMatrix = glm::mat2(
            glm::cos(offSet), -glm::sin(offSet),
            glm::sin(offSet),  glm::cos(offSet)
        );


        if (cluster == 2) {
            createEnemyBullet(render, bulletPos, bulletDir*rotationMatrix, pl.bulletSpeed);
            createEnemyBullet(render, bulletPos, bulletDir*glm::transpose(rotationMatrix), pl.bulletSpeed);
            return;
        }

        for (int i = 0; i < cluster; i++) {
            if (i == 0) {
                createEnemyBullet(render, bulletPos, bulletDir, pl.bulletSpeed);
                continue;
            }
            for (int j = 0; j < i; j++) {
                if (i % 2 == 0) {
                    bulletDir = bulletDir * rotationMatrix;
                }
                else {
                    bulletDir = bulletDir * glm::transpose(rotationMatrix);
                }
            }
            createEnemyBullet(render, bulletPos, bulletDir, pl.bulletSpeed);
        }
    }
}





