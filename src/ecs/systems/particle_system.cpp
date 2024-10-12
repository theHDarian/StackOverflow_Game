#include "particle_system.hpp"
#include <glm/gtc/constants.hpp>
#include <glm/gtx/compatibility.hpp>
#include "../utils/random.hpp"
#include "tiny_ecs_registry.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "render_system.hpp"

ParticleSystem::ParticleSystem() {
    particlePool.resize(1000);
}

void ParticleSystem::init() {
    //setup projection matrix
    int window_width_px,window_height_px;
	const GLFWvidmode* vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	window_width_px = vidMode->width;
	window_height_px = vidMode->height;    
    projection = glm::ortho(0.0f,static_cast<float>(window_width_px),0.0f,static_cast<float>(window_height_px));

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    GLuint vbo,ib;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3, GL_FLOAT,GL_FALSE,3*sizeof(float),0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    gl_has_errors();
    

    uint32_t indices[] = { 0, 1, 2, 2, 3, 0 }; //triangles that make up the quad

    glGenBuffers(1, &ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    gl_has_errors();
    glBindVertexArray(0);
    gl_has_errors();

    bool is_valid = loadEffectFromFile(shader_path("text.vs.glsl").c_str(), shader_path("text.fs.glsl").c_str(), shaderProgram);
    assert(is_valid && (GLuint)shaderProgram != 0);
    gl_has_errors();

    //Clean up ib and vbo
    glDeleteBuffers(1,&vbo);
    glDeleteBuffers(1,&ib);
}

ParticleSystem::~ParticleSystem() {
    if (vao)
    {
        glDeleteVertexArrays(1, &vao); 
    }
    if (shaderProgram) {
        glDeleteProgram(shaderProgram);
    }
}

void ParticleSystem::step(float elapsed_ms) {
    float ts = elapsed_ms / 1000.0f;
    for (auto& particle : particlePool) {
        if (!particle.active) {
            continue;
        }
        if (particle.lifeRemaining <= 0.0f) {
            particle.active = false;
            continue;
        }
        particle.lifeRemaining -= elapsed_ms;
        particle.position += particle.velocity * ts;
        particle.rotation += 0.01f * ts;
    }
}

void ParticleSystem::emit(const ParticleProps& props) {
    Particle& particle = particlePool[poolIndex];
	particle.active = true;
	particle.position = props.position;
	particle.rotation = Random::Float() * 2.0f * glm::pi<float>();

	// Velocity
	particle.velocity = props.velocity;
	particle.velocity.x += props.velocityVariation.x * (Random::Float() - 0.5f);
	particle.velocity.y += props.velocityVariation.y * (Random::Float() - 0.5f);

	// Color
	particle.colorBegin = props.colorBegin;
	particle.colorEnd = props.colorEnd;

	particle.lifetime = props.lifetime;
	particle.lifeRemaining = props.lifetime;
	particle.sizeBegin = props.sizeBegin + props.sizeVariation * (Random::Float() - 0.5f);
	particle.sizeEnd = props.sizeEnd;

	poolIndex = --poolIndex % particlePool.size();
}

void ParticleSystem::render() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(shaderProgram);
    gl_has_errors();
    unsigned int projectionLoc = glGetUniformLocation(shaderProgram,"projection");
    unsigned int transformLoc = glGetUniformLocation(shaderProgram,"transform");
    unsigned int colorLoc = glGetUniformLocation(shaderProgram,"fcolor");
    glUniformMatrix4fv(projectionLoc,1,GL_FALSE,glm::value_ptr(projection));
    gl_has_errors();

    for (auto& particle : particlePool) {
        if (!particle.active) {
            continue;
        }
        float lifePassed = (particle.lifetime-particle.lifeRemaining) / particle.lifetime;
        
        glm::vec4 color = glm::lerp(particle.colorBegin,particle.colorEnd,lifePassed);
        float size = glm::lerp(particle.sizeBegin,particle.sizeEnd,lifePassed);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), { particle.position.x, particle.position.y, 0.0f })
            * glm::rotate(glm::mat4(1.0f), particle.rotation, { 0.0f, 0.0f, 1.0f })
            * glm::scale(glm::mat4(1.0f), { size, size, 1.0f });

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glUniform4fv(colorLoc, 1, glm::value_ptr(color));
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        gl_has_errors();
    }
}

ParticleProps ParticleSystem::createParticle() {
    ParticleProps props;
    props.position = {0,0};
    props.velocity = {0,0};
    props.velocityVariation = { 3.0f,1.0f};
    props.colorBegin = {254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f};	
    props.colorEnd = {254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f};
    props.sizeBegin = 0.5f;
    props.sizeVariation = 0.3f;
    props.sizeEnd = 0.0f;		
    props.lifetime = 10000.0f;
    return props;
}