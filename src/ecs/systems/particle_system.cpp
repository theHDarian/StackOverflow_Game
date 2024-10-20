#include "particle_system.hpp"
#include <glm/gtc/constants.hpp>
#include <glm/gtx/compatibility.hpp>
#include "../utils/random.hpp"
#include "tiny_ecs_registry.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "ai_system.hpp"
#include "ai_system.hpp"
#include "render_system.hpp"
#include "components/presets/particle_presets.hpp"

ParticleSystem::ParticleSystem() {
    particlePool.resize(1000);
}

bool ParticleSystem::initScreenTexture()
{
    int framebuffer_width, framebuffer_height;
    glfwGetFramebufferSize(const_cast<GLFWwindow*>(window), &framebuffer_width, &framebuffer_height);

    glGenTextures(1, &off_screen_render_buffer_color);
    glBindTexture(GL_TEXTURE_2D, off_screen_render_buffer_color);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, framebuffer_width, framebuffer_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gl_has_errors();

    glGenRenderbuffers(1, &off_screen_render_buffer_depth);
    glBindRenderbuffer(GL_RENDERBUFFER, off_screen_render_buffer_depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, framebuffer_width, framebuffer_height);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, off_screen_render_buffer_color, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, off_screen_render_buffer_depth);
    gl_has_errors();

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "ERROR::FRAMEBUFFER::INCOMPLETE_FRAMEBUFFER: " << status << std::endl;
        return false;
    }

    return true;
}

void ParticleSystem::init(GLFWwindow* window) {
    this->window = window;
    glfwMakeContextCurrent(window);

    frame_buffer = 0;
    glGenFramebuffers(1, &frame_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
    gl_has_errors();

    // Setup projection matrix
    assert(registry.windowStates.components.size() > 0);
    WindowState& windowState = registry.windowStates.components[0]; 
    projection = glm::ortho(0.0f, static_cast<float>(windowState.width), static_cast<float>(windowState.height),0.0f);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    GLuint vbo, ib;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    gl_has_errors();
    
    uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };

    glGenBuffers(1, &ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    gl_has_errors();
    glBindVertexArray(0);
    gl_has_errors();

    bool is_valid = loadEffectFromFile(shader_path("particle.vs.glsl").c_str(), shader_path("particle.fs.glsl").c_str(), shaderProgram);
    assert(is_valid && (GLuint)shaderProgram != 0);

    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    gl_has_errors();

    if (!initScreenTexture()) {
        std::cerr << "ERROR::INIT::SCREEN_TEXTURE_FAILED\n";
        return; // Prevent further execution if screen texture initialization fails
    }

    // Clean up ib and vbo
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ib);
}

ParticleSystem::~ParticleSystem() {
    if (vao) {
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

    //check emit requests
    for (auto& request : registry.emitParticles.components) {
        if (request.requestType == RequestType::EmitParticle) {
            emit(createParticle(request.position));
        }
        else if (request.requestType == RequestType::Explosion) {
            // printf("%.2f %.2f\n",request.position.x,request.position.y);
            explode(createParticle(request.position), request.requestOrigin);
        }
    }
    //test emission on mouse position
    // emit(createParticle(registry.ioStates.components[0].mousePosition));
    registry.emitParticles.clear();
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

void ParticleSystem::explode(const ParticleProps& props, vec2 origin) {
    Particle& particle = particlePool[poolIndex];
    particle.active = true;
    particle.position = props.position;
    particle.rotation = Random::Float() * 2.0f * glm::pi<float>();

    vec2 offset = origin - props.position;
    vec2 direction = normalize(offset);
    float length = offset.length();

    particle.velocity = -(direction * length) * 70.0f + props.velocity ;
    particle.velocity.x += props.velocityVariation.x * (Random::Float() - 0.5f);
    particle.velocity.y += props.velocityVariation.y * (Random::Float() - 0.5f);

    particle.colorBegin = props.colorBegin;
    particle.colorEnd = props.colorEnd;

    particle.lifetime = props.lifetime;
    particle.lifeRemaining = props.lifetime;
    particle.sizeBegin = props.sizeBegin + props.sizeVariation * (Random::Float() - 0.5f);
    particle.sizeEnd = props.sizeEnd;

    poolIndex = --poolIndex % particlePool.size();

}

void ParticleSystem::render() {
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);

    // First render to the custom framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
    gl_has_errors();

    // Clearing backbuffer
    glViewport(0, 0, w, h);
    glDepthRange(0.00001, 10);
    glClearColor(GLfloat(32 / 255), GLfloat(43 / 255), GLfloat(81 / 255), 1.0);
    glClearDepth(10.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    gl_has_errors();

    Frame& frame = registry.frames.components[0];
    if (frame.prevFrameBuffer != 0) {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, frame.prevFrameBuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame_buffer);

        // Copy contents of previous buffer to current
        glBlitFramebuffer(0, 0, w, h, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        gl_has_errors();
        glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
    }
    frame.prevFrameBuffer = frame_buffer;
    frame.prevTexture = off_screen_render_buffer_color;

    glUseProgram(shaderProgram);
    glBindVertexArray(vao);

    int success;
    glValidateProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::VALIDATION_FAILED\n" << infoLog << std::endl;
    }
    gl_has_errors();

    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
    unsigned int colorLoc = glGetUniformLocation(shaderProgram, "fcolor");

    if (projectionLoc == -1 || transformLoc == -1 || colorLoc == -1) {
        std::cerr << "ERROR::SHADER::UNIFORM::LOCATION_NOT_FOUND\n";
        return; // Prevent further execution if uniforms are not found
    }

    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    gl_has_errors();

    for (auto& particle : particlePool) {
        if (!particle.active) {
            continue;
        }
        float lifePassed = (particle.lifetime - particle.lifeRemaining) / particle.lifetime;

        glm::vec4 color = glm::lerp(particle.colorBegin, particle.colorEnd, lifePassed);
        float size = glm::lerp(particle.sizeBegin, particle.sizeEnd, lifePassed);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), { particle.position.x, particle.position.y, 0.0f }) *
                              glm::rotate(glm::mat4(1.0f), particle.rotation, { 0.0f, 0.0f, 1.0f }) *
                              glm::scale(glm::mat4(1.0f), { size, size, 1.0f });

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glUniform4fv(colorLoc, 1, glm::value_ptr(color));
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        gl_has_errors();
    }
}

ParticleProps ParticleSystem::createParticle(vec2 pos) {
    ParticleProps p = DefaultParticle;
    p.position = pos;
    return p;
}
