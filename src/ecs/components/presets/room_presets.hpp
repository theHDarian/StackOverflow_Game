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
    {{EnemyType::EasyEnemySentry, {0.2f, 0.8f}}},
    {},
    {},
    {},
    {},
    0.0f,
    0 
};

const RoomPreset EnemyRoomSwarm{
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
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
     {EnemyType::HardEnemyBoid, {0.5f, 0.5f}}},
    {bulletPierceUpA},
    {},
    {{PushConsole, {0.5f, 0.5f}}},
{},
    0.0f,
    0};

const RoomPreset EnemyRoomDashIntro{
        {{EnemyType::EasyEnemySentry, {0.2f, 0.8f}},
         {EnemyType::EasyEnemySentry, {0.8f, 0.8f}},
         {EnemyType::EasyEnemySentry, {0.8f, 0.2f}},
         {EnemyType::EasyEnemySentry, {0.2f, 0.2f}},
         {EnemyType::TestRevampedEnemy, {0.2, 0.4}},
         {EnemyType::TestRevampedEnemy, {0.8, 0.4}},
        },
       {},
       {},
       {},
   {},
       0.0f,
       3};

const RoomPreset EnemyRoomDash1{
    {{EnemyType::EasyEnemySentry, {0.2f, 0.8f}},
     {EnemyType::EasyEnemySentry, {0.8f, 0.8f}},
     {EnemyType::EasyEnemySentry, {0.8f, 0.2f}},
     {EnemyType::EasyEnemySentry, {0.2f, 0.2f}},
     {EnemyType::TestRevampedEnemy, {0.2, 0.4}},
     {EnemyType::TestRevampedEnemy, {0.8, 0.4}},
     {EnemyType::EasyEnemySkull, {0.5f, 0.5f}},
     {EnemyType::MediumEnemyBoar, {0.3f, 0.7f}},
     {EnemyType::MediumEnemyBoar, {0.3f, 0.7f}}
     },
    {},
    {},
    {},
{},
    0.0f,
    5};

const RoomPreset EnemyRoomDash2{
    {{EnemyType::EasyEnemySentry, {0.2f, 0.8f}},
     {EnemyType::EasyEnemySentry, {0.8f, 0.8f}},
     {EnemyType::EasyEnemySentry, {0.8f, 0.2f}},
     {EnemyType::EasyEnemySentry, {0.2f, 0.2f}},
    {EnemyType::EasyEnemySentry, {0.2f, 0.4f}},
    {EnemyType::EasyEnemySentry, {0.8f, 0.4f}},
    {EnemyType::EasyEnemySentry, {0.8f, 0.6f}},
    {EnemyType::EasyEnemySentry, {0.2f, 0.6f}},

     {EnemyType::TestRevampedEnemy, {0.3, 0.2}},
    {EnemyType::TestRevampedEnemy, {0.3, 0.4}},
    {EnemyType::TestRevampedEnemy, {0.3, 0.6}},
     {EnemyType::TestRevampedEnemy, {0.7, 0.4}},
        {Snail, {0.5f, 0.7f}},
     {EnemyType::EasyEnemySkull, {0.5f, 0.5f}}
     },
    {},
    {},
    {},
{},
    0.0f,
    5};


const RoomPreset EnemyRoomDashHard{
    {{EnemyType::EasyEnemySkull, {0.2f, 0.8f}},
     {EnemyType::EasyEnemySkull, {0.8f, 0.8f}},
     {EnemyType::EasyEnemySkull, {0.8f, 0.2f}},
     {EnemyType::EasyEnemySkull, {0.2f, 0.2f}},
    {EnemyType::HardEnemyAngel, {0.4f, 0.5f}},
    {EnemyType::HardEnemyAngel, {0.8f, 0.5f}},
{EnemyType::TestRevampedEnemy, {0.3, 0.2}},
{EnemyType::TestRevampedEnemy, {0.3, 0.4}},
{EnemyType::TestRevampedEnemy, {0.3, 0.6}},
{EnemyType::TestRevampedEnemy, {0.7, 0.4}},
{EnemyType::TestRevampedEnemy, {0.7, 0.2}},
{EnemyType::TestRevampedEnemy, {0.7, 0.6}},
     {EnemyType::EvilSnail, {0.5f, 0.5f}}

     },
    {},
    {},
    {},
};

