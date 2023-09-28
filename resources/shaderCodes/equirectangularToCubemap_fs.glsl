#version 450 core

in vec3 worldPos;

out vec4 FragColor;

uniform sampler2D equirectangularMap;

const vec2 invAtan = vec2(0.1591, 0.3183);

vec2 samplerSphericalMap(vec3 V) {
    vec2 uv = vec2(atan(V.z, V.x), asin(V.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main() {
    vec2 uv = samplerSphericalMap(normalize(worldPos));
    vec3 color = texture(equirectangularMap, uv).rgb;

    FragColor = vec4(color, 1.0);
}