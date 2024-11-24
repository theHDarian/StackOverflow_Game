#pragma once
#include <vector>
#include <tuple>
#include "components/map_components.hpp"
#include "premades.hpp"
#include "common.hpp"
#include "tiny_ecs_registry.hpp"
#include "utils/random.hpp"

//---------- PRESETS-------------------
const RoomPreset TutorialRoom1Preset{
    {},
    {},
    {},
    {},
    100000.0f,
    0};

const RoomPreset TutorialRoom2Preset{
    {{EnemyType::TestRevampedEnemy, {0.5, 0.5}}},
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
    {},
    {{{PushConsole,{bulletPierceUpA}}, {0.5f, 0.5f}}},
    {},
    0.0f,
    0};

const RoomPreset EnemyRoomDashIntro1{
        {{EnemyType::EasyEnemySentry, {0.8f, 0.2f}},
         {EnemyType::EasyEnemySentry, {0.2f, 0.8f}},
         {EnemyType::EasyEnemySkull, {0.5, 0.5}}
        },
       {},
       {},
       {},
       0.0f,
       3};

const RoomPreset EnemyRoomDashIntro2{
        {{EnemyType::Snail, {0.2f, 0.8f}},
         {EnemyType::Snail, {0.8f, 0.8f}},
         {EnemyType::MediumEnemyTank, {0.5f, 0.5f}},
        },
       {},
       {},
       {},
       0.0f,
       3 };

const RoomPreset EnemyRoomDashIntro3{
        {{EnemyType::HardEnemyBoidBio, {0.2f, 0.8f}},
         {EnemyType::HardEnemyBoidBio, {0.2f, 0.8f}},
         {EnemyType::HardEnemyBoidBio, {0.2f, 0.8f}},
         {EnemyType::HardEnemyBoidBio, {0.8f, 0.2f}},
         {EnemyType::HardEnemyBoidBio, {0.8f, 0.2f}},
         {EnemyType::HardEnemyBoidBio, {0.8f, 0.2f}},
         {EnemyType::ThreeBee, {0.5f, 0.5f}},
        },
       {},
       {},
       {},
       0.0f,
       3 };

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
    0.0f,
    5};

const RoomPreset EnemyRoomDash2{
    {{EnemyType::EasyEnemySentry, {0.4f, 0.2f}},
    {EnemyType::EasyEnemySentry, {0.8f, 0.4f}},
     {EnemyType::TestRevampedEnemy, {0.7, 0.4}},
        {Snail, {0.5f, 0.7f}},
     {EnemyType::EasyEnemySkull, {0.5f, 0.5f}}
     },
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
{EnemyType::TestRevampedEnemy, {0.3, 0.2}},
{EnemyType::TestRevampedEnemy, {0.3, 0.4}},
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
        {EnemyType::LaserEnemyTank, {0.8f, 0.2f}},
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
    };

const RoomPreset EnemyRoomAngelTank{
    {{EnemyType::MediumEnemyCharge, {0.2f, 0.8f}},
     {EnemyType::MediumEnemyCharge, {0.8f, 0.2f}},
     {EnemyType::MediumEnemyTank, {0.5f, 0.5f}},
     {EnemyType::MediumEnemyHealer, {0.8, 0.8}},
     {EnemyType::MediumEnemyHealer, {0.2, 0.2}}},
     
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
        {EvilSnail, {0.7f, 0.7f}},
        {EvilSnail, {0.7f, 0.3f}},
    },
    {},
    {{{PushConsole,{bulletBounceUpA, bulletPierceUpA}}, {0.25f, 0.5f}}, {{PopConsole,{}}, {0.75f, 0.5f}},
    },
    {},
    0.0f,
    35};


const RoomPreset RestingRoomPop{
    {},
    {},
    {{{PopConsole,{}}, {0.5f, 0.5f}}},
    {SpecialEvent::RebootStation}
};

