#version 330 core // Specify core profile for compatibility

in vec4 color;         // Color input, if used
in vec2 texcoord;     // Texture coordinates input
in float texIndex;    // Texture index input (should be 0 or 1)
layout(location = 0) out vec4 out_color; // Output color

uniform sampler2D sampler1; // Array of 2D textures

void main()
{
    // Ensure texIndex is a valid index for the texture array
    int index = int(texIndex);
    if (index < 0 || index >= 2) {
        // Handle error: Use a default color or texture
        out_color = vec4(1.0, 0.0, 0.0, 1.0); // Red color as an error signal
        return;
    }
    
    // Sample the texture based on the index and texture coordinates
    out_color = texture(sampler1, texcoord);
}