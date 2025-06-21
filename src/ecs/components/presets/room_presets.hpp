#pragma once
#include <vector>
#include <tuple>
#include "components/map_components.hpp"
#include "premades.hpp"
#include "common.hpp"
#include "tiny_ecs_registry.hpp"
#include "utils/random.hpp"

//struct RoomPreset {
//    std::list<std::vector<std::tuple<EnemyType, vec2>>>enemies;
//    std::vector<std::tuple<RoomProp, vec2>> roomProps; //background props
//    std::vector<std::tuple<RoomInteractable, vec2>> interactables; //for interactables
//    float spawnDelay; //in seconds - for enemies and bosses
//    int numSpecialBulletsToSpawn = 5;
//    int numKeyBulletsToSpawn = 2;
//    std::string ID;
//    bool oneTime = false; //if true, room will not appear again
//    vec2 roomSize = { 1600, 1600 };
//    bool hasElite = false;
//    bool operator==(const RoomPreset& other) const {
//        return ID == other.ID;
//    }
//};

//---------- PRESETS-------------------

const RoomPreset TestRoom{
    {{  {EnemyTestLightningRotate,  {0.1f, 0.1f}},
        {EnemyTestLightningShuffle, {0.1f, 0.2f}},
        {EnemyTestEffectPlusOne,    {0.1f, 0.3f}},
        {EnemyTestEffectPlusTwo,    {0.1f, 0.4f}},
        {EnemyTestEffectPlusThree,  {0.1f, 0.5f}},
        {EnemyTestEffectBlunt,      {0.1f, 0.6f}},
        {EnemyTestEffectMinusOne,   {0.1f, 0.7f}},
        {EnemyTestEffectMinusTwo,   {0.1f, 0.8f}},
        {EnemyTestEffectMinusThree, {0.1f, 0.9f}}}},
    {},
    {},
    0.0f,
    0,
    0,
    "Testing Chamber",
    false,
    {1000, 2000}
};

const RoomPreset TestRoom2{
    {{  {EnemyMaw,  {0.1f, 0.1f}}
    }},
{},
{},
0.0f,
0,
0,
"Testing Chamber2",
false,
{1000, 1000}
};


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
    {{{EnemyType::ScientistBoss, {0.5, 0.2}}}},
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
    0,
    0,
    "TutorialRoom2",
    false,
    {1000,800}
};

const RoomPreset BioEnemyRoomSwarm{
    {
        {
            {EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},

        },
        {
            {EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
        },
        {
        {EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},},
        {
            {EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyBioBoid, random_vec2},
{EnemyType::EnemyTwoBee, random_vec2},
        },
    },

    {},
    { {{PushConsole,{ bulletPierceUp,  bulletPierceUp,bulletPierceUp,}}, {0.25f, 0.5f}}, {{PopConsole,{}}, {0.75f, 0.5f}}},
    7.5f,
    0,
0,
"Infested Hallway",
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
    "Uneven Quadrants",
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
    "A Date With Snails",
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
                    {EnemyType::EnemyBioBoid, random_vec2},
                    {EnemyType::EnemyBioBoid, random_vec2},
                    {EnemyType::EnemyBioBoid, random_vec2},
                    {EnemyType::EnemyBioBoid, random_vec2},
                    {EnemyType::EnemyBioBoid, random_vec2},
                    {EnemyType::EnemyBioBoid, random_vec2},
                    {EnemyType::EnemyBioBoid, random_vec2},
                    {EnemyType::EnemyBioBoid, random_vec2},
                    {EnemyType::EnemyBioBoid, random_vec2},
                    {EnemyType::EnemyBioBoid, random_vec2},
               }
            },
       {},
       {},
       0.0f,
       3,
         1,
            "Pupal Training",
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
                {EnemyType::EnemySword, random_vec2},
                {EnemyType::EnemySword, random_vec2}
            },
            {
                {EnemyType::EnemySword, random_vec2},
                {EnemyType::EnemyPufferfish, random_vec2},
{EnemyType::EnemyPufferfish, random_vec2},
{EnemyType::EnemyPufferfish, random_vec2}
            },
        },
    {},
    {},
    10.0f,
    5,
    2,
    "Spiny Troubles",
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
    "Tacit Understanding",
};


const RoomPreset EnemyRoomDashHard{
        {
            {
                {EnemyType::EnemyEvilSkull, {0.5f, 0.5f}},
                {EnemyType::EnemyPufferfish, {0.3, 0.2}},
            },
            {
                    {EnemyType::EnemyPufferfish, random_vec2},
                {EnemyType::EnemyEvilSnail, random_vec2}
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
    "Brains in Jars",
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
    "Sector Disinfestation",

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
                    {EnemyType::EnemyEvilSnail, {random_float, random_float}},
                    {EnemySnail, {random_float, random_float}},
                    {EnemySkull, {0.5f, 0.5f}},

                }
            },
    {},
    {},
    15.0f,
        5,
        2,
        "Slimy Sanctuary",
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
    "Crabby Cove",
};

const RoomPreset BioRoomSmall{
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
                {EnemyType::EnemyTwoBee, {random_float, random_float}},
                   {EnemyEvilSnail, {random_float, random_float}},
                 {EnemyType::EnemyTwoBee, {random_float, random_float}},
        {EnemyType::EnemyTwoBee, {random_float, random_float}},

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
    "Order of Importance",
};

const RoomPreset EnemyRoomAngelTank{
        {
            {{EnemyType::EnemyMagnet, {0.2f, 0.8f}},
            {EnemyType::EnemyMagnet, {0.8f, 0.2f}},
            {EnemyType::EnemyCrab, {0.5f, 0.5f}},
            {EnemyType::EnemyHealer, {0.8, 0.8}},
            {EnemyType::EnemyHealer, {0.2, 0.2}}},
{{EnemyType::EnemySword, {0.2f, 0.8f}},
{EnemyType::EnemySword, {0.8f, 0.2f}},
{EnemyType::EnemyEvilCrab, {0.5f, 0.5f}},
{EnemyType::EnemyHealer, {0.8, 0.8}},
{EnemyType::EnemyHealer, {0.2, 0.2}}}
        },

    {},
    {},
    12.25f,
    5,
    2,
    "An Angel's Touch",
};

const RoomPreset EnemyRoomMagic{
        {
            {
                {EnemyType::EnemyQuadshooter, {0.25f, 0.25f}},
                    {EnemyType::EnemyQuadshooter, {0.75f, 0.75f}},
                    {EnemyType::EnemyQuadshooter, {0.25f, 0.75f}},
                    {EnemyType::EnemyQuadshooter, {0.75f, 0.25f}},

                    { EnemyType::EnemyMage, {0.5f, 0.5f}},
           },


            {
                {EnemyType::EnemyScissors, {random_float, random_float}},
                {EnemyType::EnemySword, {random_float, random_float}},
                    {EnemyType::EnemyHealer, {random_float, random_float}},
                {EnemyType::EnemyHealer, {random_float, random_float}},
            },

            {
                    {EnemyType::EnemyMage, {0.5f, 0.5f}},
            }
        },
    {},
    {
        {{PushConsole, {bulletBounceUp, bulletBounceUp, bulletPierceUp, bulletPierceUp}}, {0.25f, 0.5f}},
        {{PopConsole, {}}, {0.75f, 0.5f}},
    },
    25.0f,
    35,
    5,
    "Magical Trickeries",
    false,
    {3000, 3000}
};

const RoomPreset EnemyRoomInvisible{
            {
                {
                    {EnemyType::EnemyEvilSnail, {0.5f, 0.5f}},
                    {EnemyType::EnemyTwoBee, {0.8f, 0.2f}},
                    {EnemyMagnet, {0.3f, 0.4f}},
                    {InvisibleGranterRoomWide, {0.5f, 0.5f}},
                    // {InvisibleGranter, {0.8f, 0.2f}},
                    // {InvisibleGranter, {0.3f, 0.4f}},
            },
                {
                                           {EnemyPufferfish, {0.3f, 0.7f}},
                        {EnemyPufferfish, {0.4f, 0.6f}},
                        // {InvisibleGranter, {0.3f, 0.7f}},
                        // {InvisibleGranter, {0.4f, 0.6f}},

                },
                {
                               {EnemyType::EnemyCrab, {0.8f, 0.8f}},
            {EnemyType::EnemyEvilCrab, {0.8f, 0.2f}},
            {EnemyType::EnemyCrab, {0.2f, 0.2f}},
            //     {InvisibleGranter, {0.8f, 0.8f}},
            // {InvisibleGranter, {0.8f, 0.2f}},
            // {InvisibleGranter, {0.2f, 0.2f}},

                    },
                {
                    {EnemyTwoBee, {0.5f, 0.5f}},
            {EnemyTwoBee , {0.1f, 0.9f}},
            {EnemyThreeBee , {0.9f, 0.1f}},
                            {EnemyThreeBee, {0.4f, 0.6f}},
// {InvisibleGranter, {0.5f, 0.5f}},
// {InvisibleGranter , {0.1f, 0.9f}},
// {InvisibleGranter, {0.4f, 0.6f}},
// {InvisibleGranter , {0.9f, 0.1f}},
                }
            },
        {},
        {
            {
                {
                    PushConsole,
                    {bulletBounceUp, bulletBounceUp, bulletBounceUp, bulletRangeUp, bulletRangeUp, bulletRangeUp}
                },
                {0.25f, 0.5f}
            },
            {{PopConsole, {}}, {0.75f, 0.5f}},
        },
        20.0f,
        35,
        5,
        "Phantasmal Menace",
        false,
        {1800, 2300}
};

const RoomPreset EnemyRoomCloaked{
        {
            {
                {EnemySword, {0.5f, 0.5f}},
                {EnemySword,{0.3f, 0.5f}},
                {EnemySword,{0.7f, 0.5f}},
                    {CloakedGranterRoomWide, {0.5f, 0.5f}},
                    // {CloakedGranter,{0.3f, 0.5f}},
                    // {CloakedGranter,{0.7f, 0.5f}}
            },
            {
                {EnemyScissors, {0.5f, 0.5f}},
                {EnemyScissors,{0.5f, 0.3f}},
                {EnemyScissors,{0.5f, 0.7f}},
                    // {CloakedGranter, {0.5f, 0.5f}},
                    // {CloakedGranter,{0.5f, 0.3f}},
                    // {CloakedGranter,{0.5f, 0.7f}}
            },
                {
                    {EnemyHifiChargerHard, {0.5f, 0.5f}},
                    {EnemyHifiCharger,{0.3f, 0.3f}},
                    {EnemyHifiCharger,{0.7f, 0.7f}},
                    // {CloakedGranter, {0.5f, 0.5f}},
                    // {CloakedGranter,{0.3f, 0.3f}},
                    // {CloakedGranter,{0.7f, 0.7f}}
                },
            {
                {EnemyMedicalRodA, {0.5f, 0.5f}},
                {EnemyMedicalRodA,{0.3f, 0.7f}},
                {EnemyMedicalRodA,{0.7f, 0.3f}},
                    // {CloakedGranter, {0.5f, 0.5f}},
                    // {CloakedGranter,{0.3f, 0.7f}},
                    // {CloakedGranter,{0.7f, 0.3f}}
            },
            {
                    {EnemyMedicalSyringe,{0.3f, 0.7f}},
                    {EnemyMedicalSyringe,{0.7f, 0.3f}},
                        {EnemyMedicalBMP, {0.5f, 0.5f}},
                        // {CloakedGranter,{0.3f, 0.7f}},
                        // {CloakedGranter,{0.7f, 0.3f}},
                        // {CloakedGranter, {0.5f, 0.5f}},
            },
        },
        {},
        {},
        10.0f,
        10,
        5,
        "Cloak and Daggers",
        false,
        {1600, 1600}
};

const RoomPreset MedicalEnemyRoomInvincible{
            {
                {
                    {EnemyProstheticHand, {0.5f, 0.5f}},
                    {EnemyMedicalPillBoidSpawner,{0.3f, 0.5f}},
                    {EnemyMedicalPillBoidSpawner,{0.7f, 0.5f}},
                        {InvincibleGranterRoomWide, {0.5f, 0.5f}},
                        // {InvincibleGranter,{0.3f, 0.5f}},
                        // {InvincibleGranter,{0.7f, 0.5f}}
                },
                {
                    {EnemyScissors, {0.5f, 0.5f}},
                    {EnemyScissors,{0.5f, 0.3f}},
                    {EnemyScissors,{0.5f, 0.7f}},
                        // {InvincibleGranter, {0.5f, 0.5f}},
                        // {InvincibleGranter,{0.5f, 0.3f}},
                        // {InvincibleGranter,{0.5f, 0.7f}}
                },
                    {
                        {EnemyQuadshooterElite, {0.5f, 0.5f}},
                        {EnemySpineChainedWormHead,{0.3f, 0.3f}},
                        {EnemySpineChainedWormHead,{0.7f, 0.7f}},
                        // {InvincibleGranter, {0.5f, 0.5f}},
                        // {InvincibleGranter,{0.3f, 0.3f}},
                        // {InvincibleGranter,{0.7f, 0.7f}}
                    },

                {
                        {EnemyMedicalSyringe,{0.3f, 0.7f}},
                        {EnemyMedicalSyringe,{0.7f, 0.3f}},
                            {EnemySpinePatrolWormHead, {0.1f, 0.9f}},
                            // {InvincibleGranter,{0.3f, 0.7f}},
                            // {InvincibleGranter,{0.7f, 0.3f}},
                            // {InvincibleGranter, {0.1f, 0.9f}},
                },
            {
                        {EnemyMedicalRodC, {0.5f, 0.5f}},
                        {EnemyMedicalRodA,{0.3f, 0.7f}},
                        {EnemyMedicalRodA,{0.7f, 0.3f}},
                            // {InvincibleGranter, {0.5f, 0.5f}},
                            // {InvincibleGranter,{0.3f, 0.7f}},
                            // {InvincibleGranter,{0.7f, 0.3f}}
                },
            },
            {},
{{{PushConsole,{bulletBounceUp,bulletBounceUp,bulletBounceUp, bulletRangeUp,bulletRangeUp,bulletRangeUp}}, {0.25f, 0.5f}}, {{PopConsole,{}}, {0.75f, 0.5f}},
},
            15.0f,
            10,
            5,
            "Loyal to the beat",
            false,
            {2400, 2400}
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
    {1200,1200}
};

const RoomPreset RestingRoomPopLarge{
        {},
        {},
        {{{PopConsole,{}}, {0.5f, 0.5f}}},
        0.0f,
        0,
        0,
        "Resting Room",
        false,
        {2500, 2500}
};

const RoomPreset RestingRoomPopLong{
        {},
        {},
        {{{PopConsole,{}}, {0.5f, 0.5f}}},
        0.0f,
        0,
        0,
        "Resting Room",
        false,
        { 2500, 800}
};

