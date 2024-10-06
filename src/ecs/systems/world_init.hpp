#pragma once

#include "common.hpp"
#include "tiny_ecs.hpp"
#include "render_system.hpp"

// These are hardcoded to the dimensions of the entity texture
// BB = bounding box
const float FISH_BB_WIDTH  = 0.6f * 165.f;
const float FISH_BB_HEIGHT = 0.6f * 165.f;
const float EEL_BB_WIDTH   = 0.6f * 300.f;	// 1001
const float EEL_BB_HEIGHT  = 0.6f * 202.f;	// 870

// the player
Entity createPlayer(RenderSystem* renderer, vec2 pos);

// basic enemy
Entity createBlob(RenderSystem* renderer, vec2 pos);

// Wall for testing, denoted by 2 blobs
Entity createTestWall(RenderSystem* renderer, vec2 startPosition, vec2 endPosition);

// Poly for testing
Entity createTestPoly(RenderSystem* renderer, vec2 position, std::vector<vec2> points, float angle);

// a red line for debugging purposes
Entity createLine(vec2 position, vec2 size);

// a player-fired bullet
Entity createPlayerBullet(RenderSystem* renderer, vec2 position, vec2 direction, float damage, float range, float speed, float size, int pierce, int bounce);

