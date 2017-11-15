#version 400 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 globalTextureMatrix;

in vec3 position;
in vec4 colour;
in vec3 normal;
in vec3 tangent;
in vec2 texCoord;

out Vertex {
  vec4 colour;
  vec3 normal;
  vec3 tangent;
  vec2 texCoord;
} OUT;

void main(void) {
  mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));

  OUT.colour = colour;
  OUT.texCoord = texCoord;

  OUT.normal = normal;
  OUT.tangent = tangent;

  gl_Position = vec4(position, 1.0);
}
