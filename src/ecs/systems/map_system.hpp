#pragma once
#include "components/map_components.hpp"
#include "render_system.hpp"
#include "SDL_mixer.h"

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

    void loadMusic();

    Mix_Music* backgroundMusic;
    std::vector<Sound> normalRoomMusic;
    std::vector<Sound> bossRoomMusic;
    std::vector<Sound> specialRoomMusic;

    void nextMusic();
};