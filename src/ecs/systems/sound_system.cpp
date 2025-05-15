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
    for (int i = (int) registry.persistentSounds.components.size()-1; i>=0; --i) {
        PersistentSounds &persistentSounds = registry.persistentSounds.components[i];
        int notPlaying = 0;
        for (auto& channel : persistentSounds.channels) {
            if (channel.second.x != -1) {
                channel.second.y -= elapsed_ms;
                if (channel.second.y <= 0) {
                    Mix_HaltChannel(channel.second.x);
                    channel.second.x = -1;
                } else if (!Mix_Playing(channel.second.x)) {
                    auto& soundRequest = registry.soundRequests.emplace_with_duplicates(registry.persistentSounds.entities[i]);
                    soundRequest.type = channel.first;
                    soundRequest.ticks = channel.second.y;
                    soundRequest.songIndex = 0;
                }
            } else {
                notPlaying++;
            }
        }
        if (notPlaying == persistentSounds.channels.size()) {
            registry.persistentSounds.remove(registry.persistentSounds.entities[i]);
        }
    }

    std::vector<SoundRequest> soundRequests;
    for (int i = (int)registry.soundRequests.components.size()-1; i>=0; --i)
    {
        SoundRequest &soundRequest = registry.soundRequests.components[i];

        if (soundRequest.delay > 0)
        {
            soundRequest.delay -= elapsed_ms;
            continue;
        }

        switch (soundRequest.type) {
            case SoundType::CombatBGM:
                playNextMusic();
            break;

            case SoundType::BossBGM:
                if (soundRequest.songIndex == -1)
                    playBossMusic(Random::Int(bossRoomMusic.size()));
                else
                    playBossMusic(soundRequest.songIndex);
            break;

            case SoundType::ClearedBGM:
                if (soundRequest.songIndex == -1)
                    playSpecialMusic();
                else
                    playSpecialMusic(soundRequest.songIndex);
            break;

            case SoundType::PlayerHurt:
                playPlayerHurtSound();
            break;

            case SoundType::DashSound:
                playPlayerDashSound();
            break;

            case SoundType::PlayerShoot:
                playPlayerShootSound(soundRequest.ticks);
            break;

            case SoundType::EnemyShoot:
                playEnemyShootSound(soundRequest.songIndex, soundRequest.loops);
            break;

            case SoundType::IncomingDialogue:
                playIncomingDialogueSound();
            break;

            case SoundType::NormalDialogue:
                playNextDialogueSound();
            break;

            case SoundType::DoorOpen:
                playDoorOpenSound();
            break;

            case SoundType::itemPickup:
                playItemPickupSound();
            break;

            case SoundType::rareItemPickup:
                playRareItemPickupSound();
            break;

            case SoundType::explosion:
                playExplosionSound(soundRequest.songIndex);
            break;

            case SoundType::DoorClose:
                playDoorCloseSound();
            break;

            case SoundType::TitleBGM:
                playTitleMusic();
            break;

            case SoundType::PlayerZapped:
                playPlayerZappedSound();
            break;

            case SoundType::AlarmSound:
                if (soundRequest.songIndex == -1)
                    playAlarmSound();
                else
                    playAlarmSound(soundRequest.songIndex);
            break;

            case SoundType::FanFare:
                playFanFareSound();
            break;

            case SoundType::EnemyDeathSound:
                playEnemyDeathSound(soundRequest.songIndex);
            break;

            case SoundType::PlayerDodgeSound:
                playPlayerDodgeSound(soundRequest.songIndex);
            break;

            case SoundType::LaserSound:
                playLaserSound(soundRequest.ticks, soundRequest.songIndex, &registry.soundRequests.entities[i]);
            break;

            case SoundType::DiggingSound:
                if (soundRequest.songIndex == -1)
                    playDiggingSound(soundRequest.ticks, &registry.soundRequests.entities[i]);
                else
                    stopDiggingSound();
            break;

            default:
                std::cerr << "Unknown sound type."  << std::endl;
            break;
        }
        registry.soundRequests.remove(registry.soundRequests.entities[i]);
    }
}

SoundSystem::SoundSystem()
{
    loadMusic();
    loadSoundEffects();
    Mix_AllocateChannels(24);
}

