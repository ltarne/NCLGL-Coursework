#version 330

uniform sampler2D diffuseTex;
uniform sampler2D emissiveTex;
uniform sampler2D specularTex;

in Vertex {
  vec2 texCoord;
} IN;

out vec4 fragColor;

void main(void) {
  vec3 diffuse = texture(diffuseTex, IN.texCoord).xyz;
  vec3 light = texture(emissiveTex, IN.texCoord).xyz;
  vec3 specular = texture(specularTex, IN.texCoord).xyz;

  fragColor.xyz = diffuse * 0.2;
  fragColor.xyz += diffuse * light;
  fragColor.xyz += specular;
  fragColor.a = 1.0;
}
