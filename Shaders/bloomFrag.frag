#version 330

uniform sampler2D screenTex;

in Vertex {
  vec2 texCoord;
} IN;

out vec4 fragColor[2];

void main(void) {
  vec4 color = texture(screenTex, IN.texCoord);

  float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));

  fragColor[0] = color;

  if(brightness > 0.7) {
    fragColor[1] = vec4(color.rgb, 1.0);
  }
  else {
    fragColor[1] = vec4(0.0, 0.0, 0.0, 1.0);
  }
}
