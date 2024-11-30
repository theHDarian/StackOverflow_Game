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
    {{{WhiteBoard,{}}, {0.2f, 0.2f}}},
    0,
    0,
    0,
    "TutorialRoom1",
    false,
    {800,1200}
};

const RoomPreset TutorialRoom2Preset{
    {{EnemyType::EasyEnemySkull, {0.5, 0.5}}},
    {},
    {},
    0.0f,
    0 ,
    0,
    "TutorialRoom2",
    false,
    {1920,1080}
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
    0.0f,
    0,
0,
"EnemyRoomSwarm",
    false,
    {3000, 3000}
};

const RoomPreset EnemyRoomDashIntro1{
        {{EnemyType::EasyEnemySentry, {0.8f, 0.2f}},
         {EnemyType::EasyEnemySentry, {0.2f, 0.8f}},
         {EnemyType::EasyEnemySkull, {0.5, 0.5}}
        },
       {},
       {},
       0.0f,
       3,
    1,
    "EnemyRoomDashIntro1",
};

const RoomPreset EnemyRoomDashIntro2{
        {{EnemyType::Snail, {0.2f, 0.8f}},
         {EnemyType::Snail, {0.8f, 0.8f}},
         {EnemyType::MediumEnemyTank, {0.5f, 0.5f}},
        },
       {},
       {},
       0.0f,
       3,
    1,
    "EnemyRoomDashIntro2",
};

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
       0.0f,
       3,
         1,
            "EnemyRoomDashIntro3",
};

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
    0.0f,
    5,
    2,
    "EnemyRoomDash1",
};

const RoomPreset EnemyRoomDash2{
    {{EnemyType::EasyEnemySentry, {0.4f, 0.2f}},
    {EnemyType::EasyEnemySentry, {0.8f, 0.4f}},
     {EnemyType::TestRevampedEnemy, {0.7, 0.4}},
        {Snail, {0.5f, 0.7f}},
     {EnemyType::EasyEnemySkull, {0.5f, 0.5f}}
     },
    {},
    {},
    0.0f,
    5,
    2,
    "EnemyRoomDash2",
};


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
    0.0f,
    5,
    2,
    "EnemyRoomDashHard",
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
0.0f,
    5,
    2,
    "EnemyRoomSnails",

};

const RoomPreset EnemyRoomSnailsHard {
        {
            {EnemyType::EvilSnail, {0.5f, 0.5f}},
            {Snail, {0.3f, 0.7f}},
            {Snail, {0.4f, 0.6f}},
            {EnemyType::LaserEnemyTank, {0.8f, 0.2f}},
            {MediumEnemyCharge, {0.8f, 0.4f}},
    },
    {},
    {},
    0.0f,
        5,
        2,
        "EnemyRoomSnailsHard",
        false,
        {1000, 750}

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
    0.0f,
    5,
    2,
    "EnemyRoomBees1",
};

const RoomPreset EnemyRoomBees2{
    {{EnemyType::TwoBee, {0.2f, 0.8f}},
     {EnemyType::TwoBee, {0.8f, 0.8f}},
     {EnemyType::TwoBee, {0.8f, 0.2f}},
     {EnemyType::TwoBee, {0.2f, 0.2f}},
        {EvilSnail, {0.5f, 0.6f}},
     {EnemyType::BeeHive, {0.2f, 0.4f}},
     {EnemyType::BeeHive, {0.8f, 0.4f}}},

    {},
    {},
    0.5f,
    7,
    2,
    "EnemyRoomBees2",
    };

const RoomPreset EnemyRoomAngelTank{
    {{EnemyType::MediumEnemyCharge, {0.2f, 0.8f}},
     {EnemyType::MediumEnemyCharge, {0.8f, 0.2f}},
     {EnemyType::MediumEnemyTank, {0.5f, 0.5f}},
     {EnemyType::MediumEnemyHealer, {0.8, 0.8}},
     {EnemyType::MediumEnemyHealer, {0.2, 0.2}}},
     
    {},
    {},
    0.25f,
    5,
    2,
    "EnemyRoomAngelTank",
};

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
    0.0f,
    35,
    5,
    "EnemyRoomTripleBuffEX"
};


