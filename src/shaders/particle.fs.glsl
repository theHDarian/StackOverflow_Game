#version 330 core 

in vec4 color;         
in vec2 texcoord;     
in float texIndex;    
layout(location = 0) out vec4 out_color;

uniform sampler2D particle_sampler; 

void main()
{
    int index = int(texIndex);
    if (index < 0) {
        //use color
        out_color = color;
    } else {
        vec2 base = vec2(index % 4,floor(index / 4)) / vec2(4.0,4.0); //texture coordinate of the base of the texture
        vec2 offset = texcoord / 4.0;
        out_color = texture(particle_sampler, base + offset);
        // out_color = texture(particle_sampler, texcoord);
    }
}