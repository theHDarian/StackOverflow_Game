#version 330 core 

in vec4 color;         
in vec2 texcoord;     
in float texIndex;    
layout(location = 0) out vec4 out_color;

uniform sampler2D particle_sampler[2]; 

void main()
{
    int index = int(texIndex);
    if (index < 0) {
        //use color
        out_color = color;
    } else if (index >= 2) {
        out_color = vec4(1.0, 0.0, 0.0, 1.0); 
    } else {
        if (index == 0) {
            out_color = texture(particle_sampler[0], texcoord);
        } else {
            out_color = texture(particle_sampler[1], texcoord);
        }
    }
}