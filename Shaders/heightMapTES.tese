#version 400 core

layout(quads, equal_spacing, ccw) in;


uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 globalTextureMatrix;

//uniform float scale;

in Vertex {
  vec4 colour;
  vec3 normal;
  vec3 tangent;
  vec2 texCoord;
} IN[];

out Vertex {
  vec4 colour;
  vec2 texCoord;
  vec3 normal;
  vec3 tangent;
  vec3 binormal;
  vec3 worldPos;
  vec4 shadowProj;
} OUT;

// Simplex 2D noise
// Source: https://github.com/SRombauts/SimplexNoise.
vec3 permute(vec3 x) { return mod(((x*34.0)+1.0)*x, 289.0); }

float snoise(vec2 v){
  const vec4 C = vec4(0.211324865405187, 0.366025403784439,
           -0.577350269189626, 0.024390243902439);
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);
  vec2 i1;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;
  i = mod(i, 289.0);
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
  + i.x + vec3(0.0, i1.x, 1.0 ));
  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy),
    dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;
  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}
//End of Simplex noise implementation

// Adding frequency and octaves to the noise
// Source: https://cmaher.github.io/posts/working-with-simplex-noise/
float sumOctave(int numIterations, float x, float y, float persistence, float scale, float low, float high) {

    float maxAmp = 0;
    float amp = 1;
    float frequency = scale;
    float noise = 0;

    //Add successively smaller, higher-frequency terms
    for(int i=0; i < numIterations; ++i) {
        noise += snoise(vec2(x*frequency, y * frequency)) * amp;
        maxAmp += amp;
        amp *= persistence;
        frequency *= 2;
    }

    //Take the average value of the iterations
    noise /= maxAmp;

    //Normalize the result
    noise = noise * (high - low) / 2 + (high + low) / 2;

    return noise;
}
// End of frequency and octaves implementation

vec4 QuadMixVec4(vec4 a, vec4 b, vec4 c, vec4 d) {
  vec4 p0 = mix(a,c,gl_TessCoord.x);
  vec4 p1 = mix(b,d,gl_TessCoord.x);

  return mix(p0,p1,gl_TessCoord.y);
}

vec3 QuadMixVec3(vec3 a, vec3 b, vec3 c, vec3 d) {
    vec3 p0     = mix(a,c, gl_TessCoord.x);
    vec3 p1     = mix(b,d, gl_TessCoord.x);

    return mix(p0,p1,gl_TessCoord.y);
}

vec2    QuadMixVec2(vec2 a, vec2 b, vec2 c, vec2 d) {
    vec2 p0     = mix(a,c, gl_TessCoord.x);
    vec2 p1     = mix(b,d, gl_TessCoord.x);

    return mix(p0,p1, gl_TessCoord.y);
}

void main() {
    float scale = 0.0001;

    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));


    vec3 combinedPos = QuadMixVec3( gl_in[0].gl_Position.xyz,
                                    gl_in[1].gl_Position.xyz,
                                    gl_in[2].gl_Position.xyz,
                                    gl_in[3].gl_Position.xyz );

    OUT.texCoord = QuadMixVec2( IN[0].texCoord,
                                IN[1].texCoord,
                                IN[2].texCoord,
                                IN[3].texCoord);

    OUT.texCoord  = OUT.texCoord * 64;

    OUT.colour = QuadMixVec4(IN[0].colour,
                            IN[1].colour,
                            IN[2].colour,
                            IN[3].colour);


    vec3 normal = QuadMixVec3(IN[0].normal,
                              IN[1].normal,
                              IN[2].normal,
                              IN[3].normal);

    vec3 tangent = QuadMixVec3(IN[0].tangent,
                              IN[1].tangent,
                              IN[2].tangent,
                              IN[3].tangent);



    OUT.normal = normalize(normalMatrix * normalize(normal));
    OUT.tangent = normalize(normalMatrix * normalize(tangent));
    OUT.binormal = normalize(normalMatrix * normalize(cross(normal, tangent)));



    vec4 worldPos = (modelMatrix * vec4(combinedPos, 1));
    float height = sumOctave(16,worldPos.x,worldPos.z,0.5f, scale, 5, 2000);
    worldPos.y += height;
    OUT.worldPos = worldPos.xyz;

    OUT.shadowProj = (globalTextureMatrix * vec4(OUT.worldPos + (OUT.normal * 10.5), 1));

    gl_Position = projMatrix * viewMatrix * worldPos;

}
