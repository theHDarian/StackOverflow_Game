#include "enemy_system.hpp"
#include <iostream>
#include <glm/trigonometric.hpp>

#include "tiny_ecs_registry.hpp"
#include "common.hpp"
#include "world_init.hpp"
#include "actor_components.hpp"

float COOLDOWN_SHOOT_MS = 1000;
float BASE_BULLET_SPEED = 1;
float PLACEHOLDER_FOR_ANGLE = 0.f;


EnemySystem:: EnemySystem(RenderSystem* render)
{
    render = render;
};

EnemySystem:: ~EnemySystem()
{
};

void EnemySystem:: step(float elapsed_ms) {
    // auto& enemy_registry = registry.enemies;
    // auto& motion_registry = registry.motions;
    // for(uint i = 0; i < enemy_registry.components.size(); i++) {
    //     Enemy& enemy = enemy_registry.components[i];
    //     Entity& entity = enemy_registry.entities[i];
    //
    //     Motion& motion = motion_registry.get(entity);
    //     vec2 pos = motion.position;
    //     float angle = motion.angle;
    //
    //     enemy.attackCooldown -= elapsed_ms;
    //     //std::cout << "enemy attack in:" << enemy.attackCooldown << std::endl;
    //     // if (enemy.attackCooldown < 0.f) {
    //         if (enemy.attackPattern == EnemyAttackPattern::SINGLE_SHOT)  {
    //             // shoot(enemy, pos, angle);
    //             enemy.attackCooldown = COOLDOWN_SHOOT_MS;
    //         } else if (enemy.attackPattern == EnemyAttackPattern::DOUBLE_SHOT) {
    //             // shoot(enemy, pos, angle);
    //             // shoot(enemy, pos, angle + 30);
    //             shoot(entity, pos, angle + 180, elapsed_ms, 3, 30);
    //             enemy.attackCooldown = COOLDOWN_SHOOT_MS;
    //         }
    //     // }
    // }

    for (Entity& entity : registry.enemies.entities) {
        if (registry.enemies.has(entity)) {
            Enemy& enemy = registry.enemies.get(entity);
            Motion& motion = registry.motions.get(entity);
            float angle = motion.angle;
            vec2 pos = motion.position;
            shoot(entity, pos, angle + 180, elapsed_ms, 3, 30);
        }
    }
}

void EnemySystem:: shoot(Entity& enemy, vec2 pos, float angle, float elapsed_ms_since_last_update, int cluster, float BulletSpread) {
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

        float angleRad = radians(angle);
        vec2 bulletDir = glm::normalize(vec2(cos(angleRad), sin(angleRad)));
        vec2 bulletPos = pos;

        if (cluster == 1) {
            createBulletEnemy(render, bulletPos, bulletDir, pl.bulletSpeed);
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
            createBulletEnemy(render, bulletPos, bulletDir*rotationMatrix, pl.bulletSpeed);
            createBulletEnemy(render, bulletPos, bulletDir*glm::transpose(rotationMatrix), pl.bulletSpeed);
            return;
        }

        for (int i = 0; i < cluster; i++) {
            if (i == 0) {
                createBulletEnemy(render, bulletPos, bulletDir, pl.bulletSpeed);
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
            createBulletEnemy(render, bulletPos, bulletDir, pl.bulletSpeed);
        }
    }
}





