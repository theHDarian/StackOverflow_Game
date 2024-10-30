#pragma once
#include <vector>
#include <tuple>
#include "components/map_components.hpp"
#include "premades.hpp"
#include "common.hpp"
#include "tiny_ecs_registry.hpp"
#include "utils/random.hpp"


//---------- PRESETS-------------------
const EnemyRoomPreset EnemyRoomDash1 {
    {
        {EnemyType::EasyEnemySentry,{0.2f,0.8f}},
        {EnemyType::EasyEnemySentry,{0.8f,0.8f}},
        {EnemyType::EasyEnemySentry,{0.8f,0.2f}},
        {EnemyType::EasyEnemySentry,{0.2f,0.2f}}
    },
    5.0f,
    {  },
    {},
    5
};

const RestingRoomPreset RestingRoom1 {
    { 0.5f,0.5f}
};
const TreasureRoomPreset TreasureRoom1 {
    sizeUpA
};
const BossRoomPreset BossRoomBigC {
    {0.5,0.5}, //position
    BossType::BigCBoss,
    5.0f, //delay
    0 //special bullet count
};

const std::vector<EnemyRoomPreset> enemyRoomDashList = { EnemyRoomDash1 };
const std::vector<RestingRoomPreset> restingRoomList = { RestingRoom1 };
const std::vector<TreasureRoomPreset> treasureRoomList = { TreasureRoom1 };

EnemyRoomPreset* getEnemyRoom(RoomType type) {
    // Map& map = registry.maps.components[0];
    switch (type) {
        case RoomType::EnemyRoomDash:
            return Random::ListItem(enemyRoomDashList);
        default:
            printf("WARNING: other roomtypes not yet implemented, returning default enemy room\n");
            return Random::ListItem(enemyRoomDashList);
    }       
}
RestingRoomPreset* getRestingRoom() {
    return Random::ListItem(restingRoomList);
}
TreasureRoomPreset* getTreasureRoom() {
    return Random::ListItem(treasureRoomList);
}
BossRoomPreset* getBossRoom(RoomType type) {
    if (type == RoomType::BossBigC) {
        return const_cast<BossRoomPreset*>(&BossRoomBigC);
    }
    throw "invalid room type";
}





