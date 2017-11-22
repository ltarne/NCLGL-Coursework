#version 330

uniform sampler2D tex;
uniform sampler2D grassTex;

in Vertex {
  vec4 colour;
  vec2 texCoord;
  vec3 normal;
  vec3 tangent;
  vec3 binormal;
  vec3 worldPos;
} IN;

out vec4 fragColor[2];

void main(void) {
  vec4 grass = texture2D(grassTex, IN.texCoord);
  vec4 stone = texture2D(tex, IN.texCoord);
  stone.r -= (IN.worldPos.y-800) * 0.0004;
  stone.g -= (IN.worldPos.y-800) * 0.0004;
  stone.b -= (IN.worldPos.y-800) * 0.0004;

  if(IN.worldPos.y < 800) {
    fragColor[0] = grass;
  }
  else if(IN.worldPos.y < 900) {
    // stone.r -= 0.0004;
    // stone.g -= 0.0004;
    // stone.b -= 0.0004;
    fragColor[0] = mix(grass, stone, (IN.worldPos.y - 800)/100);
  }
  else {
    fragColor[0] = stone;
    // fragColor[0].r -= (IN.worldPos.y-800) * 0.0004;
    // fragColor[0].g -= (IN.worldPos.y-800) * 0.0004;
    // fragColor[0].b -= (IN.worldPos.y-800) * 0.0004;
    //fragColor[0] = normalize(fragColor[0]);
  }

  fragColor[1] = fragColor[1];
}
