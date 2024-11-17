#version 330

/*
    
    This shader is a heavily modified version of the shader found here:
    https://github.com/libretro/glsl-shaders/blob/master/crt/shaders/crt-lottes.glsl

    Original comment on use of the shader:
        // PUBLIC DOMAIN CRT STYLED SCAN-LINE SHADER
        //
        //   by Timothy Lottes
        //
        // This is more along the style of a really good CGA arcade monitor.
        // With RGB inputs instead of NTSC.
        // The shadow mask example has the mask rotated 90 degrees for less chromatic aberration.
        //
        // Left it unoptimized to show the theory behind the algorithm.
        //
        // It is an example what I personally would want as a display option for pixel art games.
        // Please take and use, change, or whatever.

    Changes include:
        Removal of unnecessary / unused parts
        Rotation of the scanlines
        Custom values
        Addition of a chromatic abberation effect

*/

out vec4 FragColor;

uniform sampler2D screen_texture;
uniform float time;
uniform float chromatic_abberation_intensity;
in vec2 texcoord;

// ------------- //

vec4 chromatic_abberation(sampler2D Source, vec2 loc)
{
    vec2 off_red = vec2(-0.002, 0.002) * chromatic_abberation_intensity;
    vec2 off_green = vec2(-0.002, -0.002) * chromatic_abberation_intensity;
    vec2 off_blue = vec2(0.00075, 0.0) * chromatic_abberation_intensity;
    return vec4(texture(Source, loc + off_red).r, texture(Source, loc + off_green).g, texture(Source, loc + off_blue).b, 1.0);
}

void main()
{
    FragColor = chromatic_abberation(screen_texture, texcoord);
}