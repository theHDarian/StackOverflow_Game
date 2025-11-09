//
// Created by sky on 2024-11-12.
//
#pragma once
#include "common.hpp"
#include "tiny_ecs.hpp"
#include "render_system.hpp"
#include "map_components.hpp"

#ifndef INTERACTABLE_EFFECTS_H
#define INTERACTABLE_EFFECTS_H

#endif //INTERACTABLE_EFFECTS_H

class SoundSystem;

void resetStack(const Entity& player, RenderSystem* renderer, float offset = 150, const std::vector<BulletStackEffect> &effects = {});
void extendStack (Entity player, int extension);
void addEffect(const Entity &player, const std::vector<BulletStackEffect> &effects, SoundSystem* soundPlayer);
void interact(float elapsed_ms, Entity &player, RenderSystem* renderer, SoundSystem* soundPlayer);
void resetDashes ();
void CreateXPopBullets(RenderSystem* renderer, vec2 position, float direction, std::vector<BulletStackEffect> effects, float angleRange = 2.0f * M_PI, float offset = 150);
void knockEffectsOffStack (const Entity &player, RenderSystem* renderer, int numEffects);
void eatEffectsOffStack (const Entity &player, const Entity &other, int numEffects);
