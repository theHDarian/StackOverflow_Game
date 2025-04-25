#include "tiny_ecs_registry.hpp"

ECSRegistry registry;

bool checkTierThreshold(BulletEffectType bf) {
    Entity& pl = registry.players.entities[0];
    return registry.stackCompile.get(pl).values[bf] >= registry.stackCompile.get(pl).tierThresholds[bf];
}