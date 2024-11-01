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

precision mediump float;
#define COMPAT_PRECISION mediump

#define OutputSize vec2(1920,1080)
#define TextureSize vec2(1920/1.5,1080/1.5)
#define InputSize vec2(1920/1.5,1080/1.5)

uniform sampler2D screen_texture;
uniform float time;
uniform float chromatic_abberation_intensity;
in vec2 texcoord;

// compatibility #defines
#define vTexCoord texcoord.xy
#define Source screen_texture
#define SourceSize vec4(TextureSize, 1.0 / TextureSize) //either TextureSize
#define OutSize vec4(OutputSize, 1.0 / OutputSize)

#define hardScan -1.0
#define hardPix -3.0
#define warpX 0.031
#define warpY 0.041
#define maskDark 0.5
#define maskLight 1.5
#define scaleInLinearGamma 1.0
#define brightBoost 2.0
#define hardBloomPix -1.5
#define hardBloomScan -2.0
#define bloomAmount 0.05
#define shape 2.0

#define DO_BLOOM true

// ------------- //

vec4 chromatic_abberation(sampler2D Source, vec2 loc)
{
    vec2 off_red = vec2(-0.002, 0.002) * chromatic_abberation_intensity;
    vec2 off_green = vec2(-0.002, -0.002) * chromatic_abberation_intensity;
    vec2 off_blue = vec2(0.00075, 0.0) * chromatic_abberation_intensity;
    return vec4(texture(Source, loc + off_red).r, texture(Source, loc + off_green).g, texture(Source, loc + off_blue).b, 1.0);
}

#define COMPAT_TEXTURE chromatic_abberation

float ToLinear1(float c)
{
    if (scaleInLinearGamma == 0.)
        return c;

    return(c <= 0.04045) ? c / 12.92 : pow((c + 0.055) / 1.055, 2.4);
}

vec3 ToLinear(vec3 c)
{
    if (scaleInLinearGamma == 0.)
        return c;

    return vec3(ToLinear1(c.r), ToLinear1(c.g), ToLinear1(c.b));
}

float ToSrgb1(float c)
{
    if (scaleInLinearGamma == 0.)
        return c;

    return(c < 0.0031308 ? c * 12.92 : 1.055 * pow(c, 0.41666) - 0.055);
}

vec3 ToSrgb(vec3 c)
{
    if (scaleInLinearGamma == 0.)
        return c;

    return vec3(ToSrgb1(c.r), ToSrgb1(c.g), ToSrgb1(c.b));
}

// Nearest emulated sample given floating point position and texel offset.
// Also zero's off screen.
vec3 Fetch(vec2 pos, vec2 off) {
    pos = (floor(pos * SourceSize.xy + off) + vec2(0.5, 0.5)) / SourceSize.xy;
    return ToLinear(brightBoost * pow(COMPAT_TEXTURE(Source, pos.xy).rgb, vec3(2.2)));
}

// Distance in emulated pixels to nearest texel.
vec2 Dist(vec2 pos)
{
    pos = pos * SourceSize.xy;

    return -((pos - floor(pos)) - vec2(0.5));
}

// 1D Gaussian.
float Gaus(float pos, float scale)
{
    return exp2(scale * pow(abs(pos), shape));
}

// 3-tap Gaussian filter along horz line.
vec3 Horz3(vec2 pos, float off)
{
    vec3 b = Fetch(pos, vec2(-1.0, off));
    vec3 c = Fetch(pos, vec2(0.0, off));
    vec3 d = Fetch(pos, vec2(1.0, off));
    float dst = Dist(pos).x;

    // Convert distance to weight.
    float wb = Gaus(dst - 1.0, hardPix);
    float wc = Gaus(dst + 0.0, hardPix);
    float wd = Gaus(dst + 1.0, hardPix);

    // Return filtered sample.
    return (b * wb + c * wc + d * wd) / (wb + wc + wd);
}

// 5-tap Gaussian filter along horz line.
vec3 Horz5(vec2 pos, float off) {
    vec3 a = Fetch(pos, vec2(-2.0, off));
    vec3 b = Fetch(pos, vec2(-1.0, off));
    vec3 c = Fetch(pos, vec2(0.0, off));
    vec3 d = Fetch(pos, vec2(1.0, off));
    vec3 e = Fetch(pos, vec2(2.0, off));

    float dst = Dist(pos).x;
    // Convert distance to weight.
    float wa = Gaus(dst - 2.0, hardPix);
    float wb = Gaus(dst - 1.0, hardPix);
    float wc = Gaus(dst + 0.0, hardPix);
    float wd = Gaus(dst + 1.0, hardPix);
    float we = Gaus(dst + 2.0, hardPix);

    // Return filtered sample.
    return (a * wa + b * wb + c * wc + d * wd + e * we) / (wa + wb + wc + wd + we);
}

