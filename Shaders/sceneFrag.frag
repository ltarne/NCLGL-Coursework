#version 330 core

uniform sampler2D tex;
//uniform bool useTexture;


in Vertex	{
	vec3 position;
	vec4 colour;
	vec2 texCoord;
} IN;

out vec4 fragColor;


void main(void)	{
	fragColor = IN.colour;

	//if(useTexture) {
		fragColor = texture(tex, IN.texCoord);
		fragColor.a = 1;
	//}
}
