#version 450 core

in vec2 pTexCoord;

out vec4 FragColor;

uniform sampler2D texture2D;

void main() {
    FragColor = texture(texture2D, pTexCoord);
}