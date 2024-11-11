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
    WindowState &ws = registry.windowStates.components[0];
    // create door colliders
    float offsetRightLeft = ws.width / (6 * ws.width/(float)ws.height);
    float offsetTop = ws.height / (6 * ws.width / (float)ws.height);
    float offsetBot = ws.height / (6 * ws.width / (float)ws.height);
    float doorWidthX = 200;
    float doorWidthY = 80.0f;
    createDoor(renderer, {ws.width / 2 - doorWidthX / 2, offsetTop + 10}, {ws.width / 2 + doorWidthX / 2, offsetTop + 10});
    createDoor(renderer, {ws.width - offsetRightLeft - 10, ws.height / 2 - doorWidthY / 2}, {ws.width - offsetRightLeft - 10, ws.height / 2 + doorWidthY / 2});
    createDoor(renderer, {ws.width / 2 - doorWidthX / 2, ws.height - offsetBot - 10}, {ws.width / 2 + doorWidthX / 2, ws.height - offsetBot - 10});
    createDoor(renderer, {offsetRightLeft + 10, ws.height / 2 - doorWidthY / 2}, {offsetRightLeft + 10, ws.height / 2 + doorWidthY / 2});

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
    if (registry.enemies.entities.empty() && map.currRoom.preset.enemies.empty() && map.currRoom.type != TutorialRoom1)
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
    // lame fix for splitting bullet persisting after reset
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
        return "none.png";
    } else if (type == RoomType::TutorialRoom1 || type == RoomType::TutorialRoom2) {
        return "door_symbol_tutorial.png";
    } else {
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

    //play door close sound
    soundPlayer->playDoorCloseSound();


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
    if (door.room == RoomType::TutorialRoom2) {
        map.currRoom.preset = TutorialRoom2Preset;
    }
    else {
        std::vector<RoomPreset> presets = roomDirectory.at(door.room);
        RoomPreset randomPreset = Random::ListItem(presets);
        map.currRoom.preset = randomPreset;
    }
    map.roomsTraversed++;
    map.currRoom.type = door.room;
    if (map.currRoom.type == RoomType::BossBigCRoom) {
        soundPlayer->playBossMusic(0);
    } else if (map.currRoom.type == RoomType::TreasureRoom) {
        soundPlayer->playSpecialMusic(0);
    }
    else {
        soundPlayer->playNextMusic();
    }


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

    IOState& iostate = registry.ioStates.components[0];
    if (iostate.tutorialOn) {
        // preset doors for first tutorial room
        for (int i = 0; i < 4; i++)
        {
            Door& d = registry.doors.components[i];
            d.isPrev = false;
            d.room = RoomType::None;
            registry.renderRequests.get(registry.doorSymbols.entities[i]).texture_name = getSymbol(d.room);
        }
        registry.doors.components[2].room = RoomType::TutorialRoom2; // bottom door
        registry.renderRequests.get(registry.doorSymbols.entities[2]).texture_name = getSymbol(registry.doors.components[2].room);

        Map& map = registry.maps.components[0];
        map.currRegion = MapRegion::Tutorial;
        map.roomsTraversed = 0;

        map.currRoom = Room();
        map.currRoom.preset = TutorialRoom1Preset;
        map.currRoom.type = TutorialRoom1;
    }
    else {
        bool excludeNone = false;
        for (int i = 0; i < 4; i++)
        {
            Door& d = registry.doors.components[i];
            d.room = randomRoomType(excludeNone);
            if (d.room == RoomType::None)
                excludeNone = true;

            registry.renderRequests.get(registry.doorSymbols.entities[i]).texture_name = getSymbol(d.room);
        }

        for (Door& d : registry.doors.components)
        {
            d.isPrev = false;
        }

        Map& map = registry.maps.components[0];
        map.currRegion = MapRegion::Tutorial;
        map.roomsTraversed = 0;

        /*
        // set initial room to enemy
        map.currRoom = Room();
        std::vector<RoomPreset> presets = roomDirectory.at(RoomType::EnemyRoomBee);
        RoomPreset randomPreset = Random::ListItem(presets);
        map.currRoom.preset = randomPreset;
        */
        map.currRoom = Room();
        std::vector<RoomPreset> presets = roomDirectory.at(RoomType::RestRoom);
        RoomPreset randomPreset = Random::ListItem(presets);
        map.currRoom.preset = randomPreset;
    }

    // clear ongoing dialogue to prepare for next
    // perhaps reset should be a main-system level thing to easily tell each system to reset itself?
    GameState& gameState = registry.gameStates.components[0];
    DialogueLines& lines = registry.dialogueLines.components[0];
    lines = DialogueLines();
    gameState.dialogueScene = false;
}

void MapSystem::nextMusic()
{
    int nextMusicIndex = rand() % normalRoomMusic.size();
    if (nextMusicIndex == currMusicIndex)
    {
        return;
    }
    else
    {
        currMusicIndex = nextMusicIndex;
    }
    SoundRequest &currentBGM = normalRoomMusic[currMusicIndex];
    Mix_FreeMusic(backgroundMusic);
    Mix_Music *newbackgroundMusic = Mix_LoadMUS(currentBGM.path.c_str());
    Mix_FadeInMusic(newbackgroundMusic, currentBGM.loops, 1000);
    backgroundMusic = newbackgroundMusic;
    if (!backgroundMusic)
    {
        fprintf(stderr, "Failed to load background music: %s\n", Mix_GetError());
    }
    int volume = currentBGM.volume * MIX_MAX_VOLUME;
    Mix_VolumeMusic(volume);
}
