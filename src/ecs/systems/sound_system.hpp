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

    bool setVolume(float volume);

    void playNextMusic();

    void playNextMusic(int songIndex);

    void playTitleMusic();

    void playBossMusic(int songIndex);

    void playSpecialMusic();

    void playSpecialMusic(int songIndex);

    void playPlayerHurtSound();
    void playPlayerZappedSound();
    void playPlayerDashSound();

    void playPlayerShootSound(float ticks);

    void playEnemyShootSound(int sfxNumber, int loops);

    void playItemPickupSound();

    void playRareItemPickupSound();

    void playExplosionSound(int sfxNumber);

    void playDoorOpenSound();

    void playDoorCloseSound();

    void playIncomingDialogueSound();

    bool isPlayingIncomingDialogueSound();

    void stopIncomingDialogueSound();

    void playNextDialogueSound();

    void stopNextDialogueSound();

    float volume = 0.5f; //note this is overridden by gamestate

private:
    int currMusicIndex;
    SoundRequest* currentBGM;
    Mix_Music* backgroundMusic;
    std::vector<SoundRequest> normalRoomMusic;
    std::vector<SoundRequest> bossRoomMusic;
    std::vector<SoundRequest> specialRoomMusic;
    SoundRequest titleScreenMusic;
    Mix_Chunk* playerHurtSound;
    Mix_Chunk* playerShootSound;
    Mix_Chunk* playerDashSound;
    Mix_Chunk* playerZappedSound; //hit by lightning bullet

    Mix_Chunk* incomingDialogueSound;
    Mix_Chunk* nextDialogueSound;
    Mix_Chunk* itemGetSound;
    Mix_Chunk* rareItemGetSound;

    // Mix_Chunk* playerDeathSound;
    // Mix_Chunk* enemyHurtSound;
    // Mix_Chunk* enemyDeathSound;
    std::vector<Mix_Chunk*> enemyShootSounds;
    std::vector<Mix_Chunk*> explosionSounds;

    Mix_Chunk* doorOpenSound;
    Mix_Chunk* doorCloseSound;

    void loadMusic();
    void loadSoundEffects();


};
