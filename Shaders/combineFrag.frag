#version 330

uniform sampler2D diffuseTex;
uniform sampler2D emissiveTex;
uniform sampler2D specularTex;
uniform sampler2D depthTex;


in Vertex {
  vec2 texCoord;
} IN;

out vec4 fragColor[2];

void main(void) {

  vec3 diffuse = texture(diffuseTex, IN.texCoord).xyz;
  vec3 light = texture(emissiveTex, IN.texCoord).xyz;
  vec3 specular = texture(specularTex, IN.texCoord).xyz;
  vec3 depth = texture(depthTex, IN.texCoord).xyz;

  if(depth.r < 1.0 && depth.g < 1.0 && depth.b < 1.0 ) {
    fragColor[0].xyz = diffuse * 0.2;
    fragColor[0].xyz += diffuse * light;
    fragColor[0].xyz += specular;
    fragColor[0].a = 1.0;
  }
  else {
      fragColor[0].xyz = diffuse;
      fragColor[0].a = 1.0;
  }


  fragColor[1] = fragColor[0];
}
