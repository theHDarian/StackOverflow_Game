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

void resetStack(Entity player, RenderSystem* renderer, float offset = 150);
void extendStack (Entity player, int extension);
void addEffect(Entity player, std::vector<BulletStackEffect> effects, SoundSystem* soundPlayer);
void interact(float elapsed_ms, Entity player, RenderSystem* renderer, SoundSystem* soundPlayer);
void resetDashes ();