const RoomPreset RestingRoomPop{
    {},
    {},
    {{{PopConsole,{}}, {0.5f, 0.5f}}},
    0.0f,
    0,
    0,
    "RestingRoomPop",
    false,
    {1000,1000}
};

const RoomPreset RestingRoomGardener{
        {},
        {},
        {{{Gardener,{}}, {0.5f, 0.5f}}},
    0.0f,
    0,
    0,
    "RestingRoomGardener",
    true,
    {1800, 1000}
};

const RoomPreset RestRoomBibleTree{
    {},
    {},
    {{{InteractableItem::BibleTree,{}}, {0.5f, 0.5f}}},
    0.0f,
    0,
    0,
    "RestRoomBibleTree",
    true,
    {2000,2000}
};

const RoomPreset RestRoomEmpty{
    {},
    {},
    {},
    0.0f,
    5,
    0,
    "RestRoomEmpty",
    false
};

const RoomPreset RestRoomBaru{
        {},
        {},
{{{Baru,{key}}, {0.5f, 0.5f}}},
        0.0f,
        5,
        0,
    "RestRoomBaru",
        true,
    {3000, 2000}
    };

const RoomPreset RestRoomOracleCrab{
        {},
        {},
        {{{OracleCrab,{key}}, {0.5f, 0.3f}}},
        0.0f,
        5,
        0,
    "RestRoomOracleCrab",
        true
    };


const RoomPreset TreasureRoom1{
    {},
    {},
    {{{PushConsole,{numBulletsUpA, sizeUpA}}, {0.5f, 0.5f}}},
    0.0f,
    5,
    0,
    "TreasureRoom1",
    false,
     {1300, 1300},
};

const RoomPreset TreasureRoom2{
    {},
    {},
    {{{PushConsole,{dashUpA}}, {0.5f, 0.5f}}},
    0.0f,
    5,
    0,
    "TreasureRoom2",
    false,
     {1300, 1300},
};

const RoomPreset TreasureRoom3{
    {},
    {},
    {{{FightConsole,{dmgUpM}}, {0.5f, 0.5f}}},
    0.0f,
    5,
    0,
    "TreasureRoom3",
    false,
     {1300, 1300},
};

const RoomPreset TreasureRoom4{
    {},
    {},
    {{{PushConsole,{dashCDRDownA}}, {0.5f, 0.5f}}},
    0.0f,
    5,
    0,
    "TreasureRoom4",
    false,
     {1300, 1300},
};

const RoomPreset TreasureRoom5{
    {},
    {},
    {{{PushConsole,{dashCDRDownM}}, {0.5f, 0.5f}}},
    0.0f,
    5,
    0,
    "TreasureRoom5",
    false,
     {1300, 1300},
};

const RoomPreset TreasureRoomKey{
        {},
        {},
        {{{PushConsole,{key}}, {0.5f, 0.5f}}},
        0.0f,
        5,
    0,
    "TreasureRoomKey",
    false,
     {1300, 1300},
    };

const RoomPreset TreasureRoomBlunt{
    {},
    {},
    {{{PushConsole,{blunt, blunt, blunt}}, {0.5f, 0.5f}}},
    0.0f,
    0,
    0,
    "TreasureRoomBlunt",false,
     {1300, 1300},
};

const RoomPreset TreasureRoomRam{
    {},
    {},
    {{{InteractableItem::PushConsole,{stackSizeUpA}}, {0.5f, 0.5f}}},
    0.0f,
    0,
    0,
    "TreasureRoomRam",
    false,
     {1300, 1300},
};

const RoomPreset TreasureRoomSniper{
    {},
    {},
    {{{PushConsole,{SniperPower, SniperBurden, SniperSpeed, SniperLethargy}}, {0.5f, 0.5f}}},
    0.0f,
    5,
    0,
    "TreasureRoomSniper",
    false,
     {1300, 1300},
};

const RoomPreset TreasureRoomHoney{
    {},
    {},
    {{{HoneyCanister,{}}, {0.5f, 0.5f}}},
    0.0f,
    5,
    0,
    "TreasureRoomHoney",
    true,
     {1300, 1300},
};

