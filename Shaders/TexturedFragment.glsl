#version 150
uniform sampler2D diffuseTex;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 fragColor[2];

void main(void){
	fragColor[0] = texture(diffuseTex, IN.texCoord);
	fragColor[1] = fragColor[0];
	//fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
