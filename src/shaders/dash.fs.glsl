#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D texture1;
uniform float chargeLevel; // value between 0.0 and 1.0
uniform float window_height;

void main() {
    vec4 texColor = texture(texture1, TexCoord);
    if(gl_FragCoord.y / window_height < chargeLevel) {
        FragColor = texColor; // Recharging texture
    } else {
        FragColor = vec4(texColor.rgb * 0.3, texColor.a); // Dimmed texture for empty part
    }
}
