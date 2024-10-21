#version 330

uniform sampler2D screen_texture;
uniform float time;
uniform float chromatic_abberation_intensity;

in vec2 texcoord;

layout(location = 0) out vec4 color;

vec4 chromatic_abberation() 
{
    vec2 off_red = vec2(-0.002,0.002) * chromatic_abberation_intensity;
    vec2 off_green = vec2(-0.002,-0.002) * chromatic_abberation_intensity;
    vec2 off_blue = vec2(0.00075,0.0) * chromatic_abberation_intensity;
    return vec4(texture(screen_texture, texcoord+off_red).r, texture(screen_texture, texcoord + off_green).g, texture(screen_texture, texcoord + off_blue).b, 1.0);
}

void main()
{
    vec4 in_color = chromatic_abberation();
    //vec4 in_color = texture(screen_texture, texcoord);
    color = in_color;
}