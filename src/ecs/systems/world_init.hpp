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

Entity createAimIndicator(RenderSystem* renderer = nullptr);

Entity createCursor();

Entity createCritter(RenderSystem* renderer, vec2 pos);

Entity createPhone(RenderSystem* renderer, vec2 pos);

Entity createJournal(RenderSystem* renderer, vec2 pos);

Entity createDesk(RenderSystem* renderer, vec2 pos);

Entity createDeskWithMouse(RenderSystem* renderer, vec2 pos);

Entity createOracleTurret ( RenderSystem *renderer, vec2 pos );

Entity CreateOven ( RenderSystem *renderer, vec2 pos );

Entity createWhiteBoard(RenderSystem* renderer, vec2 pos);

Entity createPopConsole(RenderSystem* renderer, vec2 pos);

Entity createGardener(RenderSystem* renderer, vec2 pos);

Entity createBibleTree(RenderSystem* renderer, vec2 pos);

Entity createProp(RenderSystem* renderer, vec2 pos, std::string filename, vec2 scale, vec2 shrink = vec2(1.f), bool solid = true);

Entity createProp3D(RenderSystem* renderer, vec2 pos, std::string filename, vec2 scale, vec2 wallOffset, float baseOffset, EFFECT_ASSET_ID effect = EFFECT_ASSET_ID::TEXTURED);

Entity createRamStick(RenderSystem* renderer, vec2 pos);

Entity createBaru(RenderSystem *renderer, vec2 pos);

Entity createWishGranter(RenderSystem *renderer, vec2 pos);

Entity createOracleCrab(RenderSystem *renderer, vec2 pos);

Entity createHoneyCanister(RenderSystem *renderer, vec2 pos);

Entity createInteractable(RenderSystem* renderer, vec2 pos, InteractableItem item, std::vector<BulletStackEffect> effects);

Entity createPushConsole(RenderSystem* renderer, vec2 pos, std::vector<BulletStackEffect> effects);

Entity createFightConsole(RenderSystem *renderer, vec2 pos, std::vector<BulletStackEffect> effects);

Entity createFloorDeco(RenderSystem* renderer, vec2 pos, std::string filename);

Entity createSwarm(RenderSystem* renderer, vec2 pos);

Entity createWall(RenderSystem* renderer, vec2 startPosition, vec2 endPosition);

Entity createDoor(RenderSystem* renderer, vec2 startPos,vec2 endPos);

Entity createDoors(RenderSystem* renderer, vec2 position, float angle, vec2 scale, float doorAngle, vec3 axis, vec3 offset, char side);

void createRoomBounds(RenderSystem* renderer, vec2 roomCenter, vec2 roomSize);

Entity createWallThickness(vec2 pos, vec2 scale);

// Poly for testing
Entity createTestPoly(RenderSystem* renderer, vec2 position, std::vector<vec2> points, float angle);

// Floor testing
Entity createFloor(RenderSystem* renderer, vec2 pos, vec2 scale);

// a red line for debugging purposes
Entity createLine(vec2 position, vec2 size);

Entity drawLineAtoB(RenderSystem* renderer, vec2 a, vec2 b);

// the enemy
Entity createEnemy(RenderSystem* renderer, vec2 pos, EnemyType type);

void createWormBody(RenderSystem* renderer, vec2 pos, EnemyType type, Entity head, int index);

void createEnemyGroup(RenderSystem * renderer, vec2 pos, EnemyType type);

// Entity createBulletEnemy(RenderSystem* renderer, vec2 pos, vec2 velocity, float angle);
Entity createEnemyBullet(RenderSystem* renderer, vec2 pos, vec2 velocity, vec2 veer, AttackData atkData);

Entity createEnemyBulletDeath(RenderSystem* renderer, vec2 pos, vec2 velocity, EnemyBulletDeath onDeath);

Entity createEnemyLaser(RenderSystem* renderer, vec2 pos, float angle, Entity start, AttackData atkData);

Entity createLightningBullet(RenderSystem* renderer, vec2 pos);

Entity createKeyBullet(RenderSystem* renderer, vec2 pos);

// a player-fired bullet
Entity createPlayerBullet(RenderSystem* renderer, vec2 position, vec2 direction);

Entity createSkipDialogue();

float getModifiedValue(BulletEffectType bf, float value);

std::vector<BulletStackEffect> getBulletEffects(AttackData atkData, bool& isSpecial);

vec2 lerpToRoom(vec2 point);