SoundSystem::~SoundSystem()
{
    if (playerHurtSound != nullptr)
        Mix_FreeChunk(playerHurtSound);
    if (playerDashSound != nullptr)
        Mix_FreeChunk(playerDashSound);
    if (playerShootSound != nullptr)
        Mix_FreeChunk(playerShootSound);
    if (doorOpenSound != nullptr)
        Mix_FreeChunk(doorOpenSound);
    if (doorCloseSound != nullptr)
        Mix_FreeChunk(doorCloseSound);
    if (incomingDialogueSound != nullptr)
        Mix_FreeChunk(incomingDialogueSound);
    if (nextDialogueSound != nullptr)
        Mix_FreeChunk(nextDialogueSound);
    if (itemGetSound != nullptr)
        Mix_FreeChunk(itemGetSound);

    for (int i = 0; i < 4; i++)
    {
        if (enemyShootSounds[i] != nullptr)
            Mix_FreeChunk(enemyShootSounds[i]);
    }

    for (int i = 0; i < 4; i++)
    {
        if (explosionSounds[i] != nullptr)
            Mix_FreeChunk(explosionSounds[i]);
    }

    for (int i = 0; i < 8; i++)
    {
        if (alarmSounds[i] != nullptr)
            Mix_FreeChunk(alarmSounds[i]);
    }

    if (Mix_PlayingMusic())
        Mix_HaltMusic();
    for (auto& track : normalRoomMusic)
    {
        if (track.music != nullptr)
            Mix_FreeMusic(track.music);
    }
    for (auto& track : bossRoomMusic)
    {
        if (track.music != nullptr)
            Mix_FreeMusic(track.music);
    }
    for (auto& track : specialRoomMusic)
    {
        if (track.music != nullptr)
            Mix_FreeMusic(track.music);
    }
    if (titleScreenMusic.music != nullptr)
        Mix_FreeMusic(titleScreenMusic.music);
    for (auto& track : playerDodgeSounds)
    {
        if (track != nullptr)
            Mix_FreeChunk(track);
    }
    for (auto& track : enemyDeathSounds)
    {
        if (track != nullptr)
            Mix_FreeChunk(track);
    }


    Mix_CloseAudio();
}

void SoundSystem::loadMusic() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    }

    // Preload normal room music
    normalRoomMusic = {
                {SoundType::CombatBGM, audio_path("room/game-music-loop-1.wav"), 0.25f, -1},
        {SoundType::CombatBGM, audio_path("room/game-music-loop-2.wav"), 0.25f, -1},
        {SoundType::CombatBGM, audio_path("room/game-music-loop-3.wav"), 0.25f, -1},
        {SoundType::CombatBGM, audio_path("room/game-music-loop-4.wav"), 0.3f, -1},
    };

    for (auto& track : normalRoomMusic) {
        track.music = Mix_LoadMUS(track.path.c_str());
        if (!track.music) {
            fprintf(stderr, "Failed to load background music: %s\n", Mix_GetError());
        }
    }

    // Preload boss room music
    bossRoomMusic = {
        {SoundType::BossBGM, audio_path("boss/boss-music-1.wav"), 0.5f, -1},
        // {SoundType::BossBGM, audio_path("boss/boss-music-2.wav"), 0.5f, -1},
    };

    for (auto& track : bossRoomMusic) {
        track.music = Mix_LoadMUS(track.path.c_str());
        if (!track.music) {
            fprintf(stderr, "Failed to load boss music: %s\n", Mix_GetError());
        }
    }

    // Preload special room music
    specialRoomMusic = {
        {SoundType::ClearedBGM, audio_path("event/event-room-1.wav"), 0.4f, -1},
        {SoundType::ClearedBGM, audio_path("event/event-room-2.wav"), 0.6f, -1},
        {SoundType::ClearedBGM, audio_path("event/event-room-3.wav"), 0.3f, -1},
    };

    for (auto& track : specialRoomMusic) {
        track.music = Mix_LoadMUS(track.path.c_str());
        if (!track.music) {
            fprintf(stderr, "Failed to load special music: %s\n", Mix_GetError());
        }
    }

    this->titleScreenMusic = {SoundType::TitleBGM, audio_path("title/title.wav"), 0.15f, -1};
    this->titleScreenMusic.music = Mix_LoadMUS(titleScreenMusic.path.c_str());
}


