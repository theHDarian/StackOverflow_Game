#pragma once
#include <unordered_map>
#include "components/actor_components.hpp"

//used for debug name mapping
const std::unordered_map<BulletEffectType, std::string> bulletEffectTypeNames = {
    {BulletDamage, "BulletDamage"},
    {ProjectileSize, "ProjectileSize"},
    {FireRate, "FireRate"},
    {BulletRange, "BulletRange"},
    {BulletAccuracy, "BulletAccuracy"},
    {BulletNum, "BulletNum"},
    {Bounce, "Bounce"},
    {Pierce, "Pierce"},
    {Homing, "Homing"},
    {PlayerSpeed, "PlayerSpeed"},
    {PlayerNumDash, "PlayerNumDash"},
    {PlayerStackSize, "PlayerStackSize"},
    {PlayerDashRecharge, "PlayerDashRecharge"},
    {Inert, "Inert"}
};

const std::unordered_map<BulletEffectType, std::string> bulletEffectDescriptions = {
    {BulletDamage, "damage"},
    {ProjectileSize, "bullet size"},
    {FireRate, "fire rate"},
    {BulletRange, "bullet range and speed"},
    {BulletAccuracy, "spread"},
    {BulletNum, "number of bullets (at once)"},
    {Bounce, "bounce"},
    {Pierce, "pierce"},
    {Homing, "homing effect"},
    {PlayerSpeed, "player speed"},
    {PlayerNumDash, "number of dashes increased"},
    {PlayerStackSize, "stack size increased"},
    {PlayerDashRecharge, "reduced dash cooldown"},
};

// TODO: double check units
const std::unordered_map<BulletEffectType, std::string> bulletEffectUnits = {
    {BulletDamage, ""},
    {ProjectileSize, "px"},
    {FireRate, "s"},
    {BulletRange, "px"},
    {BulletAccuracy, "px"},
    {BulletNum, ""},
    {Bounce, ""},
    {Pierce, ""},
    {Homing, "homing effect"}, //???
    {PlayerSpeed, "px/s"}, //???
    {PlayerNumDash, ""},
    {PlayerStackSize, ""},
    {PlayerDashRecharge, "s"},
};

// TODO: think of better names
const std::map<BulletEffectType, std::string> tierNames = {
    {BulletDamage,      "Burning Bullets"},
    {ProjectileSize,    "Splitting Bullets"},     // Bullet explodes into smaller bullets on deletion
    {FireRate,          "Burst Fire"},             // Activates burst-fire mode, +1 per firerate about threshold
    {BulletRange,       "Hawk-eyed"},     // Deal more damage the further away from the player the enemy is (up to 2x)
    {BulletAccuracy,    "Critical Weakness"},     // Inflict vulnerable for 4000
    {BulletNum,         "Many Mini Menaces"},     // Fires 4 * (1 + value-threshold) mini bullets
    {Bounce,            "Heat-seeking Bounces"},     // Bouncing towards random enemy
    {Pierce,            "To Pierce Through All Defences"},     // Deal more dmg to protected enemies, and vulnerable effect stronger
    {Homing,            "Greed"},
    {PlayerSpeed,       "Momentary Reprieve"}, //Enter bullet time (non-player entities slowed by 80%) upon taking damage
    {PlayerNumDash,     "Clear The Path"},
    {PlayerStackSize,   "PlayerStackSize"},
    {PlayerDashRecharge,"Outrun Inert(ia)"},      // 50% Chance to dodge inert effect from bullets or enemies
};

const std::unordered_map<std::string, InteractableItem> interactableItemNames = {
    {"Ram", InteractableItem::Ram},
    {"PopConsole", InteractableItem::PopConsole},
    {"OpenDoor", InteractableItem::OpenDoor},
    {"LockedDoor", InteractableItem::LockedDoor},
    {"BibleTree", InteractableItem::BibleTree},
    {"Gardener", InteractableItem::Gardener}
};