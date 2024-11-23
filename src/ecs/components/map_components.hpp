#pragma once
#include "common.hpp"
#include "components/actor_components.hpp"
#include "utils/random.hpp"

enum Side : char {
    Left = 'L',Top = 'T',Bottom = 'B',Right = 'R'
};

enum RoomType : int {
    EnemyRoom,
    TreasureRoom,
    RestRoom,
    None, //Keep None at the end of the list to be compatible with existing get random function

    //Special rooms that are not spawned via getRandomRoomType function
    TutorialRoom1,
    TutorialRoom2,
    BossBigCRoom,
};

inline RoomType getRandomRoomType(bool excludeNone, int roomsTraversed)
{
    const int bossRoomNum = 12;
    if (roomsTraversed % bossRoomNum == bossRoomNum-1) {
        //Make every 5 rooms the boss room
        return BossBigCRoom;
    }
    if (Random::Float() < 0.5f) { //enemy room has higher chance of being rolled
        return RoomType::EnemyRoom;
    }
    
    return static_cast<RoomType>(Random::Int(excludeNone ? RoomType::None - 1 : RoomType::None));
}

enum SpecialEvent { BouncingDisc,RebootStation };
enum RoomProp { Plant1 };
enum BossType { BigCBoss };

struct RoomInteractable {
    InteractableItem item;
    std::vector<BulletStackEffect> pushConsoleEffects;
};
struct RoomPreset {
    std::vector<std::tuple<EnemyType,vec2>> enemies;
    std::vector<std::tuple<RoomProp,vec2>> roomProps; //background props
    std::vector<std::tuple<RoomInteractable, vec2>> interactables; //for interactables
    std::vector<SpecialEvent> specialEvents; 
    float spawnDelay; //in seconds - for enemies and bosses
    int numSpecialBulletsToSpawn = 5;
    int numKeyBulletsToSpawn = 2;
};

struct Door {
	RoomType room = None; //room the door leads to
    bool isPrev; //if is previous room, block it
    bool isLocked;
    vec2 startPos, endPos;
    char side = 'L';
    int doorIndex = -1; // used by interactibles for now

    void reset() {
        room = None;
        isPrev = false;
        isLocked = false;
    }
};
struct DoorSymbol {
    float angle;
    vec3 axis;
    vec3 offset;
    int doorType;
    bool door = false;
    char side = 'L';
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
    vec2 roomSize = { 1920, 1080 };
    vec2 roomPosition = { 1920 / 2, 1080 / 2 };
    float wallThickness = 100.f;
};
enum MapRequestType {
    RestartGame = 'R',
    ChangeRoom = 'C',
    NewGame = 'N'
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
    int choice;

    //// ref: https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
    bool operator==(const Scene& other) const
    {
        return (room == other.room
            && dialogueCount == other.dialogueCount
            && cutSceneCount == other.cutSceneCount
            && roomCleared == other.roomCleared
            && choice == other.choice);
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
        std::size_t h5 = std::hash<int>{}(s.choice);

        return h1 ^ ((h2 << 1) >> 1) ^ (h3 << 1) ^ (h4 << 1) ^ ((h5 << 1) >> 1);
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