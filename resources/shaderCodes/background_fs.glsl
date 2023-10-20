#version 450 core

in vec3 worldPos;

out vec4 FragColor;

uniform samplerCube environmentMap;

void main() {
    vec3 envColor = texture(environmentMap, worldPos).rgb;

    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0 / 2.2));

    FragColor = vec4(envColor, 1.0);
}