// ref: https://learnopengl.com/In-Practice/Text-Rendering
// optimizations made based on: https://www.youtube.com/watch?v=S0PyZKX4lyI

#version 330 core
// in vec2 TexCoords;
// since we're now passing out a struct, we need to declare same struct in 

in VS_OUT{
    vec2 TexCoords;
    vec4 TexColor;
    flat int index;
}fs_in;

uniform sampler2DArray sampler0;

out vec4 color;

//uniform sampler2DArray text; // an array of 128 textures, each is one character
//uniform int letterMap[100];

void main()
{
    //color = vec4(color * texture(sampler0, vec3(TexCoords.xy, frame)));
    color = vec4(texture(sampler0, vec3(fs_in.TexCoords.xy, 0)));
    //color = vec4(1);
}