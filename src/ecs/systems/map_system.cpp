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
    soundPlayer->nextMusic();
    WindowState &ws = registry.windowStates.components[0];
    // create door colliders
    float offsetRightLeft = 160;
    float offsetTop = 70;
    float offsetBot = 120;
    float doorWidthX = 200;
    float doorWidthY = 80.0f;
    createDoor(renderer, {ws.width / 2 - doorWidthX / 2, offsetTop}, {ws.width / 2 + doorWidthX / 2, offsetTop});
    createDoor(renderer, {ws.width - offsetRightLeft, ws.height / 2 - doorWidthY / 2}, {ws.width - offsetRightLeft, ws.height / 2 + doorWidthY / 2});
    createDoor(renderer, {ws.width / 2 - doorWidthX / 2, ws.height - offsetBot}, {ws.width / 2 + doorWidthX / 2, ws.height - offsetBot});
    createDoor(renderer, {offsetRightLeft, ws.height / 2 - doorWidthY / 2}, {offsetRightLeft, ws.height / 2 + doorWidthY / 2});
    resetMap();
}

void MapSystem::step(float elapsed_ms)
{
    Map &map = registry.maps.components[0];
    map.currRoom.timeElapsed += elapsed_ms / 1000.0f;

    handleMapRequests();

    // spawn enemy based on current time
    WindowState &wS = registry.windowStates.components[0];
    if (map.currRoom.timeElapsed > map.currRoom.preset.spawnDelay)
    {
        for (auto &e : map.currRoom.preset.enemies)
        {
            createEnemy(renderer, std::get<vec2>(e) * vec2(wS.width, wS.height), std::get<EnemyType>(e));
        }
        map.currRoom.preset.enemies = {};

        //spawn treasures
        for(auto& e : map.currRoom.preset.treasures) {
            createEnemyBullet(renderer,std::get<vec2>(e)* vec2(wS.width,wS.height),vec2(0.8,0.8),vec2(0),std::get<AttackData>(e));
        }
        map.currRoom.preset.treasures = {};
    }
    

    // set room to cleared if all enemies are defeated
    if (registry.enemies.entities.empty() && map.currRoom.preset.enemies.empty())
    {
        map.currRoom.cleared = true;
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

    registry.emitParticles.emplace(Entity(), ParticleRequestType::ClearParticles, 0.0f, 0);
}

RoomType randomRoomType(bool excludeNone)
{
    return static_cast<RoomType>(rand() % (excludeNone ? RoomType::None - 1 : RoomType::None));
}
std::string getSymbol(RoomType type) {
    if (type == RoomType::BossBigCRoom) {
        return "door_symbol_boss.png";
    } else if (type == RoomType::TreasureRoom) {
        return "door_symbol_treasure.png";
    } else if (type == RoomType::RestRoom) {
        return "door_symbol_resting.png";
    } else if (type == RoomType::None) {
        return "none"; 
    }else {
        return "door_symbol_enemy.png";
    }
}

void MapSystem::changeRoom(RoomType type, int doorIndex)
{
    std::vector<Door> &doors = registry.doors.components;
    Map &map = registry.maps.components[0];
    Door &door = doors[doorIndex];
    if (door.room == RoomType::None || !map.currRoom.cleared)
        return;
    //play door sound

    // change music
    soundPlayer->nextMusic();

    // move player to the starting side of the room
    Entity &playerEntity = registry.players.entities[0];
    Motion &playerMotion = registry.motions.get(playerEntity);

    // index of door to spawn at
    int spawnIndex = doorIndex == 0 ? 2 : doorIndex == 1 ? 3
                                      : doorIndex == 2   ? 0
                                                         : 1;
    vec2 spawnPosition = (doors[spawnIndex].startPos + doors[spawnIndex].endPos) / 2.0f;
    playerMotion.position = spawnPosition;

    // printf("Changing Room %c, enter door %d spawn at %.1f %.1f\n", type, doorIndex,spawnPosition.x,spawnPosition.y);

    // clear enemies and obstacles
    clearRoomActors();

    // change current room in the map
    map.currRoom = Room();
    assert(door.room != RoomType::None);
    std::vector<RoomPreset> presets = roomDirectory.at(door.room);
    RoomPreset randomPreset = Random::ListItem(presets);
    map.currRoom.preset = randomPreset;
    map.roomsTraversed++;

    // randomize the doors other than the one you came from
    doors[spawnIndex].room = doors[doorIndex].room;
    doors[spawnIndex].isPrev = true;
    registry.renderRequests.get(registry.doorSymbols.entities[spawnIndex]).texture_name = getSymbol(doors[spawnIndex].room);

    bool excludeNone = false;
    for (int i = 0; i < doors.size(); i++)
    {
        // reset previous room type
        if (i == spawnIndex)
            continue;

        Door &d = registry.doors.components[i];
        d.room = randomRoomType(excludeNone);
        d.isPrev = false;
        if (d.room == RoomType::None)
            excludeNone = true;
        registry.renderRequests.get(registry.doorSymbols.entities[i]).texture_name = getSymbol(d.room);
    }
}

void MapSystem::resetMap()
{
    clearRoomActors();

    bool excludeNone = false;
    for (int i  =  0; i < 4; i++)
    {
        Door& d = registry.doors.components[i];
        d.room = randomRoomType(excludeNone);
        if (d.room == RoomType::None)
            excludeNone = true;

        registry.renderRequests.get(registry.doorSymbols.entities[i]).texture_name = getSymbol(d.room);
    }

    for (Door &d : registry.doors.components)
    {
        d.isPrev = false;
    }

    Map &map = registry.maps.components[0];
    map.currRegion = MapRegion::Tutorial;
    map.roomsTraversed = 0;

    // set initial room to enemy
    map.currRoom = Room();
    std::vector<RoomPreset> presets = roomDirectory.at(RoomType::EnemyRoomBee);
    RoomPreset randomPreset = Random::ListItem(presets);
    map.currRoom.preset = randomPreset;
}
