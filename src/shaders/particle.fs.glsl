#version 330 core 

in vec4 color;         
in vec2 texcoord;     
in float texIndex;    
layout(location = 0) out vec4 out_color;

uniform sampler2D sampler1[2]; 

void main()
{
    int index = int(texIndex);
    if (index < 0 || index >= 2) {
        out_color = vec4(1.0, 0.0, 0.0, 1.0); 
        return;
    }
    
    out_color = texture(sampler1[index], texcoord);
}