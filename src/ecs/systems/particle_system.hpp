#pragma once
#include "common.hpp"
#include <array>
#include <utility>
#include <vector>
#include <glm/glm.hpp>

struct ParticleProps {
    vec2 position;
    vec2 velocity,velocityVariation;
    vec4 colorBegin, colorEnd;
    float sizeBegin, sizeEnd, sizeVariation;
    float lifetime;
};

class ParticleSystem {
public:
    ParticleSystem();
    ~ParticleSystem();
    void init();
    void step(float elapsed_ms);
    void emit(const ParticleProps& particleProps);
    void render();

    ParticleProps createParticle();
private:
    struct Particle {
        vec2 position;
        vec2 velocity;
        vec4 colorBegin,colorEnd;
        float rotation;
        float sizeBegin, sizeEnd;

        float lifetime = 1000.0f;
        float lifeRemaining = 0.0f; //in milliseconds

        bool active = false;
    };

    std::vector<Particle> particlePool;
    uint poolIndex = 999;
    
    GLuint vao;
	GLuint shaderProgram;
    glm::mat4 projection;
	GLint m_ParticleShaderViewProj, m_ParticleShaderTransform, m_ParticleShaderColor;
};