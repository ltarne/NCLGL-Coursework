#version 330 core

uniform sampler2D diffuseTex;
uniform bool useTexture;


in Vertex	{
	vec4 	colour;
	vec2 	texCoord;
	vec3 	normal;
	vec3 	tangent;
	vec3 	worldPos;
} IN;

out vec4 fragColor;


void main(void)	{
	//fragColor = IN.colour;


	fragColor = texture(diffuseTex, IN.texCoord);

	//fragColor = vec4(0.8,0.5,0.5,1.0);
}
