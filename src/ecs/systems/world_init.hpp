#pragma once

#include "common.hpp"
#include "tiny_ecs.hpp"
#include "render_system.hpp"

// These are hardcoded to the dimensions of the entity texture
// BB = bounding box

// the player
Entity createPlayer(RenderSystem* renderer, vec2 pos);

Entity createAimIndicator(RenderSystem* renderer);

// basic enemy
Entity createBlob(RenderSystem* renderer, vec2 pos);

// Wall for testing, denoted by 2 blobs
Entity createTestWall(RenderSystem* renderer, vec2 startPosition, vec2 endPosition);

// Poly for testing
Entity createTestPoly(RenderSystem* renderer, vec2 position, std::vector<vec2> points, float angle);

// Floor testing
Entity createTestFloor(RenderSystem* renderer, vec2 pos);

// a red line for debugging purposes
Entity createLine(vec2 position, vec2 size);

Entity drawLineAtoB(RenderSystem* renderer, vec2 a, vec2 b);

// the enemy
Entity createEnemy(RenderSystem* renderer, vec2 pos, vec2 velocity, EnemyAttackPattern atkPattern, EnemyBehavior behavior);

// Entity createBulletEnemy(RenderSystem* renderer, vec2 pos, vec2 velocity, float angle);
Entity createEnemyBullet(RenderSystem* renderer, vec2 pos, vec2 velocity, vec2 veer, AttackData atkData);

// a player-fired bullet
Entity createPlayerBullet(RenderSystem* renderer, vec2 position, vec2 direction);

float getModifiedValue(BulletEffectType bf, float value);

// create dialogue box
Entity createDialogueBox(vec2 position, vec2 scale);

Entity createPauseMenu(vec2 position, vec2 scale);

Entity createGameOverMenu(vec2 position, vec2 scale);