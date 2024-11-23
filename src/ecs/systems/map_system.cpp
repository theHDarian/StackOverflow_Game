#include "map_system.hpp"
#include "components.hpp"
#include "tiny_ecs_registry.hpp"
#include "world_init.hpp"
#include "utils/random.hpp"
#include "SDL.h"
#include "components/presets/room_presets.hpp"
#include "sound_system.hpp"


MapSystem::MapSystem()
{
    if (registry.maps.components.size() == 0)
    {
        registry.maps.emplace(Entity());
    }
}
MapSystem::~MapSystem()
{
    registry.maps.clear();
}
void MapSystem::init(RenderSystem *renderer, SoundSystem *soundPlayer_arg)
{
    this->renderer = renderer;
    this->soundPlayer = soundPlayer_arg;
    assert(registry.maps.components.size() > 0);
    soundPlayer->playNextMusic();
}

void MapSystem::step(float elapsed_ms)
{
    Map &map = registry.maps.components[0];
    map.currRoom.timeElapsed += elapsed_ms / 1000.0f;

    handleMapRequests();

    // spawn enemy based on current time
    WindowState &wS = registry.windowStates.components[0];
    if (map.currRoom.timeElapsed > map.currRoom.preset.spawnDelay) {
        for (auto &e : map.currRoom.preset.enemies)
        {
            createEnemy(renderer, std::get<vec2>(e) * vec2(wS.width, wS.height), std::get<EnemyType>(e));

        }
        map.currRoom.preset.enemies = {};

        if (map.currRoom.cleared) {
            for (auto &e : map.currRoom.preset.interactables)
            {
                createInteractable(renderer, std::get<vec2>(e) * vec2(wS.width, wS.height), std::get<RoomInteractable>(e).item, std::get<RoomInteractable>(e).pushConsoleEffects);
            }
            map.currRoom.preset.interactables = {};

        }
    }
    

    // set room to cleared if all enemies are defeated
    if (registry.enemies.entities.empty() && map.currRoom.preset.enemies.empty() && map.currRoom.type != TutorialRoom1)
    {
        map.currRoom.cleared = true;
    }

    if (map.currRoom.cleared) {
        // make all doors unlocked doors
        for (int i = 0; i < 4; i++)
        {
            if (registry.interactables.get(registry.doors.entities[i]).name == "OpenDoor") {
                continue;
            }
            if(registry.doors.components[i].room != RoomType::None && !registry.doors.components[i].isPrev && registry.interactables.get(registry.doors.entities[i]).name != "LockedDoor") {
                if (map.currRoom.type != RoomType::TutorialRoom1) {
                    soundPlayer->playDoorOpenSound();
                }
                registry.interactables.get(registry.doors.entities[i]).name = "OpenDoor";
                registry.interactables.get(registry.doors.entities[i]).interactType = InteractableType::ActionInteractable;
            }
        }
    }
}

void MapSystem::handleMapRequests()
{
    if (registry.mapRequests.components.size() > 0)
    {
        auto &r = registry.mapRequests.components[0];
        if (r.requestType == MapRequestType::ChangeRoom)
        {
            changeRoom(r.type, r.doorIndex);
        }
        else if (r.requestType == MapRequestType::RestartGame)
            resetMap();
        else if (r.requestType == MapRequestType::NewGame)
            newMap();
        registry.mapRequests.clear();
    }
}

