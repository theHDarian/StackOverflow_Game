#include "map_system.hpp"
#include "components.hpp"
#include "tiny_ecs_registry.hpp"
#include "world_init.hpp"
#include "utils/random.hpp"
#include "SDL.h"
#include "components/presets/room_presets.hpp"

MapSystem::MapSystem()
{
    if (registry.maps.components.size() == 0)
    {
        registry.maps.emplace(Entity());
    }
}
MapSystem::~MapSystem()
{
    if (backgroundMusic != nullptr)
    {
        Mix_FreeMusic(backgroundMusic);
    }
    registry.maps.clear();
}
void MapSystem::init(RenderSystem *renderer)
{
    this->renderer = renderer;
    assert(registry.maps.components.size() > 0);
    loadMusic();
    WindowState &ws = registry.windowStates.components[0];
    // create door colliders
    float offsetRightLeft = 100;
    float offsetTop = 60;
    float offsetBot = 90;
    float doorWidthX = 200;
    float doorWidthY = 80.0f;
    createDoor(renderer, {ws.width / 2 - doorWidthX / 2, offsetTop}, {ws.width / 2 + doorWidthX / 2, offsetTop});
    createDoor(renderer, {ws.width - offsetRightLeft, ws.height / 2 - doorWidthY / 2}, {ws.width - offsetRightLeft, ws.height / 2 + doorWidthY / 2});
    createDoor(renderer, {ws.width / 2 - doorWidthX / 2, ws.height - offsetBot}, {ws.width / 2 + doorWidthX / 2, ws.height - offsetBot});
    createDoor(renderer, {offsetRightLeft, ws.height / 2 - doorWidthY / 2}, {offsetRightLeft, ws.height / 2 + doorWidthY / 2});
    resetMap();
}

void MapSystem::loadMusic()
{
    Sound roomMusic[] = {
        {SoundType::normalBGM, audio_path("room/game-music-loop-1.wav"), 0.1f, -1},
        {SoundType::normalBGM, audio_path("room/game-music-loop-2.wav"), 0.1f, -1},
        {SoundType::normalBGM, audio_path("room/game-music-loop-3.wav"), 0.1f, -1},
        {SoundType::normalBGM, audio_path("room/game-music-loop-4.wav"), 1.0f, -1},
    };

    // Loading music and sounds with SDL
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        fprintf(stderr, "Failed to initialize SDL Audio: %s\n", SDL_GetError());
        throw std::runtime_error("Failed to initialize SDL Audio");
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
    {
        fprintf(stderr, "Failed to open audio device: %s\n", Mix_GetError());
        throw std::runtime_error("Failed to open audio device");
    }

    for (Sound &track : roomMusic)
    {
        normalRoomMusic.push_back(track);
        std::cout << "Loaded background music " << track.path << std::endl;
    }

    Sound boss = {SoundType::bossBGM, audio_path("boss/boss-music.wav"), 0.2f, -1};
    bossRoomMusic.push_back(boss);

    Sound special = {SoundType::specialBGM, audio_path("special/special-room.wav"), 0.2f, -1};
    specialRoomMusic.push_back(special);

    Sound &currentBGM = normalRoomMusic[0];

    backgroundMusic = Mix_LoadMUS(currentBGM.path.c_str());
    if (!backgroundMusic)
    {
        fprintf(stderr, "Failed to load background music: %s\n", Mix_GetError());
    }

    nextMusic();
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

        for (auto& b : map.currRoom.preset.bosses) {
            createBoss(renderer, std::get<vec2>(b) * vec2(wS.width, wS.height), std::get<BossType>(b));
        }
        map.currRoom.preset.bosses = {};
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

void MapSystem::changeRoom(RoomType type, int doorIndex)
{
    std::vector<Door> &doors = registry.doors.components;
    Map &map = registry.maps.components[0];
    Door &door = doors[doorIndex];
    if (door.room == RoomType::None || !map.currRoom.cleared)
        return;

    nextMusic();

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
    }
}

void MapSystem::resetMap()
{
    clearRoomActors();

    bool excludeNone = false;
    for (Door &d : registry.doors.components)
    {
        d.room = randomRoomType(excludeNone);
        if (d.room == RoomType::None)
            excludeNone = true;
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
    std::vector<RoomPreset> presets = roomDirectory.at(RoomType::EnemyRoomDash);
    RoomPreset randomPreset = Random::ListItem(presets);
    map.currRoom.preset = randomPreset;
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
    Sound &currentBGM = normalRoomMusic[currMusicIndex];
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