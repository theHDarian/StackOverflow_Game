//
// Created by sky on 2024-11-01.
//

#include "sound_system.hpp"
#include "components.hpp"
#include "tiny_ecs_registry.hpp"
#include "SDL.h"
#include <SDL_mixer.h>
#include <iostream>

SoundSystem::SoundSystem()
{
    volume = 0.5f;
    loadMusic();
    loadSoundEffects();
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
    if (Mix_PlayingMusic())
        Mix_HaltMusic();
    Mix_CloseAudio();
}


void SoundSystem::loadMusic()
{
    Sound roomMusic[] = {
        {SoundType::normalBGM, audio_path("room/game-music-loop-1.wav"), 0.1f, -1},
        {SoundType::normalBGM, audio_path("room/game-music-loop-2.wav"), 0.1f, -1},
        {SoundType::normalBGM, audio_path("room/game-music-loop-3.wav"), 0.1f, -1},
        {SoundType::normalBGM, audio_path("room/game-music-loop-4.wav"), 0.1f, -1},
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

    for (Sound &track : roomMusic)
    {
        normalRoomMusic.push_back(track);
        std::cout << "Loaded background music " << track.path << std::endl;
    }

    Sound boss = {SoundType::bossBGM, audio_path("boss/boss-music.wav"), 0.2f, -1};
    bossRoomMusic.push_back(boss);

    Sound special = {SoundType::specialBGM, audio_path("special/special-room.wav"), 0.2f, -1};
    specialRoomMusic.push_back(special);

    Sound &currentBGM = normalRoomMusic[0];

    backgroundMusic = Mix_LoadMUS(currentBGM.path.c_str());
    if (!backgroundMusic)
    {
        fprintf(stderr, "Failed to load background music: %s\n", Mix_GetError());
        throw std::runtime_error("Failed to load background music");
    }

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
    playerHurtSound->volume = 1.0f * MIX_MAX_VOLUME;

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
    playerShootSound->volume = 0.8f * MIX_MAX_VOLUME;

    for (int i = 0; i < 4; i++) {
        enemyShootSounds.push_back(Mix_LoadWAV(audio_path("sfx/Shoot_0" + std::to_string(i) + ".wav").c_str()));
        if (!enemyShootSounds[i]) {
            fprintf(stderr, "Failed to load enemy death sound: %s\n", Mix_GetError());
            throw std::runtime_error("Failed to load enemy death sound");
        }
        enemyShootSounds[i]->volume = 0.8f * MIX_MAX_VOLUME;
    }
    doorOpenSound = Mix_LoadWAV(audio_path("sfx/door_open.wav").c_str());
    if (!doorOpenSound) {
        fprintf(stderr, "Failed to load door open sound: %s\n", Mix_GetError());
        throw std::runtime_error("Failed to load door open sound");
    }

}

void SoundSystem::nextMusic()
{
    int nextMusicIndex = rand() % normalRoomMusic.size();
    if (nextMusicIndex == currMusicIndex)
    {
        return;
    }
    currMusicIndex = nextMusicIndex;
    Sound &currentBGM = normalRoomMusic[currMusicIndex];
    Mix_FreeMusic(backgroundMusic);
    Mix_Music *newbackgroundMusic = Mix_LoadMUS(currentBGM.path.c_str());
    Mix_FadeInMusic(newbackgroundMusic, currentBGM.loops, 1000);
    backgroundMusic = newbackgroundMusic;
    if (!backgroundMusic)
    {
        fprintf(stderr, "Failed to load background music: %s\n", Mix_GetError());
    }
    Mix_VolumeMusic(volume * MIX_MAX_VOLUME);
}

void SoundSystem::playPlayerHurtSound() {
    Mix_PlayChannel(3, playerHurtSound, 0);
    Mix_Volume(3, playerHurtSound->volume * volume);
}

void SoundSystem::playPlayerDashSound() {
    Mix_PlayChannelTimed(2, playerDashSound, 0, 200);
    Mix_Volume(3, playerDashSound->volume * volume);
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


bool SoundSystem::increaseVolume() {
    std::cout << "Volume increased" << std::endl;
    if (this->volume < 1.0f) {
        std::cout << "old Volume: " << volume << std::endl;
        this->volume += 0.1f;
        std::cout << "Volume: " << volume << std::endl;
        Mix_VolumeMusic(volume * MIX_MAX_VOLUME * 0.3);
        Mix_Volume(1, playerShootSound->volume * volume);
        Mix_Volume(2, playerDashSound->volume * volume);
        Mix_Volume(3, playerHurtSound->volume * volume);
        return true;
    }
    return false;
}

bool SoundSystem::decreaseVolume() {
    std::cout << "Volume decreased" << std::endl;
    if (this->volume > 0.0f) {
        std::cout << "old Volume: " << volume << std::endl;
        this->volume -= 0.1f;
        std::cout << "Volume: " << volume << std::endl;
        Mix_VolumeMusic(volume * MIX_MAX_VOLUME * 0.3);
        Mix_Volume(1, playerShootSound->volume * volume);
        Mix_Volume(2, playerDashSound->volume * volume);
        Mix_Volume(3, playerHurtSound->volume * volume);
        return true;
    }
    return false;
}

bool SoundSystem::setVolume(float volume) {
    if (volume >= 0.0f && volume <= 1.0f && this->volume != volume) {
        std::cout << "old Volume: " << volume << std::endl;
        this->volume = volume;
        std::cout << "Volume: " << volume << std::endl;
        Mix_VolumeMusic(volume * MIX_MAX_VOLUME);
        Mix_Volume(1, playerShootSound->volume * volume);
        Mix_Volume(2, playerDashSound->volume * volume);
        Mix_Volume(3, playerHurtSound->volume * volume);
        return true;
    }
    return false;
}



