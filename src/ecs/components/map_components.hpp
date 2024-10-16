#pragma once

struct Room;

enum RoomType {
    EnemyRoom,
    TreasureRoom,
    BossRoom,
    RestRoom,
    None
};

struct Door {
	RoomType room; //room the door leads to
    bool isPrev; //if is previous room, block it
};

struct Room {
    RoomType type;
    int variant; // the variant within the room type
    bool cleared;

    Door leftDoor;
    Door rightDoor;
    Door topDoor;
    Door bottomDoor;

    float timeElapsed; //time passed since enter room
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
