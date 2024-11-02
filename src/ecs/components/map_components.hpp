#pragma once
#include "common.hpp"
#include "components/actor_components.hpp"

enum Side : char {
    Left = 'L',Top = 'T',Bottom = 'B',Right = 'R'
};

enum RoomType : char {
    EnemyRoomDash,
    EnemyRoomTripleBuff,
    // EnemyRoomBulletSize,
    // EnemyRoomDmg,
    // EnemyRoomSpeed,
    TreasureRoom,
    RestRoom,
    BossBigCRoom, //remove for now to prevent bug
    None //Keep None at the end of the list to be compatible with existing get random function
};

enum SpecialEvent { BouncingDisc,RebootStation };
enum RoomProp { Plant1 };
enum BossType { BigCBoss };

struct RoomPreset {
    std::vector<std::tuple<EnemyType,vec2>> enemies;
    std::vector<std::tuple<BulletStackEffect,vec2>> treasures; //for treasure rooms
    std::vector<std::tuple<RoomProp,vec2>> roomProps; //background props
    std::vector<SpecialEvent> specialEvents; 
    float spawnDelay; //in seconds - for enemies and bosses
    int numSpecialBulletsToSpawn = 5;
};

struct Door {
	RoomType room; //room the door leads to
    bool isPrev; //if is previous room, block it
    vec2 startPos, endPos;
};

struct Room {
    RoomPreset preset;
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