const RoomPreset EnemyRoomSnails {
    {
        {EnemyType::EvilSnail, {0.5f, 0.5f}},
        {Snail, {0.2f, 0.8f}},
        {Snail, {0.8f, 0.8f}},
        {Snail, {0.8f, 0.2f}},
        {Snail, {0.2f, 0.2f}},
        {MediumEnemyCharge, {0.2f, 0.4f}},
        {MediumEnemyCharge, {0.8f, 0.4f}},
},
{},
{},
{},

};

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

const RoomPreset EnemyRoomBees2{
    {{EnemyType::TwoBee, {0.2f, 0.8f}},
     {EnemyType::TwoBee, {0.8f, 0.8f}},
     {EnemyType::TwoBee, {0.8f, 0.2f}},
     {EnemyType::TwoBee, {0.2f, 0.2f}},
        {EvilSnail, {0.5f, 0.6f}},
     {EnemyType::BeeHive, {0.2f, 0.4f}},
        {EnemyType::BeeHive, {0.5f, 0.4f}},
     {EnemyType::BeeHive, {0.8f, 0.4f}}},

    {},
    {},
    {},
    {},
    };

const RoomPreset EnemyRoomAngelTank{
    {{EnemyType::MediumEnemyCharge, {0.2f, 0.8f}},
     {EnemyType::MediumEnemyCharge, {0.8f, 0.2f}},
     {EnemyType::MediumEnemyTank, {0.5f, 0.5f}},
     {EnemyType::HardEnemyAngel, {0.2f, 0.4f}},
     {EnemyType::HardEnemyAngel, {0.8f, 0.4f}},
     {EnemyType::MediumEnemyHealer, {0.8, 0.8}},
     {EnemyType::MediumEnemyHealer, {0.2, 0.2}}},
     
    {},
    {},
    {},
    {},
    0.0f,
    5};

const RoomPreset EnemyRoomTripleBuffEX{
    {{EnemyType::MediumEnemyCharge, {0.2f, 0.8f}},
        {EnemyType::MediumEnemyTank, {0.2f, 0.2f}},
    {EnemyType::MediumEnemyTank, {0.6f, 0.2f}},
    {EnemyType::MediumEnemyTank, {0.2f, 0.6f}},
        {EnemyType::HardEnemyAngel, {0.8f, 0.4f}},
    {EnemyType::HardEnemyAngel, {0.8f, 0.4f}},
        {EnemyType::HardEnemyAngel, {0.8f, 0.6f}},
        {EvilSnail, {0.7f, 0.7f}},
        {EvilSnail, {0.7f, 0.3f}},
    },
    {bulletBounceUpA, bulletPierceUpA},
    {},
    {{PushConsole, {0.25f, 0.5f}}, {PopConsole, {0.75f, 0.5f}},
    },
    {},
    0.0f,
    35};


const RoomPreset RestingRoomPop{
    {},
    {},
    {},
    {{InteractableItem::PopConsole, {0.5f, 0.5f}}},
    {SpecialEvent::RebootStation}
};

const RoomPreset RestingRoomGardener{
        {},
        {},
        {},
        {{InteractableItem::Gardener, {0.65f, 0.5f}}, {InteractableItem::BibleTree, {0.5f, 0.5f}}},
        {SpecialEvent::RebootStation}
};

