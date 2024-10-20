#include "map_system.hpp"
#include "components.hpp"
#include "tiny_ecs_registry.hpp"
#include "world_init.hpp"
#include "utils/random.hpp"


MapSystem::MapSystem() {
    if (registry.maps.components.size() == 0) {
        registry.maps.emplace(Entity());
    }
}
MapSystem::~MapSystem() {
    registry.maps.clear();
}
void MapSystem::init(RenderSystem* renderer) {
    this->renderer = renderer;
    assert(registry.maps.components.size() > 0);
    Map& map = registry.maps.components[0];
    WindowState& ws = registry.windowStates.components[0];
    map.currRegion = MapRegion::Tutorial;
    map.roomsTraversed = 0;

    //create door colliders
    float offset = 20;
    float doorWidth = 30;
    createDoor(renderer, {ws.width/2 - doorWidth/2,offset}, {ws.width/2 + doorWidth/2, offset});
	createDoor(renderer, {ws.width - offset,ws.height/2 - doorWidth/2}, {ws.width - offset, ws.height/2 + doorWidth/2});
	createDoor(renderer, {ws.width/2 - doorWidth/2, ws.height - offset}, {ws.width/2 + doorWidth/2, ws.height - offset});
	createDoor(renderer, {offset, ws.height/2 - doorWidth/2}, {offset,ws.height/2 + doorWidth/2});
    registry.doors.components[0].room = RoomType::None;
    registry.doors.components[1].room = RoomType::EnemyRoom;
    registry.doors.components[2].room = RoomType::None;
    registry.doors.components[3].room = RoomType::None;

    map.currRoom = {
        .type = RoomType::EnemyRoom,
        .variant = 0,
        .cleared = false
    };
}

void MapSystem::step(float elapsed_ms) {
    Map& map = registry.maps.components[0];
    map.currRoom.timeElapsed += elapsed_ms / 1000.0f;
    //switch rooms if needed
    for (auto& r : registry.changeRoomRequests.components) {
        changeRoom(r.type,r.spawnSide);
    }
    registry.changeRoomRequests.clear();

    //spawn enemy based on current time 
    WindowState& wS = registry.windowStates.components[0];
	if (registry.enemies.size() < 1) {
		for (int i = 0; i < rand()%10 + 1; i++) {
			EnemyBehavior behavior = Random::Float() < 0.5f ? EnemyBehavior::RANDOM : EnemyBehavior::FOLLOW_PLAYER;
			Entity e = createEnemy(renderer, vec2(wS.width * Random::Float(),wS.height * Random::Float()), vec2(0, 0), behavior);
		}
	}
}

vec2 getSpawnPosition(Side side, Motion& motion) {
    WindowState& ws = registry.windowStates.components[0];
    vec2 pos = {ws.width / 2, ws.height / 2};
    switch(side) {
        case Side::Left:
            pos.x = motion.scale.x / 2;
            break;
        case Side::Top:
            pos.y = motion.scale.y / 2; // going down is positive coordinates
            break;
        case Side::Right:
            pos.x = ws.width - motion.scale.x / 2;
            break;
        case Side::Bottom:
            pos.y = ws.height - motion.scale.y / 2;
            break;
        default: break;
    }
    return pos;
}

void MapSystem::changeRoom(RoomType type, Side playerSpawnSide) {
    printf("Changing Room %c, Side:%c \n", type, playerSpawnSide);
    //move player to the starting side of the room
    Entity& playerEntity = registry.players.entities[0];
    Motion& playerMotion = registry.motions.get(playerEntity);
    std::vector<Door>& doors = registry.doors.components;
        
    playerMotion.position = getSpawnPosition(playerSpawnSide,playerMotion);    

    //clear enemies and obstacles
    for (Entity ent : registry.enemies.entities) {
        if (!registry.deleteds.has(ent))
            registry.deleteds.emplace(ent);
    }
    for (Entity ent : registry.walls.entities) {
        if (!registry.deleteds.has(ent) && !registry.bounds.has(ent)) {
            registry.deleteds.emplace(ent);
        }
    }

    //change current room in the map
    Map& map = registry.maps.components[0];
    Entity ent = registry.maps.entities[0];
    map.currRoom = {
        .type = type,
        .variant = 0,
        .cleared = false    
    };
    map.roomsTraversed++;

    //replace door objects in registry
    Door newDoors[4] = {
        {.room = RoomType::None,.isPrev = false },
        {.room = RoomType::EnemyRoom,.isPrev = false },
        {.room = RoomType::None,.isPrev = false },
        {.room = RoomType::None,.isPrev = false }
    };
    for (int i = 0; i < 4; i++) {
        doors[i] = newDoors[i];
    }
    //block the side where player spawns
    if (playerSpawnSide == Side::Top) doors[0].isPrev = true;
    else if (playerSpawnSide == Side::Right) doors[1].isPrev = true;
    else if (playerSpawnSide == Side::Bottom) doors[2].isPrev = true;
    else if (playerSpawnSide == Side::Left) doors[3].isPrev = true;
}