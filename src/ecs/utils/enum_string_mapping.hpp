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
    {"Gardener", InteractableItem::Gardener},
    {"Ramlet", InteractableItem::Ramlet},
    {"PushConsole", InteractableItem::PushConsole},
    {"FightConsole", InteractableItem::FightConsole},
    {"ClosedDoor", InteractableItem::ClosedDoor},


};

const std::unordered_map<EnemyType, MapRegion> EnemyTypeToRegion = {
    {EnemyMage, MapRegion::Biology},
    {EnemySkull, MapRegion::Biology},
    {EnemyEvilSkull, MapRegion::Biology},
    {EnemySword, MapRegion::Biology},
    {EnemyHealer, MapRegion::Biology},
    {EnemyQuadshooter, MapRegion::Biology},
    {EnemyQuadshooterElite, MapRegion::Biology},
    {EnemyMagnet, MapRegion::Biology},
    {TutorialEnemyQuadshooter, MapRegion::Biology},
    {BossCrab, MapRegion::Biology},
    {BigCLaserSniper, MapRegion::Biology},
    {BossBeehiveGun, MapRegion::Biology},
    {BossBeehiveMain, MapRegion::Biology},
    {EnemySnail, MapRegion::Biology},
    {EnemyEvilSnail, MapRegion::Biology},
    {EnemyOneBee, MapRegion::Biology},
    {EnemyTwoBee, MapRegion::Biology},
    {EnemyThreeBee, MapRegion::Biology},
    {EnemyCrab, MapRegion::Biology},
    {EnemyEvilCrab, MapRegion::Biology},
    {EnemyLaserCrab, MapRegion::Biology},
    {EnemyBioBoid, MapRegion::Biology},
    {EnemyPufferfish, MapRegion::Biology},
    {EnemyBeeHive, MapRegion::Biology},
    {HardEnemyAngel, MapRegion::Biology},

    {EnemyMiningBoulderSmall, MapRegion::Mining},
    {EnemyMiningBoulderBig, MapRegion::Mining},
    {EnemySmallMole, MapRegion::Mining},
    {EnemySurfaceMole, MapRegion::Mining},
    {EnemyDynamite, MapRegion::Mining},
    {EnemyBulldozer, MapRegion::Mining},
    {EnemySmallBoulder, MapRegion::Mining},
    {EnemyBigBoulder, MapRegion::Mining},
    {BossDrillWormHead, MapRegion::Mining},
    {BossDrillWormBody, MapRegion::Mining},
    {EnemyChainDogHead, MapRegion::Mining},
    {EnemyChainDogBody, MapRegion::Mining},
    {EnemyMiningBoidWormHead, MapRegion::Mining},
    {EnemyMiningBoidWormBody, MapRegion::Mining},
    {EnemyPileDriverTurret, MapRegion::Mining},
    {EnemySmallMiningWormHead, MapRegion::Mining},
    {EnemySmallMiningWormBody, MapRegion::Mining},
    {BossMole, MapRegion::Mining},

    {EnemyHifiBoidFish, MapRegion::Physics},
    {EnemyTwinLaserVertical1, MapRegion::Physics},
    {EnemyTwinLaserVertical2, MapRegion::Physics},
    {EnemyHifiTwinLaserHorizontal1, MapRegion::Physics},
    {EnemyHifiTwinLaserHorizontal2, MapRegion::Physics},
    {EnemySniper, MapRegion::Physics},
    {EnemyHifiCharger, MapRegion::Physics},
    {EnemyHifiTrail, MapRegion::Physics},
    {EnemyHifiCannon, MapRegion::Physics},
    {EnemyHifiChargerHard, MapRegion::Physics},
    {EnemyHifiSniperHard, MapRegion::Physics},
    {EnemyHifiTrailHard, MapRegion::Physics},
    {EnemyHifiCannonHard, MapRegion::Physics},
    {EnemyLaserSniper, MapRegion::Physics},
    {EnemyLaserSniperHard, MapRegion::Physics},
    {EnemyHifiTemporaryBoid, MapRegion::Physics},
    {EnemyHifiJellyFish, MapRegion::Physics},
    {EnemyHifiTackShooter, MapRegion::Physics},
    {EnemyHifiBallLauncher, MapRegion::Physics},
    {EnemyHifiWhip, MapRegion::Physics},
    {BossBigC, MapRegion::Physics},
    {BossBigCShield, MapRegion::Physics},
    {EnemyHifiBoid, MapRegion::Physics},
    {EnemyFishBoid, MapRegion::Physics},
    {BossMultiCube, MapRegion::Physics},
    {BossConstructYELLOW, MapRegion::Physics},
    {BossConstructPURPLE, MapRegion::Physics},
    {BossConstructGREEN, MapRegion::Physics},
    {BossConstructRED, MapRegion::Physics},


    // Medical
    {EnemyProstheticHand, MapRegion::Medical},
    {EnemyMedicalBoid, MapRegion::Medical},
    {EnemySpinePatrolWormHead, MapRegion::Medical},
    {EnemySpinePatrolWormBody, MapRegion::Medical},
    {EnemySpineFollowWormHead, MapRegion::Medical},
    {EnemySpineFollowWormBody, MapRegion::Medical},
    {EnemySpineChainedWormHead, MapRegion::Medical},
    {EnemySpineChainedWormBody, MapRegion::Medical},
    {EnemyMedicalBMP, MapRegion::Medical},
    {EnemyScissors, MapRegion::Medical},
    {EnemyMedicalRodA, MapRegion::Medical},
    {EnemyMedicalRodC, MapRegion::Medical},
    {EnemyMedicalPillBoid, MapRegion::Medical},
    {EnemyMedicalPillBoidSpawner, MapRegion::Medical},
    {EnemyMedicalSyringe, MapRegion::Medical},

    // Scientist
    {ScientistradialAttack, MapRegion::Medical},
    {ScientistexplosiveAttack, MapRegion::Medical},
    {ScientistHomingAttack, MapRegion::Medical},
    {ScientistLaserGridAttack, MapRegion::Medical},
    {ScientistLaserGridVerticalAttack, MapRegion::Medical},
    {ScientistShield, MapRegion::Medical},
    {ScientistBoss, MapRegion::Medical},
    {ScientistHand, MapRegion::Medical},
    {ScientistlaserAttack, MapRegion::Medical},

    // Military
    {EnemyEyeCube, MapRegion::Military},
    {EnemyPhantom, MapRegion::Military},
    {EnemyMaw, MapRegion::Military},
    {EnemySkullMissile, MapRegion::Military},
    {EnemyCross, MapRegion::Military},

    // Testing
    {EnemyTestLightningRotate, MapRegion::Military},
    {EnemyTestLightningShuffle, MapRegion::Military},
    {EnemyTestEffectBlunt, MapRegion::Military},
    {EnemyTestEffectPlusOne, MapRegion::Military},
    {EnemyTestEffectPlusTwo, MapRegion::Military},
    {EnemyTestEffectPlusThree, MapRegion::Military},
    {EnemyTestEffectMinusOne, MapRegion::Military},
    {EnemyTestEffectMinusTwo, MapRegion::Military},
    {EnemyTestEffectMinusThree, MapRegion::Military},

    // event room enemies
    // single target buffs, place at the same position as target
    {InvincibleGranter, MapRegion::Military},
    {InvisibleGranter, MapRegion::Military},
    {VulnerableGranter, MapRegion::Military},
    {ProtectedGranter, MapRegion::Military},
    {UnderGroundGranter, MapRegion::Military},
    {RegeneratingGranter, MapRegion::Military},
    {CloakedGranter, MapRegion::Military},
    {HastyGranter, MapRegion::Military},
    {SluggishGranter, MapRegion::Military},

    // room wide buffs, place at the center of the room
    {InvincibleGranterRoomWide, MapRegion::Military},
    {InvisibleGranterRoomWide, MapRegion::Military},
    {VulnerableGranterRoomWide, MapRegion::Military},
    {ProtectedGranterRoomWide, MapRegion::Military},
    {UnderGroundGranterRoomWide, MapRegion::Military},
    {RegenerateGranterRoomWide, MapRegion::Military},
    {CloakedGranterRoomWide, MapRegion::Military},
    {HastyGranterRoomWide, MapRegion::Military},
    {SluggishGranterRoomWide, MapRegion::Military},

     // Enemyparts, regions with tutorial will not to be scaled
     {EnemyBubbleShield,MapRegion::Tutorial},
    {EnemySmallCShield, MapRegion::Tutorial},
};