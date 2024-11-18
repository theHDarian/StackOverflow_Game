//
// Created by sky on 2024-11-01.
//

#include "sound_system.hpp"
#include "components.hpp"
#include "tiny_ecs_registry.hpp"
#include "SDL.h"
#include <SDL_mixer.h>
#include <iostream>

void SoundSystem::step(float elapsed_ms)
{
    for (int i = (int)registry.soundRequests.components.size() - 1; i >= 0; --i)
    {
        SoundRequest& soundRequest = registry.soundRequests.components[i];

            if (soundRequest.type == SoundType::normalBGM)
            {
                playNextMusic();
            }
            else if (soundRequest.type == SoundType::bossBGM)
            {
                if (soundRequest.songIndex == -1)
                    playBossMusic(Random::Int(bossRoomMusic.size()));
                playBossMusic(soundRequest.songIndex);
            }
            else if (soundRequest.type == SoundType::specialBGM)
            {
                if (soundRequest.songIndex == -1)
                    playSpecialMusic();
                playSpecialMusic(soundRequest.songIndex);
            }
            else if (soundRequest.type == SoundType::PlayerHurt)
            {
                playPlayerHurtSound();
            }
            else if (soundRequest.type == SoundType::DashSound)
            {
                playPlayerDashSound();
            }
            else if (soundRequest.type == SoundType::PlayerShoot)
            {
                playPlayerShootSound(soundRequest.ticks);
            }
            else if (soundRequest.type == SoundType::EnemyShoot)
            {
                playEnemyShootSound( soundRequest.songIndex, soundRequest.loops);
            }
            else if (soundRequest.type == SoundType::IncomingDialogue)
            {
                playIncomingDialogueSound();
            }
            else if (soundRequest.type == SoundType::NormalDialogue)
            {
                playNextDialogueSound();
            }
            else if (soundRequest.type == SoundType::DoorOpen)
            {
                playDoorOpenSound();
            } else if (soundRequest.type == SoundType::itemPickup) {
                playItemPickupSound();
            } else if (soundRequest.type == SoundType::rareItemPickup) {
                playRareItemPickupSound();
            }
    }
    registry.soundRequests.clear();
}




SoundSystem::SoundSystem()
{
    loadMusic();
    loadSoundEffects();
    Mix_AllocateChannels(16);
}

SoundSystem::~SoundSystem()
{
    if (backgroundMusic != nullptr)
        Mix_FreeMusic(backgroundMusic);
    if (playerHurtSound != nullptr)
        Mix_FreeChunk(playerHurtSound);
    if (playerDashSound != nullptr)
        Mix_FreeChunk(playerDashSound);
    if (playerShootSound != nullptr)
        Mix_FreeChunk(playerShootSound);
    if (doorOpenSound != nullptr)
        Mix_FreeChunk(doorOpenSound);

    for (int i = 0; i < 4; i++)
    {
        if (enemyShootSounds[i] != nullptr)
            Mix_FreeChunk(enemyShootSounds[i]);
    }


    if (Mix_PlayingMusic())
        Mix_HaltMusic();
    Mix_CloseAudio();
}


