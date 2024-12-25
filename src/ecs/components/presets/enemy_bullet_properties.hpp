#pragma once

#include <array>
#include <utility>
#include "actor_components.hpp"
#include "common.hpp"
#include "utils/colours.hpp"

const std::unordered_map<BulletEffectType, vec3> bulletEffectColors = {
			{BulletDamage,      COLOR_PURPLE_MED},
			{ProjectileSpeed,   COLOR_PINK},
			{ProjectileSize,    COLOR_MAGENTA_LIGHT},
			{FireRate,          COLOR_MAGENTA_MED},
			{BulletRange,       COLOR_YELLOW},
			{BulletSpread,      COLOR_ORANGE},
			{BulletNum,         COLOR_TEAL_LIGHT},
			{BulletBurst,       COLOR_LAET_LIGHT},
			{Bounce,            COLOR_BLUE},
			{Pierce,            COLOR_BLUE_SKY},
			{Homing,            COLOR_TURQUOISE},
			{PlayerSpeed,       COLOR_GREEN_LIGHT},
			{PlayerNumDash,     COLOR_GREEN},
			{PlayerStackSize,   {0.f, 0.f, 0.f}},
			{PlayerDashCDR,     COLOR_BROWN},
			{Inert,             COLOR_GREY_LIGHT},
			{Lightning,			{1.f,1.f,1.f}},
			{Key,				{1.0, 1.0, 1.0}},
				{ Pop,				{1.0, 1.0, 1.0} }
	};

const std::unordered_map<BulletEffectType, std::string> bulletEffectShapes = {
			{BulletDamage,      "enemy_bullet_square.png"},
			{ProjectileSpeed,   "enemy_bullet_square.png"},
			{ProjectileSize,    "enemy_bullet_square.png"},
			{FireRate,          "enemy_bullet_square.png"},
			{BulletRange,       "enemy_bullet_square.png"},
			{BulletSpread,      "enemy_bullet_square.png"},
			{BulletNum,         "enemy_bullet_square.png"},
			{BulletBurst,       "enemy_bullet_square.png"},
			{Bounce,            "enemy_bullet_square.png"},
			{Pierce,            "enemy_bullet_square.png"},
			{Homing,            "enemy_bullet_square.png"},
			{PlayerSpeed,       "enemy_bullet_square.png"},
			{PlayerNumDash,     "enemy_bullet_square.png"},
			{PlayerStackSize,   "enemy_bullet_square.png"},
			{PlayerDashCDR,     "enemy_bullet_square.png"},
			{Inert,             "enemy_bullet_square.png"},
			{Key,				"enemy_bullet_key (2).png"},
			{ Pop,				"enemy_bullet_square.png" }
	};

const std::map<BulletEffectType, Vec4StartEnd> enemyBulletParticleColors = {
    {BulletDamage, {{ bulletEffectColors.at(BulletDamage), 1.0f },{ bulletEffectColors.at(BulletDamage), 1.0f }}},
    {ProjectileSpeed,   {{bulletEffectColors.at(ProjectileSpeed),1.f},{bulletEffectColors.at(ProjectileSpeed),1.f}}},
    {ProjectileSize,    {{bulletEffectColors.at(ProjectileSize),1.f},{bulletEffectColors.at(ProjectileSize),1.f}}},
    {FireRate,          {{bulletEffectColors.at(FireRate),1.f},{bulletEffectColors.at(FireRate),1.f}}},
    {BulletRange,       {{bulletEffectColors.at(BulletRange),1.f},{bulletEffectColors.at(BulletRange),1.f}}},
    {BulletSpread,      {{bulletEffectColors.at(BulletSpread),1.f},{bulletEffectColors.at(BulletSpread),1.f}}},
    {BulletNum,         {{bulletEffectColors.at(BulletNum),1.f},{bulletEffectColors.at(BulletNum),1.f}}},
    {BulletBurst,       {{bulletEffectColors.at(BulletBurst),1.f},{bulletEffectColors.at(BulletBurst),1.f}}},
    {Bounce,            {{bulletEffectColors.at(Bounce),1.f},{bulletEffectColors.at(Bounce),1.f}}},
    {Pierce,            {{bulletEffectColors.at(Pierce),1.f},{bulletEffectColors.at(Pierce),1.f}}},
    {Homing,            {{bulletEffectColors.at(Homing),1.f},{bulletEffectColors.at(Homing),1.f}}},
    {PlayerSpeed,       {{bulletEffectColors.at(PlayerSpeed),1.f},{bulletEffectColors.at(PlayerSpeed),1.f}}},
    {PlayerNumDash,     {{bulletEffectColors.at(PlayerNumDash),1.f},{bulletEffectColors.at(PlayerNumDash),1.f}}},
    {PlayerStackSize,   {{bulletEffectColors.at(PlayerStackSize),1.f},{bulletEffectColors.at(PlayerStackSize),1.f}}},
    {PlayerDashCDR,     {{bulletEffectColors.at(PlayerDashCDR),1.f},{bulletEffectColors.at(PlayerDashCDR),1.f}}},
    {Inert,             {{0,0,0,0},{0,0,0,0}}},
	{Lightning,			{{0,0,0,0},{0,0,0,0}}},
    {Key, 				{{ 255 / 255.0f, 215 / 255.0f, 0 / 255.0f, 1.0f },{ 250 / 255.0f, 250 / 255.0f, 210 / 255.0f, .9f }}},
	{ Pop,				{{ 0, 0, 0, 0 },{ 0, 0, 0, 0 }}}
};