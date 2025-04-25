#include "map_system.hpp"
#include "components.hpp"
#include "tiny_ecs_registry.hpp"
#include "world_init.hpp"
#include "utils/random.hpp"
#include "SDL.h"
#include "components/presets/room_presets.hpp"
#include "sound_system.hpp"
#include <glm/gtx/compatibility.hpp>

const float ELITE_SPAWN_CHANCE = 0.2f;

MapSystem::MapSystem()
{
    if (registry.maps.components.size() == 0)
    {
        auto& map = registry.maps.emplace(Entity());
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

void SpawnEnemiesInList(std::vector<std::tuple<EnemyType,vec2>> enemies, Entity& bossEnemy, RenderSystem *renderer, bool isElite = false)
{
    Map& map = registry.maps.components[0];
    for (auto &e : enemies)
    {
        vec2 location = std::get<vec2>(e);
        location.x = location.x == random_float ? Random::Float() : location.x;
        location.y = location.y == random_float ? Random::Float() : location.y;
        vec2 pos = glm::lerp(map.currRoom.roomStart, map.currRoom.roomEnd, location);
        if (std::get<EnemyType>(e) == EnemyType::EnemyTwinLaserVertical1 || std::get<EnemyType>(e) == EnemyType::EnemyHifiTwinLaserHorizontal1) {
            createEnemyGroup(renderer,pos, std::get<EnemyType>(e));
        } else {
            Entity enemy = createEnemy(renderer, pos, std::get<EnemyType>(e));
            if (registry.bosses.has(enemy)) {
                bossEnemy = enemy;
            }
            registry.spawnings.emplace(enemy);
            if (isElite) {
                auto& elt = registry.elites.emplace(enemy);
                int randomlvl = Random::Int((registry.maps.components[0].currRegion) * 1.5) + 1;
                elt.eliteLevel = randomlvl > 9 ? randomlvl : 9;
            }
        }
    }
}

void SpawnEnemiesInList(std::vector<std::tuple<EnemyType,vec2>> enemies, RenderSystem *renderer)
{
    Entity bossEnemy;
    SpawnEnemiesInList( enemies, bossEnemy, renderer);
}


void MapSystem::step(float elapsed_ms)
{
    Map &map = registry.maps.components[0];
    map.currRoom.timeElapsed += elapsed_ms / 1000.0f;

    handleMapRequests();

    WindowState &wS = registry.windowStates.components[0];
    vec2 roomCenter = vec2(wS.width,wS.height)/2.f;
    
    if ((map.currRoom.timeElapsed > map.currRoom.preset.spawnDelay /*|| map.currRoom.currentWave == 0*/) && !map.currRoom.preset.enemies.empty()) {
        Entity bossEnemy;
        SpawnEnemiesInList( map.currRoom.preset.enemies.front(), bossEnemy, renderer);
        map.currRoom.preset.enemies.pop_front();
        map.currRoom.timeElapsed = 0;
        map.currRoom.currentWave++;
        if (map.currRoom.currentWave > 1) {
            soundPlayer->playAlarmSound(1);
        }

        // look at boss and zoom out when boss spawns
        if (registry.motions.has(bossEnemy)) {
            CameraRequest& cameraReq2 = registry.cameraRequests.emplace_with_duplicates(registry.maps.entities[0]);
            cameraReq2.type = CameraRequestType::ChangeTargetAndZoom;
            cameraReq2.newTarget = bossEnemy;
            cameraReq2.transitionTime = 500;
            cameraReq2.newZoom = 0.75f;
            CameraRequest& cameraReq3 = registry.cameraRequests.emplace_with_duplicates(registry.maps.entities[0]);
            cameraReq3.type = CameraRequestType::HoldCamera;
            cameraReq3.transitionTime = 1000;
            CameraRequest& cameraReq4 = registry.cameraRequests.emplace_with_duplicates(registry.maps.entities[0]);
            cameraReq4.type = CameraRequestType::ChangeTarget;
            cameraReq4.newTarget = registry.players.entities[0];
            cameraReq4.transitionTime = 500;
        }
    }

    if (map.currRoom.preset.hasElite) {
        if (map.currRoom.eliteTimer < map.currRoom.timeElapsed || (map.currRoom.preset.enemies.empty() && registry.enemies.entities.empty()) ) {
            Entity bossEnemy;
            SpawnEnemiesInList( Random::ListItem(eliteEnemies.at(map.currRegion)), bossEnemy, renderer, true);
            map.currRoom.preset.hasElite = false;
            map.currRoom.spawnedElite = true;
            soundPlayer->playAlarmSound(3);
        }
    }

    if (!map.currRoom.enemiesToSpawn.empty()) {
        SpawnEnemiesInList(map.currRoom.enemiesToSpawn, renderer);
        map.currRoom.enemiesToSpawn.clear();
    }

    if (map.currRoom.cleared || map.currRoom.type == TutorialRoom1) {
        for (auto& e : map.currRoom.preset.interactables)
        {
            vec2 pos = glm::lerp(map.currRoom.roomStart, map.currRoom.roomEnd, std::get<vec2>(e));
            createInteractable(renderer, pos, std::get<RoomInteractable>(e).item, std::get<RoomInteractable>(e).pushConsoleEffects);
        }
        map.currRoom.preset.interactables = {};

    }

    // set room to cleared if all enemies are defeated
    if (!map.currRoom.cleared && registry.enemies.entities.empty() && map.currRoom.preset.enemies.empty() && map.currRoom.type != TutorialRoom1 && !map.currRoom.preset.hasElite)
    {
        map.currRoom.cleared = true;
        if (map.currRoom.type == BossRoom) {
            CameraRequest& cameraReq2 = registry.cameraRequests.emplace_with_duplicates(registry.maps.entities[0]);
            cameraReq2.type = CameraRequestType::ChangeTargetAndZoom;
            cameraReq2.newTarget = registry.players.entities[0];
            cameraReq2.transitionTime = 500;
            cameraReq2.newZoom = 1.0f;
        }
        if (map.currRoom.type == BossRoom || map.currRoom.type == EnemyRoom || map.currRoom.type == TutorialRoom2) {
            if (soundPlayer->currentMusicState != MusicState::FadingOut) {
                soundPlayer->FadeOutMusic(2000);
            }
            UIRequest& uiReq = registry.uiRequests.emplace_with_duplicates(registry.maps.entities[0]);
            uiReq.type = UIRequestType::DisplayFlashMessage;
            if (map.currRoom.type == BossRoom) {
                uiReq.text = "Boss Defeated";
            } else {
                uiReq.text = "Room Cleared";
            }
        }
            
    } else if (!map.currRoom.cleared && registry.enemies.entities.empty() && map.currRoom.type != TutorialRoom1) {
        //spawn next wave by setting timeElapsed to spawnDelay
        map.currRoom.timeElapsed += map.currRoom.preset.spawnDelay;
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

    if (soundPlayer->currentMusicState == MusicState::FadingOut && !soundPlayer->isPlayingMusic()) {
        soundPlayer->playSpecialMusic();
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
            newMap(r.region, r.type);
        registry.mapRequests.clear();
    }
}

void MapSystem::clearRoomInteractables() {
    for (Entity ent : registry.interactables.entities)
    {
        if (!registry.deleteds.has(ent) && !registry.doors.has(ent) && !registry.menuUIs.has(ent))
            registry.deleteds.emplace(ent);
    }
    for (Entity ent : registry.walls.entities)
    {
        if (!registry.deleteds.has(ent) && !registry.bounds.has(ent))
        {
            registry.deleteds.emplace(ent);
        }
    }
    for (Entity ent : registry.critters.entities)
    {
        if (!registry.deleteds.has(ent))
            registry.deleteds.emplace(ent);
    }
}

void MapSystem::clearRoomActors()
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
    for (Entity ent : registry.stackAddNotifs.entities)
    {
        if (!registry.deleteds.has(ent))
            registry.deleteds.emplace(ent);
    }
    for (Entity ent : registry.objects.entities)
    {
        if (!registry.deleteds.has(ent))
            registry.deleteds.emplace(ent);
    }
    for (Entity ent : registry.backgrounds.entities)
    {
        if (!registry.deleteds.has(ent) && !registry.roomSizeScaleds.has(ent) )
        {
            registry.deleteds.emplace(ent);
        }
    }
    registry.invincibles.clear();
    registry.spriteTimers.clear();
    registry.dashes.clear();

    registry.emitParticles.emplace(Entity(),ParticleRequestType::ClearParticles, ParticleProps(),0.0f, 0);
}

void MapSystem::changeRoom(RoomType type, int doorIndex)
{
    registry.gameStates.components[0].resetRoom = true;
    
    std::vector<Door> &doors = registry.doors.components;
    Map &map = registry.maps.components[0];
    Door &door = doors[doorIndex];

    if (door.room == RoomType::None || (!map.currRoom.cleared && registry.interactables.get(registry.doors.entities[doorIndex]).name.compare("OpenDoor") != 0))
        return;

    //play door close sound
    soundPlayer->playDoorCloseSound();

    if (map.currRoom.type != RoomType::TutorialRoom1) {
        map.roomsTraversed++;
        registry.gameReports.components[0].roomsCleared++;
    }

    //update Map Region
    if (map.currRoom.type == TutorialRoom2) {
        map.currRegion = Biology; //Go to bio region at end of tutorial
    } else if (map.currRoom.type == BossRoom) {
        map.currRegion = (MapRegion)(map.currRegion + 1);
        map.directory = getDirectory( map.currRegion );
    }

    int roomTraversed = map.roomsTraversed;
    MapRegion region = map.currRegion;

    if (door.room == RoomType::BossRoom && map.currRegion != MapRegion::Military) {
        region = (MapRegion) (region + 1);
        roomTraversed = 1;
    }

    // move player to the starting side of the room
    Entity &playerEntity = registry.players.entities[0];
    Motion &playerMotion = registry.motions.get(playerEntity);

    // index of door to spawn at
    int spawnIndex = doorIndex == 0 ? 2 : doorIndex == 1 ? 3
                                        : doorIndex == 2 ? 0
                                                         : 1;

    // change current room in the map
    map.currRoom = Room();
    assert(door.room != RoomType::None);
    map.currRoom.preset = door.preset;
    map.currRoom.type = door.room;

    // randomize the doors other than the one you came from
    doors[spawnIndex].room = doors[doorIndex].room;
    doors[spawnIndex].isPrev = true;
    doors[spawnIndex].isLocked = false;
    registry.interactables.get(registry.doors.entities[spawnIndex]).name = "PrevDoor";
    registry.animations.get(registry.doorSymbols.entities[spawnIndex]).frame = 5;
    registry.interactables.get(registry.doors.entities[spawnIndex]).interactType = InteractableType::DialogueInteractable;

    int lockedRooms = 0;
    int noneRooms = 0;
    bool excludeNone = false;
    std::vector<RoomType> newRooms = getRandomRoomTypes(excludeNone, map.roomsTraversed);

    // first count the number of none rooms
    for (int i = 0; i < doors.size(); i++)
    {
        if (newRooms[i] == RoomType::None) {
            noneRooms++;
        }
    }

    for (int i = 0; i < doors.size(); i++)
    {
        // reset counters
        registry.interactables.get(registry.doors.entities[i]).timer = registry.interactables.get(registry.doors.entities[i]).base;

        // reset previous room type
        if (i == spawnIndex)
            continue;
        registry.interactables.get(registry.doors.entities[i]).name = "ClosedDoor";
        registry.interactables.get(registry.doors.entities[i]).interactType = InteractableType::DialogueInteractable;
        Door &d = registry.doors.components[i];
        DoorSymbol &ds = registry.doorSymbols.components[i];
        d.reset();

        d.room = newRooms[i];

        if (d.room == RoomType::BossRoom && ((spawnIndex + 2) % 4 != i)) {
            d.room = RoomType::None;
        }

        if ((lockedRooms + noneRooms < 2) && !hasUnlocked(d.room,map.roomsTraversed + 1) && hasLocked(d.room,map.roomsTraversed + 1)) {
            //if there are no unlocked rooms but still are locked rooms, spawn locked rooms
            std:: cout << "Spawning locked room" << std::endl;
            d.isLocked = true;
        }
        else if ((lockedRooms + noneRooms < 2) && (hasLocked(d.room,map.roomsTraversed + 1) && hasUnlocked(d.room,map.roomsTraversed + 1))) { //check if next room has locked
            //have a chance of spawning locked rooms
            d.isLocked = Random::Float() < 0.3f; //probability of 30% of being locked
        } else {
            if (d.room != RoomType::None && d.room != RoomType::BossRoom) {
                std::cout << "ran out of rooms for room type: " << d.room << std::endl;
                d.room = RoomType::EnemyRoom;
            }
        }

        if (d.room == RoomType::None) {
            registry.interactables.get(registry.doors.entities[i]).name = "EmptyDoor";
            registry.interactables.get(registry.doors.entities[i]).interactType = InteractableType::DialogueInteractable;
        }
        if (d.isLocked) {
            registry.interactables.get(registry.doors.entities[i]).name = "LockedDoor";
            registry.interactables.get(registry.doors.entities[i]).interactType = InteractableType::DialogueInteractable;
            lockedRooms++;
        }

        d.preset = getRoomPreset(d.room, region, d.isLocked, roomTraversed);

        if ( d.room == RoomType::EnemyRoom && Random:: Float() < ELITE_SPAWN_CHANCE) {
            d.preset.hasElite = true;
        }

        registry.animations.get(registry.doorSymbols.entities[i]).frame = roomTypeToSymbols.at(d.room);
    }

    updateBgPositions();

    vec2 spawnPosition = (doors[spawnIndex].startPos + doors[spawnIndex].endPos) / 2.0f;
    playerMotion.position = spawnPosition;
    playerMotion.velocity = vec2(0);

    // clear enemies and obstacles
    clearRoomActors();

    SoundType song = roomTypeToMusic.at(type);
        if (song == SoundType::normalBGM && soundPlayer->currentMusicState != MusicState::PlayingNormal) {
            std::cout << "Playing normal music" << std::endl;
            // soundPlayer->playNextMusic();
            auto& req = registry.soundRequests.emplace(Entity());
            req.type = SoundType::normalBGM;
        } else if (song == SoundType::bossBGM && soundPlayer->currentMusicState != MusicState::PlayingBoss) {
            std::cout << "Playing boss music" << std::endl;
            // soundPlayer->playBossMusic(0);
            auto& req = registry.soundRequests.emplace(Entity());
            req.type = SoundType::bossBGM;
        } else if (song == SoundType::specialBGM && soundPlayer->currentMusicState != MusicState::PlayingSpecial) {
            std::cout << "Playing special music" << std::endl;
            // soundPlayer->playSpecialMusic(0);
            auto& req = registry.soundRequests.emplace(Entity());
            req.type = SoundType::specialBGM;
        }

    decorateRoom();
    UIRequest& uiReq = registry.uiRequests.emplace_with_duplicates(registry.maps.entities[0]);
    uiReq.type = UIRequestType::DisplayFlashMessage;
    uiReq.text = map.currRoom.preset.ID;

    Player& player = registry.players.get(playerEntity);
    // refresh player Dash charges and cooldown
    player.currDashCharges = getModifiedValue( PlayerNumDash, player.maxDashCharges);
    player.currDashCooldown = getModifiedValue( PlayerDashRecharge, player.baseDashCDR);
}

void MapSystem::decorateRoom() {
    // Create floor decorations
    Map& map = registry.maps.components[0];
    WindowState& ws = registry.windowStates.components[0];
    std::string filename = (map.currRegion == Biology) ? "bio_floor_addons" : (map.currRegion == Physics) ? "hifi_floor_addons" : (map.currRegion == Mining) ? "mining_floor_addons" : (map.currRegion == Medical) ? "medical_floor_addons" : "tutorial_floor_addons";
    vec2 placements = vec2(floor(0.8 * map.currRoom.preset.roomSize.x / 192.f), floor( 0.8 * map.currRoom.preset.roomSize.y / 192.f));
    vec2 dividers = vec2(0.9090 * map.currRoom.preset.roomSize.x / placements.x, 0.9090 * map.currRoom.preset.roomSize.y / placements.y);
    vec2 roomOffset = vec2(-map.currRoom.preset.roomSize.x / 2.2f, -map.currRoom.preset.roomSize.y / 2.2f) + vec2(ws.width, ws.height) / 2.f;
    vec2 wiggle = (dividers - vec2(192)) / 2.f;
    for (int i = 0; i < placements.x; i++) {
        for (int j = 0; j < placements.y; j++) {
            // Controls how many will be spawned
            if (rand() % 100 < 70) continue;
            vec2 midPosition = roomOffset + dividers / 2.f + dividers * vec2(i,j);
            midPosition += wiggle * vec2((rand()%100 - 50) / 50.f, (rand() % 100 - 50) / 50.f);
            createFloorDeco(renderer, midPosition, filename);

            if (map.currRegion == Biology && (rand() % 100 < 20) && map.currRoom.type != RoomType::EnemyRoom && map.currRoom.type != RoomType::BossRoom) {
                createCritter(renderer, midPosition);
            }

        }
    }

}
 
void MapSystem::resetMap() {
    registry.maps.components[0].currRoom = Room();
    clearRoomActors();
}

void MapSystem::newMap(MapRegion region, RoomType roomType)
{
    IOState& iostate = registry.ioStates.components[0];
    Map& map = registry.maps.components[0];
    map.roomsTraversed = 0;
    if (iostate.tutorialOn) {
        // preset doors for first tutorial room
        for (int i = 0; i < 4; i++)
        {
            Door& d = registry.doors.components[i];
            d.preset.ID = "";
            d.reset();
            registry.animations.get(registry.doorSymbols.entities[i]).frame = roomTypeToSymbols.at(d.room);
            registry.interactables.get(registry.doors.entities[i]).name = "EmptyDoor";
            registry.interactables.get(registry.doors.entities[i]).interactType = InteractableType::DialogueInteractable;
        }
        registry.doors.components[2].room = RoomType::TutorialRoom2; // bottom door
        registry.doors.components[2].preset = TutorialRoom2Preset;
        registry.animations.get(registry.doorSymbols.entities[2]).frame = roomTypeToSymbols.at(registry.doors.components[2].room);
        registry.interactables.get(registry.doors.entities[2]).name = "ClosedTutorialDoor";
        registry.interactables.get(registry.doors.entities[2]).interactType = InteractableType::DialogueInteractable;

        map.currRegion = MapRegion::Tutorial;

        map.currRoom = Room();
        map.currRoom.preset = TutorialRoom1Preset;
        updateBgPositions();
        map.currRoom.type = TutorialRoom1;
        map.directory = getDirectory(map.currRegion);

        registry.motions.get(registry.players.entities[0]).position =
            glm::lerp(map.currRoom.roomStart, map.currRoom.roomEnd, vec2(0.25, 0.2));
    }
    else {
        // CHANGE THIS FOR M4:
        int lockedRooms = 0;
        bool excludeNone = false;
        map.currRoom = Room();
        map.currRegion = region;
        map.currRoom.type = roomType;
        if (roomType == RoomType::TutorialRoom) {
            map.currRoom.preset = StartingRoom;
        }
        else if (roomType == RoomType::Testing) {
            map.currRoom.preset = TestRoom;
        }
        else {
            if (map.currRegion == Biology) {
                std::vector<RoomPreset> bioBossRooms = {BossRoomCrab, BossRoomBee};
                map.currRoom.preset = Random::ListItem( bioBossRooms);
            }
            else if (map.currRegion == Mining) {
                std::vector<RoomPreset> miningBossRooms = {BossRoomMole, BossRoomWorm};
                map.currRoom.preset = Random::ListItem( miningBossRooms);
                // map.currRoom.preset = MiningEnemyRoomAvenue;
            }
            else if (map.currRegion == Medical) {
                 map.currRoom.preset = ScientistBossRoom;
            }
            else if (map.currRegion == Physics) {
                std::vector<RoomPreset> physicsBossRooms = {BossRoomBigC, BossRoomMultiCube};
                map.currRoom.preset = Random::ListItem( physicsBossRooms);
                // map.currRoom.preset = BossRoomMultiCube;
                // map.currRoom.preset = TreasureRoomChoice5;
            } else {
                map.currRoom.preset = ScientistBossRoom;
            }
            SoundRequest& req = registry.soundRequests.emplace(Entity());
            req.type = SoundType::bossBGM;
            InteractableRequest &req2 = registry.interactableRequests.emplace(Entity());
            req2.type = InteractableRequestType::AddEffect;
            req2.effects = {numBulletsUp, numBulletsUp, dmgUp,dmgUp, dmgUp, fireRateUp,fireRateUp,fireRateUp, bulletSpeedUp, bulletSpeedUp, bulletSpeedUp, accuracyUp,accuracyUp,accuracyUp};
        }

        // InteractableRequest &req2 = registry.interactableRequests.emplace(Entity());
        // req2.type = InteractableRequestType::AddEffect;
        // req2.effects = {numBulletsUp, numBulletsUp,numBulletsUp, dmgUp, dmgUp, dmgUp, fireRateUp,fireRateUp,fireRateUp, bulletSpeedUp, bulletSpeedUp, bulletSpeedUp, accuracyUp,accuracyUp,accuracyUp};

        InteractableRequest &extendstack = registry.interactableRequests.emplace(Entity());
        extendstack.type = InteractableRequestType::ExtendStack;
        extendstack.choice = 4*max(0,((int)map.currRegion - 1));

        map.directory = getDirectory(map.currRegion);
        std::vector<RoomType> newRooms = getRandomRoomTypes(excludeNone, map.roomsTraversed);
        for (int i = 0; i < 4; i++)
        {
            Door& d = registry.doors.components[i];
            d.reset();
            d.room = newRooms[i];

            if (lockedRooms + excludeNone < 2 && !hasUnlocked(d.room,map.roomsTraversed + 1) && hasLocked(d.room,map.roomsTraversed + 1)) {
                //if there are no unlocked rooms but still are locked rooms, spawn locked rooms
                std:: cout << "Spawning locked room" << std::endl;
                d.isLocked = true;
            }
            else if (lockedRooms + excludeNone < 2 && (hasLocked(d.room,map.roomsTraversed + 1) && hasUnlocked(d.room,map.roomsTraversed + 1))) { //check if next room has locked
                //have a chance of spawning locked rooms
                d.isLocked = Random::Float() < 0.3f; //probability of 30% of being locked
            } else {
                if ((d.room != RoomType::None && lockedRooms + excludeNone < 2) && d.room != RoomType::BossRoom) {
                    //d.room = RoomType::EnemyRoom;
                }
            }
            registry.animations.get(registry.doorSymbols.entities[i]).frame = roomTypeToSymbols.at(d.room);
            registry.interactables.get(registry.doors.entities[i]).name = "ClosedDoor";
            if (d.room == RoomType::None) {
                excludeNone = true;
                registry.interactables.get(registry.doors.entities[i]).name = "EmptyDoor";
            }
            if (d.isLocked) {
                registry.interactables.get(registry.doors.entities[i]).name = "LockedDoor";
                lockedRooms++;
            }
            registry.interactables.get(registry.doors.entities[i]).interactType = InteractableType::DialogueInteractable;

            d.preset = getRoomPreset(d.room, map.currRegion, d.isLocked);

        }
        //map.currRoom.preset = TreasureRoom1;
        updateBgPositions();
        //
        // InteractableRequest& req = registry.interactableRequests.emplace(Entity());
        // req.type = InteractableRequestType::AddEffect;
        // req.effects = {key, dashUp, dashCDRDownA, dmgUpM, dashCDRDownA, dmgUpM, SniperPower, dashUp, key, dashUp, dashCDRDownA, dmgUpM, dashCDRDownA, dmgUpM, SniperPower, dashUp,};

        //createProp3D(renderer, vec2(700, 300), "controls.png", vec2(576, 300), vec2(280, 80), 100);
        // createBibleTree(renderer, vec2(700, 500));
        // createGardener(renderer, vec2(1000, 700));
        // createEnemy(renderer, vec2(1000, 500), EnemyType::EnemySkull);
        // createEnemy(renderer, vec2(1000, 300), EnemyType::EnemyPufferfish);
        // createRamStick(renderer, vec2(500, 500));
         //createPushConsole(renderer, vec2(500, 500), {dashUp, dashUp, dashUp, dmgDown, dmgDown, dashUp, dmgDown2, dmgDown2, dashUp, dmgDown, dashUp, dmgDown});
        //createWishGranter(renderer, vec2(500,500));
        //createEnemy(renderer, vec2(500, 500), ScientistBoss);
    }
    decorateRoom();
}

void MapSystem::updateBgPositions() {
    //update floor scale
    Map& map = registry.maps.components[0];
    WindowState &windowState = registry.windowStates.components[0];
    vec2 roomCenter = vec2(windowState.width,windowState.height)/2.f;

    struct WallPos
    {
        vec2 colliderStart;
        vec2 colliderEnd;
        vec2 spritePosition;
        vec2 spriteScale;
    };
    vec2 floorScale = map.currRoom.preset.roomSize;

    vec2 floorPosition = roomCenter;
    float wallThickness = 100.f;
    std::vector<WallPos> wallPositions = {
    {
        // top
        vec2(floorPosition.x - floorScale.x * 1.1 / 2.f, floorPosition.y - floorScale.y / 2.f),
        vec2(floorPosition.x + floorScale.x * 1.1 / 2.f, floorPosition.y - floorScale.y / 2.f),
        vec2(floorPosition.x, floorPosition.y + floorScale.y),
        vec2(floorScale.x * 1.1, wallThickness)},
        {// right
        vec2(floorPosition.x + floorScale.x / 2.f - wallThickness * 1.1 / 2 * min(floorScale.x/1920.f, 1.f) - 20, floorPosition.y - floorScale.y * 2 / 2.f),
        vec2(floorPosition.x + floorScale.x / 2.f - wallThickness * 1.1 / 2 * min(floorScale.x / 1920.f, 1.f) - 20, floorPosition.y + floorScale.y * 2 / 2.f),
        vec2(floorPosition.x + floorScale.x / 2 * 1.1, floorPosition.y), // not sure why 1.1, is magic number rn
        vec2(floorScale.y * 2, wallThickness)},
            {// bottom
        vec2(floorPosition.x - floorScale.x * 1.1 / 2.f, floorPosition.y + floorScale.y / 2.f - wallThickness / 2.f * min(floorScale.y / 1080.f, 1.f) + 10),
        vec2(floorPosition.x + floorScale.x * 1.1 / 2.f, floorPosition.y + floorScale.y / 2.f - wallThickness / 2.f * min(floorScale.y / 1080.f, 1.f) + 10),
        vec2(floorPosition.x, floorPosition.y - floorScale.y),
        vec2(floorScale.x * 1.1, wallThickness)},
        {// left
        vec2(floorPosition.x - floorScale.x / 2.f + wallThickness * 1.1 / 2 * min(floorScale.x / 1920.f, 1.f) + 20, floorPosition.y - floorScale.y * 2 / 2.f),
        vec2(floorPosition.x - floorScale.x / 2.f + wallThickness * 1.1 / 2 * min(floorScale.x / 1920.f, 1.f) + 20, floorPosition.y + floorScale.y * 2 / 2.f),
        vec2(floorPosition.x - floorScale.x / 2 * 1.1, floorPosition.y),
        vec2(floorScale.y * 2, wallThickness)},
    };
    char sides[4] = {'B','R','T','L'};

    int wallIndex = 0;
    int symbolIndex = 0;
    float doorwidth = 100.f;
    int doorSpriteIndex = 0;
    int doorIndex = 0;
    for(Entity e : registry.roomSizeScaleds.entities) {
        RoomSizeScaled& rss = registry.roomSizeScaleds.get(e);
        Motion& motion = registry.motions.get(e);

        if (rss.name == "Floor") {
            motion.scale = map.currRoom.preset.roomSize;
        } else if (rss.name == "WallThickness") {
            motion.scale = map.currRoom.preset.roomSize + vec2(200, 200/1.33);
        } else if (rss.name == "DoorSymbol") {
            char side = sides[symbolIndex];
            vec2 position = vec2(windowState.width,windowState.height)/2.f;
            float of = 200.f;
            if(side == 'T') position += vec2(0,map.currRoom.preset.roomSize.y/2+of);
            if (side == 'R') position += vec2(map.currRoom.preset.roomSize.x/2+of,0);
            if (side == 'B') position += vec2(0,-map.currRoom.preset.roomSize.y/2-of);
            if (side == 'L') position += vec2(-map.currRoom.preset.roomSize.x/2-of,0);
            motion.position = position;
            symbolIndex++;
            for (Entity &d1 : registry.doors.entities)
            {
                if (registry.doors.get(d1).side == side)
                {
                    Door &door = registry.doors.get(d1);
                    WindowState &ws = registry.windowStates.components[0];
                    if (!registry.textRenderRequests.has(d1))
                    {
                        if (door.preset.ID != "") {
                            if (!registry.gameOverlayUITexts.has(d1))
                            {
                                GameOverlayUIText &text = registry.gameOverlayUITexts.emplace(d1);
                            }
                            TextRenderRequest &textRequest = registry.textRenderRequests.emplace(d1);
                            if (door.isPrev) {
                                textRequest.text = "Previous Room";
                            } else {
                                if (door.room == TreasureRoom && !door.isLocked) {
                                    textRequest.text = "Treasure Room";
                                } else {
                                    textRequest.text = door.preset.ID;
                                }
                            }
                            textRequest.x = position.x;
                            textRequest.y = ws.height - position.y;

                            if (!door.isPrev) {
                                if (side == 'T') {
                                    textRequest.y += 80;
                                } else if (side == 'R') {
                                    textRequest.y -= 85;
                                    textRequest.x -= 50;
                                } else if (side == 'B') {
                                    textRequest.y -= 85;
                                } else if (side == 'L') {
                                    textRequest.y -= 85;
                                    textRequest.x += 50;
                                }
                            }

                            if (side == 'L') {
                                textRequest.alignment = TextAlignment::RightAlign;
                            } else if (side == 'R') {
                                textRequest.alignment = TextAlignment::LeftAlign;
                            } else {
                                textRequest.alignment = TextAlignment::CenteredAlign;
                            }
                            textRequest.scale = 0.4f;
                            textRequest.color = COLOR_WHITE;
                            textRequest.bottomLeftBound = vec2(0);
                            textRequest.topRightBound = vec2(ws.width, ws.height);
                            std::cout << "door text added: " << textRequest.text << std::endl;
                        }
                    } else {
                        TextRenderRequest &textRequest = registry.textRenderRequests.get(d1);
                        textRequest.x = position.x;
                        textRequest.y = ws.height - position.y;
                        if (door.isPrev) {
                            textRequest.text = "Previous Room";
                        } else if (door.room == TreasureRoom && !door.isLocked) {
                            textRequest.text = "Treasure Room";
                        }
                        else {
                            textRequest.text = door.preset.ID;
                        }
                        if (!door.isPrev) {
                            if (side == 'T') {
                                textRequest.y += 80;
                            } else if (side == 'R') {
                                textRequest.y -= 85;
                                textRequest.x -= 50;
                            } else if (side == 'B') {
                                textRequest.y -= 85;
                            } else if (side == 'L') {
                                textRequest.y -= 85;
                                textRequest.x += 50;
                            }
                        }

                        if (side == 'L') {
                            textRequest.alignment = TextAlignment::RightAlign;
                        } else if (side == 'R') {
                            textRequest.alignment = TextAlignment::LeftAlign;
                        } else {
                            textRequest.alignment = TextAlignment::CenteredAlign;
                        }
                        std::cout << "door text updated: " << textRequest.text << std::endl;
                    }
                    break;
                }
            }
        } else if (rss.name == "DoorSprite") {
            float offsetAmount = -40.f;
            vec2 offsetPos = vec2(0);
            vec2 scaleOffset;
            auto& p = wallPositions[doorSpriteIndex];
            vec2 position = p.spritePosition;
            char side = sides[doorSpriteIndex];
            if(side == 'T') offsetPos.y = -offsetAmount;
            if (side == 'R') offsetPos.x = offsetAmount;
            if (side == 'B') offsetPos.y = +offsetAmount;
            if (side == 'L') offsetPos.x = -offsetAmount;
            motion.position = position + offsetPos;
            doorSpriteIndex++;
        } else if (rss.name == "Bound") {
            auto& p = wallPositions[wallIndex];
            motion.position = p.spritePosition;
            motion.scale = p.spriteScale;
            auto & wall = registry.walls.get(e);
            wall.startPosition = p.colliderStart;
		    wall.endPosition = p.colliderEnd;
            wallIndex++;
        } else if (rss.name == "Door") {
            std::vector<std::vector<vec2>> doorPositions = {
                {{ windowState.width / 2 - doorwidth / 2, wallPositions[0].colliderStart.y + 45 }, { windowState.width / 2 + doorwidth / 2, wallPositions[0].colliderStart.y + 45 }},
                {{ wallPositions[1].colliderStart.x - 30,  windowState.height / 2 - doorwidth / 2 }, { wallPositions[1].colliderStart.x - 30,  windowState.height / 2 + doorwidth / 2 }},
                {{ windowState.width / 2 - doorwidth / 2, wallPositions[2].colliderStart.y - 30 }, { windowState.width / 2 + doorwidth / 2, wallPositions[2].colliderStart.y - 30 }},
                {{ wallPositions[3].colliderStart.x + 30,  windowState.height / 2 - doorwidth / 2 }, { wallPositions[3].colliderStart.x + 30,  windowState.height / 2 + doorwidth / 2 }}
            };
            vec2 startPos = doorPositions[doorIndex][0];
            vec2 endPos = doorPositions[doorIndex][1];
            motion.position = (startPos + endPos) / 2.0f;
            motion.scale = vec2(glm::distance(startPos, endPos), 5);
            auto &door = registry.doors.get(e);
            door.startPos = startPos;
            door.endPos = endPos;
            doorIndex++;
        }
    }

    // update room start/end pos based on colliders
    Room& room = registry.maps.components[0].currRoom;
    room.roomStart = vec2(wallPositions[3].colliderStart.x, wallPositions[0].colliderStart.y);
    room.roomEnd = vec2(wallPositions[1].colliderStart.x, wallPositions[2].colliderStart.y);
}

