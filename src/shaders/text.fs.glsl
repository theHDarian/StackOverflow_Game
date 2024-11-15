#version 330 core
// in vec2 TexCoords;
// since we're now passing out a struct, we need to declare same struct in 

in VS_OUT {
    vec2 TexCoords;
    flat int index; 
}fs_in;

out vec4 color;

uniform sampler2DArray text; // an array of 128 textures, each is one character
uniform int letterMap[100];
// could be 400 long and access index for each char
// instead have this to be the texture itself we want
// and then ANOTHER array to decide which char we want to draw
// reason: directly mapping "abba" would store "a, b, b, a" which is inefficient
// much easier to have smaller arr for texture and a bigger one of ints
// (indirect accessing)
uniform vec3 textColor;

void main()
{    
    // recall with texture arrays: 3rd = layer/index
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, vec3(fs_in.TexCoords.xy, letterMap[fs_in.index])).r);
    color = vec4(textColor, 1.0) * sampled;
}  