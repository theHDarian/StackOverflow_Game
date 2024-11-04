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
    void step(float elapsed_ms);

    SoundSystem();
    ~SoundSystem();

    bool increaseVolume();

    bool decreaseVolume();

    bool setVolume(float volume);

    void playNextMusic();

    void playNextMusic(int songIndex);

    void playBossMusic(int songIndex);

    void playSpecialMusic(int songIndex);

    void playPlayerHurtSound();
    void playPlayerDashSound();

    void playPlayerShootSound(float ticks);

    void playEnemyShootSound(int sfxNumber, int loops);

    void playDoorOpenSound();

    void playDoorCloseSound();

    void playIncomingDialogueSound();

    bool isPlayingIncomingDialogueSound();

    void stopIncomingDialogueSound();

    void playNextDialogueSound();

    void stopNextDialogueSound();

    float volume;

private:
    int currMusicIndex;
    Mix_Music* backgroundMusic;
    std::vector<SoundRequest> normalRoomMusic;
    std::vector<SoundRequest> bossRoomMusic;
    std::vector<SoundRequest> specialRoomMusic;
    Mix_Chunk* playerHurtSound;
    Mix_Chunk* playerShootSound;
    Mix_Chunk* playerDashSound;

    Mix_Chunk* incomingDialogueSound;
    Mix_Chunk* nextDialogueSound;

    // Mix_Chunk* playerDeathSound;
    // Mix_Chunk* enemyHurtSound;
    // Mix_Chunk* enemyDeathSound;
    std::vector<Mix_Chunk*> enemyShootSounds;

    Mix_Chunk* doorOpenSound;
    Mix_Chunk* doorCloseSound;

    void loadMusic();
    void loadSoundEffects();


};
