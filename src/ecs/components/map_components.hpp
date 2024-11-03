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
    None = 'N',
    TutorialRoom1 = '1',
    TutorialRoom2 = '2'
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
    bool dialogueDone = false; // place here for now
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