const RoomPreset TreasureRoomKeys{
    {},
    {},
    {{{PushConsole,{key}}, {0.5f, 0.5f}}, {{PushConsole,{key}}, {0.25f, 0.5f}}, {{PushConsole,{key}}, {0.75f, 0.5f}}},
    0.0f,
    5,
    0,
    "TreasureRoomKeys",
    false,
     {1300, 1300},
};


const RoomPreset TreasureRoomWish{
    {},
    {},
    {{{WishGranter,{key}}, {0.5f, 0.5f}}},
    0.0f,
    5,
    0,
    "TreasureRoomWish",
    true,
     {1300, 1300},
};





const RoomPreset BossRoomBee{
    {{BossBeehiveMain, {0.5f, 0.5f}}, {ThreeBee,{0.5f, 0.75f}}},
    {},
    {{{Ram,{}}, {0.5f, 0.5f}}},
    0.0f,
    20,
    5,
    "BossRoomBee",

};

const RoomPreset HifiRoomTwinLaserShurikens {
    {
        {EnemyType::HifiEnemyTwinLaserHorizontal1, {0.7f,0.1f}},
        {EnemyType::HifiEnemyTwinLaserHorizontal1, {0.99f,0.1f}},
        {EnemyType::HifiEnemyTwinLaserVertical1, {0.1f,0.7f}},
        {EnemyType::HifiEnemyTrail, {0.6f,0.6f}},
        {EnemyType::HifiEnemyTrail, {0.4f,0.4f}}
    },
    {},
    {},
    0.0f,
    5,
    2,
    "HifiLaserShurikens"
};
const RoomPreset HifiRoomTwinLaserChargers {
    {
        {EnemyType::HifiEnemyTwinLaserHorizontal1, {0.7f,0.1f}},
        {EnemyType::HifiEnemyTwinLaserVertical1, {0.1f,0.3f}},
        {EnemyType::HifiEnemyChargerHard, {0.8f,0.2f}},
        {EnemyType::HifiEnemyChargerHard, {0.8f,0.8f}},
        {EnemyType::HifiEnemyChargerHard, {0.2f,0.2f}},
        {EnemyType::HifiEnemyChargerHard, {0.2f,0.8f}},
        {EnemyType::HifiEnemyChargerHard, {0.4f,0.6f}},
        {EnemyType::HifiEnemyChargerHard, {0.6f,0.4f}},
    },
    {},
    {},
    0.0f,
    5,
    2,
    "HifiLaserChargersHard"
};


