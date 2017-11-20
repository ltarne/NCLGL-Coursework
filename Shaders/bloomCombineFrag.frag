#version 330

uniform sampler2D screenTex;
uniform sampler2D lightTex;

in Vertex {
  vec2 texCoord;
} IN;

out vec4 fragColor[2];

void main(void) {
  vec3 screen = texture(screenTex, IN.texCoord).xyz;
  vec3 light = texture(lightTex, IN.texCoord).xyz;

  fragColor[0].xyz = screen;
  fragColor[0].xyz += light;
  fragColor[0].a = 1.0;

  fragColor[1] = fragColor[0];
}
