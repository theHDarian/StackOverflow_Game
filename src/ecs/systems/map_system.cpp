#include "map_system.hpp"
#include "components.hpp"
#include "tiny_ecs_registry.hpp"
#include "world_init.hpp"
#include "utils/random.hpp"
#include "SDL.h"

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
    float offset = 20;
    float doorWidth = 30;
    createDoor(renderer, {ws.width / 2 - doorWidth / 2, offset}, {ws.width / 2 + doorWidth / 2, offset});
    createDoor(renderer, {ws.width - offset, ws.height / 2 - doorWidth / 2}, {ws.width - offset, ws.height / 2 + doorWidth / 2});
    createDoor(renderer, {ws.width / 2 - doorWidth / 2, ws.height - offset}, {ws.width / 2 + doorWidth / 2, ws.height - offset});
    createDoor(renderer, {offset, ws.height / 2 - doorWidth / 2}, {offset, ws.height / 2 + doorWidth / 2});
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
    // switch rooms if needed

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

    WindowState &wS = registry.windowStates.components[0];
    // std::cout << " enemy size " << registry.enemies.size() << std::endl;
    if (registry.enemies.size() == 0)
    {
        //createEnemy(renderer, vec2(wS.width * Random::Float(), wS.height * Random::Float()), EnemyType::TestRevampedEnemy);
        //createEnemy(renderer, vec2(wS.width * Random::Float(),wS.height * Random::Float()), EnemyType::EasyEnemySentry);
        //createEnemy(renderer, vec2(wS.width / 2,wS.height / 2), EnemyType::BossBigC);
        //createEnemy(renderer, vec2(wS.width * Random::Float(),wS.height * Random::Float()), EnemyType::MediumEnemyCharge);
        // createEnemy(renderer, vec2(wS.width * Random::Float(),wS.height * Random::Float()), EnemyType::OneBee);
        // createEnemy(renderer, vec2(wS.width * Random::Float(),wS.height * Random::Float()), EnemyType::OneBee);
        createEnemy(renderer, vec2(wS.width * Random::Float(),wS.height * Random::Float()), EnemyType::OneBee);
        createEnemy(renderer, vec2(wS.width * Random::Float(),wS.height * Random::Float()), EnemyType::OneBee);
        createEnemy(renderer, vec2(wS.width * Random::Float(),wS.height * Random::Float()), EnemyType::OneBee);
        createEnemy(renderer, vec2(wS.width * Random::Float(),wS.height * Random::Float()), EnemyType::OneBee);

        
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
RoomType getRandomRoomType(bool includeNone)
{
    int r = Random::Float() * 4;
    if (r > 3)
        return RoomType::TreasureRoom;
    if (r > 2)
        return RoomType::RestRoom;
    if (r > 1)
        return RoomType::EnemyRoom;

    return includeNone ? RoomType::None : RoomType::EnemyRoom;
}

void MapSystem::changeRoom(RoomType type, int doorIndex)
{
    std::vector<Door> &doors = registry.doors.components;
    Door &door = doors[doorIndex];
    if (door.room == RoomType::None)
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
    Map &map = registry.maps.components[0];
    Entity ent = registry.maps.entities[0];
    map.currRoom.type = type;
    map.currRoom.variant = 0;
    map.currRoom.cleared = false;
    map.currRoom.timeElapsed = 0;

    map.roomsTraversed++;

    // copy room type
    doors[spawnIndex].room = doors[doorIndex].room;
    doors[spawnIndex].isPrev = true;

    bool includeNone = true;
    for (int i = 0; i < doors.size(); i++)
    {
        // reset previous room type
        if (i == spawnIndex)
            continue;

        Door &d = registry.doors.components[i];
        d.room = getRandomRoomType(includeNone);
        d.isPrev = false;
        if (d.room == RoomType::None)
            includeNone = false;
    }
}

void MapSystem::resetMap()
{
    clearRoomActors();

    bool includeNone = true;
    for (Door &d : registry.doors.components)
    {
        d.room = getRandomRoomType(includeNone);
        if (d.room == RoomType::None)
            includeNone = false;
    }

    for (Door &d : registry.doors.components)
    {
        d.isPrev = false;
    }

    Map &map = registry.maps.components[0];
    map.currRegion = MapRegion::Tutorial;
    map.roomsTraversed = 0;

    map.currRoom.type = RoomType::EnemyRoom;
    map.currRoom.variant = 0;
    map.currRoom.cleared = false;
    map.currRoom.timeElapsed = 0;

    // createBigC(renderer, vec2(600, 600));
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