void SoundSystem::loadSoundEffects()
{
    // Loading music and sounds with SDL
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        fprintf(stderr, "Failed to initialize SDL Audio: %s\n", SDL_GetError());
        throw std::runtime_error("Failed to initialize SDL Audio");
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
    {
        fprintf(stderr, "Failed to open audio device: %s\n", Mix_GetError());
        throw std::runtime_error("Failed to open audio device");
    }

    playerHurtSound = Mix_LoadWAV(audio_path("sfx/player_hurtv1.wav").c_str());
    if (!playerHurtSound)
    {
        fprintf(stderr, "Failed to load player hurt sound: %s\n", Mix_GetError());
        throw std::runtime_error("Failed to load player hurt sound");
    }
    playerHurtSound->volume = 0.7f * MIX_MAX_VOLUME;

    playerZappedSound = Mix_LoadWAV(audio_path("sfx/player_zapped.wav").c_str());
    if (!playerHurtSound)
    {
        fprintf(stderr, "Failed to load player zap sound: %s\n", Mix_GetError());
        throw std::runtime_error("Failed to load player zap sound");
    }
    playerZappedSound->volume = 0.4f * MIX_MAX_VOLUME;

    playerDashSound = Mix_LoadWAV(audio_path("sfx/dash.wav").c_str());
    if (!playerDashSound)
    {
        fprintf(stderr, "Failed to load player dash sound: %s\n", Mix_GetError());
        throw std::runtime_error("Failed to load player dash sound");
    }
    playerDashSound->volume = 0.6f * MIX_MAX_VOLUME;

    playerShootSound = Mix_LoadWAV(audio_path("sfx/player_shoot.wav").c_str());
    if (!playerShootSound)
    {
        fprintf(stderr, "Failed to load player shoot sound: %s\n", Mix_GetError());
        throw std::runtime_error("Failed to load player shoot sound");
    }
    playerShootSound->volume = 0.4f * MIX_MAX_VOLUME;

    for (int i = 0; i < 4; i++)
    {
        enemyShootSounds.push_back(Mix_LoadWAV(audio_path("sfx/Shoot_0" + std::to_string(i) + ".wav").c_str()));
        if (!enemyShootSounds[i])
        {
            fprintf(stderr, "Failed to load enemy shoot sound: %s\n", Mix_GetError());
            throw std::runtime_error("Failed to load enemy shoot sound");
        }
        enemyShootSounds[i]->volume = 0.6f * MIX_MAX_VOLUME;
    }
    doorOpenSound = Mix_LoadWAV(audio_path("sfx/door_open.wav").c_str());
    if (!doorOpenSound)
    {
        fprintf(stderr, "Failed to load door open sound: %s\n", Mix_GetError());
        throw std::runtime_error("Failed to load door open sound");
    }
    incomingDialogueSound = Mix_LoadWAV(audio_path("sfx/incoming_dialogue.wav").c_str());
    if (!incomingDialogueSound)
    {
        fprintf(stderr, "Failed to load incoming dialogue sound: %s\n", Mix_GetError());
        throw std::runtime_error("Failed to load incoming dialogue sound");
    }
    incomingDialogueSound->volume = 0.7f * MIX_MAX_VOLUME;
    nextDialogueSound = Mix_LoadWAV(audio_path("sfx/next_dialogue_short.wav").c_str());
    if (!nextDialogueSound)
    {
        fprintf(stderr, "Failed to load next dialogue sound: %s\n", Mix_GetError());
        throw std::runtime_error("Failed to load next dialogue sound");
    }
    nextDialogueSound->volume = 0.6f * MIX_MAX_VOLUME;
    doorCloseSound = Mix_LoadWAV(audio_path("sfx/door_close.wav").c_str());
    if (!doorCloseSound)
    {
        fprintf(stderr, "Failed to load door close sound: %s\n", Mix_GetError());
        throw std::runtime_error("Failed to load door close sound");
    }
    doorCloseSound->volume = 0.4f * MIX_MAX_VOLUME;

    itemGetSound = Mix_LoadWAV(audio_path("sfx/item_get.wav").c_str());
    if (!itemGetSound)
    {
        fprintf(stderr, "Failed to load item get sound: %s\n", Mix_GetError());
        throw std::runtime_error("Failed to load item get sound");
    }
    itemGetSound->volume = 0.6f * MIX_MAX_VOLUME;

    rareItemGetSound = Mix_LoadWAV(audio_path("sfx/rare_item_get.wav").c_str());
    if (!rareItemGetSound)
    {
        fprintf(stderr, "Failed to load rare item get sound: %s\n", Mix_GetError());
        throw std::runtime_error("Failed to load rare item get sound");
    }
    rareItemGetSound->volume = 0.6f * MIX_MAX_VOLUME;

    gameOversound = Mix_LoadWAV(audio_path("sfx/game_over.wav").c_str());
    if (!gameOversound)
    {
        fprintf(stderr, "Failed to load game over sound: %s\n", Mix_GetError());
        throw std::runtime_error("Failed to load game over sound");
    }
    gameOversound->volume = 0.8f * MIX_MAX_VOLUME;

    fanFareSound = Mix_LoadWAV(audio_path("sfx/fanfare.wav").c_str());
    if (!fanFareSound)
    {
        fprintf(stderr, "Failed to load fanfare sound: %s\n", Mix_GetError());
        throw std::runtime_error("Failed to load fanfare sound");
    }
    fanFareSound->volume = 0.8f * MIX_MAX_VOLUME;

    laserLoopSound = Mix_LoadWAV(audio_path("sfx/laser_loop.wav").c_str());
    if (!laserLoopSound)
    {
        fprintf(stderr, "Failed to load laser loop sound: %s\n", Mix_GetError());
        throw std::runtime_error("Failed to load laser loop sound");
    }
    laserLoopSound->volume = 0.3f * MIX_MAX_VOLUME;

    laserSound = Mix_LoadWAV(audio_path("sfx/laser.wav").c_str());
    if (!laserSound)
    {
        fprintf(stderr, "Failed to load laser sound: %s\n", Mix_GetError());
        throw std::runtime_error("Failed to load laser sound");
    }
    laserSound->volume = 0.7f * MIX_MAX_VOLUME;

    diggingSound = Mix_LoadWAV(audio_path("sfx/digging.wav").c_str());
    if (!diggingSound)
    {
        fprintf(stderr, "Failed to load digging sound: %s\n", Mix_GetError());
        throw std::runtime_error("Failed to load digging sound");
    }
    diggingSound->volume = 0.8f * MIX_MAX_VOLUME;

    for (int i = 0; i < 5; i++)
    {
        explosionSounds.push_back(Mix_LoadWAV(audio_path("sfx/Explosion_0" + std::to_string(i) + ".wav").c_str()));
        if (!explosionSounds[i])
        {
            fprintf(stderr, "Failed to load explosion sound: %s\n", Mix_GetError());
            throw std::runtime_error("Failed to load explosion sound");
        }
        explosionSounds[i]->volume = 0.6f * MIX_MAX_VOLUME;
    }

    for (int i = 0; i < 8; i++)
    {
        alarmSounds.push_back(Mix_LoadWAV(audio_path("sfx/sfx_alarm_loop" + std::to_string(i + 1) + ".wav").c_str()));
        if (!alarmSounds[i])
        {
            fprintf(stderr, "Failed to load alarm sound: %s\n", Mix_GetError());
            throw std::runtime_error("Failed to load alarm sound");
        }
        alarmSounds[i]->volume = 0.4f * MIX_MAX_VOLUME;
    }
    for (int i = 0; i < 1; i++)
    {
        playerDodgeSounds.push_back(Mix_LoadWAV(audio_path("sfx/player_dodge_" + std::to_string(i) + ".wav").c_str()));
        if (!playerDodgeSounds[i])
        {
            fprintf(stderr, "Failed to load dodge sound: %s\n", Mix_GetError());
            throw std::runtime_error("Failed to load dodge sound");
        }
        playerDodgeSounds[i]->volume = 0.6f * MIX_MAX_VOLUME;
    }
    for (int i = 1; i <= 4; i++)
    {
        enemyDeathSounds.push_back(Mix_LoadWAV(audio_path("sfx/sfx_deathscream_robot" + std::to_string(i) + ".wav").c_str()));
        if (!enemyDeathSounds[i-1])
        {
            fprintf(stderr, "Failed to load enemy death sound: %s\n", Mix_GetError());
            throw std::runtime_error("Failed to load enemy death sound");
        }
        enemyDeathSounds[i-1]->volume = 0.5f * MIX_MAX_VOLUME;
    }
}

