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
    void shoot(vec2 velocity, vec2 pos, float angle);
    RenderSystem* render;
};