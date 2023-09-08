#version 450 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 colors;

out vec3 outColors;

void main() {
    gl_Position = vec4(vertex, 1.0f);
    outColors = colors;
}