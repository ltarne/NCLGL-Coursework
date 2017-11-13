#version 330 core

uniform sampler2D tex;

in Vertex	{
	vec4 colour;
	vec2 texCoord;
} IN;

out vec4 fragColor;


void main(void)	{
  vec4 value = texture(tex, IN.texCoord).rgba;

  if(value.a == 0.0) {
    discard;
  }
	fragColor = value;
}
