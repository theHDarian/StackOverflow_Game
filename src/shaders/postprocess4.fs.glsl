#version 330

out vec4 FragColor;

uniform sampler2D screen_texture;
uniform float time;
uniform float bulletTime;
in vec2 texcoord;

void main()
{
    vec3 regular = texture(screen_texture, texcoord).rgb;
    float centerGrey = dot(texture(screen_texture, texcoord).rgb, vec3(0.299, 0.587, 0.114));

    vec2 offset = vec2(0.002);
    float r = dot(texture(screen_texture, texcoord + offset).rgb, vec3(0.299, 0.587, 0.114));
    float g = dot(texture(screen_texture, texcoord + offset).rgb, vec3(0.299, 0.587, 0.114));
    float b = dot(texture(screen_texture, texcoord + offset).rgb, vec3(0.299, 0.587, 0.114));
    vec3 chromaFringe = vec3(r, g, b);

    float fringeGrey = dot(chromaFringe, vec3(0.299, 0.587, 0.114));
    vec3 finalBT = vec3(centerGrey) + chromaFringe - vec3(fringeGrey);

    // Final color = greyscale base + fringe
    FragColor = vec4(finalBT * bulletTime + regular * (1 - bulletTime), 1.0);
}