void SoundSystem::FadeOutMusic(int ms)
{
    Mix_FadeOutMusic(ms);
    currentMusicState = MusicState::FadingOut;
}

bool SoundSystem::isPlayingMusic()
{
    return Mix_PlayingMusic();
}

void SoundSystem::playNextMusic()
{
    int nextMusicIndex = Random::Int(normalRoomMusic.size());
    playNextMusic(nextMusicIndex);
}

void SoundSystem::playNextMusic(int songIndex) {

    Mix_HaltMusic();
    currMusicIndex = songIndex;
    currentBGM = &normalRoomMusic[currMusicIndex];
    if (currentBGM && currentBGM->music) {
        std::cout << "normal music" << std::endl;
        if (Mix_FadeInMusic(currentBGM->music, currentBGM->loops, 3000) == -1) {
    fprintf(stderr, "Error fading in music: %s\n", Mix_GetError());
}

        Mix_VolumeMusic(MIX_MAX_VOLUME * currentBGM->volume * musicVolume);
    } else {
        fprintf(stderr, "Failed to play background music: %s\n", Mix_GetError());
    }
    currentMusicState = MusicState::PlayingNormal;
}


void SoundSystem::playTitleMusic()
{
    Mix_HaltMusic();
    currentBGM = &titleScreenMusic;
    if (currentBGM && currentBGM->music) {
        if (Mix_FadeInMusic(currentBGM->music, currentBGM->loops, 3000) == -1) {
    fprintf(stderr, "Error fading in music: %s\n", Mix_GetError());
    }

        Mix_VolumeMusic(MIX_MAX_VOLUME * currentBGM->volume * musicVolume);
    } else {
        fprintf(stderr, "Failed to play title music: %s\n", Mix_GetError());
    }
    currentMusicState = MusicState::PlayingTitle;
}

