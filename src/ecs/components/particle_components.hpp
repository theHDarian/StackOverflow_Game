#pragma once
#include "common.hpp"

struct ParticleProps {
    vec2 position = {0,0};
    vec2 positionVariation = {0,0};

    vec2 velocity = {0,0};
    vec2 velocityVariation = {10,10};

    vec4 colorBegin = { 143 / 255.0f, 37 / 255.0f, 132 / 255.0f, 1.0f };
    vec4 colorEnd = { 17 / 255.0f, 14 / 255.0f, 99 / 255.0f, .4f };

    float sizeBegin = 10.f;
    float sizeEnd = 2.0f;
    float sizeVariation = 3.f;
    
    int textureIndex = -1;
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

	ClearParticles // special request to clear all current particles
};

struct EmitParticle {
	ParticleRequestType requestType;
    ParticleProps props;
	
	float timeRemaining; //in seconds
	int numToEmit; //remaining number to emit, divided evenly throughout the countdown

	vec2 defaultPos; //position to fallback to if attached entity does not have motion

	EmitParticle(ParticleRequestType reqType, ParticleProps props,float duration = 0.0f, int numToEmit = 0) {
		this->requestType = reqType;
        this->props = props;
		this->timeRemaining = duration;
		this->numToEmit = numToEmit;
	}
};

