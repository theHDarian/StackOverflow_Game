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
uniform vec3 shieldColor = vec3(233.f / 255.f, 173.f / 255.f, 48.f / 255.f);
uniform vec2 scale;

uniform bool glitchToggle = false;
uniform float time;
uniform sampler2DArray glitchMask;
uniform sampler2DArray glitch;

uniform float greyscale = 0.0;

float glitchOffset = 0.05;

// for drawing gauges
uniform bool gaugeToggle = false;
uniform float chargeBoundary = 1.0;
uniform vec4 unchargedColor;
uniform int isVertical = 1;

// to give a green aura effect when regenerating
uniform bool auraToggle = false;
uniform vec3 auraColor;

// Output color
layout(location = 0) out  vec4 color;

vec2 clampedGlitchOffset(vec2 offset) {
	return clamp(texcoord + offset, vec2(0.0), vec2(1.0));
}

vec4 gaugeEffect(vec4 color)
{
	if (texcoord.y < 1.0 - chargeBoundary && isVertical == 1) {
		color.r *= fcolor.r;
		color.g *= fcolor.g;
		color.b *= fcolor.b;
		color *= unchargedColor;
	}
	if (texcoord.x > chargeBoundary && isVertical == 0) {
		color.r *= fcolor.r;
		color.g *= fcolor.g;
		color.b *= fcolor.b;
		color *= unchargedColor;
	}
	return color;
}

vec4 auraEffect(vec4 color)
{
	//return vec4(auraColor.rgb * (texcoord.y) + color.rgb * (1.0 - texcoord.y), color.w); // more of a gradient effect
	color = vec4(auraColor.rgb, (texcoord.y * alpha) * pow(1-abs(0.5 - texcoord.x), 1/2) * color.a);
	return color;
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
			color = vec4(shieldColor.x, shieldColor.y, shieldColor.z, (0.2 * sin(0.5 * time) + 0.8) - dist * 1.5);
		}
	}
	if (gaugeToggle) {
		color = gaugeEffect(color);
	}
	if (auraToggle) {
		color = auraEffect(color);
	}
	// note: branches are expensive, consider using another shader instead?
	if (changeColor == 1) {
		color.r = fcolor.r * effectAlpha + color.r * (1.0 - effectAlpha);
		color.g = fcolor.g * effectAlpha + color.g * (1.0 - effectAlpha);
		color.b = fcolor.b * effectAlpha + color.b * (1.0 - effectAlpha);
	}

	if (texcoord.y < greyscale) {
		color = vec4(vec3(0.21 * color.r + 0.71 * color.g + 0.07 * color.b), color.a);
	}
}