void SoundSystem::playBossMusic(int songIndex) {
    Mix_HaltMusic();
    currMusicIndex = songIndex;
    currentBGM = &bossRoomMusic[currMusicIndex];
    if (currentBGM && currentBGM->music) {
        std::cout << "boss music" << std::endl;
        if (Mix_FadeInMusic(currentBGM->music, currentBGM->loops, 3000) == -1) {
    fprintf(stderr, "Error fading in music: %s\n", Mix_GetError());
}

        Mix_VolumeMusic(MIX_MAX_VOLUME * currentBGM->volume * musicVolume);
    } else {
        fprintf(stderr, "Failed to play boss music: %s\n", Mix_GetError());
    }
    currentMusicState = MusicState::PlayingBoss;
}


void SoundSystem::playSpecialMusic()
{
    playSpecialMusic(Random::Int(specialRoomMusic.size()));
}

void SoundSystem::playSpecialMusic(int songIndex) {
    Mix_HaltMusic();
    currMusicIndex = songIndex;
    currentBGM = &specialRoomMusic[currMusicIndex];
    if (currentBGM && currentBGM->music) {
        std::cout << "special music" << std::endl;
        if (Mix_FadeInMusic(currentBGM->music, currentBGM->loops, 3000) == -1) {
    fprintf(stderr, "Error fading in music: %s\n", Mix_GetError());
}

        Mix_VolumeMusic(MIX_MAX_VOLUME * currentBGM->volume * musicVolume);
    } else {
        fprintf(stderr, "Failed to play special music: %s\n", Mix_GetError());
    }
    currentMusicState = MusicState::PlayingSpecial;
}


void SoundSystem::playPlayerHurtSound()
{
    Mix_PlayChannel(3, playerHurtSound, 0);
    Mix_Volume(3, playerHurtSound->volume * sfxVolume);
}

void SoundSystem::playPlayerZappedSound()
{
    Mix_PlayChannel(3, playerZappedSound, 0);
    Mix_Volume(3, playerZappedSound->volume * sfxVolume);
}

