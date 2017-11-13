#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform vec4 nodeColour;

in  vec3 position;
in  vec4 colour;
in  vec2 texCoord;

out Vertex {
	vec3 position;
	vec4 colour;
	vec2 texCoord;
} OUT;



void main(void)	{

	gl_Position	  = (projMatrix * viewMatrix * modelMatrix) * vec4(position, 1.0);
	OUT.position = position;
	OUT.colour    = nodeColour;
	OUT.texCoord 	= texCoord;
}
