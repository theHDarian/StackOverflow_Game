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
//     std::vector<std::tuple<BossType,vec2>> bosses;
//     std::vector<SpecialEvent> specialEvents; 
//     float spawnDelay; //in seconds - for enemies and bosses
//     int numSpecialBulletsToSpawn = 5;
// };

const RoomPreset EnemyRoomDash1 {
    {
        {EnemyType::EasyEnemySentry,{0.2f,0.8f}},
        {EnemyType::EasyEnemySentry,{0.8f,0.8f}},
        {EnemyType::EasyEnemySentry,{0.8f,0.2f}},
        {EnemyType::EasyEnemySentry,{0.2f,0.2f}}
    },
    {},
    {},
    {},
    {},
    5.0f,
    5
};

const RoomPreset EnemyRoomTripleBuff1 {
    {
        {EnemyType::MediumEnemyCharge,{0.2f,0.8f}}, 
        {EnemyType::MediumEnemyCharge,{0.8f,0.2f}},
    },
    {},
    {},
    {},
    {},
    5.0f,
    5
};
const RoomPreset RestingRoom1 {
    {},
    {},
    {},
    {},
    {SpecialEvent::RebootStation}
};
const RoomPreset TreasureRoom1 {
    {},
    {
        {sizeUpA,{0.5f,0.5f}}
    },
    {},
    {},
    {},
};

const std::map<RoomType,std::vector<RoomPreset>> roomDirectory = {
    {RoomType::EnemyRoomDash,{EnemyRoomDash1}},
    {RoomType::EnemyRoomTripleBuff,{EnemyRoomTripleBuff1}},
    {RoomType::RestRoom,{RestingRoom1}},
    {RoomType::TreasureRoom,{TreasureRoom1}}
};





