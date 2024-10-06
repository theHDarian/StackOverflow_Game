#include "enemy_system.hpp"
#include <iostream>
#include "tiny_ecs_registry.hpp"
#include "common.hpp"
#include "world_init.hpp"
#include "actor_components.hpp"

float COOLDOWN_SHOOT_MS = 2000;
float BASE_BULLET_SPEED = 1;
float PLACEHOLDER_FOR_ANGLE = 0.f;


EnemySystem:: EnemySystem(RenderSystem* renderer) 
{
    render = renderer;
};

EnemySystem:: ~EnemySystem()
{
};

void EnemySystem:: step(float elapsed_ms) {
    auto& enemy_registry = registry.enemies;
    auto& motion_registry = registry.motions;
    for(uint i = 0; i < enemy_registry.size(); i++) {
        Enemy& enemy = enemy_registry.components[i];
        Entity& entity = enemy_registry.entities[i];
        Motion& motion = motion_registry.get(entity);
        vec2 pos = motion.position;
        float angle = motion.angle;

        enemy.attackCooldown -= elapsed_ms;
        std::cout << "enemy attack in:" << enemy.attackCooldown << std::endl;
        if (enemy.attackCooldown < 0.f) {
            if (enemy.attackPattern == EnemyAttackPattern::SINGLE_SHOT)  {
                shoot(enemy, pos, angle);
                enemy.attackCooldown = COOLDOWN_SHOOT_MS;
            } else if (enemy.attackPattern == EnemyAttackPattern::DOUBLE_SHOT) {
                shoot(enemy, pos, angle);
                shoot(enemy, pos, angle + M_PI_2);
                enemy.attackCooldown = COOLDOWN_SHOOT_MS;
            }
        }
    }
}

void EnemySystem:: shoot(Enemy& enemy, vec2 pos, float angle) {
    //SHOOT STRAIGHT BASED ON ENEMIES DIRECTION
    createBulletEnemy(render, pos, vec2(-100, 0), angle + PLACEHOLDER_FOR_ANGLE);
}





