#version 330

// From vertex shader
in vec2 texcoord;

// Application data
uniform sampler2DArray sampler0;
uniform vec3 fcolor = vec3(1.0);
uniform int changeColor = 0;
uniform float effectAlpha = 1.0;
uniform float alpha = 1.0;
uniform int frame = 0;

uniform bool tile = false;
uniform vec2 tiling = vec2(1.0, 1.0);

uniform bool shielded = false;
uniform sampler2DArray shieldMask;
uniform vec2 scale;

uniform bool glitchToggle = false;
uniform float time;
uniform sampler2DArray glitchMask;
uniform sampler2DArray glitch;
float glitchOffset = 0.05;

// Output color
layout(location = 0) out  vec4 color;

vec2 clampedGlitchOffset(vec2 offset) {
	return clamp(texcoord + offset, vec2(0.0), vec2(1.0));
}

void main()
{
	color = vec4(fcolor, alpha) * ((!tile) ? texture(sampler0, vec3(texcoord.xy, frame)) : texture(sampler0, vec3(texcoord.xy * tiling, frame)));

	if (glitchToggle) {
		float a = texture(glitchMask, vec3(texcoord.x, texcoord.y, floor(64.0 * mod(0.01 * time, 1)))).a;
		if (a > 0.5 && (
			texture(sampler0, vec3(clampedGlitchOffset(vec2(0.0)), frame)).a > 0.0 ||
			texture(sampler0, vec3(clampedGlitchOffset(vec2(glitchOffset)), frame)).a > 0.0 ||
			texture(sampler0, vec3(clampedGlitchOffset(vec2(glitchOffset, 0.0)), frame)).a > 0.0 ||
			texture(sampler0, vec3(clampedGlitchOffset(vec2(glitchOffset, -glitchOffset)), frame)).a > 0.0 ||
			texture(sampler0, vec3(clampedGlitchOffset(vec2(0.0, -glitchOffset)), frame)).a > 0.0 ||
			texture(sampler0, vec3(clampedGlitchOffset(vec2(-glitchOffset)), frame)).a > 0.0 ||
			texture(sampler0, vec3(clampedGlitchOffset(vec2(-glitchOffset, 0.0)), frame)).a > 0.0 ||
			texture(sampler0, vec3(clampedGlitchOffset(vec2(-glitchOffset, glitchOffset)), frame)).a > 0.0 ||
			texture(sampler0, vec3(clampedGlitchOffset(vec2(0.0, glitchOffset)), frame)).a > 0.0
			)) {
			color = texture(glitch, vec3(texcoord.x, texcoord.y, floor(39.0 * mod(0.02 * time, 1))));
		}
	}

	if (shielded) {
		vec2 diff = texcoord - vec2(0.5);
		float dist = diff.x * diff.x + diff.y * diff.y;
		if (dist < 0.3 && texture(shieldMask, vec3(texcoord.xy * vec2(0.15) * scale, 0)).a > 0.5) {
			color = vec4(233.f / 255.f, 173.f / 255.f, 48.f / 255.f, 1.0 - dist * 1.5);
		}
	}
	// note: branches are expensive, consider using another shader instead?
	if (changeColor == 1) {
		color.r = fcolor.r * effectAlpha + color.r * (1.0 - effectAlpha);
		color.g = fcolor.g * effectAlpha + color.g * (1.0 - effectAlpha);
		color.b = fcolor.b * effectAlpha + color.b * (1.0 - effectAlpha);
	}
}
