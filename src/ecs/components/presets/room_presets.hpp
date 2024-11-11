#pragma once
#include <vector>
#include <tuple>
#include "components/map_components.hpp"
#include "premades.hpp"
#include "common.hpp"
#include "tiny_ecs_registry.hpp"
#include "utils/random.hpp"

//---------- PRESETS-------------------
// struct RoomPreset {
//     std::vector<std::tuple<EnemyType,vec2>> enemies;
//     std::vector<std::tuple<BulletStackEffect,vec2>> treasures; //for treasure rooms
//     std::vector<std::tuple<RoomProp,vec2>> roomProps; //background props
//     std::vector<SpecialEvent> specialEvents;
//     float spawnDelay; //in seconds - for enemies and bosses
//     int numSpecialBulletsToSpawn = 5;
// };

const RoomPreset TutorialRoom1Preset{
    {},
    {},
    {},
    {},
    100000.0f,
    0};

const RoomPreset TutorialRoom2Preset{
    {
        {EnemyType::EasyEnemySkull, {0.5f, 0.5f}},
    },
    {},
    {},
    {},
    0.0f,
    0};

const RoomPreset EnemyRoomDash1{
    {{EnemyType::EasyEnemySentry, {0.2f, 0.8f}},
     {EnemyType::EasyEnemySentry, {0.8f, 0.8f}},
     {EnemyType::EasyEnemySentry, {0.8f, 0.2f}},
     {EnemyType::EasyEnemySentry, {0.2f, 0.2f}},
     {EnemyType::TestRevampedEnemy, {0.2, 0.4}},
     {EnemyType::TestRevampedEnemy, {0.8, 0.4}},
     {EnemyType::EasyEnemySkull, {0.2f, 0.4f}},
     {EnemyType::EasyEnemySkull, {0.8f, 0.4f}}
     },
    {},
    {},
    {},
    0.0f,
    5};
const RoomPreset EnemyRoomBees1{
    {{EnemyType::OneBee, {0.2f, 0.8f}},
     {EnemyType::OneBee, {0.8f, 0.8f}},
     {EnemyType::OneBee, {0.8f, 0.2f}},
     {EnemyType::OneBee, {0.2f, 0.2f}},
     {EnemyType::BeeHive, {0.2f, 0.4f}},
     {EnemyType::BeeHive, {0.8f, 0.4f}}},
    {},
    {},
    {},
    0.0f,
    5};

const RoomPreset EnemyRoomTripleBuff1{
    {{EnemyType::MediumEnemyCharge, {0.2f, 0.8f}},
     {EnemyType::MediumEnemyCharge, {0.8f, 0.2f}},
     {EnemyType::MediumEnemyTank, {0.2f, 0.2f}},
     {EnemyType::HardEnemyAngel, {0.2f, 0.4f}},
     {EnemyType::HardEnemyAngel, {0.8f, 0.4f}}},
    {},
    {},
    {},
    0.0f,
    5};
const RoomPreset RestingRoom1{
    {},
    {},
    {},
    {SpecialEvent::RebootStation}};
const RoomPreset TreasureRoom1{
    {},
    {{threeBurst, {0.5f, 0.5f}}},
    {},
    {},
    0.0f,
    5,
};

const RoomPreset BossRoom1{
    {{EnemyType::BossBigC, {0.5f, 0.5f}}},
    {},
    {},
    {},
    0.0f,
    2};

const std::map<RoomType, std::vector<RoomPreset>> roomDirectory = {
    {RoomType::EnemyRoomDash, {EnemyRoomDash1}},
    {RoomType::EnemyRoomTripleBuff, {EnemyRoomTripleBuff1}},
    {RoomType::EnemyRoomBee, {EnemyRoomBees1}},
    {RoomType::RestRoom, {RestingRoom1}},
    {RoomType::TreasureRoom, {TreasureRoom1}},
    {RoomType::BossBigCRoom, {BossRoom1}}};
