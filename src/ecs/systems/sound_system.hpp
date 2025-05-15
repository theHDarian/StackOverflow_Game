//
// Created by sky on 2024-11-01.
//

#ifndef SOUND_SYSTEM_H
#define SOUND_SYSTEM_H
#include "SDL_mixer.h"
#include "components.hpp"
#endif // SOUND_SYSTEM_H
const int CROSSFADE_DURATION = 3000; // Duration of the crossfade in milliseconds
enum class MusicState {
    Stopped,
    FadingOut,
    FadingIn,
    PlayingNormal,
    PlayingBoss,
    PlayingSpecial,
    PlayingTitle,
    Crossfading,
};

class SoundSystem
{
public:
    void step(float elapsed_ms);

    SoundSystem();
    ~SoundSystem();

    void playEnemyDeathSound(int sfxNumber);

    void playLaserSound(float time, bool shouldPlayFiringSound = true, const Entity *entity = nullptr);

    void playDiggingSound(float time, const Entity *entity = nullptr);

    void stopDiggingSound();

    bool setMusicVolume(float volume);

    bool setSFXVolume(float volume);

    void playGameOverSound();

    void stopGameOverSound();

    void playPlayerDodgeSound(int sfxNumber);

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

    void playAlarmSound(int sfxNumber = 1);

    void playFanFareSound();

    void playDoorOpenSound();

    void playDoorCloseSound();

    void playIncomingDialogueSound();

    bool isPlayingIncomingDialogueSound();

    void stopIncomingDialogueSound();

    void playNextDialogueSound();

    void stopNextDialogueSound();
    void FadeOutMusic(int ms);

    bool isPlayingMusic();

    float sfxVolume = 0.5f; // note this is overridden by gamestate
    float musicVolume = 0.5f;

    MusicState currentMusicState = MusicState::Stopped;

private:
    int currMusicIndex;
    SoundRequest *currentBGM = nullptr;
    Mix_Music* currentPlayingMusic = nullptr;
    std::vector<SoundRequest> normalRoomMusic;
    std::vector<SoundRequest> bossRoomMusic;
    std::vector<SoundRequest> specialRoomMusic;
    SoundRequest titleScreenMusic;
    Mix_Chunk *playerHurtSound;
    Mix_Chunk *playerShootSound;
    Mix_Chunk *playerDashSound;
    Mix_Chunk *playerZappedSound; // hit by lightning bullet

    Mix_Chunk *incomingDialogueSound;
    Mix_Chunk *nextDialogueSound;
    Mix_Chunk *itemGetSound;
    Mix_Chunk *rareItemGetSound;
    Mix_Chunk *gameOversound;
    Mix_Chunk *laserSound;
    Mix_Chunk *laserLoopSound;
    Mix_Chunk *fanFareSound;
    Mix_Chunk *diggingSound;

    // Mix_Chunk* playerDeathSound;
    // Mix_Chunk* enemyHurtSound;
    // Mix_Chunk* enemyDeathSound;
    std::vector<Mix_Chunk *> enemyShootSounds;
    std::vector<Mix_Chunk *> explosionSounds;
    std::vector<Mix_Chunk *> playerDodgeSounds;
    std::vector<Mix_Chunk *> alarmSounds;
    std::vector<Mix_Chunk *> enemyDeathSounds;

    Mix_Chunk *doorOpenSound;
    Mix_Chunk *doorCloseSound;
    SoundRequest roomMusic[4];

    void loadMusic();
    void loadSoundEffects();
};
