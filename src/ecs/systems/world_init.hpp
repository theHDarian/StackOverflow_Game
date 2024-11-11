#pragma once

#include "common.hpp"
#include "tiny_ecs.hpp"
#include "render_system.hpp"
#include "map_components.hpp"

// These are hardcoded to the dimensions of the entity texture
// BB = bounding box

// the player
Entity createPlayer(RenderSystem* renderer, vec2 pos);

Entity resetPlayer();

Entity createAimIndicator(RenderSystem* renderer);

Entity createCursor();

Entity createCritter(RenderSystem* renderer, vec2 pos);

Entity createPopConsole(RenderSystem* renderer, vec2 pos);

Entity createGardener(RenderSystem* renderer, vec2 pos);

Entity createBibleTree(RenderSystem* renderer, vec2 pos);

Entity createWall(RenderSystem* renderer, vec2 startPosition, vec2 endPosition);

Entity createDoor(RenderSystem* renderer, vec2 startPos,vec2 endPos);

void createRoomBounds(RenderSystem* renderer);

// Poly for testing
Entity createTestPoly(RenderSystem* renderer, vec2 position, std::vector<vec2> points, float angle);

// Floor testing
Entity createTestFloor(RenderSystem* renderer, vec2 pos);

// a red line for debugging purposes
Entity createLine(vec2 position, vec2 size);

Entity drawLineAtoB(RenderSystem* renderer, vec2 a, vec2 b);

// the enemy
Entity createEnemy(RenderSystem* renderer, vec2 pos, EnemyType type);

// Entity createBulletEnemy(RenderSystem* renderer, vec2 pos, vec2 velocity, float angle);
Entity createEnemyBullet(RenderSystem* renderer, vec2 pos, vec2 velocity, vec2 veer, AttackData atkData);

Entity createEnemyBulletDeath(RenderSystem* renderer, vec2 pos, vec2 velocity, EnemyBulletDeath onDeath);

Entity createEnemyLaser(RenderSystem* renderer, vec2 pos, float angle, Entity start, AttackData atkData);

// a player-fired bullet
Entity createPlayerBullet(RenderSystem* renderer, vec2 position, vec2 direction);

float getModifiedValue(BulletEffectType bf, float value);

std::vector<BulletStackEffect> getBulletEffects(AttackData atkData);