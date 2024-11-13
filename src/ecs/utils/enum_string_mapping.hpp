#pragma once
#include <unordered_map>
#include "components/actor_components.hpp"

//used for debug name mapping
std::unordered_map<BulletEffectType, std::string> bulletEffectTypeNames = {
    {BulletDamage, "BulletDamage"},
    {ProjectileSpeed, "ProjectileSpeed"},
    {ProjectileSize, "ProjectileSize"},
    {FireRate, "FireRate"},
    {BulletRange, "BulletRange"},
    {BulletSpread, "BulletSpread"},
    {BulletNum, "BulletNum"},
    {BulletBurst, "BulletBurst"},
    {Bounce, "Bounce"},
    {Pierce, "Pierce"},
    {Homing, "Homing"},
    {PlayerSpeed, "PlayerSpeed"},
    {PlayerNumDash, "PlayerNumDash"},
    {PlayerStackSize, "PlayerStackSize"},
    {PlayerDashCDR, "PlayerDashCDR"},
    {Inert, "Inert"}
};

std::unordered_map<std::string, SoundType> soundEffectNames ={
    {"normalBGM", SoundType::normalBGM},
    {"bossBGM", SoundType::bossBGM},
    {"specialBGM", SoundType::specialBGM},
    {"PlayerHurt", SoundType::PlayerHurt},
    {"DashSound", SoundType::DashSound},
    {"PlayerShoot", SoundType::PlayerShoot},
    {"EnemyShoot", SoundType::EnemyShoot},
    {"IncomingDialogue", SoundType::IncomingDialogue},
    {"NormalDialogue", SoundType::NormalDialogue},
    {"DoorOpen", SoundType::DoorOpen}

};