void clearRoomActors()
{
    for (Entity ent : registry.enemies.entities)
    {
        if (!registry.deleteds.has(ent))
            registry.deleteds.emplace(ent);
    }
    for (Entity ent : registry.walls.entities)
    {
        if (!registry.deleteds.has(ent) && !registry.bounds.has(ent))
        {
            registry.deleteds.emplace(ent);
        }
    }
    for (Entity ent : registry.playerBullets.entities)
    {
        if (!registry.deleteds.has(ent))
            registry.deleteds.emplace(ent);
    }
    for (Entity ent : registry.enemyBullets.entities)
    {
        if (!registry.deleteds.has(ent))
            registry.deleteds.emplace(ent);
    }
    for (Entity ent : registry.interactables.entities)
    {
        if (!registry.deleteds.has(ent) && !registry.doors.has(ent) && !registry.menuUIs.has(ent))
            registry.deleteds.emplace(ent);
    }
    for (Entity ent : registry.critters.entities)
    {
        if (!registry.deleteds.has(ent))
            registry.deleteds.emplace(ent);
    }
    for (Entity ent : registry.interactIndicators.entities)
    {
        if (!registry.deleteds.has(ent))
            registry.deleteds.emplace(ent);
    }
    registry.invincibles.clear();
    registry.spriteTimers.clear();
    registry.dashes.clear();

    registry.emitParticles.emplace(Entity(),ParticleRequestType::ClearParticles, ParticleProps(),0.0f, 0);
}

void MapSystem::changeRoom(RoomType type, int doorIndex)
{
    std::vector<Door> &doors = registry.doors.components;
    Map &map = registry.maps.components[0];
    Door &door = doors[doorIndex];

    if (door.room == RoomType::None || (!map.currRoom.cleared && registry.interactables.get(registry.doors.entities[doorIndex]).name.compare("OpenDoor") != 0))
        return;

    //play door close sound
    soundPlayer->playDoorCloseSound();

    if (map.currRoom.type != RoomType::TutorialRoom1 && map.currRoom.type != RoomType::TutorialRoom2) {
        map.roomsTraversed++;
    }

    //update Map Region
    if (map.currRoom.type == TutorialRoom2) {
        map.currRegion = Biology; //Go to bio region at end of tutorial
    } else if (map.currRoom.type == BossRoom && map.currRegion == Biology) {
        map.currRegion = Hifi;
    }

    SoundType old_s = roomTypeToMusic.at(map.currRoom.type);

    // move player to the starting side of the room
    Entity &playerEntity = registry.players.entities[0];
    Motion &playerMotion = registry.motions.get(playerEntity);

    // index of door to spawn at
    int spawnIndex = doorIndex == 0 ? 2 : doorIndex == 1 ? 3
                                      : doorIndex == 2   ? 0
                                                         : 1;
    vec2 spawnPosition = (doors[spawnIndex].startPos + doors[spawnIndex].endPos) / 2.0f;
    playerMotion.position = spawnPosition;

    // clear enemies and obstacles
    clearRoomActors();

    // change current room in the map
    map.currRoom = Room();
    assert(door.room != RoomType::None);
    map.currRoom.preset = getRoomPreset(door.room, door.isLocked);
    map.currRoom.type = door.room;


    SoundType s = roomTypeToMusic.at(map.currRoom.type);
    if (s != old_s) {
        if (s == SoundType::normalBGM) {
            soundPlayer->playNextMusic();
        } else if (s == SoundType::bossBGM) {
            soundPlayer->playBossMusic(0);
        } else if (s == SoundType::specialBGM) {
            soundPlayer->playSpecialMusic(0);
        }
    }
    // randomize the doors other than the one you came from
    doors[spawnIndex].room = doors[doorIndex].room;
    doors[spawnIndex].isPrev = true;
    doors[spawnIndex].isLocked = false;
    registry.interactables.get(registry.doors.entities[spawnIndex]).name = "PrevDoor";
    registry.doorSymbols.get(registry.doorSymbols.entities[spawnIndex]).doorType = roomTypeToSymbols.at(doors[spawnIndex].room);
    registry.interactables.get(registry.doors.entities[spawnIndex]).interactType = InteractableType::DialogueInteractable;

    int lockedRooms = 0;
    bool excludeNone = false;
    for (int i = 0; i < doors.size(); i++)
    {
        // reset previous room type
        if (i == spawnIndex)
            continue;
        registry.interactables.get(registry.doors.entities[i]).name = "ClosedDoor";
        registry.interactables.get(registry.doors.entities[i]).interactType = InteractableType::DialogueInteractable;
        Door &d = registry.doors.components[i];
        DoorSymbol &ds = registry.doorSymbols.components[i];
        d.reset();

        d.room = getRandomRoomType(excludeNone, map.roomsTraversed);
        if (lockedRooms + excludeNone < 2 && hasLocked(d.room,map.roomsTraversed + 1)) {
            //have a chance of spawning locked rooms
            d.isLocked = Random::Float() < 0.3f; //probability of 30% of being locked
        }
        if (d.room == RoomType::None) {
            excludeNone = true;
            registry.interactables.get(registry.doors.entities[i]).name = "EmptyDoor";
            registry.interactables.get(registry.doors.entities[i]).interactType = InteractableType::DialogueInteractable;
        }
        if (d.isLocked) {
            registry.interactables.get(registry.doors.entities[i]).name = "LockedDoor";
            registry.interactables.get(registry.doors.entities[i]).interactType = InteractableType::DialogueInteractable;
            lockedRooms++;
        }
        ds.doorType = roomTypeToSymbols.at(d.room);
    }
}

