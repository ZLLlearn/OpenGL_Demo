#version 450 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;

out vec2 pTexCoord;
out vec3 worldPos;
out vec3 pNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat3 normalMatrix;

void main () {
    pTexCoord = vTexCoord;
    worldPos = vec3(model * vec4(vPosition, 1.0f));
    pNormal = normalMatrix * vNormal;

    gl_Position = proj * view * vec4(worldPos, 1.0f);
}