#pragma once
#include "components.hpp"

// defines our palette colours
// for standardizing colours used for ui 
// will update with more in the future

const vec3 COLOR_YELLOW =		{ 223 / 255.f, 173 / 255.f, 48 / 255.f };
const vec3 COLOR_ORANGE =		{ 194 / 225.f, 112 / 225.f, 50 / 255.f };
const vec3 COLOR_BROWN =		{ 120 / 225.f, 91 / 225.f, 38 / 255.f };
const vec3 COLOR_GREEN =		{ 22 / 255.f, 128 / 255.f, 25 / 255.f };
const vec3 COLOR_GREEN_LIGHT =	{ 152 / 255.f, 174 / 255.f, 60/ 255.f };
const vec3 COLOR_TURQUOISE =	{ 87 / 255.f, 186 / 255.f, 184/ 255.f };
const vec3 COLOR_BLUE =			{ 20 / 255.f, 60 / 255.f, 212 / 255.f };
const vec3 COLOR_BLUE_SKY =		{ 93 / 255.f, 135 / 255.f, 212/ 255.f };
const vec3 COLOR_PURPLE_LIGHT = { 168 / 255.f, 139 / 255.f, 224 / 255.f };
const vec3 COLOR_PURPLE_MED =	{ 100 / 255.f, 85 / 255.f, 188/ 255.f };
const vec3 COLOR_PURPLE_DARK =	{ 74 / 255.f, 39 / 255.f, 109 / 255.f };
const vec3 COLOR_PINK =			{ 194 / 255.f, 101 / 255.f, 142/ 255.f };
const vec3 COLOR_RED =			{ 173 / 255.f, 49 / 255.f, 75/ 255.f };
const vec3 COLOR_MAGENTA_LIGHT ={ 225 / 255.f, 82 / 255.f, 153 / 255.f };
const vec3 COLOR_MAGENTA_MED =	{ 155 / 255.f, 56 / 255.f, 105 / 255.f };
const vec3 COLOR_MAGENTA_DARK = { 105 / 255.f, 36 / 255.f, 75/ 255.f };
const vec3 COLOR_LAET_LIGHT =	{ 50 / 255.f, 148 / 255.f, 197 / 255.f };
const vec3 COLOR_TEAL_LIGHT =	{ 50 / 255.f, 197 / 255.f, 148/ 255.f };
const vec3 COLOR_TEAL_MED =		{ 36 / 255.f, 128 / 255.f, 115/ 255.f };
const vec3 COLOR_TEAL_DARK =	{ 21 / 255.f, 85 / 255.f, 97/ 255.f };
const vec3 COLOR_GREY_LIGHT =	{ 177 / 255.f, 184 / 255.f, 205/ 255.f };
const vec3 COLOR_GREY_MED =		{ 91 / 255.f, 98 / 255.f, 128/ 255.f };
const vec3 COLOR_GREY_DARK =	{ 63 / 255.f, 64 / 255.f, 105/ 255.f };
const vec3 COLOR_BLUE_MED =		{ 57 / 255.f, 70 / 255.f, 114/ 255.f };
const vec3 COLOR_BLUE_DARK =	{ 32 / 255.f, 43 / 255.f, 81/ 255.f };
const vec3 COLOR_BLUE_BLACK =	{ 14 / 255.f, 20 / 255.f, 38/ 255.f };
const vec3 COLOR_WHITE =		{ 1.f, 1.f, 1.f };
const vec3 COLOR_BLACK =		{ 0.f, 0.f, 0.f };

// a string to color map to make script writing easier
const std::unordered_map<std::string, vec3> colorNames = {
	{"yellow", COLOR_YELLOW },
	{"orange", COLOR_ORANGE},
	{"brown", COLOR_BROWN},
	{"green", COLOR_GREEN},
	{"green_light", COLOR_GREEN_LIGHT},
	{"turquoise", COLOR_TURQUOISE},
	{"blue", COLOR_BLUE},
	{"blue_sky", COLOR_BLUE_SKY},
	{"purple_light",COLOR_PURPLE_LIGHT},
	{"purple_med", COLOR_PURPLE_MED},
	{"purple_dark", COLOR_PURPLE_DARK},
	{"pink", COLOR_PINK},
	{"red", COLOR_RED},
	{"magenta_light", COLOR_MAGENTA_LIGHT},
	{"magenta_med", COLOR_MAGENTA_MED},
	{"magenta_dark", COLOR_MAGENTA_DARK},
	{"laet_light", COLOR_LAET_LIGHT},
	{"teal_light", COLOR_TEAL_LIGHT},
	{"teal_med", COLOR_TEAL_MED},
	{"teal_dark", COLOR_TEAL_DARK},
	{"grey_light", COLOR_GREY_LIGHT},
	{"grey_med", COLOR_GREY_MED},
	{"grey_dark", COLOR_GREY_DARK},
	{"blue_med", COLOR_BLUE_MED},
	{"blue_dark", COLOR_BLUE_DARK},
	{"blue_black", COLOR_BLUE_BLACK},
	{"white", COLOR_WHITE},
	{"black", COLOR_BLACK}
};

// map string to text animation types for script writing
const std::unordered_map<std::string, TextAnimationType> textAnimationNames = {
	{"wavy", TextAnimationType::WavyText},
	{"wobbly", TextAnimationType::WobblyText}
};

const vec2 STATUS_ICON_SCALE = { 30, 30 };
const vec2 STATUS_ICON_OFFSET = { 15, 5 };
const float STATUS_ICON_BOSS_MULTIPLIER = 1.5;
const float STATUS_TEXT_BOSS_MULTIPLIER = 1.3;
const float STATUS_TEXT_SCALE = 0.3;

const std::map<SpecialStates, vec3> specialStatesToColor = {
	{SpecialStates::NORMAL, COLOR_WHITE},
	{SpecialStates::INVINCIBLE, COLOR_YELLOW}, // old yellow was: {1, 1, 0.3}
	{SpecialStates::PROTECTED,  COLOR_TURQUOISE},
	{SpecialStates::VULNERABLE, COLOR_PURPLE_LIGHT},
	{SpecialStates::INVISIBLE, {1, 0, 1}}, // purple
	{SpecialStates::UNDERGROUND, COLOR_BROWN},
	{SpecialStates::REGENERATING, COLOR_GREEN}, // old green: {0, 1, 0}
	{SpecialStates::CLOAKED, COLOR_MAGENTA_MED },
	{SpecialStates::ONFIRE, COLOR_ORANGE},
};

const vec2 TIER_ICON_SCALE = { 45, 45 };
const vec2 TIER_ICON_OFFSET = { 15, 0 };