void SoundSystem::loadMusic()
{
    SoundRequest roomMusic[] = {
        {SoundType::normalBGM, audio_path("room/game-music-loop-1.wav"), 0.2f, -1},
        {SoundType::normalBGM, audio_path("room/game-music-loop-2.wav"), 0.2f, -1},
        {SoundType::normalBGM, audio_path("room/game-music-loop-3.wav"), 0.2f, -1},
        {SoundType::normalBGM, audio_path("room/game-music-loop-4.wav"), 0.4f, -1}, //ost
    };

    // Ensure SDL audio system is initialized only once
    if (!SDL_WasInit(SDL_INIT_AUDIO)) {
        if (SDL_Init(SDL_INIT_AUDIO) < 0) {
            fprintf(stderr, "Failed to initialize SDL Audio: %s\n", SDL_GetError());
            throw std::runtime_error("Failed to initialize SDL Audio");
        }
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
            fprintf(stderr, "Failed to open audio device: %s\n", Mix_GetError());
            throw std::runtime_error("Failed to open audio device");
        }
    }

    for (SoundRequest &track : roomMusic)
    {
        normalRoomMusic.push_back(track);
        std::cout << "Loaded background music " << track.path << std::endl;
    }

    SoundRequest boss = {SoundType::bossBGM, audio_path("boss/boss-music.wav"), 0.2f, -1};
    bossRoomMusic.push_back(boss);

    SoundRequest special = {SoundType::specialBGM, audio_path("special/special-room.wav"), 0.2f, -1};
    specialRoomMusic.push_back(special);

    currentBGM = &roomMusic[0];

    backgroundMusic = Mix_LoadMUS(currentBGM->path.c_str());
    Mix_VolumeMusic(volume * MIX_MAX_VOLUME * currentBGM->volume);
    if (!backgroundMusic)
    {
        fprintf(stderr, "Failed to load background music: %s\n", Mix_GetError());
        throw std::runtime_error("Failed to load background music");
    }

    playSpecialMusic(0);

}

void SoundSystem::loadSoundEffects() {
    // Loading music and sounds with SDL
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Failed to initialize SDL Audio: %s\n", SDL_GetError());
        throw std::runtime_error("Failed to initialize SDL Audio");
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        fprintf(stderr, "Failed to open audio device: %s\n", Mix_GetError());
        throw std::runtime_error("Failed to open audio device");
    }

    playerHurtSound = Mix_LoadWAV(audio_path("sfx/player_hurtv1.wav").c_str());
    if (!playerHurtSound) {
        fprintf(stderr, "Failed to load player hurt sound: %s\n", Mix_GetError());
        throw std::runtime_error("Failed to load player hurt sound");
    }
    playerHurtSound->volume = 0.7f * MIX_MAX_VOLUME;

    playerDashSound = Mix_LoadWAV(audio_path("sfx/dash.wav").c_str());
    if (!playerDashSound) {
        fprintf(stderr, "Failed to load player dash sound: %s\n", Mix_GetError());
        throw std::runtime_error("Failed to load player dash sound");
    }
    playerDashSound->volume = 0.6f * MIX_MAX_VOLUME;

    playerShootSound = Mix_LoadWAV(audio_path("sfx/player_shoot.wav").c_str());
    if (!playerShootSound) {
        fprintf(stderr, "Failed to load player shoot sound: %s\n", Mix_GetError());
        throw std::runtime_error("Failed to load player shoot sound");
    }
    playerShootSound->volume = 0.4f * MIX_MAX_VOLUME;

    for (int i = 0; i < 4; i++) {
        enemyShootSounds.push_back(Mix_LoadWAV(audio_path("sfx/Shoot_0" + std::to_string(i) + ".wav").c_str()));
        if (!enemyShootSounds[i]) {
            fprintf(stderr, "Failed to load enemy shoot sound: %s\n", Mix_GetError());
            throw std::runtime_error("Failed to load enemy shoot sound");
        }
        enemyShootSounds[i]->volume = 0.6f * MIX_MAX_VOLUME;
    }
    doorOpenSound = Mix_LoadWAV(audio_path("sfx/door_open.wav").c_str());
    if (!doorOpenSound) {
        fprintf(stderr, "Failed to load door open sound: %s\n", Mix_GetError());
        throw std::runtime_error("Failed to load door open sound");
    }
    incomingDialogueSound = Mix_LoadWAV(audio_path("sfx/incoming_dialogue.wav").c_str());
    if (!incomingDialogueSound) {
        fprintf(stderr, "Failed to load incoming dialogue sound: %s\n", Mix_GetError());
        throw std::runtime_error("Failed to load incoming dialogue sound");
    }
    incomingDialogueSound->volume = 0.4f * MIX_MAX_VOLUME;
    nextDialogueSound = Mix_LoadWAV(audio_path("sfx/next_dialogue_short.wav").c_str());
    if (!nextDialogueSound) {
        fprintf(stderr, "Failed to load next dialogue sound: %s\n", Mix_GetError());
        throw std::runtime_error("Failed to load next dialogue sound");
    }
    nextDialogueSound->volume = 0.2f * MIX_MAX_VOLUME;
    doorCloseSound = Mix_LoadWAV(audio_path("sfx/door_close.wav").c_str());
    if (!doorCloseSound) {
        fprintf(stderr, "Failed to load door close sound: %s\n", Mix_GetError());
        throw std::runtime_error("Failed to load door close sound");
    }
    doorCloseSound->volume = 0.4f * MIX_MAX_VOLUME;

    itemGetSound = Mix_LoadWAV(audio_path("sfx/item_get.wav").c_str());
    if (!itemGetSound) {
        fprintf(stderr, "Failed to load item get sound: %s\n", Mix_GetError());
        throw std::runtime_error("Failed to load item get sound");
    }
    itemGetSound->volume = 0.6f * MIX_MAX_VOLUME;

    rareItemGetSound = Mix_LoadWAV(audio_path("sfx/rare_item_get.wav").c_str());
    if (!rareItemGetSound) {
        fprintf(stderr, "Failed to load rare item get sound: %s\n", Mix_GetError());
        throw std::runtime_error("Failed to load rare item get sound");
    }
    rareItemGetSound->volume = 0.6f * MIX_MAX_VOLUME;

    explosionSound = Mix_LoadWAV(audio_path("sfx/explosion.wav").c_str());
    if (!explosionSound) {
        fprintf(stderr, "Failed to load explosion sound: %s\n", Mix_GetError());
        throw std::runtime_error("Failed to load explosion sound");
    }
    explosionSound->volume = 0.6f * MIX_MAX_VOLUME;

}