const RoomPreset RestingRoomGardener{
        {},
        {},
        {{{Gardener,{}}, {0.5f, 0.5f}}},
        {SpecialEvent::RebootStation}
};

const RoomPreset RestRoomBibleTree{
    {},
    {},
    {{{InteractableItem::BibleTree,{}}, {0.5f, 0.5f}}},
    {SpecialEvent::RebootStation}
};

const RoomPreset TreasureRoom1{
    {},
    {},
    {{{PushConsole,{numBulletsUpA, sizeUpA}}, {0.5f, 0.5f}}},
    {},
    0.0f,
    5,
};

const RoomPreset TreasureRoom2{
    {},
    {},
    {{{PushConsole,{dashUpA}}, {0.5f, 0.5f}}},
{},
    0.0f,
    5,
};

const RoomPreset TreasureRoom3{
    {},
    {},
    {{{PushConsole,{dmgUpM}}, {0.5f, 0.5f}}},
    {},
    0.0f,
    5,
};

const RoomPreset TreasureRoom4{
    {},
    {},
    {{{PushConsole,{dashCDRDownA}}, {0.5f, 0.5f}}},
    {},
    0.0f,
    5,
};

const RoomPreset TreasureRoom5{
    {},
    {},
    {{{PushConsole,{dashCDRDownM}}, {0.5f, 0.5f}}},
    {},
    0.0f,
    5,
};

const RoomPreset TreasureRoomKey{
        {},
        {},
        {{{PushConsole,{key}}, {0.5f, 0.5f}}},
        {},
        0.0f,
        5,
    };

const RoomPreset TreasureRoomBlunt{
    {},
    {},
    {{{PushConsole,{blunt, blunt, blunt}}, {0.5f, 0.5f}}},
    {},
    0.0f,
    0,
};

const RoomPreset TreasureRoomRam{
    {},
    {},
    {{{InteractableItem::PushConsole,{stackSizeUpA}}, {0.5f, 0.5f}}},
    {},
    0.0f,
    0,
};

const RoomPreset TreasureRoomSniper{
    {},
    {},
    {{{PushConsole,{SniperPower, SniperBurden, SniperSpeed, SniperLethargy}}, {0.5f, 0.5f}}},
    {},
    0.0f,
    5,
};

const RoomPreset TreasureRoomKeys{
    {},
    {},
    {{{PushConsole,{key}}, {0.5f, 0.5f}}, {{PushConsole,{key}}, {0.25f, 0.5f}}, {{PushConsole,{key}}, {0.75f, 0.5f}}},
{},
    0.0f,
    5,
};



const RoomPreset BossRoomBee{
    {{BossBeehiveMain, {0.5f, 0.5f}}, {ThreeBee,{0.5f, 0.75f}}},
    {},
    {{{Ram,{}}, {0.5f, 0.5f}}},
    {},
    0.0f,
    2};

const RoomPreset HifiRoomTwinLaserVertical {
    {{EnemyType::HifiEnemyTwinLaserVertical1, {0.1f,0.5f}}},
    {},
    {},
    {},
    0.0f,
    5
};


