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
    {},
    100000.0f,
    0};

const RoomPreset TutorialRoom2Preset{
    {
         {EnemyType::EvilSnail, {0.5f, 0.5f}}},
    {},
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
    {},
    0.0f,
    5};
const RoomPreset EnemyRoomTripleBuffEX{
    {{EnemyType::MediumEnemyCharge, {0.2f, 0.8f}},
        {EnemyType::MediumEnemyTank, {0.2f, 0.2f}},
    {EnemyType::MediumEnemyTank, {0.2f, 0.2f}},
    {EnemyType::MediumEnemyTank, {0.2f, 0.2f}},
    {EnemyType::MediumEnemyTank, {0.2f, 0.2f}},
        {EnemyType::HardEnemyAngel, {0.8f, 0.4f}}},
    {},
    {},
    {},
    {},
    0.0f,
    35};
const RoomPreset RestingRoom1{
    {},
    {},
    {},
    {{InteractableItem::PopConsole, {0.5f, 0.5f}}},
    {SpecialEvent::RebootStation}
};

const RoomPreset TreasureRoom1{
    {},
    {numBulletsUpA, sizeUpA},
    {},

    {{PushConsole, {0.5f, 0.5f}}},
{},
    0.0f,
    5,
};

const RoomPreset TreasureRoom2{
    {},
    {dashUpA},
    {},
    {{PushConsole, {0.5f, 0.5f}}},
{},
    0.0f,
    5,
};

const RoomPreset TreasureRoom3{
    {},
    {dmgUpM},
    {},
    {{PushConsole, {0.5f, 0.5f}}},
{},
    0.0f,
    5,
};

const RoomPreset TreasureRoom4{
    {},
    {dashCDRDownA},
    {},
    {{PushConsole, {0.5f, 0.5f}}},
{},
    0.0f,
    5,
};

const RoomPreset TreasureRoomRam{
    {},
    {},
    {},
    { {InteractableItem::Ram, {0.5f, 0.5f}}},
    {},
    0.0f,
    0,
};

const RoomPreset BossRoom1{
    {{EnemyType::BossBigC, {0.5f, 0.5f}}},
    {},
    {},
    {},
    {},
    0.0f,
    2};

//used by roomsTraversed to determine type of enemy to spawn, some enemies only spawn in certain difficulty regions
enum DifficultyRegion {
    Intro = 3,
    Easy = 5,
    Medium = 7,
    // Hard,
};

struct RoomPresets {
    std::vector<RoomPreset> unlocked;
    std::vector<RoomPreset> locked;
};

const std::map<DifficultyRegion,std::map<RoomType, RoomPresets>> roomDirectory = {
    {DifficultyRegion::Intro,{
        {RoomType::EnemyRoom, {{EnemyRoomDash1},{}}},
        {RoomType::RestRoom, {{RestingRoom1},{}}},
        {RoomType::TreasureRoom, {{TreasureRoom1,TreasureRoom2,TreasureRoom3,TreasureRoom4},{TreasureRoomRam}}},
    }},
    {DifficultyRegion::Easy,{
        {RoomType::EnemyRoom, {{EnemyRoomBees1},{}}},
        {RoomType::RestRoom, {{RestingRoom1},{}}},
        {RoomType::TreasureRoom, {{TreasureRoom1,TreasureRoom2,TreasureRoom3,TreasureRoom4},{TreasureRoomRam}}},
    }},
    {DifficultyRegion::Medium,{
        {RoomType::EnemyRoom, {{EnemyRoomAngelTank},{EnemyRoomTripleBuffEX}}},
        {RoomType::RestRoom, {{RestingRoom1},{}}},
        {RoomType::TreasureRoom, {{TreasureRoom1,TreasureRoom2,TreasureRoom3,TreasureRoom4},{TreasureRoomRam}}},
    }},
};

inline bool hasLocked(RoomType type, int roomsTraversed) {
    if (type >= RoomType::None) {
        return false;
    }
    if (roomsTraversed < DifficultyRegion::Intro) {
        return !roomDirectory.at(Intro).at(type).locked.empty();
    } else if (roomsTraversed < DifficultyRegion::Easy) {
        return !roomDirectory.at(Easy).at(type).locked.empty();
    } else if (roomsTraversed < DifficultyRegion::Medium) {
        return !roomDirectory.at(Medium).at(type).locked.empty();
    } else {
        return !roomDirectory.at(Medium).at(type).locked.empty();
    }
}

RoomPreset getRoomPreset(RoomType type, int roomsTraversed, bool locked) {
    //boss rooms
    if (type == RoomType::BossBigCRoom) {
        return BossRoom1;
    }

    //tutorial rooms
    if (type == RoomType::TutorialRoom2) {
        return TutorialRoom2Preset;
    }

    //regular rooms
    if(!hasLocked(type,roomsTraversed) && locked) {
        assert(false);
    }
    if (roomsTraversed < DifficultyRegion::Intro) {
        return Random::ListItem(locked ? roomDirectory.at(Intro).at(type).locked : roomDirectory.at(Intro).at(type).unlocked);
    } else if (roomsTraversed < DifficultyRegion::Easy) {
        return Random::ListItem(locked ? roomDirectory.at(Easy).at(type).locked : roomDirectory.at(Easy).at(type).unlocked);
    } else if (DifficultyRegion::Medium) {
        return Random::ListItem(locked ? roomDirectory.at(Medium).at(type).locked : roomDirectory.at(Medium).at(type).unlocked);
    } else {
        return Random::ListItem(locked ? roomDirectory.at(Medium).at(type).locked : roomDirectory.at(Medium).at(type).unlocked);
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