const RoomPreset HifiRoomBasicEnemy {
    {
        {EnemyType::HifiEnemyCharger, {0.7f,0.3f}},
        {EnemyType::HifiEnemyCharger, {0.7f,0.7f}},
        {EnemyType::HifiEnemyCharger, {0.4f,0.5f}},
        {EnemyType::HifiEnemySniper, {0.5f,0.5f}},
        {EnemyType::HifiEnemySniper, {0.2f,0.2f}},
        {EnemyType::HifiEnemySniper, {0.8f,0.2f}},
        {EnemyType::HifiEnemySniper, {0.8f,0.8f}},
        {EnemyType::HifiEnemySniper, {0.2f,0.8f}},
        {EnemyType::MediumEnemyHealer, {0.5f,0.5f}},
        {EnemyType::MediumEnemyHealer, {0.2f,0.2f}},
    },
    {},
    {},
    0.0f,
    5,
    2,
    "HifiBasic"
};
const RoomPreset HifiRoomBoidSnipers{
    {
        {EnemyType::HifiEnemySniper, {0.6f,0.5f}},
        {EnemyType::HifiEnemySniper, {0.4f,0.5f}},
        {EnemyType::HifiEnemySniper, {0.5f,0.5f}},
        {EnemyType::HifiEnemySniper, {0.5f,0.6f}},
        {EnemyType::HifiEnemySniper, {0.5f,0.4f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
    },
    {},
    {},
    0.0f,
    5,
    2,
    "HifiBoidSnipers"
};

const RoomPreset HifiRoomCannonLasers{
    {
        {EnemyType::HifiEnemyCannon, {0.9f,0.2f}},
        {EnemyType::HifiEnemyCannon, {0.9f,0.8f}},
        {EnemyType::HifiEnemyCannon, {0.1f,0.2f}},
        {EnemyType::HifiEnemyCannon, {0.1f,0.8f}},
        {EnemyType::HifiEnemyTwinLaserHorizontal1, {0.7f,0.1f}},
        {EnemyType::HifiEnemyTwinLaserVertical1, {0.1f,0.3f}},
        {EnemyType::HifiEnemyTwinLaserHorizontal1, {0.3f,0.1f}},
        {EnemyType::HifiEnemyTwinLaserVertical1, {0.1f,0.7f}},
    },
    {},
    {},
    0.0f,
    5,
    2,
    "HifiBoidCannons"
};

const RoomPreset HifiRoomSniperShurikens{
    {
        {EnemyType::HifiEnemySniperHard, {0.8f,0.2f}},
        {EnemyType::HifiEnemySniperHard, {0.2f,0.2f}},
        {EnemyType::HifiEnemySniperHard, {0.8f,0.8f}},
        {EnemyType::HifiEnemySniperHard, {0.2f,0.8f}},
        {EnemyType::HifiEnemyTrailHard, {0.6f,0.6f}},
        {EnemyType::HifiEnemyTrailHard, {0.4f,0.4f}}
    },
    {},
    {},
    0.0f,
    5,
    2,
    "HifiBoidCannons"
};

const RoomPreset HifiRoomCannonBoids{
    {
        {EnemyType::HifiEnemyCannonHard, {0.7f,0.5f}},
        {EnemyType::HifiEnemyCannonHard, {0.3f,0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
        {EnemyType::HardEnemyBoid, {0.5f, 0.5f}},
    },
    {},
    {},
    0.0f,
    5,
    2,
    "HifiBoidCannons"
};



const std::map<DifficultyRegion,std::map<RoomType, RoomPresets>> bioRoomDirectory = {
    {DifficultyRegion::Intro,{
        {RoomType::EnemyRoom, {{EnemyRoomDashIntro1,EnemyRoomDashIntro2,EnemyRoomDashIntro3 },{}}},
        {RoomType::RestRoom, {{RestingRoomPop},{RestingRoomPop},}},
        {RoomType::EventRoom, {{RestingRoomGardener, RestRoomBibleTree, RestRoomOracleCrab, RestRoomEmpty}, {}}  },
        {RoomType::TreasureRoom, {{TreasureRoom2,TreasureRoom3,TreasureRoom4, TreasureRoomKey},{TreasureRoomSniper, TreasureRoom5}}},
    }},
    {DifficultyRegion::Easy,{
        {RoomType::EnemyRoom, {{EnemyRoomBees1, EnemyRoomDash1, EnemyRoomBees2, EnemyRoomDash2, EnemyRoomSnails},{EnemyRoomSwarm}}},
        {RoomType::RestRoom, {{RestingRoomPop},{RestingRoomPop}}},
        {RoomType::EventRoom, {{RestingRoomGardener, RestRoomBibleTree, RestRoomOracleCrab, RestRoomEmpty}, {}}  },
        {RoomType::TreasureRoom, {{TreasureRoom1,TreasureRoom2,TreasureRoom3,TreasureRoom4, TreasureRoomKey, TreasureRoomKeys, TreasureRoomBlunt},{TreasureRoomRam, TreasureRoomSniper, TreasureRoom5}}},
    }},
    {DifficultyRegion::Medium,{
        {RoomType::EnemyRoom, {{EnemyRoomAngelTank, EnemyRoomDashHard,  EnemyRoomDash2, EnemyRoomBees2},{EnemyRoomTripleBuffEX}}},
        {RoomType::RestRoom, {{RestingRoomPop},{RestingRoomPop}}},
        {RoomType::EventRoom, {{RestingRoomGardener, RestRoomBibleTree, RestRoomOracleCrab}, {}}  },
        {RoomType::TreasureRoom, {{TreasureRoom1,TreasureRoom2,TreasureRoom3,TreasureRoomBlunt, TreasureRoom4, TreasureRoomKey, TreasureRoomKeys},{TreasureRoomRam, TreasureRoomSniper, TreasureRoom5}}},
    }},
};

const std::map<DifficultyRegion,std::map<RoomType, RoomPresets>> physicsRoomDirectory = {
    {DifficultyRegion::Intro,{
        // {RoomType::EnemyRoom, {{HifiRoomTwinLaserShurikens,HifiRoomBasicEnemy,HifiRoomBoidSnipers,HifiRoomCannons},{}}},
        {RoomType::EnemyRoom, {{HifiRoomBasicEnemy,HifiRoomBoidSnipers,HifiRoomTwinLaserChargers,HifiRoomTwinLaserShurikens},{}}},
        {RoomType::RestRoom, {{},{RestingRoomPop}}},
            {RoomType::EventRoom, {{TreasureRoomHoney}, {RestRoomBaru, TreasureRoomHoney }}  },
        {RoomType::TreasureRoom, {{TreasureRoom2,TreasureRoom3,TreasureRoom4, TreasureRoomKey},{TreasureRoomSniper, TreasureRoom5}}},
    }},
    {DifficultyRegion::Easy,{
        {RoomType::EnemyRoom, {{HifiRoomCannonLasers,HifiRoomSniperShurikens,HifiRoomCannonBoids},{}}},
        {RoomType::RestRoom, {{RestingRoomPop,RestingRoomGardener,RestRoomBibleTree,},{RestingRoomPop}}},
        {RoomType::EventRoom, {{TreasureRoomHoney}, {RestRoomBaru, TreasureRoomHoney, TreasureRoomWish }}  },
        {RoomType::TreasureRoom, {{TreasureRoom1,TreasureRoom2,TreasureRoom3,TreasureRoom4, TreasureRoomKey, TreasureRoomKeys, TreasureRoomBlunt},{TreasureRoomRam, TreasureRoomSniper, TreasureRoom5}}},
    }},
    {DifficultyRegion::Medium,{
        {RoomType::EnemyRoom, {{HifiRoomCannonLasers,HifiRoomSniperShurikens,HifiRoomCannonBoids},{}}},
        {RoomType::RestRoom, {{RestingRoomPop,RestingRoomGardener, RestRoomBibleTree,},{}}},
            {RoomType::EventRoom, {{RestRoomBaru, TreasureRoomHoney, TreasureRoomWish}, {RestRoomBaru, TreasureRoomHoney, TreasureRoomWish }}  },
        {RoomType::TreasureRoom, {{TreasureRoom1,TreasureRoom2,TreasureRoom3,TreasureRoomBlunt, TreasureRoom4, TreasureRoomKey, TreasureRoomKeys},{TreasureRoomRam, TreasureRoomSniper, TreasureRoom5}}},
    }},
};

inline std::map<DifficultyRegion,std::map<RoomType, RoomPresets>> getDirectory(MapRegion region) {
    if (region == Biology || region == Tutorial) {
        return bioRoomDirectory;
    } else if (region == Physics) {
        return physicsRoomDirectory;
    }
    else {
        return bioRoomDirectory;
    }
    return bioRoomDirectory;
}

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

inline bool hasUnlocked(RoomType type, int roomsTraversed) {
    Map& map = registry.maps.components[0];

    if (type >= RoomType::None) {
        return false;
    }
    if (roomsTraversed < DifficultyRegion::Intro) {
        return !map.directory.at(Intro).at(type).unlocked.empty();
    } else if (roomsTraversed < DifficultyRegion::Easy) {
        return !map.directory.at(Easy).at(type).unlocked.empty();
    } else if (roomsTraversed < DifficultyRegion::Medium) {
        return !map.directory.at(Medium).at(type).unlocked.empty();
    } else {
        return !map.directory.at(Medium).at(type).unlocked.empty();
    }
}

inline RoomType getRandomRoomType(bool excludeNone, int roomsTraversed)
{
    const int bossRoomNum = 12;
    if (roomsTraversed % bossRoomNum == bossRoomNum-1) {
        return BossRoom;
    }

    if (Random::Float() < 0.5f) { //enemy room has higher chance of being rolled
        return RoomType::EnemyRoom;
    }


    std::vector<RoomType> possibleRooms;
    if (!excludeNone) {
        possibleRooms.push_back(RoomType::None);
    }
    if (hasLocked(RoomType::EventRoom, roomsTraversed) || hasUnlocked(RoomType::EventRoom, roomsTraversed)) {
        possibleRooms.push_back(RoomType::EventRoom);
    }
    if (hasLocked(RoomType::TreasureRoom, roomsTraversed) || hasUnlocked(RoomType::TreasureRoom, roomsTraversed)) {
        possibleRooms.push_back(RoomType::TreasureRoom);
    }
    if (hasLocked(RoomType::RestRoom, roomsTraversed) || hasUnlocked(RoomType::RestRoom, roomsTraversed) && Random::Float() < 0.25f) {
        possibleRooms.push_back(RoomType::RestRoom);
    }
    return Random::ListItem(possibleRooms);

    // else if (Random::Float() >  0.5f && Random::Float() < 0.7f) {
    //     return RoomType::TreasureRoom;
    // }  else if (Random::Float() > 0.7f && Random::Float() < 0.80f) {
    //     return RoomType::EventRoom;
    // }
    //
    // return static_cast<RoomType>(Random::Int(excludeNone ? RoomType::None - 1 : RoomType::None));
}

inline RoomPreset getRoomPreset(RoomType type, bool locked) {
    Map& map = registry.maps.components[0];
    //boss rooms
    if (type == RoomType::BossRoom && map.currRegion == MapRegion::Biology) {
        return BossRoomBee;
    } else if (type == RoomType::BossRoom && map.currRegion == MapRegion::Physics) {
        return BossRoomBee; //TODO change to physics boss
    }

    // Tutorial rooms
    if (type == RoomType::TutorialRoom2) {
        return TutorialRoom2Preset;
    }

    RoomPreset nextRoom;

    // Regular rooms
    if (!hasLocked(type,map.roomsTraversed) && locked) {
        assert(false);
    }

    DifficultyRegion currentRegion;
    if (map.roomsTraversed < static_cast<int>(DifficultyRegion::Intro)) {
        currentRegion = DifficultyRegion::Intro;
    } else if (map.roomsTraversed < static_cast<int>(DifficultyRegion::Easy)) {
        currentRegion = DifficultyRegion::Easy;
    } else if (map.roomsTraversed < static_cast<int>(DifficultyRegion::Medium)) {
        currentRegion = DifficultyRegion::Medium;
    } else {
        currentRegion = DifficultyRegion::Medium; // Assuming Medium for higher roomsTraversed
    }

    nextRoom = Random::ListItem(locked ? map.directory.at(currentRegion).at(type).locked : map.directory.at(currentRegion).at(type).unlocked);

    // Remove the one-time room from all relevant regions
    if (nextRoom.oneTime) {
        for (auto it = map.directory.begin(); it != map.directory.end(); ++it) {
            if (it->first >= currentRegion) { // Remove from current and future regions
                std::map<RoomType, RoomPresets>& roomMap = it->second;
                std::vector<RoomPreset>& roomList = roomMap.at(type).locked;
                roomList.erase(std::remove(roomList.begin(), roomList.end(), nextRoom), roomList.end());

                // Repeat the process for unlocked rooms if applicable
                std::vector<RoomPreset>& unlockedRoomList = roomMap.at(type).unlocked;
                unlockedRoomList.erase(std::remove(unlockedRoomList.begin(), unlockedRoomList.end(), nextRoom), unlockedRoomList.end());
            }
        }
    }

    return nextRoom;
}

// Define the comparison operator for DifficultyRegion
bool operator>=(DifficultyRegion lhs, DifficultyRegion rhs) {
    return static_cast<int>(lhs) >= static_cast<int>(rhs);
}



const std::map<RoomType,int> roomTypeToSymbols = {
    {RoomType::EnemyRoom,3},
    {RoomType::RestRoom,1},
    {RoomType::BossRoom,2},
    {RoomType::TreasureRoom,0},
{ RoomType::EventRoom, 4 },
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
    {RoomType::TutorialRoom2, SoundType::normalBGM},
        { RoomType::EventRoom, SoundType::specialBGM }
};
