#pragma once
#include "components/map_components.hpp"
#include "render_system.hpp"

class MapSystem {
    public:
        MapSystem();
        ~MapSystem();
        void init(RenderSystem* renderer);
        void step(float elapsed_ms);
    private:
        RenderSystem* renderer;
        void resetMap();
        void changeRoom(RoomType type,int doorIndex);
};