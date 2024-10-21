#version 330

out vec4 FragColor;

precision mediump float;
#define COMPAT_PRECISION mediump

#define OutputSize vec2(1920,1080)
#define TextureSize vec2(1920/4,1080/4)
#define scale vec2(1,1)

uniform sampler2D screen_texture;
uniform float time;
uniform float chromatic_abberation_intensity;
in vec2 texcoord;

// compatibility #defines
#define vTexCoord texcoord.xy
#define Source screen_texture
#define SourceSize vec4(TextureSize, 1.0 / TextureSize) //either TextureSize
#define OutSize vec4(OutputSize, 1.0 / OutputSize)

#define LOT_SHARP 1.0
#define LOT_CURV 1.0
#define maskDark 0.75
#define maskLight 1.0

#define PI   3.14159265358979323846

vec4 chromatic_abberation(vec2 loc)
{
    vec2 off_red = vec2(-0.002, 0.002) * chromatic_abberation_intensity;
    vec2 off_green = vec2(-0.002, -0.002) * chromatic_abberation_intensity;
    vec2 off_blue = vec2(0.00075, 0.0) * chromatic_abberation_intensity;
    return vec4(texture(Source, loc + off_red).r, texture(Source, loc + off_green).g, texture(Source, loc + off_blue).b, 1.0);
}

vec4 Mask(vec2 pos)
{
    vec3 mask = vec3(maskDark, maskDark, maskDark);

    pos.y = fract(pos.y * 0.333333333);

    if (pos.y < 0.333) mask.b = maskLight;
    else if (pos.y < 0.666) mask.g = maskLight;
    else                    mask.r = maskLight;

    return vec4(mask, 1.0);
}



// Distortion of scanlines, and end of screen alpha.
vec2 Warp(vec2 coord)
{
    float a, b, c, d;
    if (LOT_CURV == 0.0) { a = 0.0, b = 0.0, c = 1.0, d = 1.0; }
    if (LOT_CURV == 1.0) { a = 0.15, b = 0.225, c = 0.965, d = 0.948; }
    if (LOT_CURV == 2.0) { a = 0.0, b = 0.225, c = 1.0, d = 0.948; }

    vec2 CURVATURE_DISTORTION = vec2(a, b);
    // Barrel distortion shrinks the display area a bit, this will allow us to counteract that.
    vec2 barrelScale = vec2(c, d);
    coord -= vec2(0.5);
    float rsq = coord.x * coord.x + coord.y * coord.y;
    coord += coord * (CURVATURE_DISTORTION * rsq);
    coord *= barrelScale;
    if (abs(coord.x) >= 0.5 || abs(coord.y) >= 0.5)
        coord = vec2(-1.0);             // If out of bounds, return an invalid value.
    else
    {
        coord += vec2(0.5);
    }

    return coord;
}

#define one 1.384615
#define two 3.230769
#define w0  0.227027
#define w1  0.316216
#define w2  0.070270 

void main() {
    //This is just like "Quilez Scaling" but sharper
    vec2 pos = Warp(vTexCoord * scale);
    vec2 corn = min(pos, 1.0 - pos);    // This is used to mask the rounded
    corn.x = 0.0001 / corn.x;         // corners later on
    pos /= scale;

    vec2 p = pos * TextureSize;
    vec2 i = floor(p) + 0.5;
    vec2 f = p - i;
    p = (i + 4.0 * f * f * f) * SourceSize.zw;
    p.x = pos.x;

    vec4 final = chromatic_abberation(p);
    vec2 ps = vec2(SourceSize.z * (1.0 - LOT_SHARP), 0.0);

    final += chromatic_abberation(p + ps * one) * w1;
    final += chromatic_abberation(p - ps * one) * w1;
    final += chromatic_abberation(p - ps * two) * w2;
    final += chromatic_abberation(p + ps * two) * w2;

    vec4 clean = final;
    float l = dot(vec3(0.2), final.rgb);
    final *= Mask(vTexCoord * OutputSize.xy * scale);
    final = mix(final, clean, l);

    //corners cut
    if (LOT_CURV != 0.0) {
        if (corn.y <= corn.x || corn.x < 0.0001)final = vec4(0.0);
    }

    FragColor = final;
}