void SoundSystem::playPlayerDashSound()
{
    Mix_PlayChannelTimed(2, playerDashSound, 0, 200);
    Mix_Volume(2, playerDashSound->volume * sfxVolume);
}
void SoundSystem::playPlayerShootSound(float ticks)
{
    // if (!Mix_Playing(1)) {
    // Check if the channel is not playing
    // Mix_PlayChannelTimed(1, playerShootSound, 0, ticks);  // Play sound on specified channel);
    Mix_PlayChannel(1, playerShootSound, 0);
    Mix_Volume(1, playerShootSound->volume * sfxVolume);
    // }
}
void SoundSystem::playEnemyShootSound(int sfxNumber, int loops)
{
    if (!Mix_Playing(4))
    {
        // Check if the channel is not playing
        int i = sfxNumber % 4;
        Mix_PlayChannel(4, enemyShootSounds[i], loops);
        Mix_Volume(4, enemyShootSounds[i]->volume * sfxVolume);
    }
}

void SoundSystem::playDoorOpenSound()
{
    if (!Mix_Playing(5))
    {
        Mix_PlayChannel(5, doorOpenSound, 0);
        Mix_Volume(5, doorOpenSound->volume * sfxVolume);
    }
}

void SoundSystem::playDoorCloseSound()
{
    if (!Mix_Playing(5))
    {
        Mix_PlayChannel(5, doorCloseSound, 0);
        Mix_Volume(5, doorCloseSound->volume * sfxVolume);
    }
}

void SoundSystem::playIncomingDialogueSound()
{
    Mix_PlayChannel(6, incomingDialogueSound, 0);
    Mix_Volume(6, incomingDialogueSound->volume * sfxVolume);
}

bool SoundSystem::isPlayingIncomingDialogueSound()
{
    return Mix_Playing(6);
}

void SoundSystem::stopIncomingDialogueSound()
{
    if (Mix_Playing(6))
        Mix_HaltChannel(6);
}

void SoundSystem::playNextDialogueSound()
{
    if (!Mix_Playing(7))
    {
        Mix_PlayChannel(7, nextDialogueSound, 0);
        Mix_Volume(7, nextDialogueSound->volume * sfxVolume);
    }
    else
    {
        Mix_HaltChannel(7);
        Mix_PlayChannel(7, nextDialogueSound, 0);
        Mix_Volume(7, nextDialogueSound->volume * sfxVolume);
    }
}

void SoundSystem::stopNextDialogueSound()
{
    if (Mix_Playing(7))
        Mix_HaltChannel(7);
}

void SoundSystem::playItemPickupSound()
{
    if (!Mix_Playing(8))
    {
        Mix_PlayChannel(8, itemGetSound, 0);
        Mix_Volume(8, itemGetSound->volume * sfxVolume);
    }
}

void SoundSystem::playRareItemPickupSound()
{
    Mix_PlayChannel(8, rareItemGetSound, 0);
    Mix_Volume(8, rareItemGetSound->volume * sfxVolume);
}

void SoundSystem::playExplosionSound(int sfxNumber)
{
    int i = sfxNumber % 5;
    if (!Mix_Playing(9))
    {
        Mix_PlayChannel(9, explosionSounds[i], 0);
        Mix_Volume(9, explosionSounds[i]->volume * sfxVolume);
    }
}

void SoundSystem::playAlarmSound(int sfxNumber)
{
    int i = sfxNumber % 8;
    Mix_PlayChannel(11, alarmSounds[i], 1);
    Mix_Volume(11, alarmSounds[i]->volume * sfxVolume);
}

void SoundSystem::playFanFareSound()
{
    if (!Mix_Playing(12))
    {
        Mix_PlayChannel(12, fanFareSound, 0);
        Mix_Volume(12, fanFareSound->volume * sfxVolume);
    }
}

void SoundSystem::playGameOverSound()
{
    if (!Mix_Playing(10))
    {
        Mix_PlayChannel(10, gameOversound, -1);
        Mix_Volume(10, gameOversound->volume * sfxVolume);
    }
}

void SoundSystem::stopGameOverSound()
{
    if (Mix_Playing(10))
        Mix_HaltChannel(10);
}


void SoundSystem::playPlayerDodgeSound(int sfxNumber)
{
    int i = sfxNumber % 1;
        Mix_PlayChannel(13, playerDodgeSounds[i], 0);
        Mix_Volume(13, playerDodgeSounds[i]->volume * sfxVolume);
}

void SoundSystem::playEnemyDeathSound(int sfxNumber)
{
    int i = sfxNumber % 4;
    if (!Mix_Playing(14))
    {
        Mix_PlayChannel(14, enemyDeathSounds[i], 0);
        Mix_Volume(14, enemyDeathSounds[i]->volume * sfxVolume);
    }
}

