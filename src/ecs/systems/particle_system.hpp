#pragma once
#include "common.hpp"
#include <array>
#include <utility>
#include <vector>
#include <glm/glm.hpp>
#include "particle_components.hpp"
#define POOLSIZE 3000
#define TEXTURE_ROW_SIZE 4 //number of textures per row in the spritesheet
#define TEXTURE_NUM_ROWS 8

class ParticleSystem {
public:
    ParticleSystem();
    ~ParticleSystem();
    void init(GLFWwindow* window);
    void step(float elapsed_ms);

    int activateParticle(const ParticleProps& props);
    void explode(const ParticleProps& props, int emitCount,bool isImplosion);
    void impact(const ParticleProps& props, int emitCount, vec2 direction);
    void trail(const ParticleProps& props, int emitCount);
    void floatUp(const ParticleProps& props, int emitCount, vec2 speed);

    void render();
    bool initScreenTexture();
    void clearParticles();
private:
    struct Particle {
        vec2 position;
        vec2 velocity;
        vec4 colorBegin,colorEnd;
        int textureIndex;
        float rotation;
        float sizeBegin, sizeEnd;

        float lifetime = 1000.0f;
        float lifeRemaining = 0.0f; //in milliseconds

        bool active = false;
    };
    struct Vertex {
        vec3 position;
        vec4 color;
        vec2 texCoords;
        float texID;
    };

    void handleEmitRequests(float elapsed_ms);
    Vertex* createQuad(Vertex* target, vec4 color, mat4 transform, float texID);
    GLuint loadTexture(const std::string& path);

    std::vector<Particle> particlePool;
    uint poolIndex = 999;
    
    GLuint vao, vbo, ib;
	GLuint shaderProgram;
    GLuint frame_buffer;
    GLuint off_screen_render_buffer_color;
	GLuint off_screen_render_buffer_depth;

    GLuint texture_handle;
    
    glm::mat4 projection;
    GLFWwindow* window;
};