#pragma once
#include "common.hpp"

enum Side : char {
    Left = 'L',Top = 'T',Bottom = 'B',Right = 'R'
};

enum RoomType : char {
    EnemyRoom = 'E',
    TreasureRoom = 'T',
    BossRoom = 'B',
    RestRoom = 'R',
    None = 'N'
};

struct Door {
	RoomType room; //room the door leads to
    bool isPrev; //if is previous room, block it
    vec2 startPos, endPos;
};

struct Room {
    RoomType type;
    int variant; // the variant within the room type
    bool cleared;
    float timeElapsed; //time passed since enter room in seconds
};
struct ChangeRoomRequest {
    RoomType type;
    int doorIndex;
    ChangeRoomRequest(RoomType type, int doorIndex) { 
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
