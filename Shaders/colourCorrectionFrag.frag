#version 330

uniform sampler2D screenTex;
uniform sampler2D correctionTex;

in Vertex {
  vec2 texCoord;
} IN;

out vec4 fragColor[2];


vec4 sampleAs3DTexture(sampler2D texture, vec3 uv, float width) {
    float sliceSize = 1.0 / width;              // space of 1 slice
    float slicePixelSize = sliceSize / width;           // space of 1 pixel
    float sliceInnerSize = slicePixelSize * (width - 1.0);  // space of width pixels
    float zSlice0 = min(floor(uv.z * width), width - 1.0);
    float zSlice1 = min(zSlice0 + 1.0, width - 1.0);
    float xOffset = slicePixelSize * 0.5 + uv.x * sliceInnerSize;
    float s0 = xOffset + (zSlice0 * sliceSize);
    float s1 = xOffset + (zSlice1 * sliceSize);
    vec4 slice0Color = texture2D(texture, vec2(s0, uv.y));
    vec4 slice1Color = texture2D(texture, vec2(s1, uv.y));
    float zOffset = mod(uv.z * width, 1.0);
    vec4 result = mix(slice0Color, slice1Color, zOffset);
    return result;
}

void main()
{
    vec3 color = texture2D(screenTex, IN.texCoord).xyz;
    //fragColor[0] = vec4(texture(correctionTex, color).xyz, 1.0);

    vec4 gradedPixel = sampleAs3DTexture(correctionTex, color, 16);
    gradedPixel.a = 1;
  //  fragColor[0] = gradedPixel;
    color.g*=1.5;
    vec4 gradedColor = vec4(color, 1.0f);
  gradedColor = normalize(gradedColor);
    fragColor[0] = gradedColor;


    fragColor[1] = fragColor[0];
}
