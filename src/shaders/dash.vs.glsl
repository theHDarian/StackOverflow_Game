#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
uniform mat3 transform;
out vec2 TexCoord;

void main() {
    vec3 transformedPos = transform * aPos;
    gl_Position = vec4(transformedPos, 1.0);
    TexCoord = aTexCoord;
}
