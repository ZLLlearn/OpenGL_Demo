#version 450 core

layout (lines) in;
layout (line_strip, max_vertices = 4) out;

in vec3 gColor[];
out vec3 pColor;

void main() {
    vec4 lineCenterPos = vec4(0.0);
    lineCenterPos = (gl_in[0].gl_Position + gl_in[1].gl_Position) / 2;

    gl_Position = gl_in[0].gl_Position;
    pColor = gColor[0];
    EmitVertex();

    gl_Position = lineCenterPos;
    pColor = gColor[0];
    EmitVertex();

    gl_Position = lineCenterPos;
    pColor = gColor[1];
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    pColor = gColor[1];
    EmitVertex();
    
    EndPrimitive();
}