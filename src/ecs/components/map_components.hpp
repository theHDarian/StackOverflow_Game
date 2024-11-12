#pragma once
#include "common.hpp"
#include "components/actor_components.hpp"

enum Side : char {
    Left = 'L',Top = 'T',Bottom = 'B',Right = 'R'
};

enum RoomType : char {
    EnemyRoomDash,
    EnemyRoomTripleBuff,
    EnemyRoomBee,
    TreasureRoom,
    RestRoom,
    BossBigCRoom, //remove for now to prevent bug
    None, //Keep None at the end of the list to be compatible with existing get random function
    TutorialRoom1,
    TutorialRoom2
};

enum SpecialEvent { BouncingDisc,RebootStation };
enum RoomProp { Plant1 };
enum BossType { BigCBoss };

struct RoomPreset {
    std::vector<std::tuple<EnemyType,vec2>> enemies;
    std::vector<std::tuple<AttackData,vec2>> treasures; //for treasure rooms
    std::vector<std::tuple<RoomProp,vec2>> roomProps; //background props
    std::vector<SpecialEvent> specialEvents; 
    float spawnDelay; //in seconds - for enemies and bosses
    int numSpecialBulletsToSpawn = 5;
};

struct Door {
	RoomType room; //room the door leads to
    bool isPrev; //if is previous room, block it
    vec2 startPos, endPos;
    int doorIndex = -1; // used by interactibles for now
};
struct DoorSymbol {
    float angle;
    vec3 axis;
    vec3 offset;
};

struct Room {
    RoomPreset preset;
    RoomType type;
    bool cleared = false;
    float timeElapsed = 0; //time passed since enter room in seconds
    int dialogueCount = 0; // place here for now
    int cutsceneCount = 0;
    bool dialogueDone = true;
    bool cutSceneDone = true;
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

struct Scene {
    // keeps track of game state in a scene
    RoomType room;
    int dialogueCount;
    int cutSceneCount;
    bool roomCleared;

    //// ref: https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
    bool operator==(const Scene& other) const
    {
        return (room == other.room
            && dialogueCount == other.dialogueCount
            && cutSceneCount == other.cutSceneCount
            && roomCleared == other.roomCleared);
    }
};

// ref: https://en.cppreference.com/w/cpp/utility/hash
template <>
struct std::hash<Scene>
{
    std::size_t operator()(const Scene& s) const noexcept
    {
        std::size_t h1 = std::hash<char>{}(s.room);
        std::size_t h2 = std::hash<int>{}(s.dialogueCount);
        std::size_t h3 = std::hash<int>{}(s.cutSceneCount);
        std::size_t h4 = std::hash<bool>{}(s.roomCleared);

        return h1 ^ ((h2 << 1) >> 1) ^ (h3 << 1) ^ (h4 << 1);
    }
};

struct InteractibleDialogue {
    std::string object;
    int choice;
    int dialogueCount;

    //// ref: https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
    bool operator==(const InteractibleDialogue& other) const
    {
        return (object.compare(other.object) == 0
            && choice == other.choice
            && dialogueCount == other.dialogueCount);
    }
};

// ref: https://en.cppreference.com/w/cpp/utility/hash
template <>
struct std::hash<InteractibleDialogue>
{
    std::size_t operator()(const InteractibleDialogue& s) const noexcept
    {
        std::size_t h1 = std::hash<std::string>{}(s.object);
        std::size_t h2 = std::hash<int>{}(s.dialogueCount);
        std::size_t h3 = std::hash<int>{}(s.choice);

        return h1 ^ ((h2 << 1) >> 1) ^ (h3 << 1);
    }
};