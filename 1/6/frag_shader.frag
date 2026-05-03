#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform bool right;
uniform float altAlpha;

void main() {
    if (right) {
        FragColor = mix(texture(texture0, TexCoord), texture(texture1, vec2(1 - TexCoord.x, TexCoord.y)), altAlpha) * vec4(ourColor, 1.0f);
    } else {
        FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), altAlpha) * vec4(ourColor, 1.0f);
    }
}