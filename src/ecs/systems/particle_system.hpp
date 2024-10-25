#pragma once
#include "common.hpp"
#include <array>
#include <utility>
#include <vector>
#include <glm/glm.hpp>
#include "components.hpp"

class ParticleSystem {
public:
    ParticleSystem();
    ~ParticleSystem();
    void init(GLFWwindow* window);
    void step(float elapsed_ms);
    void emit(const ParticleProps& particleProps);

    void explode(const ::ParticleProps &props, vec2 origin);

    void render();
    bool initScreenTexture();

    ParticleProps createParticle(vec2 pos);
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
    void handleEmitRequests(float elapsed_ms);

    std::vector<Particle> particlePool;
    uint poolSize = 1000;
    uint poolIndex = 999;
    
    GLuint vao, vbo;
	GLuint shaderProgram;
    GLuint frame_buffer;
    GLuint off_screen_render_buffer_color;
	GLuint off_screen_render_buffer_depth;

    std::array<GLuint,2> texture_handles;
    
    glm::mat4 projection;
    GLFWwindow* window;
};