void SoundSystem::playLaserSound(float time, bool shouldPlayFiringSound, const Entity* entity)
{
    if (shouldPlayFiringSound)
    {
        Mix_PlayChannel(15, laserSound, 0);
        Mix_Volume(15, laserSound->volume * sfxVolume);
    }

    if (time > 2000) {
        if (entity == nullptr) {
            Mix_FadeInChannelTimed( 16, laserLoopSound, -1, 1000, time);
            Mix_Volume(16, laserLoopSound->volume * sfxVolume);
            return;
        }
        if (!registry.persistentSounds.has(*entity)) {
            registry.persistentSounds.emplace(*entity);
        }
        auto& persistentSounds = registry.persistentSounds.get(*entity);
        auto& channel = persistentSounds.channels[SoundType::LaserSound];
        if (time > channel.y) {
            channel.y = time;
        }
        if (channel.x == -1) {
            channel.x = Mix_FadeInChannelTimed(-1, laserLoopSound, -1, 1000, time);
            Mix_Volume(channel.x, laserLoopSound->volume * sfxVolume);
            channel.y = time;
        } else if (!Mix_Playing(channel.x) && channel.y > 0) {
            Mix_FadeInChannelTimed(channel.x, laserLoopSound, -1, 1000, channel.y);
            Mix_Volume(channel.x, laserLoopSound->volume * sfxVolume);
        }

    }
}

void SoundSystem::playDiggingSound(float time, const Entity* entity)
{
    if (entity == nullptr) {
        Mix_FadeInChannelTimed(17, diggingSound, -1, 1000, time);
        Mix_Volume(17, diggingSound->volume * sfxVolume);
        return;
    }

   if (!registry.persistentSounds.has(*entity)) {
        registry.persistentSounds.emplace(*entity);
    }
    auto& persistentSounds = registry.persistentSounds.get(*entity);
    auto& channel = persistentSounds.channels[SoundType::DiggingSound];
    if (time > channel.y) {
        channel.y = time;
    }
    if (channel.x == -1) {
        channel.x = Mix_FadeInChannelTimed(-1, diggingSound, -1, 1000, time);
        Mix_Volume(channel.x, diggingSound->volume * sfxVolume);
        channel.y = time;
    } else if (!Mix_Playing(channel.x) && channel.y > 0) {
        Mix_FadeInChannelTimed(channel.x, diggingSound, -1, 1000, channel.y);
        Mix_Volume(channel.x, diggingSound->volume * sfxVolume);
    }
}

void SoundSystem::stopDiggingSound()
{
    if (Mix_Playing(17))
        Mix_HaltChannel(17);
}



bool SoundSystem::setMusicVolume(float volume)
{
    if (volume >= 0.0f && volume <= 1.0f && this->musicVolume != volume)
    {
        this->musicVolume = volume;
        std::cout << "Music Volume: " << volume << std::endl;
        if (currentBGM != nullptr)
        {
            Mix_VolumeMusic(volume * MIX_MAX_VOLUME * currentBGM->volume);
        }
        else
        {
            Mix_VolumeMusic(volume * MIX_MAX_VOLUME * 0.3f);
        }
        return true;
    }
    return false;
}

bool SoundSystem::setSFXVolume(float volume)
{
    if (volume >= 0.0f && volume <= 1.0f && this->sfxVolume != volume)
    {
        this->sfxVolume = volume;
        std::cout << "SFX Volume: " << volume << std::endl;
        // Mix_Volume(1, playerShootSound->volume * volume);
        // Mix_Volume(2, playerDashSound->volume * volume);
        // Mix_Volume(3, playerHurtSound->volume * volume);
        // Mix_Volume(4, enemyShootSounds[0]->volume * volume);
        // Mix_Volume(5, doorOpenSound->volume * volume);
        // Mix_Volume(6, incomingDialogueSound->volume * volume);
        // Mix_Volume(7, nextDialogueSound->volume * volume);
        // Mix_Volume(8, itemGetSound->volume * volume);
        // Mix_Volume(9, explosionSounds[0]->volume * volume);
        // Mix_Volume(10, gameOversound->volume * volume);
        return true;
    }
    return false;
}
