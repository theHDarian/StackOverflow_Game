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
//     std::vector<std::tuple<AttackData,vec2>> treasures; //for treasure rooms
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
         {EnemyType::BeeHive, {0.5f, 0.5f}}},
    {},
    {},
    {},
    0.0f,
    0 
};

const RoomPreset TutorialRoom3Preset{
    {{EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}}},
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
     {EnemyType::EasyEnemySkull, {0.5f, 0.5f}}
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

const RoomPreset EnemyRoomAngelTank{
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

const RoomPreset TreasureRoom2{
    {},
    {{fiveBurst, {0.5f, 0.5f}}},
    {},
    {},
    0.0f,
    5,
};
const RoomPreset TreasureRoom3{
    {},
    {{laserNoRotate, {0.5f, 0.5f}}},
    {},
    {},
    0.0f,
    5,
};

const RoomPreset TreasureRoom4{
    {},
    {{twelveSpiralShot, {0.5f, 0.5f}}},
    {},
    {},
    0.0f,
    5,
};
const RoomPreset TreasureRoomBad{
    {},
    {{threeHomingShot, {0.5f, 0.5f}}},
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

//used by roomsTraversed to determine type of enemy to spawn, some enemies only spawn in certain difficulty regions
enum DifficultyRegion {
    Intro,
    Easy,
    Medium,
    // Hard,
};

const std::map<DifficultyRegion,std::map<RoomType, std::vector<RoomPreset>>> roomDirectory = {
    {DifficultyRegion::Intro,{
        {RoomType::EnemyRoom, {EnemyRoomDash1}},
        {RoomType::RestRoom, {RestingRoom1}},
        {RoomType::TreasureRoom, {TreasureRoom1,TreasureRoom2,TreasureRoom3,TreasureRoom4}},
    }},
    {DifficultyRegion::Easy,{
        {RoomType::EnemyRoom, {EnemyRoomBees1}},
        {RoomType::RestRoom, {RestingRoom1}},
        {RoomType::TreasureRoom, {TreasureRoom1,TreasureRoom2,TreasureRoom3,TreasureRoom4}},
    }},
    {DifficultyRegion::Medium,{
        {RoomType::EnemyRoom, {EnemyRoomAngelTank}},
        {RoomType::RestRoom, {RestingRoom1}},
        {RoomType::TreasureRoom, {TreasureRoom1,TreasureRoom2,TreasureRoom3,TreasureRoom4}},
    }},
};

//TODO add locked rarity items
RoomPreset getRoomPreset(RoomType type, int roomsTraversed, bool locked) {
    if (type == RoomType::TutorialRoom2) {
        return TutorialRoom2Preset;
    }
    if (roomsTraversed < 3) {
        return Random::ListItem(roomDirectory.at(Intro).at(type));
    } else if (roomsTraversed < 5) {
        return Random::ListItem(roomDirectory.at(Easy).at(type));
    } else {
        return Random::ListItem(roomDirectory.at(Medium).at(type));
    }
}

const std::map<RoomType,int> roomTypeToSymbols = {
    {RoomType::EnemyRoom,3},
    {RoomType::RestRoom,1},
    {RoomType::BossBigCRoom,2},
    {RoomType::TreasureRoom,0},
    {RoomType::None,4},
    {RoomType::TutorialRoom1,5},
    {RoomType::TutorialRoom2,5}
};