// 7-tap Gaussian filter along horz line.
vec3 Horz7(vec2 pos, float off)
{
    vec3 a = Fetch(pos, vec2(-3.0, off));
    vec3 b = Fetch(pos, vec2(-2.0, off));
    vec3 c = Fetch(pos, vec2(-1.0, off));
    vec3 d = Fetch(pos, vec2(0.0, off));
    vec3 e = Fetch(pos, vec2(1.0, off));
    vec3 f = Fetch(pos, vec2(2.0, off));
    vec3 g = Fetch(pos, vec2(3.0, off));

    float dst = Dist(pos).x;
    // Convert distance to weight.
    float wa = Gaus(dst - 3.0, hardBloomPix);
    float wb = Gaus(dst - 2.0, hardBloomPix);
    float wc = Gaus(dst - 1.0, hardBloomPix);
    float wd = Gaus(dst + 0.0, hardBloomPix);
    float we = Gaus(dst + 1.0, hardBloomPix);
    float wf = Gaus(dst + 2.0, hardBloomPix);
    float wg = Gaus(dst + 3.0, hardBloomPix);

    // Return filtered sample.
    return (a * wa + b * wb + c * wc + d * wd + e * we + f * wf + g * wg) / (wa + wb + wc + wd + we + wf + wg);
}

// Return scanline weight.
float Scan(vec2 pos, float off)
{
    float dst = Dist(pos).y;

    return Gaus(dst + off, hardScan);
}

// Return scanline weight for bloom.
float BloomScan(vec2 pos, float off)
{
    float dst = Dist(pos).y;

    return Gaus(dst + off, hardBloomScan);
}

// Allow nearest three lines to effect pixel.
vec3 Tri(vec2 pos)
{
    vec3 a = Horz3(pos, -1.0);
    vec3 b = Horz5(pos, 0.0);
    vec3 c = Horz3(pos, 1.0);

    float wa = Scan(pos, -1.0);
    float wb = Scan(pos, 0.0);
    float wc = Scan(pos, 1.0);

    return a * wa + b * wb + c * wc;
}

// Small bloom.
vec3 Bloom(vec2 pos)
{
    vec3 a = Horz5(pos, -2.0);
    vec3 b = Horz7(pos, -1.0);
    vec3 c = Horz7(pos, 0.0);
    vec3 d = Horz7(pos, 1.0);
    vec3 e = Horz5(pos, 2.0);

    float wa = BloomScan(pos, -2.0);
    float wb = BloomScan(pos, -1.0);
    float wc = BloomScan(pos, 0.0);
    float wd = BloomScan(pos, 1.0);
    float we = BloomScan(pos, 2.0);

    return a * wa + b * wb + c * wc + d * wd + e * we;
}

// Distortion of scanlines, and end of screen alpha.
vec2 Warp(vec2 pos)
{
    pos = pos * 2.0 - 1.0;
    //pos *= vec2(1.0 + (pos.y * pos.y) * warpX, 1.0 + (pos.x * pos.x) * warpY);

    return pos * 0.5 + 0.5;
}

vec3 Mask(vec2 pos)
{
    vec3 mask = vec3(maskDark, maskDark, maskDark);

    pos.y = fract(pos.y * 0.333333333);

    if (pos.y < 0.333) mask.b = maskLight;
    else if (pos.y < 0.666) mask.g = maskLight;
    else                    mask.r = maskLight;

    return mask;
}

void main()
{
    vec2 pos = Warp(texcoord * (TextureSize.xy / InputSize.xy)) * (InputSize.xy / TextureSize.xy);
    vec3 outColor = Tri(pos);

#ifdef DO_BLOOM
    //Add Bloom
    outColor.rgb += Bloom(pos) * bloomAmount;
#endif

    outColor.rgb *= Mask(gl_FragCoord.xy * 1.000001);

#ifdef GL_ES    /* TODO/FIXME - hacky clamp fix */
    vec2 bordertest = (pos);
    if (bordertest.x > 0.0001 && bordertest.x < 0.9999 && bordertest.y > 0.0001 && bordertest.y < 0.9999)
        outColor.rgb = outColor.rgb;
    else
        outColor.rgb = vec3(0.0);
#endif
    FragColor = vec4(ToSrgb(outColor.rgb), 1.0);
}