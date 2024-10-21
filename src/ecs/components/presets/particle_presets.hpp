#include "components/components.hpp"

ParticleProps DefaultParticle() {
	ParticleProps p;
	p.velocity = { 0.0f, 0.0f };
	p.velocityVariation = { 5.0f, 5.0f };
	p.colorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	p.colorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	p.sizeBegin = 10.0f;
	p.sizeVariation = 3.0f;
	p.sizeEnd = 2.0f;
	p.lifetime = 1000.0f;
	return p;
}
