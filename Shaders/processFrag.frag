#version 330
uniform sampler2D tex;
uniform vec2 pixelSize;
uniform int isVertical;


in Vertex {
	vec3 position;
	vec4 colour;
	vec2 texCoord;
} IN;

out vec4 fragColor;
const float weights[5] = float[](0.12, 0.22, 0.32, 0.22, 0.12);

void main(void){
	vec2 values[5];

	fragColor = vec4(0.0, 0.0, 0.0, 1.0);

	if(isVertical == 1) {
		values = vec2[](vec2(0.0, -pixelSize.y*3),
		vec2(0.0, -pixelSize.y*2), vec2(0.0, pixelSize.y),
		vec2(0.0, pixelSize.y*2), vec2(0.0, pixelSize.y*3));
	}
	else {
		values = vec2[](vec2(-pixelSize.x*3, 0.0),
		vec2(-pixelSize.x*2, 0.0), vec2(pixelSize.x, 0.0),
		vec2(pixelSize.x*2, 0.0), vec2(pixelSize.x*3, 0.0));
	}

	for(int i = 0; i <5; i++) {
		vec4 tmp = texture2D(tex, IN.texCoord.xy + values[i]);
		fragColor += tmp * weights[i];
	}

}
