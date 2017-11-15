#version 150
uniform sampler2D font;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 fragColor;

void main(void){
	fragColor = texture(font, IN.texCoord);
	if(length(fragColor.rgb) < 1) {
		fragColor.a = 0;
	}
	//fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
