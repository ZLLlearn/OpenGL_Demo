#version 450 core

in vec2 pTexCoord;
in vec3 worldPos;
in vec3 pNormal;

out vec4 FragColor;

uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 cameraPos;
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

const float PI = 3.14159265359;

float distributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    return num / denom;
}

float geometrySmithG1(float dot, float roughness) {
    float a = roughness;
    float r = a + 1.0;
    float k = (r * r) / 8.0;

    float num = dot;
    float denom = dot * (1.0 - k) + k;
    return num / denom;
}

float geometrySmithG2(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);

    float masking = geometrySmithG1(NdotV, roughness);
    float shadowing = geometrySmithG1(NdotL, roughness);
    return masking * shadowing;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main() {
    vec3 N = normalize(pNormal);
    vec3 V = normalize(cameraPos - worldPos);
    vec3 R = reflect(-V, N);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);
    {
        vec3 L = normalize(lightPosition - worldPos);
        vec3 H = normalize(V + L);
        float distance = length(lightPosition - worldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColor * attenuation;

        float ndf = distributionGGX(N, H, roughness);
        float geometry = geometrySmithG2(N, V, L, roughness);
        vec3 fresnel = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

        vec3 numerator = ndf * geometry * fresnel;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        vec3 kd = vec3(1.0) - fresnel;
        kd *= 1.0 - metallic;

        float NdotL = max(dot(N, L), 0.0);
        Lo += (kd * albedo / PI + specular) * radiance * NdotL;
    }

    vec3 fresnel = fresnelSchlick(clamp(dot(N, V), 0.0, 1.0), F0);
    vec3 kd = 1.0 - fresnel;
    kd *= 1.0 - metallic;

    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse = irradiance * albedo / PI;

    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilterMap, R, roughness * MAX_REFLECTION_LOD).rgb;
    vec2 brdf = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (fresnel * brdf.x + brdf.y);

    vec3 ambient = (kd * diffuse + specular) * ao;

    vec3 color = ambient + Lo;
    color = color / (color + vec3(1.0));
    color  = pow(color, vec3(1.0 / 2.2));
    
    FragColor = vec4(color, 1.0);
}