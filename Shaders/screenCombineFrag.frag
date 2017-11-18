#version 330

uniform sampler2D screenTex;

in Vertex {
  vec2 texCoord;
} IN;

out vec4 fragColor;

void main(void) {
  fragColor = texture(screenTex, IN.texCoord);
}
