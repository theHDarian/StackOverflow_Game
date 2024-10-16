#include "map_components.hpp"
#include "map_system.hpp"
#include "components.hpp"
#include "tiny_ecs_registry.hpp"


MapSystem::MapSystem() {
    if (registry.maps.components.size() == 0) {
        registry.maps.emplace(Entity());
    }
}
MapSystem::~MapSystem() {
    registry.maps.clear();
}
void MapSystem::init() {
    assert(registry.maps.components.size() > 0);
    Map& map = registry.maps.components[0];
    map.currRegion = MapRegion::Tutorial;
    map.roomsTraversed = 0;
    map.currRoom = {
        .type = RoomType::EnemyRoom,
        .variant = 0,
        .cleared = false,
        .leftDoor {.room = RoomType::None },
        .rightDoor {.room = RoomType::None },
        .topDoor {.room = RoomType::None },
        .bottomDoor {.room = RoomType::None }
    };
}

void MapSystem::step(float elapsed_ms) {
    Map& map = registry.maps.components[0];
    //switch rooms if needed
    //spawn enemy based on current time 
}