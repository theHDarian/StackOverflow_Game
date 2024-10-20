#include "components/components.hpp"

ParticleProps DefaultParticle {
	.velocity = { 0.0f, 0.0f },
	.velocityVariation = { 5.0f, 5.0f },
	.colorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f },
	.colorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f },
	.sizeBegin = 10.0f,
	.sizeVariation = 3.0f,
	.sizeEnd = 2.0f,
	.lifetime = 1000.0f,
};