#pragma once
#include "components/map_components.hpp"
#include "render_system.hpp"
#include "SDL_mixer.h"


class SoundSystem;

class MapSystem {
    public:
        MapSystem();
        ~MapSystem();
        static void clearRoomActors();
        void init(RenderSystem *renderer, SoundSystem *soundPlayer_arg);
        void step(float elapsed_ms);
    private:
        RenderSystem* renderer;
        void resetMap();
        void changeRoom(RoomType type,int doorIndex);
        void handleMapRequests();

        void newMap();
        void updateBgPositions();
        void decorateRoom();

    void loadMusic();

    SoundSystem* soundPlayer;
};
