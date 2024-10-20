#include "map_components.hpp"
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
    WindowState& wS = registry.windowStates.components[0];
	if (registry.enemies.size() < 1) {
		for (int i = 0; i < rand()%10 + 1; i++) {
			EnemyBehavior behavior = Random::Float() < 0.5f ? EnemyBehavior::RANDOM : EnemyBehavior::FOLLOW_PLAYER;
			Entity e = createEnemy(renderer, vec2(wS.width * Random::Float(),wS.height * Random::Float()), vec2(0, 0), behavior);
		}
	}
}