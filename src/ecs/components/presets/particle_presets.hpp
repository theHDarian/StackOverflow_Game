#pragma once

#include "components/particle_components.hpp"
#include "render_system.hpp"
#include "./enemy_bullet_properties.hpp"

/**
 * Presets for individual particles - set colors, size, and durations. Velocity and positions are determined by emission request type
 */

// struct ParticleProps {
//     Vec2Randomized position = {{0,0},{0,0}};
//     Vec2Randomized velocity = {{0,0},{100,100}};

//     Vec4StartEnd color = {{ 143 / 255.0f, 37 / 255.0f, 132 / 255.0f, 1.0f },{ 17 / 255.0f, 14 / 255.0f, 99 / 255.0f, .4f }}; //begin, end
//     FloatStartEndRandomized size = {10.f,2.f,3.f}; //begin, end, variation
    
//     int textureRowIndex = -1;
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

const ParticleProps enemyBulletExplosion {
    {{0,0},{30,30}},
    {{0,0},{150,150}},
    {{{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f },{ 51 / 255.0f, 43 / 255.0f, 42 / 255.0f, 1.0f }}},
    {20.f,2.f,5.f},
    -1,
    500.f
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

const ParticleProps playerDamaged {
    {{0,0},{0,0}},
    {{0,0},{100,100}},
    {{{1,1,1,1},{1,1,1,1}}},
    {20.f,5.f,2.f},
    1, //row 1 of textures
    800.f
};
const ParticleProps enemyDeath {
    {{0,0},{0,0}},
    {{0,0},{50,50}},
    {{{1,1,1,1},{1,1,1,1}}},
    {20.f,5.f,2.f},
    5, //row 3 of textures
    1200.f
};

const ParticleProps playerZappedYellow {
    {{0,0},{0,0}},
    {{0,0},{100,100}},
    {{{1,1,1,1},{1,1,1,1}}},
    {20.f,5.f,2.f},
    2, //row 3 of textures
    800.f
};

const ParticleProps playerZappedBlue {
    {{0,0},{0,0}},
    {{0,0},{100,100}},
    {{{1,1,1,1},{1,1,1,1}}},
    {20.f,5.f,2.f},
    3, 
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
const ParticleProps enemyBulletDeathParticle {
    {{0,0},{0,0}},
    {{0,0},{100,100}},
    {},
    {6.f,1.f,2.f},
    -1,
    800.f
};