const RoomPreset RestingRoomPopTall{
            {},
            {},
            {{{PopConsole,{}}, {0.5f, 0.5f}}},
            0.0f,
            0,
            0,
            "Resting Room",
            false,
            { 800, 2500}
};

const RoomPreset RestingRoomPopSmall{
        {},
        {},
        {{{PopConsole,{}}, {0.5f, 0.5f}}},
        0.0f,
        0,
        0,
        "Resting Room",
        false,
        {800, 800}
};

const RoomPreset RestingRoom3PopLarge{
        {},
        {},
        {{{PopConsole,{}}, {0.5f, 0.5f}}, {{PopConsole,{}}, {0.25f, 0.5f}}, {{PopConsole,{}}, {0.75f, 0.5f}},  {{PushConsole,{bulletPierceUp, bulletRangeUp}}, {0.5f, 0.25f}}, {{PopConsole,{dashUp, dashRechargeUp}}, {0.5f, 0.75f}}},
        0.0f,
        0,
        0,
        "Resting Room",
        false,
        {2500, 2500}
};

const RoomPreset EventRoomGardener{
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

const RoomPreset EventRoomOptimizer{
                    {},
                    {},
                    {{{Optimizer,{}}, {0.5f, 0.5f}}},
                0.0f,
                0,
                0,
                "Surgical Precision",
                true,
                {1400, 1000}
};

const RoomPreset EventRoomInverter{
                    {},
                    {},
                    {{{Inverter,{}}, {0.5f, 0.5f}}},
                0.0f,
                0,
                0,
                "Trash to Treasure",
                true,
                {1200, 1200}
};


const RoomPreset EventRoomBibleTree{
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

const RoomPreset EventRoomBaru{
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

const RoomPreset EventRoomOracleCrab{
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
    {{{FightConsole,{numBulletsUp, sizeUp}}, {0.5f, 0.5f}}},
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
    {{{PushConsole,{dashUp}}, {0.5f, 0.5f}}},
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
    {{{FightConsole,{dmgUp}}, {0.5f, 0.5f}}},
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
    {{{PushConsole,{dashRechargeUp}}, {0.5f, 0.5f}}},
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
    {{{PushConsole,{accuracyUp}}, {0.5f, 0.5f}}},
    0.0f,
    5,
    0,
    "TreasureRoom5",
    false,
     {1300, 1300},
};

const RoomPreset TreasureRoom6 {
    {},
    {},
    {{{PushConsole,{bulletRangeUp}}, {0.5f, 0.5f}}},
    0.0f,
    5,
    0,
    "TreasureRoom6",
    false,
     {1300, 1300},
};

const RoomPreset TreasureRoom7 {
    {},
    {},
    {{{PushConsole,{bulletPierceUp}}, {0.5f, 0.5f}}},
    0.0f,
    5,
    0,
    "TreasureRoom7",
    false,
     {1300, 1300},
};

const RoomPreset TreasureRoom8 {
    {},
    {},
    {{{PushConsole,{bulletBounceUp}}, {0.5f, 0.5f}}},
    0.0f,
    5,
    0,
    "TreasureRoom8",
    false,
     {1300, 1300},
};

const RoomPreset TreasureRoom9 {
    {},
    {},
    {{{PushConsole,{sizeUp}}, {0.5f, 0.5f}}},
    0.0f,
    5,
    0,
    "TreasureRoom9",
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
    {{{FightConsole,{blunt, blunt, blunt, dmgUp, dmgUp}}, {0.5f, 0.5f}}},
    0.0f,
    0,
    0,
    "TreasureRoomBlunt",false,
     {2000, 1500},
};

const RoomPreset TreasureRoomChoice1{
        {},
        {},
        {{{FightConsole,{dmgUp}}, {0.4f, 0.5f}}, {{FightConsole,{bulletRangeUp, bulletRangeUp}}, {0.6f, 0.5f}}},
        0.0f,
        5,
        0,
        "TreasureRoomChoice1",
        false,
         {2000, 1500},
    };

const RoomPreset TreasureRoomChoice2{
        {},
        {},
        {{{FightConsole,{numBulletsUp}}, {0.4f, 0.5f}}, {{FightConsole,{fireRateUp}}, {0.6f, 0.5f}}},
        0.0f,
        5,
        0,
        "TreasureRoomChoice2",
        false,
         {2000, 1500},
    };

const RoomPreset TreasureRoomChoice3{
        {},
        {},
        {{{FightConsole,{bulletPierceUp, bulletPierceUp, }}, {0.4f, 0.5f}}, {{FightConsole,{bulletBounceUp, bulletBounceUp}}, {0.6f, 0.5f}}},
        0.0f,
        5,
        0,
        "TreasureRoomChoice3",
        false,
         {2000, 1500},
    };

const RoomPreset TreasureRoomChoice4{
        {},
        {},
        {{{FightConsole,{bulletRangeUp, bulletRangeUp, }}, {0.4f, 0.5f}}, {{FightConsole,{accuracyUp, accuracyUp, accuracyUp,}}, {0.6f, 0.5f}}},
        0.0f,
        5,
        0,
        "TreasureRoomChoice4",
        false,
         {2000, 1500},
    };

const RoomPreset TreasureRoomChoice5{
        {},
        {},
        {{{FightConsole,{dashRechargeUp, dashRechargeUp, }}, {0.4f, 0.5f}}, {{FightConsole,{dashUp, dashUp, dashUp}}, {0.6f, 0.5f}}},
        0.0f,
        5,
        0,
        "TreasureRoomChoice5",
        false,
         {2000, 1500},
    };

const RoomPreset TreasureRoomChoice6{
        {},
        {},
        {{{FightConsole,{playerSpeedUp, playerSpeedUp, }}, {0.4f, 0.5f}}, {{FightConsole,{bulletRangeUp, bulletRangeUp,}}, {0.6f, 0.5f}}},
        0.0f,
        5,
        0,
        "TreasureRoomChoice6",
        false,
         {2000, 1500},
    };

const RoomPreset TreasureRoomChoice7{
        {},
        {},
        {{{FightConsole,{dashRechargeUp, dashRechargeUp, dashRechargeUp, dashDown }}, {0.4f, 0.5f}}, {{FightConsole,{bulletRangeUp, bulletRangeUp, bulletRangeUp, bulletRangeDown}}, {0.6f, 0.5f}}},
        0.0f,
        5,
        0,
        "TreasureRoomChoice7",
        false,
         {2000, 1500},
    };

const RoomPreset TreasureRoomChoice8{
        {},
        {},
        {{{FightConsole,{fireRateUp, fireRateUp, dmgDown, dmgDown, bulletRangeDown,  bulletRangeDown}}, {0.4f, 0.5f}}, {{FightConsole,{homingUp, dmgDown, dmgDown, accuracyDown, accuracyDown}}, {0.6f, 0.5f}}},
        0.0f,
        5,
        0,
        "TreasureRoomChoice8",
        false,
         {2000, 1500},
    };

const RoomPreset TreasureRoomSniper{
    {},
    {},
    {{{PushConsole,{dmgUp,dmgUp,dmgUp,  bulletRangeUp, bulletRangeUp, bulletRangeUp, playerSpeedDown,playerSpeedDown,playerSpeedDown}}, {0.5f, 0.5f}}},
    0.0f,
    5,
    0,
    "Sniper's Legacy",
    true,
     {1300, 1300},
};

const RoomPreset TreasureRoomShotgun{
        {},
        {},
        {{{PushConsole,{bulletRangeDown,bulletRangeDown,bulletRangeDown,  accuracyDown, accuracyDown, accuracyDown, numBulletsUp, numBulletsUp, numBulletsUp,  }}, {0.5f, 0.5f}}},
        0.0f,
        5,
        0,
        "Shotgun Fever",
        true,
         {1300, 1300},
    };

const RoomPreset TreasureRoomOstrich{
        {},
        {},
        {{{PushConsole,{dashDown, dashDown, dashDown, dashRechargeDown, dashRechargeDown, dashRechargeDown, playerSpeedUp,playerSpeedUp, playerSpeedUp, playerSpeedUp,playerSpeedUp, playerSpeedUp,}}, {0.5f, 0.5f}}},
        0.0f,
        5,
        0,
        "Ostrich's Progeny",
        true,
         {1300, 1300},
    };

const RoomPreset TreasureRoomMachineGun{
            {},
            {},
            {{{PushConsole,{fireRateUp, fireRateUp, fireRateUp, dmgDown, dmgDown,dmgDown, numBulletsDown, numBulletsDown, numBulletsDown}}, {0.5f, 0.5f}}},
            0.0f,
            5,
            0,
        "Frenzied Fiend",
            true,
             {1300, 1300},
        };

const RoomPreset TreasureRoomSuperSize{
                {},
                {},
                {{{PushConsole,{sizeUp, sizeUp,sizeUp, bulletPierceUp,bulletPierceUp, bulletPierceUp, bulletRangeDown, bulletRangeDown }}, {0.5f, 0.5f}}},
                0.0f,
                5,
                0,
            "Large and In Charge",
                true,
                 {1300, 1300},
            };

const RoomPreset TreasureRoomPinBall {
        {},
        {},
        {{{PushConsole,{bulletBounceUp, bulletBounceUp, bulletBounceUp, bulletRangeUp, bulletRangeUp, bulletRangeUp, sizeDown, sizeDown, sizeDown, }}, {0.5f, 0.5f}}},
        0.0f,
        5,
        0,
        "Pinball Wizard",
        true,
         {1300, 1300},
};

const RoomPreset TreasureRoomHare {
        {},
        {},
        {{{PushConsole,{playerSpeedDown, playerSpeedDown, playerSpeedDown, playerSpeedDown, playerSpeedDown, playerSpeedDown,  dashRechargeUp, dashRechargeUp, dashRechargeUp, dashUp, dashUp, dashUp}}, {0.5f, 0.5f}}},
        0.0f,
        5,
        0,
        "Hare's Heir",
        true,
         {1300, 1300},
};

const RoomPreset TreasureRoomRisky{
        {},
        {},
        {{{FightConsole,{homingUp, dmgDown,dmgDown,dmgDown,numBulletsDown,numBulletsDown,numBulletsDown,fireRateDown,fireRateDown,fireRateDown}}, {0.5f, 0.5f}}},
        0.0f,
        5,
        0,
        "Risky Business",
        true,
         {1300, 1300},
};

const RoomPreset EventRoomHoney{
    {},
    {},
    {{{HoneyCanister,{}}, {0.5f, 0.5f}}},
    0.0f,
    5,
    0,
    "A Sweet Scent",
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


const RoomPreset EventRoomWish{
    {},
    {},
    {{{WishGranter,{key}}, {0.5f, 0.5f}}},
    0.0f,
    5,
    0,
    "Unremembered Monument",
    true,
     {1800, 2000},
};



const RoomPreset BossRoomBee{
    {{{BossBeehiveMain, {0.5f, 0.5f}}, {EnemyThreeBee,{0.5f, 0.75f}}}},
    {},
    {{{Ram,{}}, {0.5f, 0.55f}}, { {PopConsole,{}}, {0.5f, 0.45f}},},
    0.0f,
    20,
    5,
    "Hivemind",

};


const RoomPreset BossRoomCrab{
    {{{BossCrab, {0.5f, 0.5f}}, {EnemyCrab,{0.25, 0.6}},{EnemyCrab,{0.75, 0.6}}}},
    {},
    {{{Ram,{}}, {0.5f, 0.55f}}, { {PopConsole,{}}, {0.5f, 0.45f}},},
    0.0f,
    20,
    5,
    "Carcinisation",
    false,
    {1800, 1800}

};

const RoomPreset BossRoomWorm{
    {{{BossDrillWormHead, {1.5f, 0.f}}}},
    //{{{EnemyMiningBoidWormHead, {0.45f, 0.45f}},{EnemyMiningBoidWormHead, {0.45f, 0.45f}},{EnemyMiningBoidWormHead, {0.45f, 0.45f}},{EnemyMiningBoidWormHead, {0.45f, 0.45f}}}},
    //{{{BossMultiCube, {0.5f, 0.5f}}}},
    {},
    {{{Ram,{}}, {0.5f, 0.55f}}, { {PopConsole,{}}, {0.5f, 0.45f}},},
    0.0f,
    20,
    5,
    "Tunnel Troubles",
    false,
    {1500, 1500}
};

const RoomPreset BossRoomMole {
        {

                {
                    {EnemyDynamite, {random_float, random_float}},
                    {EnemyDynamite, {random_float, random_float}},

            {EnemyDynamite, {random_float, random_float}},
                {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                    {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                    {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                    {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                    {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                    {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                    {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                    {EnemyDynamite, {random_float, random_float}},
                    {BossMole, {0.5f, 0.5f}},
                },
                {{EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                        {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                        {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                },
                {{EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                        {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                        {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                },
                {{EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                        {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                        {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                },
                {{EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                        {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                        {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                },
                {{EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                        {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                        {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                },
                {{EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                        {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                        {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                },
                {{EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                        {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                        {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                },
                {{EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                        {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                        {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                },
                {{EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                        {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                        {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                },
                {{EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                        {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                        {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                },

        },
    {},
        {{ {Ram,{}}, {0.5f, 0.55f}}, { {PopConsole,{}}, {0.5f, 0.45f}},},
        10.f,
        35,
        5,
        "Minesweeper",
        false,
        {2300, 1700}
};

const RoomPreset BossRoomMultiCube{
    {{{BossMultiCube, {0.5f, 0.5f}}}},
    {},
    {{{Ram,{}}, {0.5f, 0.55f}}, { {PopConsole,{}}, {0.5f, 0.45f}},},
    0.0f,
    20,
    5,
    "Colour Collective",
    false,
    {1500, 1500}
};

const RoomPreset HifiRoomTwinLaserShurikens {
        {
            {
                {EnemyType::EnemyHifiTwinLaserHorizontal1, {0.7f,0.1f}},
                {EnemyType::EnemyHifiTwinLaserHorizontal1, {0.99f,0.1f}},
                {EnemyType::EnemyTwinLaserVertical1, {0.1f,0.7f}},
                {EnemyType::EnemyHifiTrail, {0.6f,0.6f}},
                {EnemyType::EnemyHifiTrail, {0.4f,0.4f}},
{EnemyType::EnemyHifiTrail, random_vec2},
{EnemyType::EnemyHifiTrail, random_vec2},
{EnemyType::EnemyHifiTrail, random_vec2},
{EnemyType::EnemyHifiTrail, random_vec2},
{EnemyType::EnemyHifiTrailHard, random_vec2},
{EnemyType::EnemyHifiTrailHard, random_vec2},
            }
        },
    {},
    {},
    0.0f,
    5,
    2,
    "Prickly Situation"
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
{EnemyType::EnemyHifiChargerHard, random_vec2},
{EnemyType::EnemyHifiChargerHard, random_vec2},
{EnemyType::EnemyHifiChargerHard, random_vec2},
{EnemyType::EnemyHifiChargerHard, random_vec2},
{EnemyType::EnemyHifiChargerHard, random_vec2},
            }
        },
    {},
    {},
    0.0f,
    5,
    2,
    "Laser Tag"
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
                    {EnemyType::EnemyHifiSniperHard, random_vec2},
                    {EnemyType::EnemyHifiSniperHard, random_vec2},
                    {EnemyType::EnemyHifiSniperHard, random_vec2},
                    {EnemyType::EnemyHifiSniperHard, random_vec2},
                    {EnemyType::EnemyHifiSniperHard, random_vec2},
                    {EnemyType::EnemyHifiSniperHard, random_vec2},
            }
        },
    {},
    {},
    0.0f,
    5,
    2,
    "Keep Your Distance"
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
                            {EnemyType::EnemyHifiSniperHard, random_vec2},
                            {EnemyType::EnemyHifiSniperHard, random_vec2},
                },
                {
                            {EnemyType::EnemyHifiChargerHard, {0.4f,0.5f}},
                            {EnemyType::EnemyHifiSniperHard, random_vec2},
                            {EnemyType::EnemyHifiSniperHard, random_vec2},
                        {EnemyType::EnemyHealer, {0.5f,0.5f}},
                        {EnemyType::EnemyHealer, {0.2f,0.2f}},
                }
            },
        {},
        {},
        10.0f,
        5,
        2,
        "Cold Reception"
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
    {EnemyType::EnemyHifiBoid, random_vec2},
    {EnemyType::EnemyHifiBoid, random_vec2},
    {EnemyType::EnemyHifiBoid, random_vec2},
    {EnemyType::EnemyHifiBoid, random_vec2},
    {EnemyType::EnemyHifiBoid, random_vec2},
    {EnemyType::EnemyHifiBoid, random_vec2},
    {EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},

{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
        {EnemyType::EnemySniper, random_vec2},
        {EnemyType::EnemySniper, random_vec2},

            },
    {
        {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
        {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
        {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
        {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
        {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
        {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
        {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},

{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
            {EnemyType::EnemySniper, random_vec2},
            {EnemyType::EnemySniper, random_vec2},

                },
{
        {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
        {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
        {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
        {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
        {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
        {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
        {EnemyType::EnemyHifiBoid, {0.5f, 0.5f}},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},

{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},

{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},

{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
{EnemyType::EnemyHifiBoid, random_vec2},
        {EnemyType::EnemySniper, random_vec2},
        {EnemyType::EnemySniper, random_vec2},

                },
        },
    {},
    {},
    10.0f,
    5,
    2,
    "Arial Assault",
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
    {{{PopConsole,{numBulletsUp}}, {0.75f, 0.5f}},
        {{PushConsole,{numBulletsUp,numBulletsUp,numBulletsUp}}, {0.25f, 0.5f}}},
    15.0f,
    0,
0,
"Drone and Swarm",
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
                    {EnemyType::EnemyHifiCannonHard, random_vec2},
                    {EnemyType::EnemyHifiCannonHard, random_vec2},
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
    "Cannon Carnival"
};

const RoomPreset HifiRoomCannonSnipers{
        {
            {
                {EnemyType::EnemyLaserSniper, {0.1f,0.2f}},
                {EnemyType::EnemySniper, {0.6f,0.6f}},
                {EnemyType::EnemySniper, {0.4f,0.4f}}

            },
            {
                    {EnemyType::EnemyLaserSniper, random_vec2},
                    {EnemyType::EnemySniper, random_vec2},
                        {EnemyType::EnemySniper, random_vec2},
                    {EnemyType::EnemySniper, random_vec2},
            },
            {
                    {EnemyType::EnemyHifiCannon, random_vec2},
                    {EnemyType::EnemyHifiCannon, random_vec2},
            },
        },
    {},
    {},
    12.0f,
    5,
    2,
    "Ranged Assault",
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
                    {EnemyType::EnemyHifiTrailHard, random_vec2},
                        {EnemyType::EnemyHifiTrailHard, random_vec2},
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
    "Hot and Cold",
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
                    {EnemyType::EnemyHealer, random_vec2},
            {EnemyType::EnemyHealer, random_vec2},
        {EnemyType::EnemyHealer, random_vec2},
            {EnemyType::EnemyHifiCannonHard, {0.5f,0.5f}},
            {EnemyType::EnemyHealer, random_vec2},
            },

        },
    {},
    {},
    15.0f,
    5,
    2,
    "Mind the Gap",
};

const RoomPreset HifiRoomLaserFiesta {
        {
            {
                {EnemyType::EnemyHifiTwinLaserHorizontal1, {0.7f,0.1f}},
                {EnemyType::EnemyTwinLaserVertical1, {0.1f,0.3f}},
                {EnemyType::EnemyHifiTwinLaserHorizontal1, {0.3f,0.1f}},
                {EnemyType::EnemyTwinLaserVertical1, {0.1f,0.7f}},
            },
            {

    {EnemyType::EnemyLaserSniper, random_vec2},
        {EnemyType::EnemyLaserSniper, random_vec2},

            },
            {
                {EnemyType::EnemyLaserSniper, random_vec2},
        {EnemyType::EnemyLaserSniper, random_vec2},
            },
            {
                    {EnemyType::EnemyLaserSniper, random_vec2},
                    {EnemyType::EnemyLaserSniper, random_vec2},
                        {EnemyType::EnemyLaserSniper, random_vec2},
            },
            {
                                                {EnemyType::EnemyLaserSniper, random_vec2},
                            {EnemyType::EnemyLaserSniper, random_vec2},
                                {EnemyType::EnemyLaserSniper, random_vec2},
            },
                {
                        {EnemyType::EnemyLaserSniperHard, random_vec2},

                        {EnemyType::EnemyLaserSniperHard, random_vec2},
                },
            {
                                             {EnemyType::EnemyLaserSniperHard, random_vec2},
                                {EnemyType::EnemyLaserSniperHard, random_vec2},
            },
{
                        {EnemyType::EnemyLaserSniperHard, {0.7, 0.3}},
                            {EnemyType::EnemySmallCShield, {0.7, 0.3}},
                        {EnemyType::EnemyLaserSniperHard, { 0.3, 0.7}},
                            {EnemyType::EnemySmallCShield, { 0.3, 0.7}},
                },
            {
            {EnemyEvilSkull , {0.5f, 0.5f}},
            {EnemySmallCShield , {0.5f, 0.5f}},
                },
        },
    {},
{
    {{PushConsole,{fireRateUp, fireRateUp, fireRateUp, bulletRangeUp, bulletRangeUp, bulletRangeUp}}, {0.25f, 0.5f}},
    {{PopConsole,{}}, {0.75f, 0.5f}},
},
    6.5f,
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
                {EnemyHifiJellyFish, random_vec2},
                    {EnemyHifiJellyFish, random_vec2},
                    {EnemyHifiJellyFish, random_vec2},
                {EnemyHifiTackShooter, {0.8,0.2}},
            },
            {
                        {EnemyHifiJellyFish, random_vec2},
                        {EnemyHifiJellyFish, random_vec2},
                        {EnemyHifiJellyFish, random_vec2},
                        {EnemyHifiJellyFish, random_vec2},
                        {EnemyHifiJellyFish, random_vec2},
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
    "Biomimicry",
    false,
    {2500, 2000}
};

const RoomPreset HifiRoomSniperBallLauncher {
        {
            {
                {EnemyHifiBallLauncher,{0.5,0.5}},
                {ProtectedGranter, {0.5,0.5}},
                {EnemySniper, {0.7, 0.5}},
                {EnemySniper, {0.3, 0.5}},
                {EnemySniper, {0.5, 0.3}},
                {EnemySniper, {0.5, 0.7}},
            },

            {
                {EnemyHifiBallLauncher,{0.5,0.5}},

                    {EnemyHifiSniperHard,{0.7,0.5}},
                    {EnemyHifiSniperHard,{0.3,0.5}},
                    {EnemyHifiSniperHard,{0.5,0.3}},
                    {EnemyHifiSniperHard,{0.5,0.7}},
                    {EnemyHifiBallLauncher,random_vec2},
                    {EnemyHifiBallLauncher,random_vec2},
                    {EnemyHifiBallLauncher,random_vec2},
                    {EnemyHifiBallLauncher,random_vec2},
            },
        },
    {},
    {},
    15.0f,
    5,
    2,
    "Sniper's Nest",
    false,
    {2000, 1500}
};

const RoomPreset HifiRoomSmall {
        {
            {
                {EnemyHifiWhip,{0.5,0.5}},
            {EnemySmallCShield,{0.5,0.5}},
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
                    {EnemySmallCShield,{0.5,0.5}},
                    {EnemyHifiCharger,random_vec2},
                {EnemyHifiCharger,random_vec2},


                },

                {
                    // {EnemyHifiTackShooter,{0.5,0.5}},
                        {EnemyHifiCharger,random_vec2},
                    {EnemyHifiCharger,random_vec2},
                },

            },
        {},
        {},
        20.0f,
        5,
        2,
        "Jack in the box",
        false,
        {1000, 1000}
};

const RoomPreset HifiRoomSmallTackShooter {
                {
                    {
                        {EnemyHifiTackShooter,{0.5,0.5}},
                         {EnemySmallCShield,{0.5,0.5}},
        {EnemyHifiCharger,random_vec2},
            // {EnemyHifiCharger,random_vec2},
                        // {EnemyHifiCharger,random_vec2},

                    },

                    {
                        {EnemyHifiCharger,random_vec2},
                            {EnemyHifiCharger,random_vec2},
                    },

                },
            {},
            {},
            20.0f,
            15,
            2,
            "Sudden Skirmish",
            false,
            {800, 800}
};

const RoomPreset HifiRoomAvenue {
            {
                {
                    {EnemyLaserSniper,{0.9,0.2}},
                    {EnemyLaserSniper,{0.9,0.8}},
                    {EnemyHifiCannon,{0.9,0.5}},
                     {EnemyHifiBallLauncher, {0.5,0.5}},
                    {EnemyHifiBallLauncher, {0.5,random_float}},
                    {EnemyHifiBallLauncher, {0.5,random_float}},
                },

                {
                    {EnemyLaserSniper,{0.1,0.2}},
                    {EnemyLaserSniper,{0.1,0.8}},
                    {EnemyHifiCannonHard,{0.1,0.5}},
                    {EnemyHifiTrailHard, {0.5, random_float}},
                    {EnemyHifiTrailHard, {0.5, random_float}},

                },
                    {
                        {EnemyLaserSniper,{0.9,0.2}},
                        {EnemyLaserSniper,{0.9,0.8}},
                        {EnemyHifiCannon,{0.9,0.5}},
                            {EnemyLaserSniper,{0.85,0.5}},
                         {EnemyHifiBallLauncher, {0.5,0.5}},
                        {EnemyHifiBallLauncher, {0.5,random_float}},
                        {EnemyHifiBallLauncher, {0.5,random_float}},
                            {EnemyHifiJellyFish, random_vec2},
                            {EnemyHifiJellyFish, random_vec2},
                            {EnemyHifiJellyFish, random_vec2},
                    },

            },
        {},
        {},
        20.0f,
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
                        {EnemyHifiTackShooter, {0.5,0.5}},
                                {EnemyBubbleShield, {0.5,0.5}},
                        {EnemyHifiTackShooter, {random_float, 0.5f}},
                        {EnemyHifiTackShooter, {random_float, 0.5f}},
                },

                {
                    {EnemyLaserSniper,{0.2,0.1}},
                    {EnemyLaserSniper,{0.8,0.1}},
                    {EnemyHifiCannonHard,{0.5,0.1}},
                        {EnemyHifiTackShooter, {0.5,0.5}},
                    {EnemyHifiSniperHard, {0.5, random_float}},
                        {EnemyHifiSniperHard, {0.5, random_float}},

                },

                    {
                        {EnemyLaserSniper,{0.2,0.9}},
                        {EnemyLaserSniper,{0.8,0.9}},
                        {EnemyHifiCannonHard,{0.5,0.9}},
                                {EnemySmallCShield, {0.5,0.9}},
                            {EnemyLaserSniper,{0.5,0.85}},
                            {EnemyHifiTackShooter, {0.5,0.5}},
                                    {EnemyBubbleShield, {0.5,0.5}},
                                {EnemyHifiTackShooter, {random_float, 0.5f}},
                                {EnemyHifiTackShooter, {random_float, 0.5f}},
                            {EnemyHifiChargerHard, random_vec2},
                                    {EnemyHifiChargerHard, random_vec2},
                                    {EnemyHifiChargerHard, random_vec2},

                    },
            },
        {},
        {},
        20.0f,
        5,
        2,
        "Laser Laneway",
        false,
        {650, 3000}
};

const RoomPreset HifiRoomShieldedMadness{
            {
                {
                    {EnemyHifiWhip, {0.5f, 0.5f}},
                    {EnemyHifiTackShooter, {0.8,0.2}},
                    {EnemyHifiTackShooter, {0.2,0.8}},
                        {EnemyBubbleShield, {0.5,0.5}},
                        { EnemySmallCShield, {0.5f, 0.5f}},
                    {EnemySmallCShield, {0.8,0.2}},
                    { EnemySmallCShield, {0.2,0.8}},

                },
                {
                        {EnemyHifiJellyFish, {0.6f, 0.4f}},
                        {EnemyHifiJellyFish,{0.4,0.6}},
                        {EnemyHifiJellyFish, {0.6f, 0.6f}},
                        {EnemyHifiJellyFish, {0.4f, 0.4f}},

                        { EnemySmallCShield, {0.6f, 0.4f}},
                        { EnemySmallCShield, {0.2f, 0.2f}},
                        { EnemySmallCShield, {0.4f, 0.6f}},
                        { EnemySmallCShield, {0.6f, 0.6f}},
                        { EnemySmallCShield, {0.4f, 0.4f}},

                },

            },

        {},
        {{{PopConsole,{numBulletsUp}}, {0.25f, 0.5f}},
                {{PushConsole,{bulletPierceUp, bulletPierceUp, bulletPierceUp, fireRateUp, fireRateUp}}, {0.75f, 0.5f}}},
        15.0f,
        45,
    10,
    "Shielded Madness",
        false,
        {2000, 2000}
};


const RoomPreset BossRoomBigC {

            {
                {
                    {BossBigC, {0.5f, 0.5f}},
                    {EnemyLaserSniper,{0.5, 0.63}},
                        {EnemyLaserSniper,{0.38, 0.42}},
                    {EnemyLaserSniper,{1-0.38, 0.42}},
                },
            },
        {},
        {{{Ram,{}}, {0.5f, 0.55f}}, { {PopConsole,{}}, {0.5f, 0.45f}},},
        24.0f,
        20,
        5,
        "Solemn Sentry",
    false,
    {2200, 2500}

};

const RoomPreset QuadShooterRoomSmall {
            {
                {
                    {EnemyQuadshooterElite,{0.5,0.5}},
                {EnemySmallCShield,{0.5,0.5}},
                },


            },
        {},
        {},
        4.0f,
        5,
        2,
        "Concentrated Fire",
        false,
        {1200, 1200}
};

const RoomPreset MedicalEnemyRoomWares{
    {
        {
            {EnemyMagnet, {0.5f, 0.5f}},
            {EnemyMagnet,{0.3f, 0.7f}},
            {EnemyMagnet,{0.7f, 0.3f}},
                {EnemyBubbleShield, {0.5f, 0.5f}},
                {EnemyBubbleShield,{0.3f, 0.7f}},
                {EnemyBubbleShield,{0.7f, 0.3f}},
        },
        {
            {EnemyScissors, {0.5f, 0.2f}},
            {EnemyScissors,{0.2f, 0.8f}},
            {EnemyScissors,{0.8f, 0.2f}},
                {EnemyBubbleShield,{0.8f, 0.2f}},
        },
            {
                {EnemySword, {0.5f, 0.5f}},
                {EnemySword,{0.2f, 0.8f}},
                {EnemySword,{0.8f, 0.2f}},
                {EnemyBubbleShield, {0.5f, 0.2f}},
                {EnemyBubbleShield,{0.2f, 0.8f}},
                {EnemyBubbleShield,{0.8f, 0.2f}},
            },
        {
            {EnemyMedicalRodA, {0.5f, 0.5f}},
                {EnemyBubbleShield, {0.5f, 0.5f}},
            {EnemyMedicalRodA,{random_float, random_float}},
            {EnemyMedicalRodA,{random_float, random_float}}
        },
    },
    {},
    {},
    10.0f,
    10,
    5,
    "Supply Closet",
    false,
    {1500, 1500}
};

const RoomPreset MedicalEnemyRoomHeartBeat{

    {

        {
                {EnemyMedicalRodA, {0.5f, 0.5f}},
                    {EnemyMedicalRodA, {random_float, random_float}},
            {EnemyMedicalBMP , {0.75f, 0.75f}},
                    {EnemyMedicalBMP , {0.25f, 0.25f}},

        },
        {
                    {EnemyScissors,{random_float, random_float}},
                    {EnemyScissors,{random_float, random_float}},
                        {EnemyScissors,{random_float, random_float}},
                        {EnemyMedicalRodA, {0.5f, 0.5f}},
        }

    },
    {},
    {},
    12.0f,
    10,
    5,
    "Atrium",
    false,
    {2500, 2500}


};

const RoomPreset MedicalEnemyRoomHeartBeat2 {

    {

        {
                {EnemyMedicalRodA, {0.5f, 0.5f}},
                    {EnemyMedicalRodA, {random_float, random_float}},
            {EnemyMedicalBMP , {0.75f, 0.75f}},
                    {EnemyMedicalBMP , {0.25f, 0.25f}},

        },
        {
                    {EnemyScissors,{random_float, random_float}},
                    {EnemyScissors,{random_float, random_float}},
                        {EnemyScissors,{random_float, random_float}},
                        {EnemyMedicalRodA, {0.5f, 0.5f}},
        },

        {
                {EnemyMedicalRodC, {0.5f, 0.5f}},
        },

    },
    {},
    {},
    12.0f,
    10,
    5,
    "Know Your Enemy",
    false,
    {2500, 2500}
};

const RoomPreset MedicalEnemyRoomHeartBeat3 {

    {

        {
//             {EnemyMedicalSyringe, {0.2f, 0.2f}},
//                 {EnemyMedicalSyringe, {0.8f, 0.8f}},
// {EnemyMedicalSyringe, {0.6f, 0.8f}},
//             { EnemyMedicalSyringe, {0.4f, 0.2f}},
//             { EnemyMedicalSyringe, {0.2f, 0.8f}},
//             { EnemyMedicalSyringe, {0.8f, 0.2f}},
//             { EnemyMedicalSyringe, {0.6f, 0.2f}},
//             { EnemyMedicalSyringe, {0.4f, 0.8f}},
            {EnemyMedicalBoid, {random_float, random_float}},
                {EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},

{EnemyMedicalBoid, {random_float, random_float}},
            {EnemyMedicalPillBoidSpawner, {0.2f, 0.5f}},
                { EnemyMedicalPillBoidSpawner, {0.8f, 0.5f}},




            {EnemyMedicalBMP , {0.75f, 0.75f}},
                    {EnemyMedicalBMP , {0.25f, 0.25f}},

        },
        {
                            {EnemyMedicalBoid, {random_float, random_float}},
                    {EnemyMedicalBoid, {random_float, random_float}},
    {EnemyMedicalBoid, {random_float, random_float}},
    {EnemyMedicalBoid, {random_float, random_float}},
    {EnemyMedicalBoid, {random_float, random_float}},
    {EnemyMedicalBoid, {random_float, random_float}},
    {EnemyMedicalBoid, {random_float, random_float}},
    {EnemyMedicalBoid, {random_float, random_float}},
    {EnemyMedicalBoid, {random_float, random_float}},
    {EnemyMedicalBoid, {random_float, random_float}},
    {EnemyMedicalBoid, {random_float, random_float}},
    {EnemyMedicalBoid, {random_float, random_float}},
    {EnemyMedicalBoid, {random_float, random_float}},

    {EnemyMedicalBoid, {random_float, random_float}},
                        {EnemyMedicalBMP , {0.25f, 0.75f}},
                        {EnemyMedicalBMP , {0.75f, 0.25f}},
        },

        {
                            { EnemyMedicalSyringe, {0.2f, 0.8f}},
                { EnemyMedicalSyringe, {0.8f, 0.2f}},
                { EnemyMedicalSyringe, {0.6f, 0.2f}},
                { EnemyMedicalSyringe, {0.4f, 0.8f}},
                {EnemyQuadshooterElite, {0.5f, 0.5f}},
        },

        // {
        //         {EnemyMedicalRodC, {0.5f, 0.5f}},
        // },

    },
    {},
{{ {PushConsole,{bulletBounceUp, bulletBounceUp, bulletRangeUp, bulletRangeUp}}, {0.3f, 0.5f}}, { {PushConsole,{bulletPierceUp, bulletPierceUp, bulletPierceUp}}, {0.7f, 0.5f}}, { {PopConsole,{}}, {0.5f, 0.5f}}},
    16.0f,
    20,
    5,
    "Fatal Dose",
    false,
    {2000, 2000}
};

const RoomPreset MedicalEnemyRoomHeartBeat4 {

{

        {
//             {EnemyMedicalSyringe, {0.2f, 0.2f}},
//                 {EnemyMedicalSyringe, {0.8f, 0.8f}},
// {EnemyMedicalSyringe, {0.6f, 0.8f}},
//             { EnemyMedicalSyringe, {0.4f, 0.2f}},
//             { EnemyMedicalSyringe, {0.2f, 0.8f}},
//             { EnemyMedicalSyringe, {0.8f, 0.2f}},
//             { EnemyMedicalSyringe, {0.6f, 0.2f}},
//             { EnemyMedicalSyringe, {0.4f, 0.8f}},
            {EnemyMedicalBoid, {random_float, random_float}},
                {EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},

{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
    {EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},

{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},

{EnemyMedicalBoid, {random_float, random_float}},
               {EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},

{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
    {EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},

{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},

{EnemyMedicalBoid, {random_float, random_float}},


            // {EnemyMedicalBMP , {0.75f, 0.75f}},
            //         {EnemyMedicalBMP , {0.25f, 0.25f}},
{EnemyMedicalBMP , {0.25f, 0.75f}},
{EnemyMedicalBMP , {0.75f, 0.25f}},

        },
        {
                            {EnemyMedicalBoid, {random_float, random_float}},
                    {EnemyMedicalBoid, {random_float, random_float}},
    {EnemyMedicalBoid, {random_float, random_float}},
    {EnemyMedicalBoid, {random_float, random_float}},
    {EnemyMedicalBoid, {random_float, random_float}},
    {EnemyMedicalBoid, {random_float, random_float}},
    {EnemyMedicalBoid, {random_float, random_float}},
    {EnemyMedicalBoid, {random_float, random_float}},
    {EnemyMedicalBoid, {random_float, random_float}},
    {EnemyMedicalBoid, {random_float, random_float}},
    {EnemyMedicalBoid, {random_float, random_float}},
    {EnemyMedicalBoid, {random_float, random_float}},
    {EnemyMedicalBoid, {random_float, random_float}},

    {EnemyMedicalBoid, {random_float, random_float}},
                           {EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},

{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
    {EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},

{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},

{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBMP , {0.75f, 0.75f}},
{EnemyMedicalBMP , {0.25f, 0.25f}},
// {EnemyMedicalBMP , {0.25f, 0.75f}},
// {EnemyMedicalBMP , {0.75f, 0.25f}},

        },

        {
                //             { EnemyMedicalSyringe, {0.2f, 0.8f}},
                // { EnemyMedicalSyringe, {0.8f, 0.2f}},
                // { EnemyMedicalSyringe, {0.6f, 0.2f}},
                // { EnemyMedicalSyringe, {0.4f, 0.8f}},
                // {EnemyQuadshooterElite, {0.5f, 0.5f}},
                {EnemyMedicalBMP , {0.5f, 0.5f}},
                {EnemyProstheticHand , {0.75f, 0.75f}},
                        {EnemyProstheticHand , {0.25f, 0.25f}},
    {EnemyProstheticHand , {0.25f, 0.75f}},
    {EnemyProstheticHand , {0.75f, 0.25f}},

        },

        // {
        //         {EnemyMedicalRodC, {0.5f, 0.5f}},
        // },

    },
    {},
    {},
    16.0f,
    20,
    5,
    "Arrhythmia",
    false,
    {1600, 1600}
};


const RoomPreset MedicalEnemyRoomSmallRodC {
    {
        {
            {EnemyMedicalRodC, {0.5f, 0.5f}},
            {ProtectedGranter, {0.5f, 0.5f}},


        },

    },
    {},
    {},
    10.0f,
    25,
    5,
    "Forgotten Halo",
    false,
    {1200, 1200}
};

const RoomPreset MedicalEnemyRoomHand {
    {
        {
            {EnemyProstheticHand, {0.4f, 0.5f}},
            {EnemyProstheticHand, {0.4f, 0.4f}},
            {EnemyProstheticHand, {0.4f, 0.6f}},
            {EnemySmallBoulder, {0.6f, 0.55f}},
// {EnemySmallCShield, {0.6f, 0.55f}},
        },
        {
                {EnemyProstheticHand, {0.4f, 0.5f}},
                {EnemyProstheticHand, {0.4f, 0.4f}},
                {EnemyProstheticHand, {0.4f, 0.6f}},
                {EnemySmallBoulder, {0.6f, 0.55f}},
                // {EnemySmallBoulder, {0.6f, 0.45f}},
                    {EnemySmallBoulder, {0.6f, 0.5f}},
// {EnemySmallBoulder, {0.6f, 0.6f}},
                {EnemySmallBoulder, {0.6f, 0.4f}},
        },

    },
    {},
    {},
    16.0f,
    25,
    5,
    "Bloody Dodgeball",
    false,
    {1200, 1200}
};

const RoomPreset EnemyRoomSingleMage {
    {
        {
            {EnemyMage, {0.5f, 0.5f}},
    {EnemyBubbleShield, {0.5f, 0.5f}},
        },

    },
    {},
    {},
    10.0f,
    25,
    5,
    "ArchMage's Challenge",
    false,
    {1600, 1600}
};

const RoomPreset MedicalEnemyRoomSpineWormQuadShooters {

    {
        {
            {EnemySpinePatrolWormHead, {0.1f, 0.9f}},
{EnemySpinePatrolWormHead, {0.9f, 0.1f}},
            {EnemyQuadshooter, {0.2f, 0.8f}},
{EnemyQuadshooter, {0.2f, 0.2f}},
{EnemyQuadshooter, {0.8f, 0.2f}},
{EnemyQuadshooter, {0.8f, 0.8f}},
        },

        {
                {EnemyQuadshooterElite, {0.1f, 0.9f}},

            {EnemySpinePatrolWormHead, {0.1f, 0.9f}},
            {EnemyQuadshooterElite, {0.9f, 0.1f}},
        },
        {

            {EnemySpinePatrolWormHead, {0.9f, 0.1f}},

{EnemyQuadshooterElite, {0.9f, 0.9f}},

{EnemyQuadshooterElite, {0.1f, 0.1f}},
        },

        },
        {},
        {},
        20.0f,
        25,
        5,
        "Skeletal Constrictor",
        false,
        {1600, 1600}
};

const RoomPreset MedicalEnemyRoomSyringes {

        {
            {
                {EnemyMedicalSyringe, {0.2f, 0.2f}},
                {EnemyMedicalSyringe, {0.8f, 0.8f}},
                {EnemyMedicalBoid, {random_float, random_float}},
                    {EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
    {EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},

{EnemyMedicalBoid, {random_float, random_float}},
            },
                {
                        {EnemyMedicalSyringe, {0.2f, 0.8f}},
                    {EnemyMedicalSyringe, {0.8f, 0.2f}},
                        {EnemyMedicalBoid, {random_float, random_float}},
                        {EnemyMedicalBoid, {random_float, random_float}},
    {EnemyMedicalBoid, {random_float, random_float}},
    {EnemyMedicalBoid, {random_float, random_float}},
    {EnemyMedicalBoid, {random_float, random_float}},
    {EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
                    {EnemyMedicalBoid, {random_float, random_float}},


                },

            {

                {EnemyMedicalSyringe, {0.5f, 0.5f}},
                {EnemyScissors, {0.7f, 0.3f}},
                {EnemyScissors, {0.3f, 0.7f}},
// {EnemyScissors, {0.7f, 0.7f}},
// {EnemyScissors, {0.3f, 0.3f}},


{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},
{EnemyMedicalBoid, {random_float, random_float}},



                }

            },
            {},
            {},
            20.0f,
            25,
            5,
            "Surgical Swarm",
            false,
            {1600, 1600}
};

const RoomPreset MedicalRoomQuadShooter {
    {
        {
            {EnemyType::EnemyQuadshooterElite, {0.2f, 0.2f}},
           {EnemyType::EnemyQuadshooterElite, {0.4f, 0.2f}},
           {EnemyType::EnemyQuadshooterElite, {0.6f, 0.2f}},
           {EnemyType::EnemyQuadshooterElite, {0.8f, 0.2f}},
           {EnemyType::EnemyQuadshooterElite, {0.2f, 0.8f}},
           {EnemyType::EnemyQuadshooterElite, {0.4f, 0.8f}},
           {EnemyType::EnemyQuadshooterElite, {0.6f, 0.8f}},
           {EnemyType::EnemyQuadshooterElite, {0.8f, 0.8f}}
        },

            {
                // {EnemyType::EnemyQuadshooterElite, {0.2f, 0.2f}},
            {EnemyType::EnemyQuadshooterElite, {0.2f, 0.4f}},
            {EnemyType::EnemyQuadshooterElite, {0.2f, 0.6f}},
            // {EnemyType::EnemyQuadshooterElite, {0.2f, 0.8f}},
            // {EnemyType::EnemyQuadshooterElite, {0.8f, 0.2f}},
            {EnemyType::EnemyQuadshooterElite, {0.8f, 0.4f}},
            {EnemyType::EnemyQuadshooterElite, {0.8f, 0.6f}},
            // {EnemyType::EnemyQuadshooterElite, {0.8f, 0.8f}}
            },
        {
            {EnemyQuadshooterElite, {0.5f, 0.5f}},
            {EnemySmallCShield, {0.5f, 0.5f}},

        },

    },
    {},
    {{ {PushConsole,{bulletBounceUp, bulletBounceUp, bulletRangeUp, bulletRangeUp}}, {0.3f, 0.5f}}, { {PushConsole,{bulletPierceUp, bulletPierceUp, bulletPierceUp}}, {0.7f, 0.5f}}, { {PopConsole,{}}, {0.5f, 0.5f}}},
    20.0f,
    25,
    5,
    "Critical Mass",
    false,
    {3600, 1800}
};



const RoomPreset MedicalEnemyRoomWorms {
    {
        {
            {
                {EnemySpinePatrolWormHead, {0.1f, 0.9f}},
    {EnemySpinePatrolWormHead, {0.9f, 0.1f}},
                {EnemySpineChainedWormHead, {0.25f, 0.75f}},
    {EnemySpineChainedWormHead, {0.25f, 0.25f}},
    {EnemySpineChainedWormHead, {0.75f, 0.25f}},
    {EnemySpineChainedWormHead, {0.75f, 0.75f}},
            },

            {
                    {EnemyMedicalBMP, {0.5f, 0.5f}},

                {EnemySpinePatrolWormHead, {0.1f, 0.9f}},
                // {EnemyQuadshooterElite, {0.9f, 0.1f}},
            },
            {

                {EnemySpinePatrolWormHead, {0.9f, 0.1f}},

    // {EnemySpineChainedWormHead, {0.9f, 0.9f}},
    //
    // {EnemySpineChainedWormHead, {0.1f, 0.1f}},
            },

            },

    },
{},
    {{ {PushConsole,{bulletBounceUp, bulletBounceUp, bulletRangeUp, bulletRangeUp}}, {0.3f, 0.5f}}, { {PushConsole,{bulletPierceUp, bulletPierceUp, bulletPierceUp}}, {0.7f, 0.5f}}, { {PopConsole,{}}, {0.5f, 0.5f}}},
    20.0f,
    25,
    5,
    "Critical Mass",
    false,
    {1500, 1500}
};



const RoomPreset MiningEnemyRoomMineField {
        {
                {
                {EnemyBigBoulder, {0.1,0.9}},
                {EnemyBigBoulder, {0.9,0.9}},
                {EnemyDynamite, {random_float, random_float}},
                {EnemyDynamite, {random_float, random_float}},

                {EnemyDynamite, {random_float, random_float}},
                {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                },
                {
                {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                {EnemySmallMole, {random_float, random_float}},
                {EnemySmallMole, {random_float, random_float}},
                },

                {
                    {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                    {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                    {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                    {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                    {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                    { EnemySmallBoulder, {random_float, random_float}},
                    { EnemySmallBoulder, {random_float, random_float}},
                        { EnemySmallBoulder, {random_float, random_float}},
                                        { EnemySmallBoulder, {random_float, random_float}},
                        { EnemySmallBoulder, {random_float, random_float}},
                                        { EnemySmallBoulder, {random_float, random_float}},
                        {EnemyBigBoulder, {0.9,0.1}},
                        {EnemyBigBoulder, {0.1,0.1}},
                    },
{
                        {EnemyDynamite, {random_float, random_float}},
                        {EnemyDynamite, {random_float, random_float}},

                {EnemyDynamite, {random_float, random_float}},
                    {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                    {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                    {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                    {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                    {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
                    },
        },
    {},
        {{ {PushConsole,{bulletRangeUp, bulletRangeUp, bulletRangeUp, bulletBounceUp, bulletBounceUp, bulletBounceUp}}, {0.3f, 0.5f}}, { {PushConsole,{bulletPierceUp, bulletPierceUp, bulletPierceUp}}, {0.7f, 0.5f}}, { {PopConsole,{}}, {0.5f, 0.5f}}},
        10.0f,
        25,
        5,
        "Minefield Disco",
        false,
        {2100, 1600}
};

const RoomPreset MiningEnemyRoomBoulders {
        {

                {
                    {EnemyBigBoulder, {0.1, 0.1}},
                    {EnemyBigBoulder, {0.1, 0.3}},
                    {EnemyBigBoulder, {0.1, 0.5}},
                    {EnemyBigBoulder, {0.1, 0.7}},
                    {EnemyBigBoulder, {0.1, 0.9}},
                    {EnemyBigBoulder, {0.9, 0.2}},
                    {EnemyBigBoulder, {0.9, 0.4}},
                    {EnemyBigBoulder, {0.9, 0.6}},
                    {EnemyBigBoulder, {0.9, 0.8}},

                },
                {
                    { EnemySmallBoulder, {random_float, random_float}},
                        { EnemySmallBoulder, {random_float, random_float}},
                            { EnemySmallBoulder, {random_float, random_float}},
                                            { EnemySmallBoulder, {random_float, random_float}},
                            { EnemySmallBoulder, {random_float, random_float}},
                                            { EnemySmallBoulder, {random_float, random_float}},
                },
            {
                { EnemyBigBoulder, {0.2, 0.1}},
                {EnemyBigBoulder, {0.4, 0.1}},
                {EnemyBigBoulder, {0.6, 0.1}},
                {EnemyBigBoulder, {0.8, 0.1}},
            },
            {
                { EnemySmallBoulder, {random_float, random_float}},
            { EnemySmallBoulder, {random_float, random_float}},
                { EnemySmallBoulder, {random_float, random_float}},
                    { EnemySmallBoulder, {random_float, random_float}},
                { EnemySmallBoulder, {random_float, random_float}},
                        { EnemySmallBoulder, {random_float, random_float}},
                    },
            {
                { EnemyBigBoulder, {0.1, 0.9}},
                {EnemyBigBoulder, {0.3, 0.9}},
                    { EnemyBigBoulder, {0.5, 0.9}},
                { EnemyBigBoulder, {0.7, 0.9}},
                { EnemyBigBoulder, {0.9, 0.9}},
            },

        },
    {},
                    {},
                12.5f,
        25,
        5,
        "Rolling in the Deep",
        false,
        {1500, 1500}
};

const RoomPreset MiningEnemyRoomQuadShooterBoulders {
                {
                    {
                        {EnemyQuadshooterElite,{0.5,0.5}},
                        {EnemySmallBoulder, random_vec2},
                            {EnemySmallBoulder, random_vec2},
                            {EnemySmallBoulder, random_vec2},
                    },
                    {
                            {EnemySmallBoulder, random_vec2},
                        {EnemySmallBoulder, random_vec2},
                    },
                    {
                        {EnemyBigBoulder, random_vec2}
                    },


                },
            {},
            {},
            5.0f,
            5,
            2,
            "Deliberate with Care",
            false,
            {1600, 1600}
};


const RoomPreset MiningEnemyRoomMolesIntro {
        {
            {
                {EnemySmallMole, random_vec2},
                    {EnemySmallMole, random_vec2},
                    {EnemySmallMole, random_vec2},
            },
            {
                    {EnemySmallBoulder, random_vec2},
{EnemySmallBoulder, random_vec2},
            },
{
                        {EnemySmallBoulder, random_vec2},
    {EnemySmallBoulder, random_vec2},
                },
{
                        {EnemySmallBoulder, random_vec2},
    {EnemySmallBoulder, random_vec2},
                },
{
                        {EnemySmallBoulder, random_vec2},
    {EnemySmallBoulder, random_vec2},
                },
{
                        {EnemySmallBoulder, random_vec2},
    {EnemySmallBoulder, random_vec2},
                },
{
                        {EnemySmallBoulder, random_vec2},
    {EnemySmallBoulder, random_vec2},
                },
        },
                {},
                {},
            6.5f,
    10,
    5,
    "Mole's Welcome",
    false,
    {1500, 1500}
};

const RoomPreset MiningEnemyRoomMoles {
    {
        {
            {EnemySmallMole, {0.4, random_float}},
    {EnemySmallMole, {0.4, random_float}},
    {EnemySmallMole, {0.4, random_float}},
        { EnemySmallBoulder, {0.6, random_float}},
        { EnemySmallBoulder, {0.6, random_float}},
        { EnemySmallBoulder, {0.6, random_float}},
        { EnemySmallBoulder, {0.6, random_float}},

        },

        {
            {EnemySurfaceMole, {0.4, random_float}},
            {EnemySurfaceMole, {0.4, random_float}},
            { EnemyBigBoulder, {0.6, random_float}},
            { EnemyBigBoulder, {0.6, random_float}},
            { EnemyBigBoulder, {0.6, random_float}},

        },
    },
            {},
            {},
        18.5f,
10,
5,
"Mole Party",
false,
{1500, 1500}
};

const RoomPreset MiningEnemyRoomMoles2 {
        {
            {
                {EnemySurfaceMole, {0.5f, 0.5f}},
                {EnemyMagnet, {0.6f, 0.4f}},
                    {EnemyBubbleShield, {0.6f, 0.4f}},
                        {EnemyMagnet, {0.4f, 0.6f}},
                        {EnemyBubbleShield, {0.4f, 0.6f}},
            },
        },
                {},
                {},
            12.5f,
    10,
    5,
    "Homeward Bound",
    false,
    {2000, 1500}
};

const RoomPreset MiningEnemyRoomCloakedMoles {
        {
            {
                {EnemySmallMole, {0.4, 0.4}},
        {EnemySmallMole, {0.4, 0.5}},
        {EnemySmallMole, {0.4, 0.6}},
            { EnemySmallBoulder, {0.6, 0.3}},
            { EnemySmallBoulder, {0.6, 0.5}},
            { EnemySmallBoulder, {0.6, 0.7}},
            { EnemySmallBoulder, {0.6, 0.9}},
                // {CloakedGranter, {0.4, 0.4}},
                // {CloakedGranter, {0.4, 0.5}},
                // {CloakedGranter, {0.4, 0.6}},
                // { CloakedGranter, {0.6, 0.3}},
                // { CloakedGranter, {0.6, 0.5}},
                // { CloakedGranter, {0.6, 0.7}},
                // { CloakedGranter, {0.6, 0.9}},
                { CloakedGranterRoomWide, {0.6, 0.5}},

            },

            {
                {EnemySurfaceMole, {0.4, 0.4}},
                {EnemySurfaceMole, {0.4, 0.6}},
                { EnemyBigBoulder, {0.6, 0.3}},
                { EnemyBigBoulder, {0.6, 0.5}},
                { EnemyBigBoulder, {0.6, 0.7}},
                        // {CloakedGranter, {0.4, 0.4}},
                        // {CloakedGranter, {0.4, 0.6}},
                        // { CloakedGranter, {0.6, 0.3}},
                        // { CloakedGranter, {0.6, 0.5}},
                        // { CloakedGranter, {0.6, 0.7}},

            },
        },
                {},
                {

            // {{PushConsole,{numBulletsUp}}, {0.5f, 0.5f}},
        {{PopConsole}, {0.25f, 0.5f}},
        {{PushConsole,{dashUp, dashUp,dashUp,sizeUp,sizeUp}}, {0.75f, 0.5f}},


                },
            18.5f,
    10,
    5,
    "Shrouded in Darkness",
    false,
    {2100, 1700}
};

const RoomPreset MiningEnemyRoomIntro {
            {
                {
                    {EnemyPileDriverTurret, {0.5, 0.5}},
                        {EnemyBulldozer, random_vec2},
                            {EnemyBulldozer, random_vec2},
                            {EnemyBulldozer, random_vec2},
                        {EnemyMagnet, random_vec2},
                        {EnemyMagnet, random_vec2},
                },

            },
                    {},
                    {

                    },
                7.5f,
        10,
        5,
        "Construction Site",
        false,
        {1200, 1000}
};

const RoomPreset MiningEnemyRoomLane {
                {
                    {
                        {EnemyPileDriverTurret,{0.2,0.9}},
                    {EnemyPileDriverTurret,{0.8,0.9}},
                    {EnemyPileDriverTurret,{0.5,0.9}},
                            {EnemyBulldozer, random_vec2},
                                {EnemyBulldozer, random_vec2},

                    },

                    {
                        {EnemyPileDriverTurret,{0.2,0.1}},
                        {EnemyPileDriverTurret,{0.8,0.1}},
                        {EnemyPileDriverTurret,{0.5,0.1}},
                            {EnemyBubbleShield,{0.5,0.1}},
                        {EnemySmallMole, random_vec2},
                        {EnemySmallMole, random_vec2},

                    },

                },
                        {},
                        {},
                    20.f,
            10,
            5,
            "Down the Mineshaft",
            false,
{650, 3000}
};

const RoomPreset MiningEnemyRoomAvenue {
                {
                    {
                        {EnemyPileDriverTurret,{0.9,0.2}},
                        {EnemyPileDriverTurret,{0.9,0.8}},
                        {EnemyPileDriverTurret,{0.9,0.5}},
                                {EnemyBigBoulder, random_vec2},
                    },

                    {
                        {EnemyPileDriverTurret,{0.1,0.2}},
                        {EnemyPileDriverTurret,{0.1,0.8}},
                        {EnemyPileDriverTurret,{0.1,0.5}},
                            {EnemyBubbleShield,{0.1,0.5}},
                            { EnemySmallBoulder, {0.5, random_float}},
                            { EnemySmallBoulder, {0.5, random_float}},
                            { EnemySmallBoulder, {0.5, random_float}},
                                { EnemySmallBoulder, {0.5, random_float}},
                                    { EnemySmallBoulder, {0.5, random_float}},

                    },

                },
            {},
            {},
            20.0f,
            10,
            5,
            "Boulder Avenue",
            false,
            {3000, 650}
};

const RoomPreset MiningEnemyRoomPileDrivers {
                    {
                        {
                            {EnemyPileDriverTurret,{0.8,0.2}},
                            {EnemyPileDriverTurret,{0.8,0.8}},
                            {EnemyPileDriverTurret,{0.2,0.8}},
                                {EnemyPileDriverTurret,{0.2,0.2}},
                                    {EnemyPufferfish, random_vec2},
                                    {EnemyPufferfish, random_vec2},
                        },

                        {

                            {EnemyPileDriverTurret,{0.5,0.5}},
                            {EnemyBubbleShield,{0.5,0.5}},
                                { EnemyBulldozer, random_vec2},
                                { EnemyBulldozer, random_vec2},
                                {EnemySmallBoulder, random_vec2},
                                {EnemyBigBoulder, random_vec2},
                                        {EnemyPufferfish, random_vec2},
                            {EnemyPufferfish, random_vec2},
                                        {EnemyPufferfish, random_vec2},

                        },

                    },
                {},
                {},
                16.0f,
                5,
                2,
                "Sump Draining",
                false,
                {1500, 1500}
};

const RoomPreset MiningEnemyRoomPileDriversIntro {
                        {
                            {
                                {EnemyPileDriverTurret,{0.8,0.2}},
                                {EnemyPileDriverTurret,{0.8,0.8}},
                                {EnemyPileDriverTurret,{0.2,0.8}},
                                    {EnemyPileDriverTurret,{0.2,0.2}},
                                    {EnemyBigBoulder, random_vec2},
                                    {EnemySkull, {0.5, 0.5}},
                                { EnemyBubbleShield, {0.5, 0.5}},
                            },


                        },
                    {},
                    {},
                    16.0f,
                    5,
                    2,
                    "Spelunking",
                    false,
                    {1600, 1600}
};

const RoomPreset MiningEnemyRoomBulldozers {
                    {
                        {
                            {EnemyBulldozer,{0.2,0.9}},
                            {EnemyBulldozer,{0.8,0.9}},
                            {EnemyBulldozer,{0.5,0.9}},
                                {EnemyBigBoulder,{random_float, 0.5} },
                                {EnemySmallBoulder,{random_float, 0.5} },
                                {EnemySmallBoulder,{random_float, 0.5} },
                        },

                        {
                            {EnemyBulldozer,{0.2,0.1}},
                            {EnemyBulldozer,{0.8,0.1}},
                            {EnemyBulldozer,{0.5,0.1}},
                                    {EnemyBigBoulder,{random_float, 0.5f} },
                                    {EnemyBigBoulder,{random_float, 0.5f} },
                                    {EnemyBigBoulder,{random_float, 0.5f} },
                        },

                    },
                {},
                {},
                15.f,
            10,
            5,
            "Bulldozing Competition",
            false,
            {2100, 1500}
};

const RoomPreset allConsoles {
    { {
        {EnemyMedicalBMP, {random_float, random_float}},
// {EnemyScissors, {random_float, random_float}},
{EnemyScissors, {random_float, random_float}},
{EnemyMedicalRodC, {random_float, random_float}},
// {EnemyMedicalPillBoid, {random_float, random_float}},
// {EnemyMedicalPillBoid, {random_float, random_float}},
// {EnemyMedicalPillBoid, {random_float, random_float}},
// {EnemyMedicalPillBoid, {random_float, random_float}},
{EnemyMedicalPillBoidSpawner, {random_float, random_float}},

    }},
    {},
    {

        {{PushConsole,{numBulletsUp}}, {0.5f, 0.5f}},
    {{PopConsole,{fireRateDown, bulletBounceDown}}, {0.25f, 0.5f}},
    {{WishGranter,{fireRateUp, fireRateUp}}, {0.75f, 0.5f}},
},
    0.0f,
    5,
    2,
    "AllConsoles",
    false,
    {2500, 2000}
};

const RoomPreset EnemyRoomPhantom {
        {
            {
            {EnemyPhantom, {0.5, 0.5}},
                {EnemyMage, random_vec2},
                    {EnemyMage, random_vec2}
                },
                    {
                {EnemyMage, {0.5, 0.5}},
                    {EnemyPhantom, random_vec2},
                        {EnemyPhantom, random_vec2}
                    },

        },
        {},
        {{{PushConsole,{numBulletsUp}}, {0.5f, 0.5f}},
        {{PopConsole,{fireRateDown, bulletBounceDown}}, {0.25f, 0.5f}},
        {{WishGranter,{fireRateUp, bulletRangeUp}}, {0.75f, 0.5f}},
    },
        65.0f,
        35,
        12,
        "The Phantom",
        false,
        {2500, 2000}
};

const RoomPreset EnemyRoomEyes {
            {
                {
                    {EnemyEyeCube, {0.5f, 0.5f}},
                        {EnemyQuadshooterElite, {0.2f, 0.8f}},
                            {EnemyQuadshooterElite, {0.8f, 0.2f}},
                            {EnemyQuadshooter, {0.8f, 0.8f}},
                                { EnemyQuadshooter, {0.2f, 0.2f}},
                },
                    {
                        {EnemyEyeCube, random_vec2},
                            {EnemyQuadshooterElite, {0.25f, 0.25f}},
                                {EnemyQuadshooterElite, {0.75f, 0.75f}},
                                {EnemyQuadshooter, {0.25f, 0.75f}},
                                    { EnemyQuadshooter, {0.75f, 0.25f}},
                    },
                {
                        {EnemyEyeCube, random_vec2},
                                {EnemyEyeCube, random_vec2},
                },

        },
        {},
        {{{PushConsole,{numBulletsUp}}, {0.5f, 0.5f}},
        {{PopConsole,{fireRateDown, bulletBounceDown}}, {0.25f, 0.5f}},
        {{WishGranter,{fireRateUp, bulletRangeUp}}, {0.75f, 0.5f}},
    },
        25.0f,
        35,
        12,
        "Penetrating Gaze",
        false,
        {2500, 2000}
};


const std::vector<RoomPreset> bossRooms = {
    BossRoomBee, BossRoomCrab, BossRoomMole, BossRoomWorm, BossRoomBigC, BossRoomMultiCube
};

const std::vector<RoomPreset> regularTreasureRooms = {
    TreasureRoom1, TreasureRoom2, TreasureRoom3, TreasureRoom4, TreasureRoom5, TreasureRoom6, TreasureRoom7,
    TreasureRoom8, TreasureRoom9, TreasureRoomKey, TreasureRoomKeys, TreasureRoomBlunt
};

const std::vector<RoomPreset> choiceTreasureRooms = {
    TreasureRoomChoice1, TreasureRoomChoice2, TreasureRoomChoice3, TreasureRoomChoice4, TreasureRoomChoice5, TreasureRoomChoice6, TreasureRoomChoice7, TreasureRoomChoice8,
};

const std::vector<RoomPreset> bothTreasureRooms = {
    TreasureRoom1, TreasureRoom2, TreasureRoom3, TreasureRoom4, TreasureRoom5, TreasureRoom6, TreasureRoom7,
    TreasureRoom8, TreasureRoom9, TreasureRoomKey, TreasureRoomKeys, TreasureRoomBlunt,
    TreasureRoomChoice1, TreasureRoomChoice2, TreasureRoomChoice3, TreasureRoomChoice4, TreasureRoomChoice5, TreasureRoomChoice6, TreasureRoomChoice7, TreasureRoomChoice8,};

std::map<DifficultyRegion,std::map<RoomType, RoomPresets>> bioRoomDirectory = {
    {DifficultyRegion::Intro,{
        {RoomType::EnemyRoom, {{EnemyRoomDashIntro1,EnemyRoomDashIntro2,EnemyRoomDashIntro3,BioRoomSmall },{}}},
        {RoomType::RestRoom, {{RestingRoomPop, RestingRoomPopLarge, RestingRoomPopLong, RestingRoomPopTall, RestingRoomPopSmall},{RestingRoom3PopLarge},}},
        {RoomType::EventRoom, {{EventRoomGardener, EventRoomHoney, EventRoomOracleCrab}, {EventRoomMouse}}  },
        {
            RoomType::TreasureRoom,
            {
                regularTreasureRooms,
                {
                    TreasureRoomSniper, TreasureRoomShotgun, TreasureRoomOstrich, TreasureRoomMachineGun,
                    TreasureRoomSuperSize, TreasureRoomPinBall, TreasureRoomHare
                }
            }
        },
    }},
    {DifficultyRegion::Easy,{
        {RoomType::EnemyRoom, {{EnemyRoomBees1, EnemyRoomDash1,EnemyRoomCrabs,BioRoomSmall, EnemyRoomBees2, EnemyRoomBees3, EnemyRoomDash2, EnemyRoomSnails},{EnemyRoomInvisible,BioEnemyRoomSwarm}}},
        {RoomType::RestRoom, {{RestingRoomPop, RestingRoomPopLarge, RestingRoomPopLong, RestingRoomPopTall, RestingRoomPopSmall},{RestingRoomPop}}},
        {RoomType::EventRoom, {{EventRoomGardener, EventRoomHoney, EventRoomOracleCrab}, {EventRoomMouse}}  },
        {
            RoomType::TreasureRoom,
            {
                regularTreasureRooms,
                {
                    TreasureRoomSniper, TreasureRoomShotgun, TreasureRoomOstrich, TreasureRoomMachineGun,
                    TreasureRoomSuperSize, TreasureRoomPinBall, TreasureRoomHare
                }
            }
        },
    }},
    {DifficultyRegion::Medium,{
        {RoomType::EnemyRoom, {{EnemyRoomAngelTank,EnemyRoomCrabs, EnemyRoomDashHard, EnemyRoomBees3, EnemyRoomDash2, EnemyRoomBees2},{EnemyRoomInvisible}}},
        {RoomType::RestRoom, {{RestingRoomPop, RestingRoomPopLarge, RestingRoomPopLong, RestingRoomPopTall, RestingRoomPopSmall},{RestingRoom3PopLarge}}},
        {RoomType::EventRoom, {{EventRoomGardener, EventRoomHoney, EventRoomOracleCrab}, {EventRoomMouse}}  },
        {
            RoomType::TreasureRoom,
            {
                regularTreasureRooms,
                {
                    TreasureRoomSniper, TreasureRoomShotgun, TreasureRoomOstrich, TreasureRoomMachineGun,
                    TreasureRoomSuperSize, TreasureRoomPinBall, TreasureRoomHare
                }
            }
        },
    }},
};

std::map<DifficultyRegion,std::map<RoomType, RoomPresets>> physicsRoomDirectory = {
    {DifficultyRegion::Intro,{
        // {RoomType::EnemyRoom, {{HifiRoomSmall},{}}},
        {
            RoomType::EnemyRoom,
            {
                {
                    HifiRoomSniperBallLauncher, HifiRoomBasicEnemy, HifiRoomBoidSnipers, HifiRoomTwinLaserChargers,
                    HifiRoomTwinLaserShurikens, HifiRoomBasicWave, HifiRoomAvenue, HifiRoomLane, EnemyRoomSingleMage
                },
                {HifiEnemyRoomSwarmLasers, HifiRoomShieldedMadness}
            }
        },
        {RoomType::RestRoom, {{RestingRoomPop, RestingRoomPopLarge, RestingRoomPopLong, RestingRoomPopTall, RestingRoomPopSmall},{RestingRoom3PopLarge}}},
        {RoomType::EventRoom, {{EventRoomWish, EventRoomOracleCrab, EventRoomSwarm}, {EventRoomBaru, EventRoomWish }}  },
        {RoomType::TreasureRoom, {bothTreasureRooms,{TreasureRoomSniper, TreasureRoomShotgun, TreasureRoomOstrich, TreasureRoomMachineGun,TreasureRoomSuperSize,TreasureRoomPinBall,TreasureRoomHare}}},
    }},
    {DifficultyRegion::Easy,{
        {
            RoomType::EnemyRoom,
            {
                {
                    HifiRoomSniperBallLauncher, HifiRoomJellyFish, HifiRoomCannonLasers, HifiRoomCannonSnipers,
                    HifiRoomSniperShurikens, HifiRoomCannonBoids, HifiRoomSmall, HifiRoomAvenue, HifiRoomLane,HifiRoomSmallTackShooter
                },
                {HifiEnemyRoomSwarmLasers, HifiRoomShieldedMadness, HifiRoomLaserFiesta}
            }
        },
        {RoomType::RestRoom, {{RestingRoomPop, RestingRoomPopLarge, RestingRoomPopLong, RestingRoomPopTall, RestingRoomPopSmall},{RestingRoom3PopLarge}}},
        {RoomType::EventRoom, {{EventRoomWish, EventRoomSwarm, EventRoomOracleCrab, }, {EventRoomBaru, EventRoomWish }}  },
        {RoomType::TreasureRoom, {bothTreasureRooms,{TreasureRoomSniper, TreasureRoomShotgun, TreasureRoomOstrich, TreasureRoomMachineGun,TreasureRoomSuperSize,TreasureRoomPinBall,TreasureRoomHare}}},
    }},
    {DifficultyRegion::Medium,{
        {
            RoomType::EnemyRoom,
            {
                {
                    HifiRoomCannonLasers, HifiRoomJellyFish, HifiRoomCannonSnipers, HifiRoomSniperShurikens,
                    HifiRoomSmallBallLauncher, HifiRoomSmallTackShooter, HifiRoomSmall,
                },
                {HifiRoomLaserFiesta, HifiRoomShieldedMadness}
            }
        },
        {RoomType::RestRoom, {{RestingRoomPop, RestingRoomPopLarge, RestingRoomPopLong, RestingRoomPopTall, RestingRoomPopSmall},{RestingRoom3PopLarge}}},
        {RoomType::EventRoom, {{EventRoomBaru,  EventRoomWish, EventRoomOracleCrab, EventRoomSwarm}, {EventRoomBaru, EventRoomWish }}  },
        {RoomType::TreasureRoom, {bothTreasureRooms,{TreasureRoomSniper, TreasureRoomShotgun, TreasureRoomOstrich, TreasureRoomMachineGun,TreasureRoomSuperSize,TreasureRoomPinBall,TreasureRoomHare}}},
    }},
};

std::map<DifficultyRegion,std::map<RoomType, RoomPresets>> MiningRoomDirectory = {
    {DifficultyRegion::Intro,{
        {
            RoomType::EnemyRoom,
            {
                {
                    QuadShooterRoomSmall, MiningEnemyRoomIntro, MiningEnemyRoomMoles, MiningEnemyRoomPileDrivers,
                    MiningEnemyRoomBulldozers, MiningEnemyRoomPileDriversIntro, MiningEnemyRoomMoles2,
                },
                {EnemyRoomMagic, MiningEnemyRoomCloakedMoles,}
            }
        },
        {RoomType::RestRoom, {{RestingRoomPop},{RestingRoomPop, RestingRoomPopLarge, RestingRoomPopLong, RestingRoomPopTall, RestingRoomPopSmall}}},
        {RoomType::EventRoom, {{EventRoomBibleTree, EventRoomOracleCrab, }, {EventRoomBaru, EventRoomHoney }}  },
        {RoomType::TreasureRoom, {regularTreasureRooms,{TreasureRoomSniper, TreasureRoomShotgun, TreasureRoomOstrich, TreasureRoomMachineGun,TreasureRoomSuperSize,TreasureRoomPinBall,TreasureRoomHare}}},
    }},
    {DifficultyRegion::Easy,{
        {
            RoomType::EnemyRoom,
            {
                {
                     QuadShooterRoomSmall, MiningEnemyRoomBoulders, MiningEnemyRoomMoles,MiningEnemyRoomLane, MiningEnemyRoomAvenue,MiningEnemyRoomPileDrivers, MiningEnemyRoomBulldozers, MiningEnemyRoomMoles2,
                    MiningEnemyRoomPileDriversIntro, MiningEnemyRoomQuadShooterBoulders,
                },
                {EnemyRoomMagic, MiningEnemyRoomMineField, MiningEnemyRoomCloakedMoles,}
            }
        },
        {RoomType::RestRoom, {{RestingRoomPop, RestingRoomPopLarge, RestingRoomPopLong, RestingRoomPopTall, RestingRoomPopSmall},{RestingRoom3PopLarge}}},
        {RoomType::EventRoom, {{EventRoomBibleTree, EventRoomOracleCrab,}, { }}  },
        {RoomType::TreasureRoom, {regularTreasureRooms,{TreasureRoomSniper, TreasureRoomShotgun, TreasureRoomOstrich, TreasureRoomMachineGun,TreasureRoomSuperSize,TreasureRoomPinBall,TreasureRoomHare}}},
    }},
    {DifficultyRegion::Medium,{
        {
            RoomType::EnemyRoom,
            {
                {
                    EnemyRoomSingleMage, MiningEnemyRoomBoulders, MiningEnemyRoomMoles,MiningEnemyRoomLane, MiningEnemyRoomAvenue,MiningEnemyRoomPileDrivers ,MiningEnemyRoomMoles2,MiningEnemyRoomQuadShooterBoulders,MiningEnemyRoomBulldozers,
                },
                {EnemyRoomMagic, MiningEnemyRoomMineField, MiningEnemyRoomCloakedMoles,}
            }
        },
        {RoomType::RestRoom, {{RestingRoomPop, RestingRoomPopLarge, RestingRoomPopLong, RestingRoomPopTall, RestingRoomPopSmall},{RestingRoom3PopLarge}}},
        {RoomType::EventRoom, {{EventRoomBibleTree, EventRoomOracleCrab}, {}}  },
        {RoomType::TreasureRoom, {bothTreasureRooms,{TreasureRoomSniper, TreasureRoomShotgun, TreasureRoomOstrich, TreasureRoomMachineGun,TreasureRoomSuperSize,TreasureRoomPinBall,TreasureRoomHare}}},
    }},
};

const std::map<DifficultyRegion,std::map<RoomType, RoomPresets>> MedicalRoomDirectory = {
    {DifficultyRegion::Intro,{
        {
            RoomType::EnemyRoom,
            {
                {
                    MedicalEnemyRoomSpineWormQuadShooters, MedicalEnemyRoomWares,
                    MedicalEnemyRoomHeartBeat, MedicalEnemyRoomSmallRodC, MedicalEnemyRoomHand
                },
                {MedicalEnemyRoomInvincible}
            }
        },
        {RoomType::RestRoom, {{RestingRoomPop, RestingRoomPopLarge, RestingRoomPopLong, RestingRoomPopTall, RestingRoomPopSmall},{RestingRoom3PopLarge}}},
        {RoomType::EventRoom, {{EventRoomOven, EventRoomOracleCrab, EventRoomInverter}, {EventRoomOptimizer }}  },
        {RoomType::TreasureRoom, {bothTreasureRooms,{TreasureRoomSniper, TreasureRoomShotgun, TreasureRoomOstrich, TreasureRoomMachineGun,TreasureRoomSuperSize,TreasureRoomPinBall,TreasureRoomHare, TreasureRoomRisky}}},
    }},
    {DifficultyRegion::Easy,{
        {
            RoomType::EnemyRoom,
            {
                {
                    MedicalEnemyRoomHeartBeat, MedicalEnemyRoomHeartBeat2, MedicalEnemyRoomHand, MedicalEnemyRoomSpineWormQuadShooters, MedicalEnemyRoomSyringes,EnemyRoomSingleMage,
                    EnemyRoomCloaked, MedicalEnemyRoomHeartBeat4, MedicalEnemyRoomWorms

                },
                {MedicalEnemyRoomHeartBeat3, MedicalEnemyRoomInvincible}
            }
        },
        {RoomType::RestRoom, {{RestingRoomPop, RestingRoomPopLarge, RestingRoomPopLong, RestingRoomPopTall, RestingRoomPopSmall},{RestingRoom3PopLarge}}},
        {RoomType::EventRoom, {{EventRoomOracleCrab, EventRoomOven, EventRoomInverter}, {EventRoomOptimizer }}  },
        {RoomType::TreasureRoom, {choiceTreasureRooms,{TreasureRoomSniper, TreasureRoomShotgun, TreasureRoomOstrich, TreasureRoomMachineGun,TreasureRoomSuperSize,TreasureRoomPinBall,TreasureRoomHare, TreasureRoomRisky}}},
    }},
    {DifficultyRegion::Medium,{
        {RoomType::EnemyRoom, {{MedicalEnemyRoomHeartBeat4, MedicalEnemyRoomHeartBeat2, MedicalEnemyRoomHand, MedicalEnemyRoomSpineWormQuadShooters, MedicalEnemyRoomSyringes },{MedicalEnemyRoomHeartBeat3,MedicalEnemyRoomInvincible}}},
        {RoomType::RestRoom, {{RestingRoomPop, RestingRoomPopLarge, RestingRoomPopLong, RestingRoomPopTall, RestingRoomPopSmall},{}}},
        {RoomType::EventRoom, {{ EventRoomOracleCrab, EventRoomOven, EventRoomInverter}, { EventRoomOptimizer}}  },
        {RoomType::TreasureRoom, {choiceTreasureRooms,{TreasureRoomSniper, TreasureRoomShotgun, TreasureRoomOstrich, TreasureRoomMachineGun,TreasureRoomSuperSize,TreasureRoomPinBall,TreasureRoomHare, TreasureRoomRisky}}},
    }},
};

const std::map<DifficultyRegion,std::map<RoomType, RoomPresets>> MilitaryRoomDirectory = {
    {DifficultyRegion::Intro,{
        {RoomType::EnemyRoom, {{EnemyRoomInvisible, MiningEnemyRoomMineField, MiningEnemyRoomCloakedMoles, EnemyRoomMagic,EnemyRoomPhantom, EnemyRoomEyes,},bossRooms}},
        {RoomType::RestRoom, {{RestingRoomPop, RestingRoomPopLarge, RestingRoomPopLong, RestingRoomPopTall, RestingRoomPopSmall},{RestingRoom3PopLarge}}},
        {RoomType::EventRoom, {{EventRoomOracleCrab, }, { }}  },
        {RoomType::TreasureRoom, {bothTreasureRooms,{TreasureRoomSniper, TreasureRoomShotgun, TreasureRoomOstrich, TreasureRoomMachineGun,TreasureRoomSuperSize,TreasureRoomPinBall,TreasureRoomHare, TreasureRoomRisky}}},
    }},
    {DifficultyRegion::Easy,{
        {
            RoomType::EnemyRoom,
            {
                {
                    EnemyRoomMagic,HifiEnemyRoomSwarmLasers, HifiRoomShieldedMadness, HifiRoomLaserFiesta, MedicalEnemyRoomHeartBeat3, MedicalEnemyRoomInvincible, EnemyRoomPhantom, EnemyRoomEyes,

                },
                bossRooms
            }
        },
        {RoomType::RestRoom, {{RestingRoomPop, RestingRoomPopLarge, RestingRoomPopLong, RestingRoomPopTall, RestingRoomPopSmall},{RestingRoom3PopLarge}}},
        {RoomType::EventRoom, {{EventRoomOracleCrab}, { }}  },
        {RoomType::TreasureRoom, {choiceTreasureRooms,{TreasureRoomSniper, TreasureRoomShotgun, TreasureRoomOstrich, TreasureRoomMachineGun,TreasureRoomSuperSize,TreasureRoomPinBall,TreasureRoomHare, TreasureRoomRisky}}},
    }},
    {DifficultyRegion::Medium,{
        {
            RoomType::EnemyRoom,
            {
                {HifiRoomShieldedMadness, HifiRoomLaserFiesta, MedicalEnemyRoomHeartBeat3, MedicalEnemyRoomInvincible, EnemyRoomPhantom, EnemyRoomEyes,},
                bossRooms,
            }
        },
        {RoomType::RestRoom, {{RestingRoomPop, RestingRoomPopLarge, RestingRoomPopLong, RestingRoomPopTall, RestingRoomPopSmall},{RestingRoom3PopLarge}}},
        {RoomType::EventRoom, {{ EventRoomOracleCrab}, { }}  },
        {RoomType::TreasureRoom, {choiceTreasureRooms,{TreasureRoomSniper, TreasureRoomShotgun, TreasureRoomOstrich, TreasureRoomMachineGun,TreasureRoomSuperSize,TreasureRoomPinBall,TreasureRoomHare, TreasureRoomRisky}}},
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
       case MapRegion::Military:
           return MilitaryRoomDirectory;
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

// Datatype for what BulletEffects each region can choose
// The weight is relative to total weight, so doesn't need to sum to any specific value
// To balance:
// Increase a weight to increase odds of that effect, 
// Decrease a weight to decrease odss of that effect
struct weightedRegionEffects {
    std::vector<std::pair<std::vector<BulletStackEffect>, float>> weightedEffects;

    bool cumulated = false;

    void getCumulativeWeights() {
        for (int i = 1; i < weightedEffects.size(); i++) {
            weightedEffects[i].second += weightedEffects[i - 1].second;
        }
        cumulated = true;
    }

    std::vector<BulletStackEffect> getWeightedEffect() {
        if (!cumulated) getCumulativeWeights();

        float random = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / weightedEffects.back().second));
        for (int i = 0; i < weightedEffects.size(); i++) {
            if (weightedEffects[i].second > random) {
                return weightedEffects[i].first;
            }
        }
        return weightedEffects[0].first;
    }

    // Return N vectors of BulletStackEffect, ignoring any effects of type found in the ignoreList
    std::vector<std::vector<BulletStackEffect>> getNWeightedEffects(int N, std::vector<std::vector<BulletStackEffect>> ignoreList = {}) {
        std::vector<std::vector<BulletStackEffect>> output;
        if (!cumulated) getCumulativeWeights();

        std::vector<std::pair<std::vector<BulletStackEffect>, float>> tempWeightedEffects = weightedEffects;

        for (int j = 0; j < N; j++) {
            float random = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / tempWeightedEffects.back().second));
            for (int i = 0; i < tempWeightedEffects.size(); i++) {
                if (tempWeightedEffects[i].second > random) {
                    if (!checkIgnoreList(tempWeightedEffects[i].first[0].type, ignoreList)) {
                        output.push_back(tempWeightedEffects[i].first);
                    } else {
                        j--;
                    }
                    float weightToRemove = (tempWeightedEffects[i].second - ((i > 0) ? tempWeightedEffects[i - 1].second : 0));
                    for (int k = i + 1; k < tempWeightedEffects.size(); k++) {
                        tempWeightedEffects[k].second -= weightToRemove;
                    }
                    tempWeightedEffects.erase(tempWeightedEffects.begin() + i);
                    break;
                }
            }
        }
        //std::cout << N << ", " << output.size() << std::endl;
        return output;
    }

    // Returns true if the BulletEffectType is present in the ignoreList
    bool checkIgnoreList(BulletEffectType bet, std::vector<std::vector<BulletStackEffect>> ignoreList) {
        if (ignoreList.size() == 0) return false;
        for (auto& e : ignoreList) {
            for (BulletStackEffect& bst : e) {
                if (bst.type == bet) return true;
            }
        }
        return false;
    }
};

// BIOLOGY ----------------------------------------------

weightedRegionEffects biologyPositiveEffects = {
    {
        {{dmgUp},               10.f},
        {{sizeUp},              30.f},
        {{fireRateUp},          15.f},
        {{bulletRangeUp},       20.f},
        {{accuracyUp},          25.f},
        {{numBulletsUp},        5.f},
        {{bulletBounceUp},      15.f},
        {{bulletPierceUp},      10.f},
        {{homingUp},            0.f},
        {{playerSpeedUp},       20.f},
        {{dashUp},              5.f},
        {{dashRechargeUp},      20.f},
    }
};

weightedRegionEffects biologyNegativeEffects = {
    {
        {{dmgDown},             5.f},
        {{sizeDown},            5.f},
        {{fireRateDown},        5.f},
        {{bulletRangeDown},     5.f},
        {{accuracyDown},        5.f},
        {{numBulletsDown},      0.f},
        {{bulletBounceDown},    5.f},
        {{bulletPierceDown},    5.f},
        {{homingDown},          0.f},
        {{playerSpeedDown},     5.f},
        {{dashDown},            5.f},
        {{dashRechargeDown},    5.f},
    }
};

// MINING ----------------------------------------------

weightedRegionEffects miningPositiveEffects = {
    {
        {{dmgUp},               15.f},
        {{sizeUp},              10.f},
        {{fireRateUp},          15.f},
        {{bulletRangeUp},       25.f},
        {{accuracyUp},          25.f},
        {{numBulletsUp},        5.f},
        {{bulletBounceUp},      10.f},
        {{bulletPierceUp},      15.f},
        {{homingUp},            0.f},
        {{playerSpeedUp},       10.f},
        {{dashUp},              5.f},
        {{dashRechargeUp},      10.f},
    }
};

weightedRegionEffects miningNegativeEffects = {
    {
        {{dmgDown},             5.f},
        {{sizeDown},            5.f},
        {{fireRateDown},        5.f},
        {{bulletRangeDown},     5.f},
        {{accuracyDown},        5.f},
        {{numBulletsDown},      5.f},
        {{bulletBounceDown},    5.f},
        {{bulletPierceDown},    5.f},
        {{homingDown},          0.f},
        {{playerSpeedDown},     5.f},
        {{dashDown},            5.f},
        {{dashRechargeDown},    5.f},
    }
};

// HIFI ----------------------------------------------

weightedRegionEffects hifiPositiveEffects = {
    {
        {{dmgUp},               15.f},
        {{sizeUp},              25.f},
        {{fireRateUp},          15.f},
        {{bulletRangeUp},       30.f},
        {{accuracyUp},          30.f},
        {{numBulletsUp},        15.f},
        {{bulletBounceUp},      30.f},
        {{bulletPierceUp},      5.f},
        {{homingUp},            10.f},
        {{playerSpeedUp},       30.f},
        {{dashUp},              15.f},
        {{dashRechargeUp},      30.f},
    }
};

weightedRegionEffects hifiNegativeEffects = {
    {
        {{dmgDown},             5.f},
        {{sizeDown},            5.f},
        {{fireRateDown},        5.f},
        {{bulletRangeDown},     5.f},
        {{accuracyDown},        5.f},
        {{numBulletsDown},      5.f},
        {{bulletBounceDown},    5.f},
        {{bulletPierceDown},    5.f},
        {{homingDown},          5.f},
        {{playerSpeedDown},     5.f},
        {{dashDown},            5.f},
        {{dashRechargeDown},    5.f},
    }
};

// MEDICAL ----------------------------------------------

weightedRegionEffects medicalPositiveEffects = {
    {
        {{dmgUp},               20.f},
        {{sizeUp},              15.f},
        {{fireRateUp},          30.f},
        {{bulletRangeUp},       20.f},
        {{accuracyUp},          30.f},
        {{numBulletsUp},        5.f},
        {{bulletBounceUp},      5.f},
        {{bulletPierceUp},      15.f},
        {{homingUp},            10.f},
        {{playerSpeedUp},       45.f},
        {{dashUp},              30.f},
        {{dashRechargeUp},      45.f},
    }
};

weightedRegionEffects medicalNegativeEffects = {
    {
        {{dmgDown},             5.f},
        {{sizeDown},            5.f},
        {{fireRateDown},        5.f},
        {{bulletRangeDown},     5.f},
        {{accuracyDown},        5.f},
        {{numBulletsDown},      5.f},
        {{bulletBounceDown},    5.f},
        {{bulletPierceDown},    5.f},
        {{homingDown},          5.f},
        {{playerSpeedDown},     5.f},
        {{dashDown},            5.f},
        {{dashRechargeDown},    5.f},
    }
};

// MILITARY ----------------------------------------------

weightedRegionEffects militaryPositiveEffects = {
    {
        {{dmgUp},               50.f},
        {{sizeUp},              5.f},
        {{fireRateUp},          5.f},
        {{bulletRangeUp},       50.f},
        {{accuracyUp},          5.f},
        {{numBulletsUp},        5.f},
        {{bulletBounceUp},      5.f},
        {{bulletPierceUp},      5.f},
        {{homingUp},            15.f},
        {{playerSpeedUp},       50.f},
        {{dashUp},              5.f},
        {{dashRechargeUp},      50.f},
    }
};

weightedRegionEffects militaryNegativeEffects = {
    {
        {{dmgDown},             5.f},
        {{sizeDown},            5.f},
        {{fireRateDown},        5.f},
        {{bulletRangeDown},     5.f},
        {{accuracyDown},        5.f},
        {{numBulletsDown},      5.f},
        {{bulletBounceDown},    5.f},
        {{bulletPierceDown},    5.f},
        {{homingDown},          5.f},
        {{playerSpeedDown},     5.f},
        {{dashDown},            5.f},
        {{dashRechargeDown},    5.f},
    }
};

inline RoomPreset getRoomPreset(RoomType type, MapRegion currRegion, bool locked, int roomsTraversed = -1, float eliteSpawnChance = 0.2f) {
    RoomPreset nextRoom;
    
    Map& map = registry.maps.components[0];

    if (!hasLocked(type, map.roomsTraversed) && locked) {
        assert(false);
    }
    DifficultyRegion currentRegionDifficulty;
    if (roomsTraversed == -1) {
        if (map.roomsTraversed < static_cast<int>(DifficultyRegion::Intro)) {
            currentRegionDifficulty = DifficultyRegion::Intro;
        }
        else if (map.roomsTraversed < static_cast<int>(DifficultyRegion::Easy)) {
            currentRegionDifficulty = DifficultyRegion::Easy;
        }
        else if (map.roomsTraversed < static_cast<int>(DifficultyRegion::Medium)) {
            currentRegionDifficulty = DifficultyRegion::Medium;
        }
        else {
            currentRegionDifficulty = DifficultyRegion::Medium; // Assuming Medium for higher roomsTraversed
        }
    }
    else {
        if (roomsTraversed < static_cast<int>(DifficultyRegion::Intro)) {
            currentRegionDifficulty = DifficultyRegion::Intro;
        }
        else if (roomsTraversed < static_cast<int>(DifficultyRegion::Easy)) {
            currentRegionDifficulty = DifficultyRegion::Easy;
        }
        else if (roomsTraversed < static_cast<int>(DifficultyRegion::Medium)) {
            currentRegionDifficulty = DifficultyRegion::Medium;
        }
        else {
            currentRegionDifficulty = DifficultyRegion::Medium; // Assuming Medium for higher roomsTraversed
        }
    }

    //boss rooms
    if (type == RoomType::BossRoom && currRegion == MapRegion::Biology) {
        std::vector<RoomPreset> biobosses = {BossRoomBee, BossRoomCrab};
        nextRoom = Random::ListItem(biobosses);
    } else if (type == RoomType::BossRoom && currRegion == MapRegion::Medical) {
        nextRoom = ScientistBossRoom;
    } else if (type == RoomType::BossRoom && currRegion == MapRegion::Physics) {
        std::vector<RoomPreset> hifibosses = { BossRoomBigC, BossRoomMultiCube };
        nextRoom = Random::ListItem(hifibosses);
    } else if (type == RoomType::BossRoom && currRegion == MapRegion::Mining) {
        std::vector<RoomPreset> miningbosses = { BossRoomMole, BossRoomWorm };
        nextRoom = Random::ListItem(miningbosses);
    }

    // Tutorial rooms
    else if (type == RoomType::TutorialRoom2) {
        nextRoom = TutorialRoom2Preset;
    }
    
    // Regular rooms
    else {
        if (type == RoomType::None) {
            return nextRoom;
        }
        nextRoom = Random::ListItem(locked ? getDirectory(currRegion).at(currentRegionDifficulty).at(type).locked : getDirectory(currRegion).at(currentRegionDifficulty).at(type).unlocked);
    
    }
    
    // Effect controls
    // Biology  2pos 1neg
    // Mining   2pos 2neg
    // Hifi     3pos 2neg
    // Medical  3pos 3neg
    // Military 1pos 3neg

    switch (currRegion) {
    case MapRegion::Biology:
        nextRoom.positiveEffects = biologyPositiveEffects.getNWeightedEffects(2);
        nextRoom.negativeEffects = biologyNegativeEffects.getNWeightedEffects(1, nextRoom.positiveEffects);
        break;
    case MapRegion::Mining:
        nextRoom.positiveEffects = miningPositiveEffects.getNWeightedEffects(2);
        nextRoom.negativeEffects = miningNegativeEffects.getNWeightedEffects(2, nextRoom.positiveEffects);
        break;
    case MapRegion::Physics:
        nextRoom.positiveEffects = hifiPositiveEffects.getNWeightedEffects(3);
        nextRoom.negativeEffects = hifiNegativeEffects.getNWeightedEffects(2, nextRoom.positiveEffects);
        break;
    case MapRegion::Medical:
        nextRoom.positiveEffects = medicalPositiveEffects.getNWeightedEffects(3);
        nextRoom.negativeEffects = medicalNegativeEffects.getNWeightedEffects(3, nextRoom.positiveEffects);
        break;
    case MapRegion::Military:
        nextRoom.positiveEffects = militaryPositiveEffects.getNWeightedEffects(1);
        nextRoom.negativeEffects = militaryNegativeEffects.getNWeightedEffects(3, nextRoom.positiveEffects);
        break;
    default:
        std::cout << "WARNING: Enemy room in unexpected region!" << std::endl;
        nextRoom.positiveEffects = { biologyPositiveEffects.getWeightedEffect() };
        nextRoom.negativeEffects = { biologyNegativeEffects.getWeightedEffect() };
        break;
    }

    // Effect printing for testing
    std::cout << "--== Positive Effects ==--" << std::endl;
    for (auto& e : nextRoom.positiveEffects) {
        for (auto& ee : e) {
            std::cout << ee.name << std::endl;
        }
    }

    std::cout << "--== Negative Effects ==--" << std::endl;
    for (auto& e : nextRoom.negativeEffects) {
        for (auto& ee : e) {
            std::cout << ee.name << std::endl;
        }
    }

    // Remove the one-time room from all relevant regions
    if (nextRoom.oneTime) {
        for (auto it = map.directory.begin(); it != map.directory.end(); ++it) {
            if (it->first >= currentRegionDifficulty) { // Remove from current and future regions
                std::map<RoomType, RoomPresets>& roomMap = it->second;
                std::vector<RoomPreset>& roomList = roomMap.at(type).locked;
                roomList.erase(std::remove(roomList.begin(), roomList.end(), nextRoom), roomList.end());

                // Repeat the process for unlocked rooms if applicable
                std::vector<RoomPreset>& unlockedRoomList = roomMap.at(type).unlocked;
                unlockedRoomList.erase(std::remove(unlockedRoomList.begin(), unlockedRoomList.end(), nextRoom), unlockedRoomList.end());
            }
        }
    }

    // Elite
    if (type == RoomType::EnemyRoom && roomsTraversed > 6 && Random::Float() < eliteSpawnChance) {
        nextRoom.hasElite = true;
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
    {RoomType::EnemyRoom, SoundType::CombatBGM},
    {RoomType::RestRoom, SoundType::ClearedBGM},
    {RoomType::BossRoom, SoundType::BossBGM},
    {RoomType::TreasureRoom, SoundType::ClearedBGM},
    {RoomType::None, SoundType::ClearedBGM},
    {RoomType::TutorialRoom, SoundType::TitleBGM},
    {RoomType::TutorialRoom1, SoundType::TitleBGM},
    {RoomType::TutorialRoom2, SoundType::CombatBGM},
{ RoomType::EventRoom, SoundType::ClearedBGM },
{ RoomType::None, SoundType::ClearedBGM }

};

std::vector<std::vector<std::tuple<EnemyType,vec2>>> bioEliteEnemies = {
    {{EnemyType::EnemyMage, {0.5f,0.5f}}},
        {
         {EnemyType::EnemyBeeHive, {0.5f,0.5f}},
{EnemyType::EnemyBubbleShield, {0.5f,0.5f}},
            {EnemyTwoBee, random_vec2},
{EnemyTwoBee, random_vec2},
{EnemyThreeBee, random_vec2},
{EnemyTwoBee, random_vec2}
        },
};

std::vector<std::vector<std::tuple<EnemyType, vec2>> > miningEliteEnemies = {
    {
        {EnemyType::EnemyMage, {0.5f,0.5f}},
    },
    {
                {EnemyType::EnemySurfaceMole, {0.5f,0.5f}},
                {EnemyType::EnemySmallMole, random_vec2},
                    {EnemyType::EnemySmallMole, random_vec2},
                {EnemyType::EnemySmallMole, random_vec2},
                {EnemyType::EnemySmallMole, random_vec2},
                {EnemyType::EnemyBigBoulder, random_vec2},
                {EnemyType::EnemyBigBoulder, random_vec2},
        },
        {{EnemyType::EnemyEyeCube, {0.5f,0.5f}}},
        {{EnemyType::EnemyPhantom, {0.5f,0.5f}}},
};

std::vector<std::vector<std::tuple<EnemyType,vec2>> > physicsEliteEnemies = {
    {{EnemyType::EnemyPhantom, {0.5f,0.5f}}},
            {{EnemyType::EnemyEyeCube, {0.5f,0.5f}}},
{{EnemyType::EnemyMage, {0.5f,0.5f}}},
    {
                {EnemyType::EnemyMage, random_vec2},
                {EnemyType::EnemyMage, random_vec2},
        {EnemyType::EnemyMage, random_vec2},
        },
        {
                            {EnemyHifiJellyFish, {0.6f, 0.6f}},
        {EnemyHifiJellyFish, {0.4f, 0.4f}},
{EnemySmallCShield, {0.6f, 0.6f}},
    {EnemySmallCShield, {0.4f, 0.4f}},
            { EnemyHifiJellyFish, {0.4f, 0.6f}},
                            {EnemyHifiJellyFish, {0.6f, 0.4f}},
                {EnemySmallCShield, {0.4f, 0.6f}},
                {EnemySmallCShield, {0.6f, 0.4f}},
        },

};

std::vector<std::vector<std::tuple<EnemyType, vec2>> > medicalEliteEnemies = {
    {{EnemyType::EnemyMage, {0.5f,0.5f}}},
{{EnemyType::EnemyPhantom, {0.5f,0.5f}}},
    {{EnemyType::EnemyEyeCube, {0.5f,0.5f}}},
    {
                {EnemyType::EnemyMage, random_vec2},
                {EnemyType::EnemyMage, random_vec2},
        {EnemyType::EnemyMage, random_vec2},
        },
    {
        {EnemyMedicalRodC, {0.5f, 0.5f}},
        {InvisibleGranter, {0.5f, 0.5f}},
{EnemyType::EnemyMedicalBMP, random_vec2},
{EnemyType::EnemyMedicalPillBoidSpawner, random_vec2},
        { EnemyType::EnemyMedicalSyringe, random_vec2},
    },
    {
    }
};

std::vector<std::vector<std::tuple<EnemyType,vec2>>> militaryEliteEnemies = {
    {{EnemyType::BossCrab, {0.5f,0.5f}}},
{{EnemyType::BossBeehiveMain, {0.5f,0.5f}}},
{{EnemyType::BossMultiCube, {0.5f,0.5f}}},
    {{BossBigC, {0.5f, 0.5f}},
        {EnemyLaserSniper,{0.5, 0.63}},
            {EnemyLaserSniper,{0.38, 0.42}},
        {EnemyLaserSniper,{1-0.38, 0.42}},
    },
{{EnemyType::BossDrillWormHead, {1.5f,0.5f}}},
    {
        {EnemyDynamite, {random_float, random_float}},
        {EnemyDynamite, {random_float, random_float}},

          {EnemyDynamite, {random_float, random_float}},
        {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
        {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
        {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
        {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
        {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
        {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
        {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
        {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
        {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
        {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
        {EnemyDynamite, {random_float, random_float}},{EnemyDynamite, {random_float, random_float}},
        {EnemyDynamite, {random_float, random_float}},
        {BossMole, {0.5f, 0.5f}},
},
};

std::map<MapRegion,std::vector<std::vector<std::tuple<EnemyType,vec2>>> > eliteEnemies = {
    {Tutorial, bioEliteEnemies},
    {Biology, bioEliteEnemies},
    {Physics, physicsEliteEnemies},
    {Medical, medicalEliteEnemies},
    {Mining, miningEliteEnemies},
    {Military, militaryEliteEnemies},
};