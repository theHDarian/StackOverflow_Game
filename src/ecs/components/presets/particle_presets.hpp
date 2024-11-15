#pragma once

#include "components/particle_components.hpp"

/**
 * Presets for individual particles - set colors, size, and durations. Velocity and positions are determined by emission request type
 */

// struct ParticleProps {
//     Vec2Randomized position = {{0,0},{0,0}};
//     Vec2Randomized velocity = {{0,0},{100,100}};

//     Vec4StartEnd color = {{ 143 / 255.0f, 37 / 255.0f, 132 / 255.0f, 1.0f },{ 17 / 255.0f, 14 / 255.0f, 99 / 255.0f, .4f }}; //begin, end
//     FloatStartEndRandomized size = {10.f,2.f,3.f}; //begin, end, variation
    
//     int textureIndex = -1;
//     float lifetime = 1000.0f;
// };


const ParticleProps playerTrail {
    {{0,0},{0,0}},
    {{0,0},{100,100}},
    {{{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f },{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f }}},
    {8.f,0.f,2.f},
    -1,
    800.f
};

const ParticleProps sparks {
    {{0,0},{0,0}},
    {{0,0},{100,100}},
    {{{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f },{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f }}},
    {10.f,2.f,3.f},
    -1,
    1000.f
};

const ParticleProps playerBulletTrail {
    {{0,0},{0,0}},
    {{0,0},{0,0}},
    {{{ 73 / 255.0f, 214 / 255.0f, 101 / 255.0f, 1.0f },{ 123 / 255.0f, 219 / 255.0f, 131 / 255.0f, .4f }}},
    {6.f,1.f,2.f},
    -1,
    300.f
};

const ParticleProps playerBulletCollision {
    {{0,0},{0,0}},
    {{0,0},{100,100}},
    {{{ 73 / 255.0f, 214 / 255.0f, 101 / 255.0f, 1.0f },{ 66 / 255.0f, 245 / 255.0f, 194 / 255.0f, .8f }}},
    {6.f,1.f,2.f},
    -1,
    800.f
};


const ParticleProps enemyBullet {
    {{0,0},{0,0}},
    {{0,0},{0,0}},
    {},
    {6.f,1.f,2.f},
    -1,
    300.f
};

const std::map<BulletEffectType, Vec4StartEnd> enemyBulletColors = {
    {BulletDamage, {{ 255 / 255.0f, 0 / 255.0f, 255 / 255.0f, 1.0f },{ 255 / 255.0f, 0 / 255.0f, 255 / 255.0f, .5f }}},
    {ProjectileSpeed,   {{1.f, 0.f, 0.f,1.f},{1.f, 0.f, 0.f,1.f}}},
    {ProjectileSize,    {{173.f / 255.f, 49.f / 255.f, 75.f / 255.f,1.f},{173.f / 255.f, 49.f / 255.f, 75.f / 255.f,1.f}}},
    {FireRate,          {{1.f, 1.f, 1.f,1.f},{1.f, 1.f, 1.f,1.f}}},
    {BulletRange,       {{0.f, 0.f, 1.f,1.f},{0.f, 0.f, 1.f,1.f}}},
    {BulletSpread,      {{1.f, 0.f, 0.f,1.f},{1.f, 0.f, 0.f,1.f}}},
    {BulletNum,         {{0.f, 0.f, 1.f,1.f},{1.f, 0.f, 0.f,1.f}}},
    {BulletBurst,       {{0.f, 0.f, 0.f,1.f},{0.f, 0.f, 0.f,1.f}}},
    {Bounce,            {{0.f, 1.f, 0.f,1.f},{0.f, 1.f, 0.f,1.f}}},
    {Pierce,            {{1.f, 0.f, 0.f,1.f},{1.f, 0.f, 0.f,1.f}}},
    {Homing,            {{1.f, 0.f, 0.f,1.f},{1.f, 0.f, 0.f,1.f}}},
    {PlayerSpeed,       {{0.f, 0.f, 1.f,1.f},{0.f, 0.f, 1.f,1.f}}},
    {PlayerNumDash,     {{0.f, 1.f, 1.f,1.f},{0.f, 1.f, 1.f,1.f}}},
    {PlayerStackSize,   {{0.f, 1.f, 0.f,1.f},{0.f, 1.f, 0.f,1.f}}},
    {PlayerDashCDR,     {{1.f, 0.f, 0.f,1.f},{1.f, 0.f, 0.f,1.f}}},
    {Inert,             {{0,0,0,0},{0,0,0,0}}},
    {Key, {{ 255 / 255.0f, 215 / 255.0f, 0 / 255.0f, 1.0f },{ 250 / 255.0f, 250 / 255.0f, 210 / 255.0f, .9f }}},
};