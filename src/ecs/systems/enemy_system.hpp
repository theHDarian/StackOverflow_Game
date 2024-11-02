#pragma once
#include "common.hpp"
#include <array>
#include <utility>

#include "components.hpp"
#include "tiny_ecs.hpp"
#include "render_system.hpp"

class SoundSystem;
class EnemySystem {
public:
    EnemySystem(RenderSystem* renderer, SoundSystem* sound);
    ~EnemySystem();
    void step(float elapsed_ms);
private:
    //void shoot(::Entity &enemy, vec2 pos, vec2 bulletDir, float elapsed_ms_since_last_update, int cluster, float BulletSpread);
    void shootShotgun(vec2 velocity, vec2 pos, AttackData atkData);
    void shootAllDirection(vec2 pos, AttackData atkData);
    void shootRadialPolygon(vec2 pos, AttackData atkData);
    void shootBurst(vec2 velocity, vec2 pos, AttackData atkData, float elapsed_ms, Burst &burst);
    void shootWave(vec2 pos, AttackData atkData, float elapsed_ms, Burst& burst);
    void shootRadialBurst(vec2 pos, AttackData atkData, float elapsed_ms, Burst& burst);
    void shootLaser(vec2 pos, Entity enemy, AttackData atkData);
    void attack(Entity entity, EnemyPattern& currPattern, Motion playerMotion, vec2 pos, AttackData atkData, float elapsed_ms);
    void merge(Entity entity, EnemyPattern& currPattern, std::vector<Entity> &pendingDeletion);
    void creatingMergeBee(int count, vec2 pos);
    RenderSystem* render;
    SoundSystem* sound;
};