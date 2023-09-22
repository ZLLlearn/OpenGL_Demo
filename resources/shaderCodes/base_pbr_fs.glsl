#version 450 core

in vec2 pTexCoord;
in vec3 worldPos;
in vec3 pNormal;

out vec4 FragColor;

uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;
uniform vec3 lightPositions;
uniform vec3 lightColors;
uniform vec3 cameraPos;

const float pi = 3.14159265359;

float distributionGGX(vec3 n, vec3 h, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(n, h), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = pi * denom * denom;
    return num / denom;
}

float geometrySmithG1(float dot, float roughness) {
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;

    float num = dot;
    float denom = dot * (1.0 - k) + k;
    return num / denom;
}

float geometrySmithG2(vec3 n, vec3 v, vec3 l, float roughness) {
    float NdotV = max(dot(n, v), 0.0);
    float NdotL = max(dot(n, l), 0.0);

    float masking = geometrySmithG1(NdotV, roughness);
    float shading = geometrySmithG1(NdotL, roughness);
    return masking * shading;
}

vec3 fresnelSchlick(float cosTheta, vec3 f0) {
    return f0 + (1.0 - f0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main() {
    vec3 n = normalize(pNormal);
    vec3 v = normalize(cameraPos - worldPos);

    vec3 f0 = vec3(0.04);
    f0 = mix(f0, albedo, metallic);

    vec3 l = normalize(lightPositions - worldPos);
    vec3 h = normalize(v + l);
    float distance = length(lightPositions - worldPos);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = lightColors * attenuation;

    float ndf = distributionGGX(n, h, roughness);
    float geometry = geometrySmithG2(n, v, l, roughness);
    vec3 fresnel = fresnelSchlick(clamp(dot(h, v), 0.0, 1.0), f0);

    vec3 numerator = ndf * geometry * fresnel;
    float denominator = 4.0 * max(dot(n, v), 0.0) * max(dot(n, l), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;

    vec3 kd = vec3(1.0) - fresnel;
    kd *= 1.0 - metallic;

    float NdotL = max(dot(n, l), 0.0);
    vec3 Lo = (kd * albedo / pi + specular) * radiance * NdotL;

    vec3 ambient = vec3(0.03) * albedo * ao;
    
    vec3 color = ambient + Lo;
    color = color / (color + vec3(1.0));
    color  = pow(color, vec3(1.0 / 2.2));
    
    FragColor = vec4(color, 1.0);
}