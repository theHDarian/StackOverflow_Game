#pragma once
#include "common.hpp"
#include <array>
#include <utility>

#include "components.hpp"
#include "tiny_ecs.hpp"
#include "render_system.hpp"

#define MAX_ENEMY_SPAWN 300

class SoundSystem;
class EnemySystem
{
public:
    EnemySystem(RenderSystem *renderer, SoundSystem *sound);
    ~EnemySystem();
    void step(float Elapsed_ms);

private:
    // void shoot(::Entity &enemy, vec2 pos, vec2 bulletDir, float elapsed_ms_since_last_update, int cluster, float BulletAccuracy);
    void shootShotgun(vec2 velocity, vec2 pos, const AttackData &atkData);
    void shootAllDirection(vec2 pos, float offset, const AttackData &atkData);
    void shootRadialPolygon(vec2 pos, const AttackData &atkData);
    void shootBurst(vec2 velocity, vec2 pos, const AttackData &atkData, float elapsed_ms, Burst &burst);

    void grantBuff(Entity entity, const EnemyPattern &pattern);

    void shootWave(vec2 pos, const AttackData &atkData, float elapsed_ms, Burst &burst);
    void shootOneWall(const AttackData &atkData, float angle, float elapsed_ms);
    void shootTwoWall(const AttackData &atkData, float angle, float elapsed_ms);
    void shootRadialBurst(vec2 pos, const AttackData &atkData, float elapsed_ms, Burst &burst);
    void shootLaser(vec2 pos, Entity enemy, const AttackData &atkData, bool shouldPlayFiringSound = true);
    void shootTwinLaser(vec2 pos, Entity enemy, const AttackData &atkData);
    void launchBombard(const AttackData& atkData);
    void attack(Entity entity, EnemyPattern &currPattern, Motion playerMotion, vec2 pos, const AttackData &atkData, float elapsed_ms);
    void spawn(::Entity entity, ::EnemyPattern &currPattern, vec2 pos, AttackData &atkData) const;
    void merge(Entity entity, EnemyPattern &currPattern, std::vector<Entity> &pendingDeletion);
    void creatingMergeBee(int count, vec2 pos, bool isElite) const;
    void destruct(Enemy& enemy);
    void heal(Entity entoty, EnemyPattern &currPattern);
    void createAOEIndicator(vec2 pos, float radius, SpecialStates buff, float timer);

    void fetchRoomEffects(Entity entity, AttackData& atkData);
    RenderSystem *render;
    SoundSystem *sound;
};