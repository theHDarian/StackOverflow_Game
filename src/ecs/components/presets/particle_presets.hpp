#pragma once

#include "components/particle_components.hpp"

/**
 * Presets for individual particles - set colors, size, and durations. Velocity and positions are determined by emission request type
 */

// struct ParticleProps {
//     Vec2Randomized position = {{0,0},{0,0}};
//     Vec2Randomized velocity = {{0,0},{10,10}};

//     Vec4StartEnd color = {{ 143 / 255.0f, 37 / 255.0f, 132 / 255.0f, 1.0f },{ 17 / 255.0f, 14 / 255.0f, 99 / 255.0f, .4f }}; //begin, end
//     FloatStartEndRandomized size = {10.f,2.f,3.f}; //begin, end, variation
    
//     int textureIndex = -1;
//     float lifetime = 1000.0f;
// };

const ParticleProps playerTrail {
    {{0,0},{0,0}},
    {{0,0},{10,10}},
    {{ 39 / 255.0f, 217 / 255.0f, 95 / 255.0f, .7f },{ 42 / 255.0f, 124 / 255.0f, 133 / 255.0f, .4f }},
    {10.f,2.f,3.f},
    -1,
    1000.f
};

const ParticleProps sparks {
    {{0,0},{0,0}},
    {{0,0},{10,10}},
    {{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f },{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f }},
    {10.f,2.f,3.f},
    -1,
    1000.f
};