const RoomPreset RestRoomBibleTree{
    {},
    {},
    {},
    {{InteractableItem::BibleTree, {0.5f, 0.5f}}},
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

const RoomPreset TreasureRoom5{
    {},
    {dashCDRDownM},
    {},
    {{PushConsole, {0.5f, 0.5f}}},
{},
    0.0f,
    5,
};

const RoomPreset TreasureRoomKey{
        {},
        {key},
        {},
        {{PushConsole, {0.5f, 0.5f}}},
    {},
        0.0f,
        5,
    };

const RoomPreset TreasureRoomBlunt{
    {},
    {blunt, blunt, blunt},
    {},
    { {PushConsole, {0.5f, 0.5f}}},
    {},
    0.0f,
    0,
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

const RoomPreset TreasureRoomOstrich{
    {},
    {ostrichWarrior, ostrichWarriorDownside},
    {},
    {{PushConsole, {0.5f, 0.5f}}, {PushConsole, {0.25f, 0.5f}}, {PushConsole, {0.75f, 0.5f}}},
{},
    0.0f,
    5,
};

const RoomPreset TreasureRoomKeys{
    {},
    {key},
    {},
    {{PushConsole, {0.5f, 0.5f}}, {PushConsole, {0.25f, 0.5f}}, {PushConsole, {0.75f, 0.5f}}},
{},
    0.0f,
    5,
};



const RoomPreset BossRoomBee{
    {{BossBeehiveMain, {0.5f, 0.5f}}, {ThreeBee,{0.25f, 0.25f}}, {ThreeBee,{0.75f, 0.75f}}, {ThreeBee,{0.75f, 0.25f}}, {ThreeBee,{0.5f, 0.75f}}},
    {},
    {},
    {{Ram, {0.5f, 0.5f}}},
    {},
    0.0f,
    2};

//used by roomsTraversed to determine type of enemy to spawn, some enemies only spawn in certain difficulty regions
enum DifficultyRegion {
    Intro = 3,
    Easy = 10,
    Medium = 20,
    // Hard,
};

struct RoomPresets {
    std::vector<RoomPreset> unlocked;
    std::vector<RoomPreset> locked;
};

const std::map<DifficultyRegion,std::map<RoomType, RoomPresets>> roomDirectory = {
    {DifficultyRegion::Intro,{
        {RoomType::EnemyRoom, {{EnemyRoomDashIntro },{}}},
        {RoomType::RestRoom, {{RestingRoomGardener, RestRoomBibleTree, RestingRoomPop},{RestingRoomPop}}},
        {RoomType::TreasureRoom, {{TreasureRoom2,TreasureRoom3,TreasureRoom4, TreasureRoomKey},{TreasureRoomOstrich, TreasureRoom5}}},
    }},
    {DifficultyRegion::Easy,{
        {RoomType::EnemyRoom, {{EnemyRoomBees1, EnemyRoomDash1, EnemyRoomBees2, EnemyRoomDash2, EnemyRoomSnails},{EnemyRoomSwarm}}},
        {RoomType::RestRoom, {{RestingRoomPop,RestingRoomGardener, RestRoomBibleTree,},{RestingRoomPop}}},
        {RoomType::TreasureRoom, {{TreasureRoom1,TreasureRoom2,TreasureRoom3,TreasureRoom4, TreasureRoomKey, TreasureRoomKeys},{TreasureRoomRam, TreasureRoomOstrich, TreasureRoom5}}},
    }},
    {DifficultyRegion::Medium,{
        {RoomType::EnemyRoom, {{EnemyRoomAngelTank, EnemyRoomDashHard,  EnemyRoomDash2, EnemyRoomBees2},{EnemyRoomTripleBuffEX}}},
        {RoomType::RestRoom, {{RestingRoomPop,RestingRoomGardener, RestRoomBibleTree,},{}}},
        {RoomType::TreasureRoom, {{TreasureRoom1,TreasureRoom2,TreasureRoom3,TreasureRoom4, TreasureRoomKey, TreasureRoomKeys},{TreasureRoomRam, TreasureRoomOstrich, TreasureRoom5}}},
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
        return BossRoomBee;
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

const std::map<RoomType, SoundType> roomTypeToMusic = {
    {RoomType::EnemyRoom, SoundType::normalBGM},
    {RoomType::RestRoom, SoundType::specialBGM},
    {RoomType::BossBigCRoom, SoundType::bossBGM},
    {RoomType::TreasureRoom, SoundType::specialBGM},
    {RoomType::None, normalBGM},
    {RoomType::TutorialRoom1, SoundType::specialBGM},
    {RoomType::TutorialRoom2, SoundType::normalBGM}
};
