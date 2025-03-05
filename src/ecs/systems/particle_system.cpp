#include "particle_system.hpp"
#include <glm/gtc/constants.hpp>
#include <glm/gtx/compatibility.hpp>
#include "utils/random.hpp"
#include "tiny_ecs_registry.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "ai_system.hpp"
#include "ai_system.hpp"
#include "render_system.hpp"

ParticleSystem::Vertex* ParticleSystem::createQuad(Vertex* target, vec4 color, mat4 transform, float texID) {
    float size = 1.0f;
    float x = -0.5f;
    float y = -0.5f;

    vec4 positions[4] = {
        { x, y, 0.0f, 1.0f },               
        { x + size, y, 0.0f, 1.0f },        
        { x + size, y + size, 0.0f, 1.0f }, 
        { x, y + size, 0.0f, 1.0f }         
    };

    vec2 texCoords[4] = {
        { 0.0f, 0.0f }, 
        { 1.0f, 0.0f }, 
        { 1.0f, 1.0f }, 
        { 0.0f, 1.0f }  
    };

    for (int i = 0; i < 4; i++) {
        glm::vec4 transformedPos = transform * positions[i];
        target->position = { transformedPos.x, transformedPos.y, transformedPos.z };
        target->color = color;
        target->texCoords = texCoords[i];
        target->texID = texID;
        target++;
    }

    return target;
}

GLuint ParticleSystem::loadTexture(const std::string& path) {
    int w,h,bits;
    stbi_set_flip_vertically_on_load(1);
    auto* pixels = stbi_load(path.c_str(),&w,&h,&bits,STBI_rgb_alpha);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D,textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    stbi_image_free(pixels);

    return textureID;
}

ParticleSystem::ParticleSystem() {
    particlePool.resize(POOLSIZE);
}

void ParticleSystem::clearParticles() {
    for (auto& p : particlePool) {
        p.active = false;
    }
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

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4 * POOLSIZE, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex,position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex,color));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex,texCoords));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex,texID));
    glEnableVertexAttribArray(3);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    gl_has_errors();

    uint32_t indices[6*POOLSIZE];
    uint32_t offset = 0;
    for (size_t i = 0; i < 6*POOLSIZE;i+= 6) {
        indices[i+0] = 0+offset;
        indices[i+1] = 1+offset;
        indices[i+2] = 2+offset;
        indices[i+3] = 2+offset;
        indices[i+4] = 3+offset;
        indices[i+5] = 0+offset;
        offset+=4;
    }

    glGenBuffers(1, &ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 6 * POOLSIZE, indices, GL_DYNAMIC_DRAW);
    gl_has_errors();

    texture_handle = loadTexture(textures_path("Misc/particles.png"));
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
}

