#pragma once
#include <unordered_map>
#include "components/actor_components.hpp"

//used for debug name mapping
const std::unordered_map<BulletEffectType, std::string> bulletEffectTypeNames = {
    {BulletDamage, "BulletDamage"},
    {ProjectileSpeed, "ProjectileSpeed"},
    {ProjectileSize, "ProjectileSize"},
    {FireRate, "FireRate"},
    {BulletRange, "BulletRange"},
    {BulletAccuracy, "BulletAccuracy"},
    {BulletNum, "BulletNum"},
    {BulletBurst, "BulletBurst"},
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
    {ProjectileSpeed, "bullet speed"},
    {ProjectileSize, "bullet size"},
    {FireRate, "fire rate"},
    {BulletRange, "bullet range"},
    {BulletAccuracy, "spread"},
    {BulletNum, "number of bullets (at once)"},
    {BulletBurst, "number of bullets (in succession)"},
    {Bounce, "bounce"},
    {Pierce, "pierce"},
    {Homing, "homing effect"},
    {PlayerSpeed, "player speed"},
    {PlayerNumDash, "number of dashes increased"},
    {PlayerStackSize, "stack size increased"},
    {PlayerDashRecharge, "reduced dash cooldown"},
};

const std::unordered_map<BulletEffectType, std::string> bulletEffectUnits = {
    {BulletDamage, ""},
    {ProjectileSpeed, "px/ms"}, //??
    {ProjectileSize, "px"},
    {FireRate, "s"},
    {BulletRange, "px"},
    {BulletAccuracy, "px"},
    {BulletNum, ""},
    {BulletBurst, ""},
    {Bounce, ""},
    {Pierce, ""},
    {Homing, "homing effect"}, //???
    {PlayerSpeed, "px/ms"}, //???
    {PlayerNumDash, ""},
    {PlayerStackSize, ""},
    {PlayerDashRecharge, "s"},
};

// TODO: think of better names
const std::map<BulletEffectType, std::string> tierNames = {
    {BulletDamage,      "Burning Bullets"},
    {ProjectileSpeed,   "ProjectileSpeed"},
    {ProjectileSize,    "Splitting Bullets"},     // Bullet explodes into smaller bullets on deletion
    {FireRate,          "FireRate"},
    {BulletRange,       "Sniper's Prowess"},     // Deal more damage the further away from the player the enemy is (up to 2x)
    {BulletAccuracy,    "Critical Weakness"},     // Inflict vulnerable for 4000
    {BulletNum,         "Many Mini Bullets"},     // Fires 4 * (1 + value-threshold) mini bullets
    {BulletBurst,       "BulletBurst"},
    {Bounce,            "Heat-seeking Bounces"},     // Bouncing towards random enemy
    {Pierce,            "To Pierce through all Defences"},     // Deal more dmg to protected enemies, and vulnerable effect stronger
    {Homing,            "Homing"},
    {PlayerSpeed,       "PlayerSpeed"},
    {PlayerNumDash,     "PlayerNumDash"},
    {PlayerStackSize,   "PlayerStackSize"},
    {PlayerDashRecharge,"Outrun Inert(ia)"}      // 50% Chance to dodge inert effect from bullets or enemies
};

const std::unordered_map<std::string, InteractableItem> interactableItemNames = {
    {"Ram", InteractableItem::Ram},
    {"PopConsole", InteractableItem::PopConsole},
    {"OpenDoor", InteractableItem::OpenDoor},
    {"LockedDoor", InteractableItem::LockedDoor},
    {"BibleTree", InteractableItem::BibleTree},
    {"Gardener", InteractableItem::Gardener}
};