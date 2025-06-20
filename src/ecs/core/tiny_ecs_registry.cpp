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

float getAdjustedTime (float elapsed_time, Entity entity)
{
    if (registry.players.has(entity) || registry.playerBullets.has(entity))
        return elapsed_time; // no time modifiers for player or player bullets
    float adjustedTime = elapsed_time;
    if (registry.timeModifiers.has(entity))
    {
        TimeModifier& timeModifier = registry.timeModifiers.get(entity);
        if (!(timeModifier.countDown <= 0 && timeModifier.coolDown > 0 && timeModifier.coolDown != -9999))
        {
            adjustedTime *= timeModifier.modifier;
        }
    }
    if (registry.timeModifiers.has(registry.players.entities[0]))
    {
        TimeModifier& timeModifier = registry.timeModifiers.get(registry.players.entities[0]);
        if (!(timeModifier.countDown <= 0 && timeModifier.coolDown > 0 && timeModifier.coolDown != -9999))
        {
            adjustedTime *= timeModifier.modifier;
        }

    }
    return adjustedTime;
}