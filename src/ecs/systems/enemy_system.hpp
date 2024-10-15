#pragma once
#include "common.hpp"
#include <array>
#include <utility>

#include "components.hpp"
#include "tiny_ecs.hpp"
#include "render_system.hpp"


class EnemySystem {
public:
    EnemySystem(RenderSystem* renderer);
    ~EnemySystem();
    void step(float elapsed_ms);
private:
    std::vector<Entity> delete_queue;

    static void nextAtkData(Enemy& enemy, Entity& entity);
    //void shoot(::Entity &enemy, vec2 pos, vec2 bulletDir, float elapsed_ms_since_last_update, int cluster, float BulletSpread);
    void shootShotgun(vec2 velocity, vec2 pos, AttackData atkData);
    void shootAllDirection(vec2 pos, AttackData atkData);
    void shootBurst(vec2 velocity, vec2 pos, AttackData atkData, float elapsed_ms, Burst &burst, Enemy &enemy);
    RenderSystem* render;
};