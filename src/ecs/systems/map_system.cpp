#include "map_system.hpp"
#include "components.hpp"
#include "tiny_ecs_registry.hpp"
#include "world_init.hpp"
#include "utils/random.hpp"
#include "SDL.h"
#include "components/presets/room_presets.hpp"
#include "sound_system.hpp"
#include <glm/gtx/compatibility.hpp>


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

void MapSystem::step(float elapsed_ms)
{
    Map &map = registry.maps.components[0];
    map.currRoom.timeElapsed += elapsed_ms / 1000.0f;

    handleMapRequests();

    // WindowState &wS = registry.windowStates.components[0];
    WindowState &wS = registry.windowStates.components[0];
    vec2 roomCenter = vec2(wS.width,wS.height)/2.f;
    //vec2 roomStartPos = roomCenter-map.currRoom.preset.roomSize/2.f;
    //vec2 roomEndPos = roomCenter+map.currRoom.preset.roomSize/2.f;

    if (map.currRoom.timeElapsed > map.currRoom.preset.spawnDelay) {
        Entity bossEnemy;
        for (auto &e : map.currRoom.preset.enemies)
        {
            vec2 pos = glm::lerp(map.currRoom.roomStart, map.currRoom.roomEnd,std::get<vec2>(e));
            if (std::get<EnemyType>(e) == EnemyType::EnemyTwinLaserVertical1 || std::get<EnemyType>(e) == EnemyType::EnemyHifiTwinLaserHorizontal1) {
                createEnemyGroup(renderer,pos, std::get<EnemyType>(e));
            } else {
                Entity enemy = createEnemy(renderer, pos, std::get<EnemyType>(e));
                if (registry.bosses.has(enemy)) {
                    bossEnemy = enemy;
                }
            }
        }
        map.currRoom.preset.enemies = {};

        if (map.currRoom.cleared || map.currRoom.type == TutorialRoom1) {
            for (auto &e : map.currRoom.preset.interactables)
            {
                vec2 pos = glm::lerp(map.currRoom.roomStart, map.currRoom.roomEnd,std::get<vec2>(e));
                createInteractable(renderer, pos, std::get<RoomInteractable>(e).item, std::get<RoomInteractable>(e).pushConsoleEffects);
            }
            map.currRoom.preset.interactables = {};

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

    // set room to cleared if all enemies are defeated
    if (!map.currRoom.cleared && registry.enemies.entities.empty() && map.currRoom.preset.enemies.empty() && map.currRoom.type != TutorialRoom1)
    {
        map.currRoom.cleared = true;
        if (map.currRoom.type == BossRoom) {
            soundPlayer->playNextMusic();
            CameraRequest& cameraReq2 = registry.cameraRequests.emplace_with_duplicates(registry.maps.entities[0]);
            cameraReq2.type = CameraRequestType::ChangeTargetAndZoom;
            cameraReq2.newTarget = registry.players.entities[0];
            cameraReq2.transitionTime = 500;
            cameraReq2.newZoom = 1.0f;
        }
        if (map.currRoom.type == BossRoom || map.currRoom.type == EnemyRoom || map.currRoom.type == TutorialRoom2)
            registry.uiRequests.insert(registry.maps.entities[0], {UIRequestType::RoomClear});
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
            newMap(r.region, r.type);
        registry.mapRequests.clear();
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
    } else if (map.currRoom.type == BossRoom && map.currRegion == Biology) {
        map.currRegion = Physics;
        map.directory = getDirectory( map.currRegion );
        map.roomsTraversed = 1; //reset rooms traversed to reset difficulty for region
    }

    SoundType old_s = roomTypeToMusic.at(map.currRoom.type);

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
    map.currRoom.preset = getRoomPreset(door.room, door.isLocked);
    updateBgPositions();
    map.currRoom.type = door.room;

    vec2 spawnPosition = (doors[spawnIndex].startPos + doors[spawnIndex].endPos) / 2.0f;
    playerMotion.position = spawnPosition;
    playerMotion.velocity = vec2(0);

    // clear enemies and obstacles
    clearRoomActors();

    SoundType s = roomTypeToMusic.at(type);
    if (s != old_s) {
        if (s == SoundType::normalBGM) {
            std::cout << "Playing normal music" << std::endl;
            // soundPlayer->playNextMusic();
            auto& req = registry.soundRequests.emplace(Entity());
            req.type = SoundType::normalBGM;
        } else if (s == SoundType::bossBGM) {
            std::cout << "Playing boss music" << std::endl;
            // soundPlayer->playBossMusic(0);
            auto& req = registry.soundRequests.emplace(Entity());
            req.type = SoundType::bossBGM;
        } else if (s == SoundType::specialBGM) {
            std::cout << "Playing special music" << std::endl;
            // soundPlayer->playSpecialMusic(0);
            auto& req = registry.soundRequests.emplace(Entity());
            req.type = SoundType::specialBGM;
        }
    } else {
        std::cout << "Not changing music" << std::endl;
    }


    // randomize the doors other than the one you came from
    doors[spawnIndex].room = doors[doorIndex].room;
    doors[spawnIndex].isPrev = true;
    doors[spawnIndex].isLocked = false;
    registry.interactables.get(registry.doors.entities[spawnIndex]).name = "PrevDoor";
    registry.animations.get(registry.doorSymbols.entities[spawnIndex]).frame = 5;
    registry.interactables.get(registry.doors.entities[spawnIndex]).interactType = InteractableType::DialogueInteractable;

    int lockedRooms = 0;
    bool excludeNone = true;
    std::vector<RoomType> newRooms = getRandomRoomTypes(excludeNone, map.roomsTraversed);
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
        registry.animations.get(registry.doorSymbols.entities[i]).frame = roomTypeToSymbols.at(d.room);
    }

    decorateRoom();
}

void MapSystem::decorateRoom() {
    // Create floor decorations
    Map& map = registry.maps.components[0];
    WindowState& ws = registry.windowStates.components[0];
    std::string filename = (map.currRegion == Biology) ? "bio_floor_addons" : (map.currRegion == Physics) ? "hifi_floor_addons" : "tutorial_floor_addons";
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
            d.reset();
            registry.animations.get(registry.doorSymbols.entities[i]).frame = roomTypeToSymbols.at(d.room);
            registry.interactables.get(registry.doors.entities[i]).name = "EmptyDoor";
            registry.interactables.get(registry.doors.entities[i]).interactType = InteractableType::DialogueInteractable;
        }
        registry.doors.components[2].room = RoomType::TutorialRoom2; // bottom door
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
        else {
            if (map.currRegion == Biology) {
                map.currRoom.preset = BossRoomBee;
            }
            else {
                map.currRoom.preset = ScientistBossRoom;
            }
        }
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
        }

        updateBgPositions();
        //createProp3D(renderer, vec2(700, 300), "controls.png", vec2(576, 300), vec2(280, 80), 100);
        // createBibleTree(renderer, vec2(700, 500));
        // createGardener(renderer, vec2(1000, 700));
        // createEnemy(renderer, vec2(1000, 500), EnemyType::EnemySkull);
        // createEnemy(renderer, vec2(1000, 300), EnemyType::EnemyPufferfish);
        // createRamStick(renderer, vec2(500, 500));
        // createPushConsole(renderer, vec2(500, 500), {dashUpA, dashCDRDownA, dmgUpM});
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

