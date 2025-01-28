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
    {
        {{WhiteBoard,{}}, {0.25f, 0.1f}},
        {{Desk,{}}, {0.85f, 0.07f}},
        {{Phone,{}}, {0.5f, 0.8f}},
    },
    0,
    0,
    0,
    "TutorialRoom1",
    false,
    {1000,1200}
};

const RoomPreset StartingRoom{
    {},
    {},
    {
        {{WhiteBoard,{}}, {0.25f, 0.1f}},
        {{Desk,{}}, {0.85f, 0.07f}}
    },
    0,
    0,
    0,
    "StartingRoom",
    false,
    {1000,1200}
};

const RoomPreset ScientistBossRoom{
    {{{EnemyType::ScientistBoss, {0.5, 0.1}}}},
    {},
    {},
    0.0f,
    0 ,
    0,
    "\"Viceroy\"",
    false,
    {1600,1300}
};

const RoomPreset TutorialRoom2Preset{
    {{{EnemyType::TutorialEnemyQuadshooter, {0.5, 0.4}}}},
    {},
    {},
    0.0f,
    3 ,
    0,
    "TutorialRoom2",
    false,
    {1000,800}
};

const RoomPreset EnemyRoomSwarm{
    {
        {{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
     {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}}},
{{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}}},
{{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}}},
{{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}}},
{{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}}},
{{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}}},
{{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}}},
    },

    {},
    {{{PopConsole,{bulletPierceUpA, bulletPierceUpM}}, {0.5f, 0.5f}}, {{PushConsole,{ bulletPierceUpM}}, {0.25f, 0.5f}}, {{PushConsole,{bulletPierceUpA}}, {0.75f, 0.5f}}},
    15.0f,
    0,
0,
"Hallway full of Drones",
    false,
    {1600, 1000}
};

const RoomPreset EnemyRoomDashIntro1{
            {
                {{EnemyType::EnemyQuadshooter, {0.8f, 0.2f}},
                {EnemyType::EnemyQuadshooter, {0.2f, 0.8f}},
                {EnemyType::EnemySkull, {0.5, 0.5}}
                }
            },
       {},
       {},
       0.0f,
       3,
    1,
    "EnemyRoomDashIntro1",
};

const RoomPreset EnemyRoomDashIntro2{
            {
                {{EnemyType::EnemySnail, {0.2f, 0.8f}},
                {EnemyType::EnemySnail, {0.8f, 0.8f}},
                {EnemyType::EnemyCrab, {0.5f, 0.5f}},
               }
            },
       {},
       {},
       0.0f,
       3,
    1,
    "EnemyRoomDashIntro2",
};

const RoomPreset EnemyRoomDashIntro3{
            {
                {{EnemyType::EnemyBioBoid, {0.2f, 0.8f}},
                {EnemyType::EnemyBioBoid, {0.2f, 0.8f}},
                {EnemyType::EnemyBioBoid, {0.2f, 0.8f}},
                {EnemyType::EnemyBioBoid, {0.8f, 0.2f}},
                {EnemyType::EnemyBioBoid, {0.8f, 0.2f}},
                {EnemyType::EnemyBioBoid, {0.8f, 0.2f}},
                {EnemyType::EnemyThreeBee, {0.5f, 0.5f}},
               }
            },
       {},
       {},
       0.0f,
       3,
         1,
            "EnemyRoomDashIntro3",
};

const RoomPreset EnemyRoomDash1{
        {
            {{EnemyType::EnemyQuadshooter, {0.2f, 0.8f}},
            {EnemyType::EnemyQuadshooter, {0.8f, 0.8f}},
            {EnemyType::EnemyQuadshooter, {0.8f, 0.2f}},
            {EnemyType::EnemyQuadshooter, {0.2f, 0.2f}},
            {EnemyType::EnemyPufferfish, {0.2, 0.4}},
            {EnemyType::EnemySkull, {0.5f, 0.5f}},
            },
            {
                {EnemyType::EnemySword, {Random::Float(),Random::Float()}},
                {EnemyType::EnemySword, {Random::Float(),Random::Float()}}
            },
            {
                {EnemyType::EnemySword, {Random::Float(),Random::Float()}},
                {EnemyType::EnemyPufferfish, {Random::Float(),Random::Float()}},
{EnemyType::EnemyPufferfish, {Random::Float(),Random::Float()}},
{EnemyType::EnemyPufferfish, {Random::Float(),Random::Float()}}
            },
        },
    {},
    {},
    10.0f,
    5,
    2,
    "EnemyRoomDash1",
};

const RoomPreset EnemyRoomDash2{
        {
            {{EnemyType::EnemyQuadshooter, {0.4f, 0.2f}},
           {EnemyType::EnemyQuadshooter, {0.8f, 0.4f}},
            {EnemyType::EnemyPufferfish, {0.7, 0.4}},
               {EnemySnail, {0.5f, 0.7f}},
            {EnemyType::EnemySkull, {0.5f, 0.5f}}
            }
        },
    {},
    {},
    0.0f,
    5,
    2,
    "EnemyRoomDash2",
};


const RoomPreset EnemyRoomDashHard{
        {
            {
                {EnemyType::EnemyEvilSkull, {0.5f, 0.5f}},
                {EnemyType::EnemyPufferfish, {0.3, 0.2}},
            },
            {
                    {EnemyType::EnemyPufferfish, {Random::Float(),Random::Float()}},
                {EnemyType::EnemyEvilSnail, {Random::Float(),Random::Float()}}
            },
            {
                {EnemyType::EnemyEvilSnail, {0.5f, 0.5f}},
        {EnemyType::EnemySkull, {0.2f, 0.8f}},
    {EnemyType::EnemySkull, {0.8f, 0.8f}},
        {EnemyType::EnemySkull, {0.8f, 0.2f}},
        {EnemyType::EnemySkull, {0.2f, 0.2f}},
            },
        },
    {},
    {},
    20.0f,
    5,
    2,
    "EnemyRoomDashHard",
};

const RoomPreset EnemyRoomSnails {
        {
            {
                {EnemyType::EnemyEvilSnail, {0.5f, 0.5f}},
                {EnemySnail, {0.2f, 0.8f}},
                {EnemyType::EnemyLaserCrab, {0.8f, 0.2f}},
                {EnemyMagnet, {0.2f, 0.4f}},
                {EnemyMagnet, {0.8f, 0.4f}},
        }
        },
{},
{},
0.0f,
    5,
    2,
    "Snail's Sector",

};

