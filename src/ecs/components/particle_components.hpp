#pragma once
#include "common.hpp"

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
    vec4 colorBegin, colorEnd;
	
	float timeRemaining; //in seconds
	int numToEmit; //remaining number to emit, divided evenly throughout the countdown

	vec2 defaultPos; //position to fallback to if attached entity does not have motion

	EmitParticle(ParticleRequestType reqType, float duration = 0.0f, int numToEmit = 0) {
		this->requestType = reqType;
		this->timeRemaining = duration;
		this->numToEmit = numToEmit;
	}
};

struct ParticleProps {
    vec2 position;
    vec2 velocity,velocityVariation;
    vec4 colorBegin, colorEnd;
    float sizeBegin, sizeEnd, sizeVariation;
    float lifetime;
};