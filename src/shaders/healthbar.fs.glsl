#version 330 core

// From vertex shader
in vec2 texcoord;

// Application data
uniform sampler2D sampler0;
uniform vec3 fcolor;
uniform int changeColor = 0;
uniform float alpha = 1.0;
uniform float chargeBoundary = 1.0;

// Output color
layout(location = 0) out vec4 color;

void main() {
    vec4 texColor = texture(sampler0, texcoord);

    // Create a diagonal boundary
    float diagonalBoundary = 1.0 - chargeBoundary + (-0.06125 * texcoord.y);

    if (texcoord.x >= diagonalBoundary) {
        texColor.r *= fcolor.r;
        texColor.g *= fcolor.g;
        texColor.b *= fcolor.b;
    }

    color = texColor;
}
