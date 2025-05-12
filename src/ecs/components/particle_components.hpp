#pragma once
#include "common.hpp"
#define UNSET_VEC2 vec2(-1000.f)

struct ColorEffect {
    Vec4StartEnd color;
    int amount = 0;
};
struct ParticleProps {
    Vec2Randomized position = {{0,0},{0,0}};
    Vec2Randomized velocity = {{0,0},{100,100}};

    std::vector<ColorEffect> colorEffects = {{{{ 143 / 255.0f, 37 / 255.0f, 132 / 255.0f, 1.0f },{ 17 / 255.0f, 14 / 255.0f, 99 / 255.0f, .4f }},0}}; //begin, end
    FloatStartEndRandomized size = {10.f,2.f,3.f}; //begin, end, variation
    
    int textureRowIndex = -1;
    float lifetime = 1000.0f;
};

enum ParticleRequestType {
	PlayerDash,
	EnemyDeath,
	PlayerBulletCollision,

    //updated types
    PExplode,
    PPlayerTrail,
    PBulletTrail,
    PWallCollision,
    PLaser,
    PImplosion, //explosion played backwards
    FloatUpwards, // emit particles upwards

	ClearParticles // special request to clear all current particles
};

struct EmitParticle {
	ParticleRequestType requestType;
    ParticleProps props;
	
	float timeRemaining; //in milliseconds
	int numToEmitPerSecond; //remaining number to emit, divided evenly throughout the countdown

	vec2 defaultPos = UNSET_VEC2; //somewhere off screen as the unset position

    //reqtype specific parameters
    vec2 impactDirection;

	EmitParticle(ParticleRequestType reqType, ParticleProps props,float duration = 0.0f, int numToEmitPerSecond = 0) {
		this->requestType = reqType;
        this->props = props;
		this->timeRemaining = duration;
		this->numToEmitPerSecond = numToEmitPerSecond;
	}
};

