#version 450 core

in vec3 outColors;

out vec4 FragColors;

void main() {
    FragColors = vec4(outColors, 1.0f);
}