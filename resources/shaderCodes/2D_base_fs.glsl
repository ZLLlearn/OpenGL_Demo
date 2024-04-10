#version 450 core

in vec3 pColor;
in vec4 primitiveId;

out vec4 FragColor;

void main() {
    FragColor = vec4(pColor.xy, primitiveId.w, 1.0);
}