const RoomPreset EnemyRoomSnailsHard {
            {
                {
                    {EnemyType::EnemyEvilSnail, {0.5f, 0.5f}},
                    {EnemySnail, {0.3f, 0.7f}},
                    {EnemySnail, {0.4f, 0.6f}},
                    {EnemyType::EnemyLaserCrab, {0.8f, 0.2f}},
                    {EnemyMagnet, {0.8f, 0.4f}},
            },
                {
                        {EnemyType::EnemyEvilSnail, {Random::Float(), Random::Float()}},
{EnemySnail, {Random::Float(), Random::Float()}},
                    {EnemySkull, {0.5f, 0.5f}},

                }
            },
    {},
    {},
    15.0f,
        5,
        2,
        "Snail's Sanctuary",
        false,
        {1000, 750}

    };

const RoomPreset EnemyRoomCrabs{
        {
            {{EnemyType::EnemyCrab, {0.8f, 0.8f}},
            {EnemyType::EnemyLaserCrab, {0.8f, 0.2f}},
            {EnemyType::EnemyCrab, {0.2f, 0.2f}},
            {EnemyType::EnemyEvilCrab, {0.5f, 0.5f}},
           }
        },
    {},
    {},
    0.0f,
    5,
    2,
    "Crab's Cove",
};

const RoomPreset EnemyRoomSmall{
        {
            {{EnemyType::EnemySkull, {0.5f, 0.5f}},
           {EnemyType::EnemyBioBoid, {0.5f, 0.5f}},
           {EnemyType::EnemyBioBoid, {0.5f, 0.5f}},
           {EnemyType::EnemyBioBoid, {0.5f, 0.5f}},
           {EnemyType::EnemyBioBoid, {0.5f, 0.5f}},
           {EnemyType::EnemyBioBoid, {0.5f, 0.5f}},
           {EnemyType::EnemyBioBoid, {0.5f, 0.5f}},
           {EnemyType::EnemyBioBoid, {0.5f, 0.5f}},
           {EnemyType::EnemyBioBoid, {0.5f, 0.5f}},
           {EnemyType::EnemyBioBoid, {0.5f, 0.5f}},
           }
        },
    {},
    {},
    0.0f,
    5,
    2,
    "Closet Cleanout",
    false,
    {800,800}
};

const RoomPreset EnemyRoomBees1{
        {
            {{EnemyType::EnemyOneBee, {0.2f, 0.8f}},
            {EnemyType::EnemyOneBee, {0.8f, 0.8f}},
            {EnemyType::EnemyOneBee, {0.8f, 0.2f}},
            {EnemyType::EnemyOneBee, {0.2f, 0.2f}},
            {EnemyType::EnemyBeeHive, {0.2f, 0.4f}},
            {EnemyType::EnemyBeeHive, {0.8f, 0.4f}}}
        },
    {},
    {},
    0.0f,
    5,
    2,
    "Bee Brigade",
};

const RoomPreset EnemyRoomBees2{
        {
            {{EnemyType::EnemyTwoBee, {0.2f, 0.8f}},
            {EnemyType::EnemyTwoBee, {0.8f, 0.8f}},
            {EnemyType::EnemyBeeHive, {0.2f, 0.4f}},
            {EnemyType::EnemyBeeHive, {0.8f, 0.4f}}},
            {
                {EnemyType::EnemyTwoBee, {Random::Float(), Random::Float()}},
                   {EnemyEvilSnail, {Random::Float(), Random::Float()}},
                 {EnemyType::EnemyTwoBee, {Random::Float(), Random::Float()}},
        {EnemyType::EnemyTwoBee, {Random::Float(), Random::Float()}},

            },
        },

    {},
    {},
    22.5f,
    7,
    2,
    "Bee Squadron",
    };

const RoomPreset EnemyRoomBees3{
        {
            {{EnemyType::EnemyTwoBee, {0.2f, 0.8f}},
            {EnemyType::EnemyOneBee, {0.8f, 0.8f}},
            {EnemyType::EnemyTwoBee, {0.8f, 0.2f}},
            {EnemyType::EnemyOneBee, {0.2f, 0.2f}},
            {EnemyType::EnemyEvilSkull, {0.5f, 0.5f}},
           }
        },
    {},
    {},
    0.0f,
    5,
    2,
    "Bee Blitz",
};

const RoomPreset EnemyRoomAngelTank{
        {
            {{EnemyType::EnemyMagnet, {0.2f, 0.8f}},
            {EnemyType::EnemyMagnet, {0.8f, 0.2f}},
            {EnemyType::EnemyCrab, {0.5f, 0.5f}},
            {EnemyType::EnemyHealer, {0.8, 0.8}},
            {EnemyType::EnemyHealer, {0.2, 0.2}}}
        },
     
    {},
    {},
    0.25f,
    5,
    2,
    "An Angel's Touch",
};