ParticleSystem::~ParticleSystem() {
    if (vao) {
        glDeleteVertexArrays(1, &vao); 
    }
    if (vbo) {
        glDeleteBuffers(1,&vbo);
    }
    if (ib) {
        glDeleteBuffers(1, &ib);
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
    handleEmitRequests(elapsed_ms);
}

void ParticleSystem::handleEmitRequests(float elapsed_ms) {
    std::vector<Entity> removeRequestQueue;
    bool shouldClear = false;
    for (int i = 0; i < registry.emitParticles.components.size();i++) {
        Entity& ent = registry.emitParticles.entities[i];
        EmitParticle& request = registry.emitParticles.components[i];
        if (request.requestType == ParticleRequestType::ClearParticles) {
            shouldClear = true;
            break;
        }
        
        int emitCount = (int) ceil(request.numToEmitPerSecond * elapsed_ms/1000.f);
        request.timeRemaining -= elapsed_ms;
        if (request.timeRemaining <= 0) {
            removeRequestQueue.push_back(ent);
        }

        //emit based on type of request
        if (request.defaultPos != UNSET_VEC2) {
            request.props.position.base = request.defaultPos;
        } else if (registry.motions.has(ent)) {
            const Motion& motion = registry.motions.get(ent);
            request.props.position.variation = motion.scale/2.f;
            request.props.position.base = motion.position;
        }

        if (request.requestType == ParticleRequestType::PExplode) {
            explode(request.props,emitCount,false);
        } else if (request.requestType == ParticleRequestType::PWallCollision) {
            impact(request.props,emitCount,request.impactDirection);
        } else if (request.requestType == ParticleRequestType::PBulletTrail) {
            const Motion& motion = registry.motions.get(ent);
            request.props.velocity.base = -motion.velocity * 0.4f;
            request.props.velocity.variation = normalize(-motion.velocity) * Random::Float(100.f);
            
            if (motion.velocity == vec2(0.f,0.f)) { //stationary trails have special emission
                request.props.velocity.variation = {100,100};
                for (ColorEffect& ce : request.props.colorEffects) {
                    ce.color.start.w = 0.5f;
                    ce.color.end.w = 0.f;
                }
                request.props.size.end = 0.f;
                emitCount = min(1,emitCount);
            } else {
                request.props.velocity.variation = normalize(-motion.velocity) * Random::Float(100.f);
            }
            trail(request.props,emitCount);
        } else if (request.requestType == ParticleRequestType::PPlayerTrail) {
            const Motion& motion = registry.motions.get(ent);
            //should be attached to player already, set the velocity to upwards
            request.props.position.base.y += motion.scale.y/2.f; //appear at feet level
            request.props.position.variation.y *= 0.4f;
            request.props.velocity.base += vec2(0,-20.f);
            request.props.velocity.variation += vec2(10,0);
            trail(request.props,emitCount);
        } else if (request.requestType == PLaser && registry.lasers.has(ent)) {
            const Motion& motion = registry.motions.get(ent);
            const Laser& laser = registry.lasers.get(ent);
            if (laser.length != motion.scale.x && registry.collisions.has(ent)) {
                Collision& c = registry.collisions.get(ent);
                if (registry.walls.has(c.other)) {
                    WallCollider& wall = registry.walls.get(c.other);
                    request.props.position.base = motion.position + vec2(cos(motion.angle), sin(motion.angle)) * motion.scale.x * 0.5f;
                    request.props.position.variation = {10.f,10.f};
                    vec2 a = wall.endPosition-wall.startPosition;
                    vec2 b = -vec2(cos(motion.angle), sin(motion.angle));
                    vec2 p = dot(a,b)/dot(a,a)*a;
                    request.impactDirection = normalize(b-p);
                }
                
                impact(request.props,emitCount,request.impactDirection);
            }
            
        }
    }
    if (shouldClear) {
        clearParticles();
        registry.emitParticles.clear();
    } else {
        for (auto& e : removeRequestQueue) {
            registry.emitParticles.remove(e);
        }
    }
}

int ParticleSystem::activateParticle(const ParticleProps& props) {
    int index = poolIndex;
    Particle& particle = particlePool[poolIndex];
    particle.active = true;
    if (props.position.variation == vec2(0.f)) {
        particle.position = props.position.base + Random::Direction() * 0.1f;
    } else {
        particle.position = props.position.base + (Random::Vec2(props.position.variation * 2.f) - props.position.variation);
    }
    particle.velocity = props.velocity.base + (Random::Vec2(props.velocity.variation * 2.f) - props.velocity.variation);
    ColorEffect effect = Random::ListItem(props.colorEffects);
    particle.colorBegin = effect.color.start;
    particle.colorEnd = effect.color.end;
    if (props.textureRowIndex < 0) {
        //assign the plus/minus/rectangular particle
        if(effect.amount > 0) {
            //render a plus sign particle which is in last row
            particle.textureIndex = Random::Int(2) * 2 + 1;
        } else if (effect.amount < 0) {
            // render minus sign particle
            particle.textureIndex = Random::Int(2) * 2;
            
        } else {
            particle.textureIndex = -1;
        }
    } else {
        //flip so that index starts at top row (instead of bottom for textures)
        particle.textureIndex = (TEXTURE_ROW_SIZE * (TEXTURE_NUM_ROWS - 1 - props.textureRowIndex)) + Random::Int(TEXTURE_ROW_SIZE); //get random texture in row
    }

    particle.lifetime = props.lifetime;
    particle.lifeRemaining = props.lifetime;
    particle.sizeBegin = props.size.start + (Random::Float(props.size.variation*2.f) - props.size.variation);
    particle.sizeEnd = props.size.end;

    //plus and minus texture should be more visible
    if (effect.amount != 0) {
        particle.sizeBegin *= 4;
        particle.sizeEnd *= 2;
        particle.velocity /= 4;
        particle.lifetime *= 2;
        particle.lifeRemaining *= 2;
        //also emit half as often
        if (Random::Int(2) == 1) {
            particle.lifeRemaining = 0;
        }
    }
    if (props.textureRowIndex == 5 || props.textureRowIndex < 0 || props.textureRowIndex == 7) {
        particle.rotation = M_PI;
    } else {
        particle.rotation = Random::Float(2.0f) * glm::pi<float>();
    }

    poolIndex = (poolIndex-1) % particlePool.size();
    return index;
}

void ParticleSystem::impact(const ParticleProps& props, int emitCount, vec2 direction = {0,1}) {
    for (int j = 0; j < emitCount; j++) {
        Particle& particle = particlePool[activateParticle(props)];

        //override velocity
        vec2 offset = particle.position - props.position.base;
        if (dot(particle.velocity,direction) < 0) {
            particle.velocity *= -1.f;
        }
        if (dot(offset,direction) < 0) {
            particle.position = props.position.base - offset;
        }
        vec2 dir = normalize(particle.position - props.position.base);
        float explosionSpeed = 70.f;
        particle.velocity += dir * explosionSpeed;
        // printf("Emit: %.1f %.1f\n",dir.x,dir.y);
    }
}

void ParticleSystem::explode(const ParticleProps& props, int emitCount, bool isImplosion = false) {
    for (int j = 0; j < emitCount; j++) {
        Particle& particle = particlePool[activateParticle(props)];

        //override position and velocity
        vec2 direction = normalize(particle.position - props.position.base);
        float explosionSpeed = 70.f;
        particle.velocity += (isImplosion?-1.f:1.f) * direction * explosionSpeed;
    }
}

void ParticleSystem::trail(const ParticleProps& props, int emitCount) {
    for (int j = 0; j < emitCount; j++) {
        Particle& particle = particlePool[activateParticle(props)];
    }
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
    if (frame.prevFrameBuffer != 0 ) {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame_buffer);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, frame.prevFrameBuffer);

        if (glCheckFramebufferStatus(GL_READ_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE 
        && glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
            // Copy contents of previous buffer to current
            glBlitFramebuffer(0, 0, w, h, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
            gl_has_errors();
            glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
        } else {
            printf("Warning failed to copy: Frame buffer status %d\n",glCheckFramebufferStatus(GL_FRAMEBUFFER));
            return;
        }
    }
    frame.prevFrameBuffer = frame_buffer;
    frame.prevTexture = off_screen_render_buffer_color;

    glUseProgram(shaderProgram);
    glBindVertexArray(vao);

    gl_has_errors();

    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    if (projectionLoc == -1) {
        std::cerr << "ERROR::SHADER::UNIFORM::LOCATION_NOT_FOUND\n";
        return; // Prevent further execution if uniforms are not found
    }
    gl_has_errors();


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texture_handle);
    gl_has_errors();

    // set up texture sheet params
    glUniform1i(glGetUniformLocation(shaderProgram, "particle_texture_row_size"),(GLint) TEXTURE_ROW_SIZE);
    glUniform1i(glGetUniformLocation(shaderProgram, "particle_texture_num_rows"),(GLint) TEXTURE_NUM_ROWS);
    gl_has_errors();
    
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    gl_has_errors();

    uint32_t indexCount = 0;
    std::array<Vertex,POOLSIZE * 4> vertices;
    Vertex* buffer = vertices.data();
    for (auto& particle : particlePool) {
        if (!particle.active) {
            continue;
        }

        gl_has_errors();
        float lifePassed = (particle.lifetime - particle.lifeRemaining) / particle.lifetime;

        glm::vec4 color = glm::lerp(particle.colorBegin, particle.colorEnd, lifePassed); //TODO
        float size = glm::lerp(particle.sizeBegin, particle.sizeEnd, lifePassed);

        Motion motion = Motion();
        motion.position = { particle.position.x, particle.position.y };
        motion.angle = particle.rotation;
        motion.scale = { size, size };

        glm::mat4 transform = createFollowCameraModel(motion, vec2(0));

        buffer = createQuad(buffer,color,transform,particle.textureIndex); //-1 to use color, or any valid texture_handles index
        indexCount+=6;
        gl_has_errors();
    }
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(Vertex) * vertices.size(),vertices.data());
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
}

