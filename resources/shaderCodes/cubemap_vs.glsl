#version 450 core

layout (location = 0) in vec3 vPosition;

out vec3 worldPos;

uniform mat4 proj;
uniform mat4 view;

void main() {
    worldPos = vPosition;
    gl_Position = proj * view * vec4(worldPos, 1.0);
}