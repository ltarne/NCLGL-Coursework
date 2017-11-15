#version 400 core

uniform int tessFactor;
layout(vertices =4) out;

in Vertex {
    vec4 colour;
    vec2 texCoord;
} IN[];

out Vertex {
    vec4 colour;
    vec2 texCoord;
} OUT[];


void main() {
    gl_TessLevelInner[0] = tessFactor;
    gl_TessLevelInner[1] = tessFactor;

    gl_TessLevelOuter[0] = tessFactor;
    gl_TessLevelOuter[1] = tessFactor;
    gl_TessLevelOuter[2] = tessFactor;
    gl_TessLevelOuter[3] = tessFactor;

    OUT[gl_InvocationID].texCoord = IN[gl_InvocationID].texCoord;

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

}
