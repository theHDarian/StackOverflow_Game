#include "tiny_ecs_registry.hpp"

ECSRegistry registry;

float getModifiedValue(BulletEffectType bf, float value)
{
    Entity& pl = registry.players.entities[0];
    return registry.stackCompile.get(pl).Call(bf) + value;
}

bool checkTierThreshold(BulletEffectType bf) {
    Entity& pl = registry.players.entities[0];
    return registry.stackCompile.get(pl).values[bf] >= registry.stackCompile.get(pl).tierThresholds[bf];
}

int getEffectValueTierThresholdDifference(BulletEffectType bf) {
    Entity& pl = registry.players.entities[0];
    return registry.stackCompile.get(pl).values[bf] - registry.stackCompile.get(pl).tierThresholds[bf];
}

int getEffectValue(BulletEffectType bf) {
    Entity& pl = registry.players.entities[0];
    return registry.stackCompile.get(pl).values[bf];
}

int getEffectTierThreshold(BulletEffectType bf) {
    Entity& pl = registry.players.entities[0];
    return registry.stackCompile.get(pl).tierThresholds[bf];
}