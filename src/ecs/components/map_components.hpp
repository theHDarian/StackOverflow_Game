#pragma once

struct Room;

struct Door {
	Room* nextRoom; //room the door leads to
    bool blocked; //blocked to prevent going to previous room
};

struct Room {
    vec2 dimensions; //probably be fixed for now
    int type; //change to enum once list of room types has been made ex Resting, Enemy, Boss
    int layout; //change type once list of layouts is made, layouts store enemy info (position, spawn time), as well as items in the room
    Door doors[4]; //doors on each side, put null for sides without doors

    float m_timeToNextRoom; //timer for room transitions
    //TODO perhaps add a pointer to dialogues that appear for certain rooms
};
struct Map {
    Room startingRoom;
    int roomsTraversed; //for procedural linking rooms, the more rooms progress, tougher enemies, tougher rooms
    int currRegion;
};
