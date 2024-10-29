#pragma once
#include "common.hpp"
#include "components/actor_components.hpp"

enum Side : char {
    Left = 'L',Top = 'T',Bottom = 'B',Right = 'R'
};

enum RoomType : char {
    EnemyRoomDash,
    EnemyRoomBulletSize,
    EnemyRoomDmg,
    EnemyRoomSpeed,
    TreasureRoom,
    RestRoom,
    BossBigC,
    None //Keep None at the end of the list to be compatible with existing get random function
};

enum SpecialEvent { BouncingDisc };
enum RoomProp { Plant1 };
enum BossType { BigC };

struct EnemyRoomPreset {
    std::vector<std::tuple<EnemyType,vec2>> enemies;
    float spawnDelay; //in seconds
    std::vector<SpecialEvent> specialEvents;
    std::vector<std::tuple<RoomProp,vec2>> roomProps;
    int numSpecialBulletsToSpawn = 5;
};

struct RestingRoomPreset {
    vec2 rebootStationLocation;
};
struct TreasureRoomPreset { 
    BulletStackEffect effect;
};
struct BossRoomPreset {
    vec2 spawnLocation;
    BossType boss;
    float spawnDelay;
};

struct Door {
	RoomType room; //room the door leads to
    bool isPrev; //if is previous room, block it
    vec2 startPos, endPos;
};

enum RoomFormatType {EnemyFT,RestingFT,TreasureFT,BossFT};
struct Room {
    RoomFormatType formatType;
    union Preset { //the room preset can be one of 3 formats
        EnemyRoomPreset* enemy;
        RestingRoomPreset* resting;
        TreasureRoomPreset* treasure;
        BossRoomPreset* boss;
        Preset(){}
    } preset;
    bool cleared;
    float timeElapsed; //time passed since enter room in seconds
};
enum MapRequestType {
    RestartGame = 'R',
    ChangeRoom = 'C'
};
struct MapRequest {
    MapRequestType requestType;
    RoomType type;
    int doorIndex;
    MapRequest(MapRequestType requestType, RoomType type = RoomType::None, int doorIndex = 0) { 
        this->requestType = requestType;
        this->type = type; 
        this->doorIndex = doorIndex;
    }
};

enum MapRegion {
    Tutorial,
    Biology,
    Final
};

struct Map {
    Room currRoom;
    int roomsTraversed; //for procedural linking rooms, the more rooms progress, tougher enemies, tougher rooms
    MapRegion currRegion;
};
