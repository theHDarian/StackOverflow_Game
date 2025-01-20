#version 330

// From vertex shader
in vec2 texcoord;

// Application data
uniform sampler2D sampler0;
uniform vec3 fcolor = vec3(1.0);
uniform int changeColor = 0;
uniform float effectAlpha = 1.0;
uniform bool tile = false;
uniform vec2 tiling = vec2(1.0, 1.0);

// Output color
layout(location = 0) out  vec4 color;

void main()
{
	color = vec4(fcolor, 1.0) * ((!tile) ? texture(sampler0, texcoord.xy) : texture(sampler0, texcoord.xy * tiling));
	// note: branches are expensive, consider using another shader instead?
	if (changeColor == 1) {
		color.r = fcolor.r * effectAlpha + color.r * (1.0 - effectAlpha);
		color.g = fcolor.g * effectAlpha + color.g * (1.0 - effectAlpha);
		color.b = fcolor.b * effectAlpha + color.b * (1.0 - effectAlpha);
	}
}
