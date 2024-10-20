#pragma once
#include "components/map_components.hpp"
#include "render_system.hpp"

class MapSystem {
    public:
        MapSystem();
        ~MapSystem();
        void init(RenderSystem* renderer);
        void step(float elapsed_ms);
        void changeRoom(RoomType type,Side playerSpawnSide);

    private:
        RenderSystem* renderer;
};