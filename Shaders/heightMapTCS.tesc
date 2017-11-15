#version 400 core

//uniform int tessFactor;
layout(vertices =4) out;

in Vertex {
  vec4 colour;
  vec3 normal;
  vec3 tangent;
  vec2 texCoord;
} IN[];

out Vertex {
  vec4 colour;
  vec3 normal;
  vec3 tangent;
  vec2 texCoord;
} OUT[];


void main() {
  int tessFactor = 64;

    gl_TessLevelInner[0] = tessFactor;
    gl_TessLevelInner[1] = tessFactor;

    gl_TessLevelOuter[0] = tessFactor;
    gl_TessLevelOuter[1] = tessFactor;
    gl_TessLevelOuter[2] = tessFactor;
    gl_TessLevelOuter[3] = tessFactor;


    OUT[gl_InvocationID].colour = IN[gl_InvocationID].colour;
    OUT[gl_InvocationID].normal = IN[gl_InvocationID].normal;
    OUT[gl_InvocationID].tangent = IN[gl_InvocationID].tangent;
    OUT[gl_InvocationID].texCoord = IN[gl_InvocationID].texCoord;

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

}
