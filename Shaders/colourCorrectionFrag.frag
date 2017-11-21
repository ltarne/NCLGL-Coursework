#version 330

uniform sampler2D screenTex;
uniform sampler3D correctionTex;

in Vertex {
  vec2 texCoord;
} IN;

out vec4 fragColor[2];

void main()
{
    vec3 color = texture2D(screenTex, IN.texCoord).xyz;
    fragColor[0] = vec4(texture(correctionTex, color).xyz, 1.0);

    fragColor[1] = fragColor[0];
}
