#pragma once
#include "components/map_components.hpp"
#include "render_system.hpp"
#include "SDL_mixer.h"
#include "presets/particle_presets.hpp"
#include "utils/enum_string_mapping.hpp"

class SoundSystem;

class MapSystem {
    public:
        MapSystem();
        ~MapSystem();
        static void clearRoomActors();
        static void clearRoomInteractables();
        void init(RenderSystem *renderer, SoundSystem *soundPlayer_arg);
        void step(float elapsed_ms);
    private:
        RenderSystem* renderer;
        void resetMap();
        void changeRoom(RoomType type,int doorIndex);
        void handleMapRequests();

        void newMap(MapRegion region, RoomType roomType);
        void updateBgPositions();
        void decorateRoom();

    void loadMusic();

    SoundSystem* soundPlayer;
};