void MapSystem::resetMap() {
    registry.maps.components[0].currRoom = Room();
    clearRoomActors();
    soundPlayer->playSpecialMusic(0);
}

void MapSystem::newMap()
{
    IOState& iostate = registry.ioStates.components[0];
    if (iostate.tutorialOn) {
        // preset doors for first tutorial room
        for (int i = 0; i < 4; i++)
        {
            Door& d = registry.doors.components[i];
            d.reset();
            registry.doorSymbols.get(registry.doorSymbols.entities[i]).doorType = roomTypeToSymbols.at(d.room);
            registry.interactables.get(registry.doors.entities[i]).name = "EmptyDoor";
            registry.interactables.get(registry.doors.entities[i]).interactType = InteractableType::DialogueInteractable;
        }
        registry.doors.components[2].room = RoomType::TutorialRoom2; // bottom door
        registry.doorSymbols.get(registry.doorSymbols.entities[2]).doorType = roomTypeToSymbols.at(registry.doors.components[2].room);
        registry.interactables.get(registry.doors.entities[2]).name = "ClosedTutorialDoor";
        registry.interactables.get(registry.doors.entities[2]).interactType = InteractableType::DialogueInteractable;

        Map& map = registry.maps.components[0];
        map.currRegion = MapRegion::Tutorial;
        map.roomsTraversed = 0;

        map.currRoom = Room();
        map.currRoom.preset = TutorialRoom1Preset;
        map.currRoom.type = TutorialRoom1;
    }
    else {
        Map& map = registry.maps.components[0];
        map.currRegion = MapRegion::Tutorial;
        map.roomsTraversed = 0;

        bool excludeNone = false;
        for (int i = 0; i < 4; i++)
        {
            Door& d = registry.doors.components[i];
            d.reset();
            d.room = getRandomRoomType(excludeNone, map.roomsTraversed);
            if (d.room == RoomType::None)
                excludeNone = true;

            registry.doorSymbols.get(registry.doorSymbols.entities[i]).doorType = roomTypeToSymbols.at(d.room);
            registry.interactables.get(registry.doors.entities[i]).name = "ClosedDoor";
            registry.interactables.get(registry.doors.entities[i]).interactType = InteractableType::DialogueInteractable;
        }

        // temporarily set start room to empty, create pop console
        map.currRoom = Room();
        map.currRoom.preset = getRoomPreset(RoomType::RestRoom,false);
        // createBibleTree(renderer, vec2(700, 500));
        // createGardener(renderer, vec2(1000, 700));
        // createEnemy(renderer, vec2(1000, 500), EnemyType::EasyEnemySkull);
        // createEnemy(renderer, vec2(1000, 300), EnemyType::TestRevampedEnemy);
        // createRamStick(renderer, vec2(500, 500));
        // createPushConsole(renderer, vec2(500, 500), {dashUpA, dashCDRDownA, dmgUpM});

    }
}