const RoomPreset EnemyRoomTripleBuffEX{
        {
            {{EnemyType::EnemyMagnet, {0.2f, 0.8f}},
               {EnemyType::EnemyCrab, {0.2f, 0.2f}},
           {EnemyType::EnemyEvilCrab, {0.6f, 0.2f}},
           {EnemyType::EnemyCrab, {0.2f, 0.6f}},
               {EnemySnail, {0.7f, 0.7f}},
               {EnemyEvilSnail, {0.7f, 0.3f}},
           }
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
    "Resting Room",
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
    "Gardener's Abode",
    true,
    {1800, 1000}
};

const RoomPreset EventRoomSwarm{
        {},
        {},
        {{{Swarm,{}}, {0.5f, 0.45f}}},
    0.0f,
    0,
    0,
    "Swarms' Retreat",
    true,
    {1800, 1000}
};

const RoomPreset EventRoomOven{
            {},
            {},
            {{{Oven,{}}, {0.5f, 0.5f}}},
        0.0f,
        0,
        0,
        "Oven Room",
        false,
        {1200, 1200}
};

const RoomPreset EventRoomOracle{
            {},
            {},
            {{{OracleTurret,{}}, {0.5f, 0.45f}}},
        0.0f,
        0,
        0,
        "The Oracle",
        true,
        {1800, 1000}
};

const RoomPreset EventRoomMouse{
                {},
                {},
                {{{Mouse,{}}, {0.5f, 0.5f}}},
            0.0f,
            0,
            0,
            "EventRoomMouse",
            true,
            {1200, 1200}
};


const RoomPreset RestRoomBibleTree{
    {},
    {},
    {{{InteractableItem::BibleTree,{}}, {0.5f, 0.5f}}},
    0.0f,
    0,
    0,
    "Tree of Knowledge",
    true,
    {2000,2000}
};

const RoomPreset RestRoomBaru{
        {},
        {},
{{{Baru,{key}}, {0.5f, 0.5f}}},
        0.0f,
        5,
        0,
    "Unknown Machine",
        true,
    {3000, 2000}
    };

const RoomPreset RestRoomOracleCrab{
        {},
        {},
        {{{OracleCrab,{key}}, {0.5f, 0.5f}}},
        0.0f,
        5,
        0,
    "Strange Crab",
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
     {2000, 1500},
};

const RoomPreset TreasureRoom4{
    {},
    {},
    {{{FightConsole,{dashCDRDownA}}, {0.5f, 0.5f}}},
    0.0f,
    5,
    0,
    "TreasureRoom4",
    false,
     {2000, 1500},
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

const RoomPreset TreasureRoomSniper{
    {},
    {},
    {{{PushConsole,{SniperPower,  SniperSpeed, SniperLethargy}}, {0.5f, 0.5f}}},
    0.0f,
    5,
    0,
    "TreasureRoomSniper",
    true,
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
    true,
     {1300, 1800},
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
     {1800, 2000},
};





const RoomPreset BossRoomBee{
    {{{BossBeehiveMain, {0.5f, 0.5f}}, {EnemyThreeBee,{0.5f, 0.75f}}}},
    {},
    {{{Ram,{}}, {0.5f, 0.5f}}},
    0.0f,
    20,
    5,
    "\"Apiarist\"",

};


const RoomPreset BossRoomCrab{
    {{{BossCrab, {0.5f, 0.5f}}, {EnemyCrab,{Random::Float(), Random::Float()}}}},
    {},
    {{{Ram,{}}, {0.5f, 0.5f}}},
    0.0f,
    20,
    5,
    "Carcinisation",
    false,
    {3000, 3000}

};

const RoomPreset HifiRoomTwinLaserShurikens {
        {
            {
                {EnemyType::EnemyHifiTwinLaserHorizontal1, {0.7f,0.1f}},
                {EnemyType::EnemyHifiTwinLaserHorizontal1, {0.99f,0.1f}},
                {EnemyType::EnemyTwinLaserVertical1, {0.1f,0.7f}},
                {EnemyType::EnemyHifiTrail, {0.6f,0.6f}},
                {EnemyType::EnemyHifiTrail, {0.4f,0.4f}}
            }
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
            {
                {EnemyType::EnemyHifiTwinLaserHorizontal1, {0.7f,0.1f}},
                {EnemyType::EnemyTwinLaserVertical1, {0.1f,0.3f}},
                {EnemyType::EnemyHifiChargerHard, {0.8f,0.2f}},
                {EnemyType::EnemyHifiChargerHard, {0.8f,0.8f}},
                {EnemyType::EnemyHifiChargerHard, {0.2f,0.2f}},
                {EnemyType::EnemyHifiChargerHard, {0.2f,0.8f}},
                {EnemyType::EnemyHifiChargerHard, {0.4f,0.6f}},
                {EnemyType::EnemyHifiChargerHard, {0.6f,0.4f}},
            }
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
            {
                {EnemyType::EnemyHifiCharger, {0.7f,0.3f}},
                {EnemyType::EnemyHifiCharger, {0.7f,0.7f}},
                {EnemyType::EnemyHifiCharger, {0.4f,0.5f}},
                {EnemyType::EnemySniper, {0.5f,0.5f}},
                {EnemyType::EnemySniper, {0.2f,0.2f}},
                {EnemyType::EnemySniper, {0.8f,0.2f}},
                {EnemyType::EnemySniper, {0.8f,0.8f}},
                {EnemyType::EnemySniper, {0.2f,0.8f}},
                {EnemyType::EnemyHealer, {0.5f,0.5f}},
                {EnemyType::EnemyHealer, {0.2f,0.2f}},
            }
        },
    {},
    {},
    0.0f,
    5,
    2,
    "HifiBasic"
};

const RoomPreset HifiRoomBasicWave {
            {
                {
                    {EnemyType::EnemyHifiCharger, {0.7f,0.3f}},
                    {EnemyType::EnemyHifiCharger, {0.7f,0.7f}},
                    {EnemyType::EnemyHifiCharger, {0.4f,0.5f}},
                },
                {
                        {EnemyType::EnemySniper, {0.5f,0.5f}},
                        {EnemyType::EnemySniper, {0.2f,0.2f}},
                        {EnemyType::EnemySniper, {0.8f,0.2f}},
                        {EnemyType::EnemySniper, {0.8f,0.8f}},
                        {EnemyType::EnemySniper, {0.2f,0.8f}},
                },
                {
                            {EnemyType::EnemyHifiCharger, {0.4f,0.5f}},
                        {EnemyType::EnemyHealer, {0.5f,0.5f}},
                        {EnemyType::EnemyHealer, {0.2f,0.2f}},
                }
            },
        {},
        {},
        20.0f,
        5,
        2,
        "HifiWave"
    };

const RoomPreset HifiRoomBoidSnipers{
        {
            {
                {EnemyType::EnemySniper, {0.6f,0.5f}},
                {EnemyType::EnemySniper, {0.4f,0.5f}},
                {EnemyType::EnemySniper, {0.5f,0.5f}},
                {EnemyType::EnemySniper, {0.5f,0.6f}},
                {EnemyType::EnemySniper, {0.5f,0.4f}},
            },
{
    {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
    {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
    {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
    {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
    {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
    {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
    {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},

            },
    {
        {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
        {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
        {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
        {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
        {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
        {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
        {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},

                },
{
        {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
        {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
        {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
        {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
        {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
        {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
        {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},

                },
        },
    {},
    {},
    10.0f,
    5,
    2,
    "HifiBoidSnipers"
};

const RoomPreset HifiEnemyRoomSwarmLasers {
        {
            {{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
            {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
            {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},

       {EnemyType::EnemyHifiTwinLaserHorizontal1, {0.7f,0.1f}},
           {EnemyType::EnemyTwinLaserVertical1, {0.1f,0.3f}},
       {EnemyType::EnemyHifiTwinLaserHorizontal1, {0.3f,0.1f}},
       {EnemyType::EnemyTwinLaserVertical1, {0.1f,0.7f}},

            },
            {
                       {EnemyType::EnemyHifiTrail, {0.6f,0.6f}},
           {EnemyType::EnemyHifiTrail, {0.4f,0.4f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
            },

        {{EnemyType::EnemyHifiTrailHard, {0.4f,0.6f}},
{EnemyType::EnemyHifiTrailHard, {0.6f,0.4f}},
    {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
    {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
    {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
    {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
    {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
    {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
    {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
    {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
    {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
    {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
    {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
                },

        },

    {},
    {{{PopConsole,{numBulletsUpA}}, {0.5f, 0.5f}},
        {{PushConsole,{numBulletsUpA}}, {0.25f, 0.5f}},
        {{PushConsole,{bulletBurstUpA}}, {0.75f, 0.5f}}},
    15.0f,
    0,
0,
"HifiEnemyRoomSwarmLasers",
    false,
    {2000, 2000}
};

const RoomPreset HifiRoomCannonLasers{
        {
            {
                {EnemyType::EnemyHifiCannon, {0.9f,0.2f}},
                {EnemyType::EnemyHifiCannon, {0.9f,0.8f}},
                {EnemyType::EnemyHifiCannon, {0.1f,0.2f}},
                {EnemyType::EnemyHifiCannon, {0.1f,0.8f}},
        {EnemyType::EnemyHealer, {0.5f,0.5f}},
        {EnemyType::EnemyHealer, {0.2f,0.2f}},
                {EnemyType::EnemyHifiTwinLaserHorizontal1, {0.7f,0.1f}},
                {EnemyType::EnemyTwinLaserVertical1, {0.1f,0.3f}},
                {EnemyType::EnemyHifiTwinLaserHorizontal1, {0.3f,0.1f}},
                {EnemyType::EnemyTwinLaserVertical1, {0.1f,0.7f}},
            }
        },
    {},
    {},
    0.0f,
    5,
    2,
    "HifiBoidCannons"
};

const RoomPreset HifiRoomCannonSnipers{
        {
            {
                {EnemyType::EnemyLaserSniper, {0.1f,0.2f}},
                {EnemyType::EnemySniper, {0.6f,0.6f}},
                {EnemyType::EnemySniper, {0.4f,0.4f}}

            },
            {
                    {EnemyType::EnemyLaserSniper, {Random::Float(),Random::Float()}},
                    {EnemyType::EnemySniper, {Random::Float(),Random::Float()}},
                        {EnemyType::EnemySniper, {Random::Float(),Random::Float()}},
                    {EnemyType::EnemySniper, {Random::Float(),Random::Float()}},
            },
            {
                    {EnemyType::EnemyHifiCannon, {Random::Float(),Random::Float()}},
                    {EnemyType::EnemyHifiCannon, {Random::Float(),Random::Float()}},
            },
        },
    {},
    {},
    12.0f,
    5,
    2,
    "HifiBoidCannons"
};

const RoomPreset HifiRoomSniperShurikens{
        {
            {
                {EnemyType::EnemyHifiSniperHard, {0.8f,0.2f}},
                {EnemyType::EnemyHifiTrailHard, {0.4f,0.4f}},
            {EnemyType::EnemyHealer, {0.5f,0.5f}},
            {EnemyType::EnemyHealer, {0.2f,0.2f}},
            },
            {
                    {EnemyType::EnemyHifiTrailHard, {Random::Float(),Random::Float()}},
                        {EnemyType::EnemyHifiTrailHard, {Random::Float(),Random::Float()}},
            },
            {
                {EnemyType::EnemyHifiSniperHard, {0.8f,0.8f}},
                    {EnemyType::EnemyHifiSniperHard, {0.2f,0.8f}},
                {EnemyType::EnemyHealer, {0.5f,0.5f}},
                {EnemyType::EnemyHealer, {0.2f,0.2f}},
            },
        },
    {},
    {},
    10.0f,
    5,
    2,
    "HifiBoidCannons"
};

const RoomPreset HifiRoomCannonBoids{
        {
            {
                {EnemyType::EnemyHifiCannonHard, {0.7f,0.5f}},
                {EnemyType::EnemyHifiCannonHard, {0.3f,0.5f}},
        {EnemyType::EnemyHealer, {0.5f,0.5f}},
        {EnemyType::EnemyHealer, {0.2f,0.2f}},
            },
{{EnemyHifiTackShooter, {0.2,0.2}},
        {EnemyHifiTackShooter, {0.2,0.8}},},
            {
                    {EnemyType::EnemyHealer, {Random::Float(),Random::Float()}},
            {EnemyType::EnemyHealer, {Random::Float(),Random::Float()}},
        {EnemyType::EnemyHealer, {Random::Float(),Random::Float()}},
            {EnemyType::EnemyHifiCannonHard, {0.5f,0.5f}},
            {EnemyType::EnemyHealer, {Random::Float(),Random::Float()}},
            },

        },
    {},
    {},
    15.0f,
    5,
    2,
    "HifiBoidCannons"
};

const RoomPreset EnemyRoomLaserFiesta {
        {
            {
                {EnemyType::EnemyHifiTwinLaserHorizontal1, {0.7f,0.1f}},
                {EnemyType::EnemyTwinLaserVertical1, {0.1f,0.3f}},
                {EnemyType::EnemyHifiTwinLaserHorizontal1, {0.3f,0.1f}},
                {EnemyType::EnemyTwinLaserVertical1, {0.1f,0.7f}},
            },
{{EnemyType::EnemyLaserSniper, {Random::Float(),Random::Float()}},

        {EnemyType::EnemyLaserSniper, {Random::Float(),Random::Float()}},

            },
            {
                    {EnemyType::EnemyLaserSniper, {Random::Float(),Random::Float()}},

                    {EnemyType::EnemyLaserSniper, {Random::Float(),Random::Float()}},
            },
                {
                        {EnemyType::EnemyLaserSniper, {Random::Float(),Random::Float()}},

                        {EnemyType::EnemyLaserSniper, {Random::Float(),Random::Float()}},
                },
{
                        {EnemyType::EnemyLaserSniper, {Random::Float(),Random::Float()}},

                        {EnemyType::EnemyLaserSniper, {Random::Float(),Random::Float()}},
                },
            {
            {EnemyEvilSkull , {0.5f, 0.5f}},
                {EnemySkull, {0.25f, 0.25f}},
                {EnemySkull, {0.75f, 0.75f}},
                },
        },
    {},
{{{PopConsole,{numBulletsUpA}}, {0.5f, 0.5f}},
    {{PushConsole,{fireRateUpM, bulletBounceUpM}}, {0.25f, 0.5f}},
    {{PushConsole,{bulletBurstUpA, bulletSpeedUpA}}, {0.75f, 0.5f}},
},
    12.0f,
    5,
    2,
    "Laser Fiesta",
    false,
    {2500, 2000}
};

const RoomPreset HifiRoomJellyFish { //Test room for new enemies
        {
            {
                {EnemyHifiWhip, {0.5f, 0.5f}},
                {EnemyHifiJellyFish, {0.5f, 0.5f}},
                {EnemyHifiTackShooter, {0.8,0.2}},
            },
            {
                    {EnemyHifiJellyFish, {0.5f, 0.5f}},
                    {EnemyHifiTackShooter, {0.2,0.2}},
                    {EnemyHifiTackShooter, {0.2,0.8}},
                    {EnemyHifiBallLauncher,{0.4,0.6}},
            },
        },
    {},
    {},
    0.0f,
    5,
    2,
    "HifiRoomJellyFish",
    false,
    {2500, 2000}
};

const RoomPreset HifiRoomSniperBallLauncher {
        {
            {
                {EnemyHifiBallLauncher,{0.5,0.5}},
                {EnemySniper,{0.7,0.5}},
                {EnemySniper,{0.3,0.5}},
                {EnemySniper,{0.5,0.3}},
                {EnemySniper,{0.5,0.7}},
            }
        },
    {},
    {},
    0.0f,
    5,
    2,
    "HifiRoomJellyFish",
    false,
    {2500, 2000}
};

const RoomPreset HifiRoomSmall {
        {
            {
                {EnemyHifiWhip,{0.5,0.5}},
            },

            {
                    {EnemyHifiBallLauncher,{Random::Float(),Random::Float()}},
            },
            {
                        {EnemyHifiBallLauncher,{Random::Float(),Random::Float()}},
            },

            {
                    {EnemyHifiWhip,{0.5,0.5}},
            },

        },
    {},
    {},
    4.0f,
    5,
    2,
    "Wave in a bottle",
    false,
    {1200, 1200}
};

const RoomPreset HifiRoomSmallBallLauncher {
            {
                {
                    {EnemyHifiBallLauncher,{0.5,0.5}},
                    {EnemyHifiCharger,{Random::Float(),Random::Float()}},

                },

                {
                    {EnemyHifiTackShooter,{0.5,0.5}},
                    {EnemyHifiCharger,{Random::Float(),Random::Float()}},
                        {EnemyHifiCharger,{Random::Float(),Random::Float()}},
                    {EnemyHifiCharger,{Random::Float(),Random::Float()}},
                },

            },
        {},
        {},
        100.0f,
        5,
        2,
        "Jack in the box",
        false,
        {800, 800}
};

const RoomPreset HifiRoomAvenue {
            {
                {
                    {EnemyLaserSniper,{0.9,0.2}},
                    {EnemyLaserSniper,{0.9,0.8}},
                    {EnemyHifiCannon,{0.9,0.5}},

                },

                {
                    {EnemyLaserSniper,{0.1,0.2}},
                    {EnemyLaserSniper,{0.1,0.8}},
                    {EnemyHifiCannonHard,{0.1,0.5}},


                },

            },
        {},
        {},
        30.0f,
        5,
        2,
        "Avenue Ambush",
        false,
        {3000, 650}
};

const RoomPreset HifiRoomLane {
            {
                {
                    {EnemyLaserSniper,{0.2,0.9}},
                    {EnemyLaserSniper,{0.8,0.9}},
                    {EnemyHifiCannon,{0.5,0.9}},

                },

                {
                    {EnemyLaserSniper,{0.2,0.1}},
                    {EnemyLaserSniper,{0.8,0.1}},
                    {EnemyHifiCannonHard,{0.5,0.1}},


                },

            },
        {},
        {},
        100.0f,
        5,
        2,
        "Laneway Lasers",
        false,
        {650, 3000}
};




const RoomPreset allConsoles {
    {},
    {},
    {{{PushConsole,{numBulletsUpA}}, {0.5f, 0.5f}},
    {{PopConsole,{fireRateUpM, bulletBounceUpM}}, {0.25f, 0.5f}},
    {{WishGranter,{bulletBurstUpA, bulletSpeedUpA}}, {0.75f, 0.5f}},
},
    0.0f,
    5,
    2,
    "AllConsoles",
    false,
    {2500, 2000}
};


const std::map<DifficultyRegion,std::map<RoomType, RoomPresets>> bioRoomDirectory = {
    {DifficultyRegion::Intro,{
        {RoomType::EnemyRoom, {{EnemyRoomDashIntro1,EnemyRoomDashIntro2,EnemyRoomDashIntro3,EnemyRoomSmall },{}}},
        {RoomType::RestRoom, {{RestingRoomPop},{RestingRoomPop},}},
        {RoomType::EventRoom, {{RestingRoomGardener, RestRoomBibleTree, RestRoomOracleCrab}, {EventRoomMouse}}  },
        {RoomType::TreasureRoom, {{TreasureRoom2,TreasureRoom3,TreasureRoom4, TreasureRoomKey},{TreasureRoomSniper, TreasureRoom5}}},
    }},
    {DifficultyRegion::Easy,{
        {RoomType::EnemyRoom, {{EnemyRoomBees1, EnemyRoomDash1,EnemyRoomCrabs,EnemyRoomSmall, EnemyRoomBees2, EnemyRoomBees3, EnemyRoomDash2, EnemyRoomSnails},{EnemyRoomSwarm}}},
        {RoomType::RestRoom, {{RestingRoomPop},{RestingRoomPop}}},
        {RoomType::EventRoom, {{RestingRoomGardener, RestRoomBibleTree, RestRoomOracleCrab}, {EventRoomMouse}}  },
        {RoomType::TreasureRoom, {{TreasureRoom1,TreasureRoom2,TreasureRoom3,TreasureRoom4, TreasureRoomKey, TreasureRoomKeys, TreasureRoomBlunt},{TreasureRoomSniper, TreasureRoom5}}},
    }},
    {DifficultyRegion::Medium,{
        {RoomType::EnemyRoom, {{EnemyRoomAngelTank,EnemyRoomCrabs, EnemyRoomDashHard, EnemyRoomBees3, EnemyRoomDash2, EnemyRoomBees2},{EnemyRoomTripleBuffEX}}},
        {RoomType::RestRoom, {{RestingRoomPop},{RestingRoomPop}}},
        {RoomType::EventRoom, {{RestingRoomGardener, RestRoomBibleTree, RestRoomOracleCrab}, {EventRoomMouse}}  },
        {RoomType::TreasureRoom, {{TreasureRoom1,TreasureRoom2,TreasureRoom3,TreasureRoomBlunt, TreasureRoom4, TreasureRoomKey, TreasureRoomKeys},{TreasureRoomSniper, TreasureRoom5}}},
    }},
};

const std::map<DifficultyRegion,std::map<RoomType, RoomPresets>> physicsRoomDirectory = {
    {DifficultyRegion::Intro,{
        // {RoomType::EnemyRoom, {{HifiRoomSmall},{}}},
        {RoomType::EnemyRoom, {{HifiRoomSniperBallLauncher,HifiRoomBasicEnemy,HifiRoomBoidSnipers,HifiRoomTwinLaserChargers,HifiRoomTwinLaserShurikens, HifiRoomBasicWave, Random::Float() < 0.5f ?  HifiRoomAvenue : HifiRoomLane },{HifiEnemyRoomSwarmLasers}}},
        {RoomType::RestRoom, {{},{RestingRoomPop}}},
        {RoomType::EventRoom, {{TreasureRoomHoney, RestRoomOracleCrab, EventRoomSwarm, EventRoomOven}, {RestRoomBaru, TreasureRoomHoney }}  },
        {RoomType::TreasureRoom, {{TreasureRoom2,TreasureRoom3,TreasureRoom4, TreasureRoomKey},{TreasureRoomSniper, TreasureRoom5}}},
    }},
    {DifficultyRegion::Easy,{
        {RoomType::EnemyRoom, {{HifiRoomSniperBallLauncher,HifiRoomJellyFish, HifiRoomCannonLasers,HifiRoomCannonSnipers, HifiRoomSniperShurikens,HifiRoomCannonBoids, HifiRoomSmall, Random::Float() < 0.5f ?  HifiRoomAvenue : HifiRoomLane},{HifiEnemyRoomSwarmLasers, EnemyRoomLaserFiesta}}},
        {RoomType::RestRoom, {{RestingRoomPop},{RestingRoomPop}}},
        {RoomType::EventRoom, {{TreasureRoomHoney, EventRoomSwarm, RestRoomOracleCrab, EventRoomOven}, {RestRoomBaru, TreasureRoomHoney, TreasureRoomWish }}  },
        {RoomType::TreasureRoom, {{TreasureRoom1,TreasureRoom2,TreasureRoom3,TreasureRoom4, TreasureRoomKey, TreasureRoomKeys, TreasureRoomBlunt},{TreasureRoomSniper, TreasureRoom5}}},
    }},
    {DifficultyRegion::Medium,{
        {RoomType::EnemyRoom, {{HifiRoomCannonLasers,HifiRoomJellyFish, HifiRoomCannonSnipers, HifiRoomSniperShurikens,HifiRoomSmallBallLauncher,HifiRoomSmall, },{EnemyRoomLaserFiesta}}},
        {RoomType::RestRoom, {{RestingRoomPop},{}}},
        {RoomType::EventRoom, {{RestRoomBaru, TreasureRoomHoney, TreasureRoomWish, EventRoomSwarm, EventRoomOven}, {RestRoomBaru, TreasureRoomHoney, TreasureRoomWish }}  },
        {RoomType::TreasureRoom, {{TreasureRoom1,TreasureRoom2,TreasureRoom3,TreasureRoomBlunt, TreasureRoom4, TreasureRoomKey, TreasureRoomKeys},{TreasureRoomSniper, TreasureRoom5}}},
    }},
};

const std::map<DifficultyRegion,std::map<RoomType, RoomPresets>> MiningRoomDirectory = {
    {DifficultyRegion::Intro,{
        {RoomType::EnemyRoom, {{HifiRoomSniperBallLauncher,HifiRoomBasicEnemy,HifiRoomBoidSnipers,HifiRoomTwinLaserChargers,HifiRoomTwinLaserShurikens, HifiRoomBasicWave, Random::Float() < 0.5f ?  HifiRoomAvenue : HifiRoomLane },{HifiEnemyRoomSwarmLasers}}},
        {RoomType::RestRoom, {{},{RestingRoomPop}}},
        {RoomType::EventRoom, {{TreasureRoomHoney, RestRoomOracleCrab, EventRoomSwarm, EventRoomOven}, {RestRoomBaru, TreasureRoomHoney }}  },
        {RoomType::TreasureRoom, {{TreasureRoom2,TreasureRoom3,TreasureRoom4, TreasureRoomKey},{TreasureRoomSniper, TreasureRoom5}}},
    }},
    {DifficultyRegion::Easy,{
        {RoomType::EnemyRoom, {{HifiRoomSniperBallLauncher,HifiRoomJellyFish, HifiRoomCannonLasers,HifiRoomCannonSnipers, HifiRoomSniperShurikens,HifiRoomCannonBoids, HifiRoomSmall, Random::Float() < 0.5f ?  HifiRoomAvenue : HifiRoomLane},{HifiEnemyRoomSwarmLasers, EnemyRoomLaserFiesta}}},
        {RoomType::RestRoom, {{RestingRoomPop},{RestingRoomPop}}},
        {RoomType::EventRoom, {{TreasureRoomHoney, EventRoomSwarm, RestRoomOracleCrab, EventRoomOven}, {RestRoomBaru, TreasureRoomHoney, TreasureRoomWish }}  },
        {RoomType::TreasureRoom, {{TreasureRoom1,TreasureRoom2,TreasureRoom3,TreasureRoom4, TreasureRoomKey, TreasureRoomKeys, TreasureRoomBlunt},{TreasureRoomSniper, TreasureRoom5}}},
    }},
    {DifficultyRegion::Medium,{
        {RoomType::EnemyRoom, {{HifiRoomCannonLasers,HifiRoomJellyFish, HifiRoomCannonSnipers, HifiRoomSniperShurikens,HifiRoomSmallBallLauncher,HifiRoomSmall, },{EnemyRoomLaserFiesta}}},
        {RoomType::RestRoom, {{RestingRoomPop},{}}},
        {RoomType::EventRoom, {{RestRoomBaru, TreasureRoomHoney, TreasureRoomWish, EventRoomSwarm, EventRoomOven}, {RestRoomBaru, TreasureRoomHoney, TreasureRoomWish }}  },
        {RoomType::TreasureRoom, {{TreasureRoom1,TreasureRoom2,TreasureRoom3,TreasureRoomBlunt, TreasureRoom4, TreasureRoomKey, TreasureRoomKeys},{TreasureRoomSniper, TreasureRoom5}}},
    }},
};

const std::map<DifficultyRegion,std::map<RoomType, RoomPresets>> MedicalRoomDirectory = {
    {DifficultyRegion::Intro,{
        {RoomType::EnemyRoom, {{HifiRoomSniperBallLauncher,HifiRoomBasicEnemy,HifiRoomBoidSnipers,HifiRoomTwinLaserChargers,HifiRoomTwinLaserShurikens, HifiRoomBasicWave, Random::Float() < 0.5f ?  HifiRoomAvenue : HifiRoomLane },{HifiEnemyRoomSwarmLasers}}},
        {RoomType::RestRoom, {{},{RestingRoomPop}}},
        {RoomType::EventRoom, {{TreasureRoomHoney, RestRoomOracleCrab, EventRoomSwarm, EventRoomOven}, {RestRoomBaru, TreasureRoomHoney }}  },
        {RoomType::TreasureRoom, {{TreasureRoom2,TreasureRoom3,TreasureRoom4, TreasureRoomKey},{TreasureRoomSniper, TreasureRoom5}}},
    }},
    {DifficultyRegion::Easy,{
        {RoomType::EnemyRoom, {{HifiRoomSniperBallLauncher,HifiRoomJellyFish, HifiRoomCannonLasers,HifiRoomCannonSnipers, HifiRoomSniperShurikens,HifiRoomCannonBoids, HifiRoomSmall, Random::Float() < 0.5f ?  HifiRoomAvenue : HifiRoomLane},{HifiEnemyRoomSwarmLasers, EnemyRoomLaserFiesta}}},
        {RoomType::RestRoom, {{RestingRoomPop},{RestingRoomPop}}},
        {RoomType::EventRoom, {{TreasureRoomHoney, EventRoomSwarm, RestRoomOracleCrab, EventRoomOven}, {RestRoomBaru, TreasureRoomHoney, TreasureRoomWish }}  },
        {RoomType::TreasureRoom, {{TreasureRoom1,TreasureRoom2,TreasureRoom3,TreasureRoom4, TreasureRoomKey, TreasureRoomKeys, TreasureRoomBlunt},{TreasureRoomSniper, TreasureRoom5}}},
    }},
    {DifficultyRegion::Medium,{
        {RoomType::EnemyRoom, {{HifiRoomCannonLasers,HifiRoomJellyFish, HifiRoomCannonSnipers, HifiRoomSniperShurikens,HifiRoomSmallBallLauncher,HifiRoomSmall, },{EnemyRoomLaserFiesta}}},
        {RoomType::RestRoom, {{RestingRoomPop},{}}},
        {RoomType::EventRoom, {{RestRoomBaru, TreasureRoomHoney, TreasureRoomWish, EventRoomSwarm, EventRoomOven}, {RestRoomBaru, TreasureRoomHoney, TreasureRoomWish }}  },
        {RoomType::TreasureRoom, {{TreasureRoom1,TreasureRoom2,TreasureRoom3,TreasureRoomBlunt, TreasureRoom4, TreasureRoomKey, TreasureRoomKeys},{TreasureRoomSniper, TreasureRoom5}}},
    }},
};

inline std::map<DifficultyRegion,std::map<RoomType, RoomPresets>> getDirectory(MapRegion region) {
   switch ( region ) {
       case MapRegion::Tutorial:
           return bioRoomDirectory;
       case MapRegion::Biology:
           return bioRoomDirectory;
       case MapRegion::Physics:
           return physicsRoomDirectory;
       case MapRegion::Mining:
           return MiningRoomDirectory;
       case MapRegion::Medical:
           return MedicalRoomDirectory;
       default:
           return bioRoomDirectory;
   }
}

inline bool hasLocked(RoomType type, int roomsTraversed) {
    Map& map = registry.maps.components[0];
    std::map<DifficultyRegion, std::map<RoomType, RoomPresets>> directory = map.directory;

    // magic number: means room after boss room 
    // if (roomsTraversed == 13) {
    //     directory = getDirectory((MapRegion)(map.currRegion + 1));
    //     std::cout << "using next dir: " << map.currRegion + 1 << std::endl;
    // }
    
    if (type >= RoomType::None) {
        return false;
    }

    DifficultyRegion currentRegion;
    if (map.roomsTraversed < static_cast<int>(DifficultyRegion::Intro)) {
        currentRegion = DifficultyRegion::Intro;
    }
    else if (map.roomsTraversed < static_cast<int>(DifficultyRegion::Easy)) {
        currentRegion = DifficultyRegion::Easy;
    }
    else if (map.roomsTraversed < static_cast<int>(DifficultyRegion::Medium)) {
        currentRegion = DifficultyRegion::Medium;
    }
    else {
        currentRegion = DifficultyRegion::Medium; // Assuming Medium for higher roomsTraversed
    }

    int roomCount = directory.at(currentRegion).at(type).locked.size();
    //std::cout << "locked: " << roomCount << std::endl;
    return roomCount > 0;
}

inline bool hasUnlocked(RoomType type, int roomsTraversed) {
    Map& map = registry.maps.components[0];
    std::map<DifficultyRegion, std::map<RoomType, RoomPresets>> directory = map.directory;

    // magic number: means room after boss room 
    // if (roomsTraversed == 13) {
    //     directory = getDirectory((MapRegion)(map.currRegion + 1));
    //     std::cout << "using next dir: " << map.currRegion + 1 << std::endl;
    // }

    if (type >= RoomType::None) {
        return false;
    }

    DifficultyRegion currentRegion;
    if (map.roomsTraversed < static_cast<int>(DifficultyRegion::Intro)) {
        currentRegion = DifficultyRegion::Intro;
    }
    else if (map.roomsTraversed < static_cast<int>(DifficultyRegion::Easy)) {
        currentRegion = DifficultyRegion::Easy;
    }
    else if (map.roomsTraversed < static_cast<int>(DifficultyRegion::Medium)) {
        currentRegion = DifficultyRegion::Medium;
    }
    else {
        currentRegion = DifficultyRegion::Medium; // Assuming Medium for higher roomsTraversed
    }

    int roomCount = directory.at(currentRegion).at(type).unlocked.size();
    //std::cout << "unlocked: " << roomCount << std::endl;
    return roomCount > 0;
}

inline RoomType getRandomRoomType(bool excludeNone, int roomsTraversed)
{
    const int bossRoomNum = 12;
    if (roomsTraversed % bossRoomNum == bossRoomNum-1) {
        return BossRoom;
    }

    float r = Random::Float();

    if (r < 0.65f) { //enemy room has higher chance of being rolled
        return RoomType::EnemyRoom;
    }
    if (r < 0.75f) { //enemy room has higher chance of being rolled
        return RoomType::RestRoom;
    }


    std::vector<RoomType> possibleRooms;
    if (!excludeNone) {
        possibleRooms.push_back(RoomType::EnemyRoom);
    }
    if (hasLocked(RoomType::EventRoom, roomsTraversed) || hasUnlocked(RoomType::EventRoom, roomsTraversed)) {
        possibleRooms.push_back(RoomType::EventRoom);
    }
    if (hasLocked(RoomType::TreasureRoom, roomsTraversed) || hasUnlocked(RoomType::TreasureRoom, roomsTraversed)) {
        possibleRooms.push_back(RoomType::TreasureRoom);
    }
    return Random::ListItem(possibleRooms);
}

inline std::vector<RoomType> getRandomRoomTypes(bool excludeNone, int roomsTraversed)
{
    const int bossRoomNum = 12;
    if (roomsTraversed % bossRoomNum == bossRoomNum - 1) {
        return { RoomType::BossRoom,RoomType::BossRoom ,RoomType::BossRoom ,RoomType::BossRoom };
    }

    std::vector<RoomType> out = { RoomType::EnemyRoom, RoomType::EnemyRoom, RoomType::EnemyRoom, RoomType::EnemyRoom};
    bool treasure = false;
    bool rest = false;
    bool event = false;
    for (int i = 0; i < 4; i++) {
        float r = Random::Float();
        if (r < 0.600f) continue;
        if (r < 0.700f && !rest && !(treasure && event)) {
            out[i] = RoomType::RestRoom;
            rest = true;
        }
        else if (r < 0.850f && !treasure && !(rest && event)) {
            out[i] = RoomType::TreasureRoom;
            treasure = true;
        } 
        else if (r < 1.000f && !event && !(treasure && rest)) {
            out[i] = RoomType::EventRoom; 
            event = true;
        }
    }

    if (!treasure && !rest && !event && Random::Float() < 0.3) out[rand() % 4] = RoomType::RestRoom;

    return out;
}

inline RoomPreset getRoomPreset(RoomType type, MapRegion currRegion, bool locked, int roomsTraversed = -1) {
    Map& map = registry.maps.components[0];
    //boss rooms
    if (type == RoomType::BossRoom && currRegion == MapRegion::Biology) {
        std::vector<RoomPreset> biobosses = {BossRoomBee, BossRoomCrab};
        return Random::ListItem(biobosses);
    } else if (type == RoomType::BossRoom && currRegion == MapRegion::Physics) {
        return ScientistBossRoom; //TODO change to physics boss
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
    if (roomsTraversed == -1) {

        if (map.roomsTraversed < static_cast<int>(DifficultyRegion::Intro)) {
            currentRegion = DifficultyRegion::Intro;
        } else if (map.roomsTraversed < static_cast<int>(DifficultyRegion::Easy)) {
            currentRegion = DifficultyRegion::Easy;
        } else if (map.roomsTraversed < static_cast<int>(DifficultyRegion::Medium)) {
            currentRegion = DifficultyRegion::Medium;
        } else {
            currentRegion = DifficultyRegion::Medium; // Assuming Medium for higher roomsTraversed
        }
    } else {
        if (roomsTraversed < static_cast<int>(DifficultyRegion::Intro)) {
            currentRegion = DifficultyRegion::Intro;
        } else if (roomsTraversed < static_cast<int>(DifficultyRegion::Easy)) {
            currentRegion = DifficultyRegion::Easy;
        } else if (roomsTraversed < static_cast<int>(DifficultyRegion::Medium)) {
            currentRegion = DifficultyRegion::Medium;
        } else {
            currentRegion = DifficultyRegion::Medium; // Assuming Medium for higher roomsTraversed
        }
    }

    nextRoom = Random::ListItem(locked ? getDirectory(currRegion).at(currentRegion).at(type).locked : getDirectory(currRegion).at(currentRegion).at(type).unlocked);

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
    {RoomType::None,5},
    {RoomType::TutorialRoom1,6},
    {RoomType::TutorialRoom2,6}
};

const std::map<RoomType, SoundType> roomTypeToMusic = {
    {RoomType::EnemyRoom, SoundType::normalBGM},
    {RoomType::RestRoom, SoundType::specialBGM},
    {RoomType::BossRoom, SoundType::bossBGM},
    {RoomType::TreasureRoom, SoundType::specialBGM},
    {RoomType::None, normalBGM},
    {RoomType::TutorialRoom, SoundType::titleBGM},
    {RoomType::TutorialRoom1, SoundType::titleBGM},
    {RoomType::TutorialRoom2, SoundType::normalBGM},
{ RoomType::EventRoom, SoundType::specialBGM }

};

std::vector<std::vector<std::tuple<EnemyType,vec2>>> bioEliteEnemies = {
    {{EnemyType::EnemyTwoBee, {0.5f,0.5f}}},
};

std::vector<std::vector<std::tuple<EnemyType,vec2>> > physicsEliteEnemies = {
    {{EnemyType::EnemyHifiCannonHard, {0.5f,0.5f}}},
};

std::map<MapRegion,std::vector<std::tuple<EnemyType,vec2>> > eliteEnemies = {
    {Biology, Random::ListItem(bioEliteEnemies)},
    {Physics, Random::ListItem(physicsEliteEnemies)}
};