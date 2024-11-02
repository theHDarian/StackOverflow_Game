//
// Created by sky on 2024-11-01.
//

#ifndef SOUND_SYSTEM_H
#define SOUND_SYSTEM_H
#include "SDL_mixer.h"
#include "components.hpp"
#endif //SOUND_SYSTEM_H

class SoundSystem {
public:
    SoundSystem();
    ~SoundSystem();

    bool increaseVolume();

    bool decreaseVolume();

    bool setVolume(float volume);

    void nextMusic();

    void playPlayerHurtSound();
    void playPlayerDashSound();

    void playPlayerShootSound(float ticks);

    void playEnemyShootSound(int sfxNumber, int loops);

    void playDoorOpenSound();

    float volume;

private:
    int currMusicIndex;
    Mix_Music* backgroundMusic;
    std::vector<Sound> normalRoomMusic;
    std::vector<Sound> bossRoomMusic;
    std::vector<Sound> specialRoomMusic;
    Mix_Chunk* playerHurtSound;
    Mix_Chunk* playerShootSound;
    Mix_Chunk* playerDashSound;

    // Mix_Chunk* playerDeathSound;
    // Mix_Chunk* enemyHurtSound;
    // Mix_Chunk* enemyDeathSound;
    std::vector<Mix_Chunk*> enemyShootSounds;

    Mix_Chunk* doorOpenSound;

    void loadMusic();
    void loadSoundEffects();


};
