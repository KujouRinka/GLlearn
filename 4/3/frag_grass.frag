#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D window_texture;

void main() {
    vec4 texColor = texture(window_texture, TexCoords);
    // if (texColor.a < 0.1) {
    //     discard;
    // }
    FragColor = texColor;
}