void SoundSystem::playNextMusic() {
    int nextMusicIndex = Random::Int(normalRoomMusic.size());
    playNextMusic(nextMusicIndex);
}

void SoundSystem::playNextMusic(int songIndex)
{
    int nextMusicIndex = songIndex;
    if (nextMusicIndex == currMusicIndex)
    {
        return;
    }
    currMusicIndex = nextMusicIndex;
    currentBGM = &normalRoomMusic[currMusicIndex];
    Mix_FreeMusic(backgroundMusic);
    Mix_Music *newbackgroundMusic = Mix_LoadMUS(currentBGM->path.c_str());
    Mix_FadeInMusic(newbackgroundMusic, currentBGM->loops, 3000);
    backgroundMusic = newbackgroundMusic;
    if (!backgroundMusic)
    {
        fprintf(stderr, "Failed to load background music: %s\n", Mix_GetError());
    }
    Mix_VolumeMusic( MIX_MAX_VOLUME * currentBGM->volume);
}



void SoundSystem::playBossMusic(int songIndex)
{
    currentBGM = &bossRoomMusic[songIndex];
    Mix_FreeMusic(backgroundMusic);
    Mix_Music *newbackgroundMusic = Mix_LoadMUS(currentBGM->path.c_str());
    Mix_FadeInMusic(newbackgroundMusic, currentBGM->loops, 3000);
    backgroundMusic = newbackgroundMusic;
    if (!backgroundMusic)
    {
        fprintf(stderr, "Failed to load background music: %s\n", Mix_GetError());
    }
    Mix_VolumeMusic( MIX_MAX_VOLUME * currentBGM->volume);
}

void SoundSystem::playSpecialMusic()
{
    playSpecialMusic(0);
}

void SoundSystem::playSpecialMusic(int songIndex)
{
    currentBGM = &specialRoomMusic[songIndex];
    Mix_Music *newbackgroundMusic = Mix_LoadMUS(audio_path("event/event-room.wav").c_str());
    Mix_FreeMusic(backgroundMusic);
    Mix_FadeInMusic(newbackgroundMusic,-1, 3000);
    backgroundMusic = newbackgroundMusic;
    if (!backgroundMusic)
    {
        fprintf(stderr, "Failed to load background music: %s\n", Mix_GetError());
    }
    Mix_VolumeMusic( MIX_MAX_VOLUME * currentBGM->volume);
}

