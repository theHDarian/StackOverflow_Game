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
        if (reg.currStack.size() == 0) {
            return;
        }
        int size = reg.baseStackSize;
        int i = 0;
        for (BulletStackEffect b : reg.currStack) {
            AttackData atkData = AttackData();
            atkData.defaultEffect = b;
            atkData.rareBulletEffects = {b};
            atkData.speed = 300;
            atkData.size *= 2;
            atkData.bulletRange = 7500;
            atkData.bulletBounce = 3;
            float angle = (2 * M_PI / size) * i;
            createEnemyBullet( renderer, registry.motions.get(player).position + 150.f * vec2(cos(angle), sin(angle)), {cos(angle), sin(angle)}, vec2(0), atkData);
            i++;
        }
        // reg.currStack.clear();
        registry.stackCompile.remove(player);
        StackCompile& newreg = registry.stackCompile.emplace(player);
        newreg.baseStackSize = size;
        Player& pl = registry.players.get(player);
        pl.currDashCharges = pl.baseDashNum;
        pl.currDashCooldown = pl.baseDashCDR;
        StackUI& ui = registry.stackUI.components[0];
        ui.updateStackUISize(reg.baseStackSize);

    }
}

void extendStack (Entity player, int extension) {
    if (registry.stackCompile.has(player)) {
        StackCompile& reg = registry.stackCompile.get(player);
        reg.baseStackSize += extension;
        StackUI& ui = registry.stackUI.components[0];
        ui.updateStackUISize(reg.baseStackSize);
    }
}

void addEffect(Entity player, std::vector<BulletStackEffect> effects) {
    if (registry.stackCompile.has(player)) {
        StackCompile& reg = registry.stackCompile.get(player);
        for (BulletStackEffect b : effects) {
            printf("Adding: %s\n",b.name.c_str());
            reg.add(b);
        }
    }
}