const std::map<DifficultyRegion,std::map<RoomType, RoomPresets>> roomDirectory = {
    {DifficultyRegion::Intro,{
        {RoomType::EnemyRoom, {{EnemyRoomDashIntro1,EnemyRoomDashIntro2,EnemyRoomDashIntro3 },{}}},
        {RoomType::RestRoom, {{RestingRoomGardener, RestRoomBibleTree, RestingRoomPop},{RestingRoomPop}}},
        {RoomType::TreasureRoom, {{TreasureRoom2,TreasureRoom3,TreasureRoom4, TreasureRoomKey},{TreasureRoomSniper, TreasureRoom5}}},
    }},
    {DifficultyRegion::Easy,{
        {RoomType::EnemyRoom, {{EnemyRoomBees1, EnemyRoomDash1, EnemyRoomBees2, EnemyRoomDash2, EnemyRoomSnails},{EnemyRoomSwarm}}},
        {RoomType::RestRoom, {{RestingRoomPop,RestingRoomGardener, RestRoomBibleTree,},{RestingRoomPop}}},
        {RoomType::TreasureRoom, {{TreasureRoom1,TreasureRoom2,TreasureRoom3,TreasureRoom4, TreasureRoomKey, TreasureRoomKeys, TreasureRoomBlunt},{TreasureRoomRam, TreasureRoomSniper, TreasureRoom5}}},
    }},
    {DifficultyRegion::Medium,{
        {RoomType::EnemyRoom, {{EnemyRoomAngelTank, EnemyRoomDashHard,  EnemyRoomDash2, EnemyRoomBees2},{EnemyRoomTripleBuffEX}}},
        {RoomType::RestRoom, {{RestingRoomPop,RestingRoomGardener, RestRoomBibleTree,},{}}},
        {RoomType::TreasureRoom, {{TreasureRoom1,TreasureRoom2,TreasureRoom3,TreasureRoomBlunt, TreasureRoom4, TreasureRoomKey, TreasureRoomKeys},{TreasureRoomRam, TreasureRoomSniper, TreasureRoom5}}},
    }},
};

inline bool hasLocked(RoomType type, int roomsTraversed) {
    Map& map = registry.maps.components[0];
    
    if (type >= RoomType::None) {
        return false;
    }
    if (roomsTraversed < DifficultyRegion::Intro) {
        return !map.directory.at(Intro).at(type).locked.empty();
    } else if (roomsTraversed < DifficultyRegion::Easy) {
        return !map.directory.at(Easy).at(type).locked.empty();
    } else if (roomsTraversed < DifficultyRegion::Medium) {
        return !map.directory.at(Medium).at(type).locked.empty();
    } else {
        return !map.directory.at(Medium).at(type).locked.empty();
    }
}

inline RoomPreset getRoomPreset(RoomType type, bool locked) {
    Map& map = registry.maps.components[0];
    //boss rooms
    if (type == RoomType::BossRoom && map.currRegion == MapRegion::Biology) {
        return BossRoomBee;
    }

    //tutorial rooms
    if (type == RoomType::TutorialRoom2) {
        return TutorialRoom2Preset;
    }

    //regular rooms
    if(!hasLocked(type,map.roomsTraversed) && locked) {
        assert(false);
    }
    if (map.roomsTraversed < DifficultyRegion::Intro) {
        return Random::ListItem(locked ? map.directory.at(Intro).at(type).locked : map.directory.at(Intro).at(type).unlocked);
    } else if (map.roomsTraversed < DifficultyRegion::Easy) {
        return Random::ListItem(locked ? map.directory.at(Easy).at(type).locked : map.directory.at(Easy).at(type).unlocked);
    } else if (DifficultyRegion::Medium) {
        return Random::ListItem(locked ? map.directory.at(Medium).at(type).locked : map.directory.at(Medium).at(type).unlocked);
    } else {
        return Random::ListItem(locked ? map.directory.at(Medium).at(type).locked : map.directory.at(Medium).at(type).unlocked);
    }
}


const std::map<RoomType,int> roomTypeToSymbols = {
    {RoomType::EnemyRoom,3},
    {RoomType::RestRoom,1},
    {RoomType::BossRoom,2},
    {RoomType::TreasureRoom,0},
    {RoomType::None,4},
    {RoomType::TutorialRoom1,5},
    {RoomType::TutorialRoom2,5}
};

const std::map<RoomType, SoundType> roomTypeToMusic = {
    {RoomType::EnemyRoom, SoundType::normalBGM},
    {RoomType::RestRoom, SoundType::specialBGM},
    {RoomType::BossRoom, SoundType::bossBGM},
    {RoomType::TreasureRoom, SoundType::specialBGM},
    {RoomType::None, normalBGM},
    {RoomType::TutorialRoom1, SoundType::specialBGM},
    {RoomType::TutorialRoom2, SoundType::normalBGM}
};