void SoundSystem::playPlayerHurtSound() {
    Mix_PlayChannel(3, playerHurtSound, 0);
    Mix_Volume(3, playerHurtSound->volume * volume);
}

void SoundSystem::playPlayerDashSound() {
    Mix_PlayChannelTimed(2, playerDashSound, 0, 200);
    Mix_Volume(2, playerDashSound->volume * volume);
}
void SoundSystem::playPlayerShootSound(float ticks) {
    if (!Mix_Playing(1)) {
        // Check if the channel is not playing
        Mix_PlayChannelTimed(1, playerShootSound, 0, ticks);  // Play sound on specified channel);
        Mix_Volume(1, playerShootSound->volume * volume);
    }
}
void SoundSystem::playEnemyShootSound(int sfxNumber, int loops) {
    if (!Mix_Playing(4)) {
        // Check if the channel is not playing
        int i = sfxNumber % 4;
        Mix_PlayChannel(4, enemyShootSounds[i], loops);
        Mix_Volume(4, enemyShootSounds[i]->volume * volume);
    }
}

void SoundSystem::playDoorOpenSound() {
    if (!Mix_Playing(5)) {
        Mix_PlayChannel(5, doorOpenSound, 0);
        Mix_Volume(5, doorOpenSound->volume * volume);
    }
}

void SoundSystem::playDoorCloseSound() {
    if (!Mix_Playing(5)) {
        Mix_PlayChannel(5, doorCloseSound, 0);
        Mix_Volume(5, doorCloseSound->volume * volume);
    }
}

void SoundSystem::playIncomingDialogueSound() {
    if (!Mix_Playing(6)) {
        Mix_PlayChannel(6, incomingDialogueSound, 0);
        Mix_Volume(6, incomingDialogueSound->volume * volume);
    }
}

bool SoundSystem::isPlayingIncomingDialogueSound() {
    return Mix_Playing(6);
}

void SoundSystem::stopIncomingDialogueSound() {
    if (Mix_Playing(6))
        Mix_HaltChannel(6);
}

void SoundSystem::playNextDialogueSound() {
    if (!Mix_Playing(7)) {
        Mix_PlayChannel(7, nextDialogueSound, 0);
        Mix_Volume(7, nextDialogueSound->volume * volume);
    } else {
        Mix_HaltChannel(7);
        Mix_PlayChannel(7, nextDialogueSound, 0);
        Mix_Volume(7, nextDialogueSound->volume * volume);
    }
}

void SoundSystem::stopNextDialogueSound() {
    if (Mix_Playing(7))
        Mix_HaltChannel(7);
}

void SoundSystem::playItemPickupSound() {
    if (!Mix_Playing(8)) {
        Mix_PlayChannel(8, itemGetSound, 0);
        Mix_Volume(8, itemGetSound->volume * volume);
    }
}

void SoundSystem::playRareItemPickupSound() {
    if (!Mix_Playing(8)) {
        Mix_PlayChannel(8, rareItemGetSound, 0);
        Mix_Volume(8, rareItemGetSound->volume * volume);
    }
}

void SoundSystem::playExplosionSound() {
    if (!Mix_Playing(9)) {
        Mix_PlayChannel(9, explosionSound, 0);
        Mix_Volume(9, explosionSound->volume * volume);
    }
}

bool SoundSystem::setVolume(float volume) {
    if (volume >= 0.0f && volume <= 1.0f && this->volume != volume) {
        std::cout << "old Volume: " << volume << std::endl;
        this->volume = volume;
        std::cout << "Volume: " << volume << std::endl;
        if (currentBGM != nullptr) {
            Mix_VolumeMusic(volume * MIX_MAX_VOLUME * currentBGM->volume);
        } else {
            Mix_VolumeMusic(volume * MIX_MAX_VOLUME * 0.3f);
        }
        Mix_Volume(1, playerShootSound->volume * volume);
        Mix_Volume(2, playerDashSound->volume * volume);
        Mix_Volume(3, playerHurtSound->volume * volume);
        return true;
    }
    return false;
}



