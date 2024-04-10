#version 450 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

in vec3 gColor[];

out vec4 primitiveId;
out vec3 pColor;

void main() {
    primitiveId.w = gl_PrimitiveIDIn;

    vec4 triangleCenterPos = vec4(0.0);
    for (int i = 0; i < 3; ++i) {
        triangleCenterPos += gl_in[i].gl_Position;
    }
    triangleCenterPos /= 3;
    
    vec4 lineCenterPos[3];
    lineCenterPos[0] = (gl_in[0].gl_Position + gl_in[1].gl_Position) / 2;
    lineCenterPos[1] = (gl_in[1].gl_Position + gl_in[2].gl_Position) / 2;
    lineCenterPos[2] = (gl_in[2].gl_Position + gl_in[0].gl_Position) / 2;

    vec3 newColor1 = gColor[0];
    vec3 newColor2 = gColor[1];
    vec3 newColor3 = gColor[2];

    //if (gl_PrimitiveIDIn == 1) {
    //    newColor1 = vec3(1.0, 0.0, 0.0);
    //    newColor2 = vec3(0.0, 0.0, 0.0);
    //    newColor3 = vec3(0.0, 0.0, 0.0);
    //}

    gl_Position = gl_in[0].gl_Position;
    pColor = newColor1;
    EmitVertex();

    gl_Position = triangleCenterPos;
    pColor = newColor1;
    EmitVertex();

    gl_Position = lineCenterPos[0];
    pColor = newColor1;
    EmitVertex();

    gl_Position = triangleCenterPos;
    pColor = newColor2;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    pColor = newColor2;
    EmitVertex();

    gl_Position = lineCenterPos[0];
    pColor = newColor2;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    pColor = newColor2;
    EmitVertex();

    gl_Position = triangleCenterPos;
    pColor = newColor2;
    EmitVertex();

    gl_Position = lineCenterPos[1];
    pColor = newColor2;
    EmitVertex();

    gl_Position = triangleCenterPos;
    pColor = newColor3;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    pColor = newColor3;
    EmitVertex();

    gl_Position = lineCenterPos[1];
    pColor = newColor3;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    pColor = newColor3;
    EmitVertex();

    gl_Position = triangleCenterPos;
    pColor = newColor3;
    EmitVertex();

    gl_Position = lineCenterPos[2];
    pColor = newColor3;
    EmitVertex();

    gl_Position = triangleCenterPos;
    pColor = newColor1;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position;
    pColor = newColor1;
    EmitVertex();

    gl_Position = lineCenterPos[2];
    pColor = newColor1;
    EmitVertex();
    
    EndPrimitive();
}