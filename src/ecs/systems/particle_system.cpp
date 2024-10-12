#include "particle_system.hpp"
#include <glm/gtc/constants.hpp>
#include <glm/gtx/compatibility.hpp>
#include "../utils/random.hpp"
#include "tiny_ecs_registry.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "render_system.hpp"

ParticleSystem::ParticleSystem() {
    particlePool.resize(1000);
    for (int i = 0; i < 100; i++) {
        emit(createParticle());
    }
}

bool ParticleSystem::initScreenTexture()
{
	int framebuffer_width, framebuffer_height;
	glfwGetFramebufferSize(const_cast<GLFWwindow*>(window), &framebuffer_width, &framebuffer_height);  // Note, this will be 2x the resolution given to glfwCreateWindow on retina displays

	glGenTextures(1, &off_screen_render_buffer_color);
	glBindTexture(GL_TEXTURE_2D, off_screen_render_buffer_color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, framebuffer_width, framebuffer_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	gl_has_errors();

	glGenRenderbuffers(1, &off_screen_render_buffer_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, off_screen_render_buffer_depth);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, off_screen_render_buffer_color, 0);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, framebuffer_width, framebuffer_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, off_screen_render_buffer_depth);
	gl_has_errors();

	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	return true;
}

void ParticleSystem::init(GLFWwindow* window) {
    this->window = window;
    glfwMakeContextCurrent(window);

    frame_buffer = 0;
	glGenFramebuffers(1, &frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
	gl_has_errors();

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

    initScreenTexture();

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
    // Getting size of window
	int w, h;
	glfwGetFramebufferSize(window, &w, &h); // Note, this will be 2x the resolution given to glfwCreateWindow on retina displays

	// First render to the custom framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
	gl_has_errors();
	// Clearing backbuffer
	glViewport(0, 0, w, h);
	glDepthRange(0.00001, 10);
	glClearColor(GLfloat(32/ 255), GLfloat(43 / 255), GLfloat(81 / 255), 1.0);
	glClearDepth(10.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	gl_has_errors();
    
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