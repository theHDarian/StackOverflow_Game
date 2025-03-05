#version 330 core 

in vec4 color;         
in vec2 texcoord;     
in float texIndex;    
layout(location = 0) out vec4 out_color;

uniform sampler2D particle_sampler; 
uniform int particle_texture_row_size = 0;
uniform int particle_texture_num_rows = 0;

void main()
{
    int index = int(texIndex);
    if (index < 0) {
        //use color
        out_color = color;
    } else {
        float currRow = floor(texIndex / float(particle_texture_row_size));
        float currCol = texIndex - (currRow * float(particle_texture_row_size));
        vec2 base = vec2(currCol, currRow) / vec2(float(particle_texture_row_size), float(particle_texture_num_rows));
        vec2 offset = texcoord / vec2(particle_texture_row_size, particle_texture_num_rows);
        out_color = texture(particle_sampler, base + offset);
        if ( currRow == 0 ) {
            //handle plus/minus particles
            if (out_color.w > 0.0) {
                //replace the color of this with the specified color
                out_color.xyz = color.xyz;
            }
        }
        // out_color = texture(particle_sampler, texcoord);
    }
}