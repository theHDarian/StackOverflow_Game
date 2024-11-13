//
// Created by sky on 2024-11-12.
//

#include "interactable_effects.h"

#include "tiny_ecs.hpp"
#include "tiny_ecs_registry.hpp"
#include "world_init.hpp"

void resetStack(Entity player, RenderSystem* renderer) {

    Invincible& inv =registry.invincibles.emplace(player);
    inv.countdown = 1000.0f;

    if (registry.stackCompile.has(player)) {
        StackCompile& reg = registry.stackCompile.get(player);
        int size = reg.baseStackSize;
        int i = 0;
        for (BulletStackEffect b : reg.currStack) {
            AttackData atkData = AttackData();
            atkData.defaultEffect = b;
            atkData.rareBulletEffects = {b};
            atkData.speed = 100;
            atkData.size *= 2;
            atkData.bulletRange = 7500;
            float angle = (2 * M_PI / size) * i;
            createEnemyBullet( renderer, registry.motions.get(player).position, {cos(angle), sin(angle)}, {0, 0}, atkData);
            i++;
        }
        registry.stackCompile.remove(player);
        StackCompile& newreg = registry.stackCompile.emplace(player);

    }
}

void unlockDoor(Entity door) {
    //todo
}



