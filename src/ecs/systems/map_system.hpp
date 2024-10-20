#pragma once
#include "components/map_components.hpp"

class MapSystem {
    public:
        MapSystem();
        ~MapSystem();
        void init();
        void step(float elapsed_ms);
        void changeRoom(RoomType type);
};