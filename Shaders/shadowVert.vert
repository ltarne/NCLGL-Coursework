#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix0;

in vec3 position;
in vec3 colour;
in vec3 normal;
in vec3 tangent;
in vec2 texCoord;
