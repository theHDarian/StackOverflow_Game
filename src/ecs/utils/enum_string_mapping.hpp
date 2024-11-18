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


const std::unordered_map<std::string, InteractableItem> interactableItemNames = {
    {"Ram", InteractableItem::Ram},
    {"PopConsole", InteractableItem::PopConsole},
    {"OpenDoor", InteractableItem::OpenDoor},
    {"LockedDoor", InteractableItem::LockedDoor},
    {"BibleTree", InteractableItem::BibleTree},
    {"Gardener", InteractableItem::Gardener}
};