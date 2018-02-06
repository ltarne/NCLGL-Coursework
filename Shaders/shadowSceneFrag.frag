#version 400 core

uniform sampler2D tex;
uniform sampler2D bumpTex;
uniform sampler2DShadow shadowTex[5];

// uniform vec4 lightColour;
// uniform vec3 lightPos;
 uniform vec3 cameraPos;
// uniform float lightRadius;

 // struct Light {
 //   vec4 lightColour;
 //   vec3 lightPosition;
 // 	 float lightRadius;
 // 	 mat4 textureMatrix;
 // };

uniform vec4 lightColours[5];
uniform vec3 lightPositions[5];
uniform float lightRadius[5];
uniform mat4 lightTextureMatrix[5];

uniform int usedLights;

// uniform Light lights[10];
 //layout(std140) uniform lightUBO {
//   Light lights[10];
 //};

in Vertex {
  vec4 colour;
  vec2 texCoord;
  vec3 normal;
  vec3 tangent;
  vec3 binormal;
  vec3 worldPos;
  vec4 shadowProj;
} IN;

out vec4 fragColor[2];

void main(void) {
  mat3 TBN = mat3(IN.tangent, IN.binormal, IN.normal);
  vec3 normal = normalize(TBN * (texture2D(bumpTex, IN.texCoord).rgb * 2.0 - 1.0));
  vec4 finalColour = vec4(0.0, 0.0, 0.0, 0.0);
  //normal = IN.normal;

  vec4 diffuse = texture2D(tex, IN.texCoord);

  // float lambert = 0.0f;
  // float sFactor = 0.0f;
  // float rFactor = 0.0f;
  for (int i = 0; i < usedLights; ++i) {
    vec3 incident = normalize(lightPositions[i] - IN.worldPos);
    float lambert = max(0.0, dot(incident, normal));

    float dist = length(lightPositions[i] - IN.worldPos);
    float atten = 1.0 - clamp(dist / lightRadius[i], 0.0, 1.0);

    vec3 viewDir = normalize(cameraPos - IN.worldPos);
    vec3 halfDir = normalize(incident + viewDir);

    float rFactor = max(0.0, dot(halfDir, normal));
    float sFactor = pow(rFactor, 33.0);

    float shadow = 1.0;

     vec4 shadowProj = (lightTextureMatrix[i] * vec4(IN.worldPos + (IN.normal * 2), 1));
   if(shadowProj.w > 0.0 && shadowProj.x > -shadowProj.w  && shadowProj.x < shadowProj.w && shadowProj.y > -shadowProj.w && shadowProj.y < shadowProj.w ) {

      shadow = textureProj(shadowTex[i], shadowProj);
    }

    lambert *= shadow;


    vec3 colour = (diffuse.rgb * lightColours[i].rgb);
    colour += (lightColours[i].rgb * sFactor) * 0.33;

    vec4 litColour = vec4(colour * (atten) * lambert, diffuse.a);
    litColour.rgb   += (diffuse.rgb * lightColours[i].rgb) * 0.1;

    finalColour += litColour;
    //lambert += my_lambert;
  }

 //  vec3 incident = normalize(lightPos - IN.worldPos);
 //  float lambert = max(0.0, dot(incident, normal));
 //
 //  float dist = length(lightPos - IN.worldPos);
 //  float atten = 1.0 - clamp(dist / lightRadius, 0.0, 1.0);
 //
 //  vec3 viewDir = normalize(cameraPos - IN.worldPos);
 //  vec3 halfDir = normalize(incident + viewDir);
 //
 //  float rFactor = max(0.0, dot(halfDir, normal));
 //  float sFactor = pow(rFactor, 33.0);
 //
 //  float shadow = 1.0;
 //
 // if(IN.shadowProj.w > 0.0) {
 //    shadow = textureProj(shadowTex[0], IN.shadowProj);
 //  }
 //
 //  lambert *= shadow;


  // fragColor[0] = vec4(colour * (atten) * lambert, diffuse.a);
  // fragColor[0].rgb += (diffuse.rgb * lightColour.rgb) * 0.1;
  //fragColor[0] = normal;

  fragColor[0] = finalColour;

  fragColor[1] = fragColor[0];

//   for (int i = 0; i  < 10; i++)
//     {
// //  fragColor[0].x += lights[i].lightColour.r;
// //  fragColor[0].x += lights[i].lightPosition.r;
// //  /fragColor[0].x += lights[i].lightRadius;
//   fragColor[0].x += mycolours[i].x+mycolours[i].y+mycolours[i].z+mycolours[i].w;
// }
//  fragColor = vec4(normal, 1.0);

  //fragColor.rgb = vec3(lambert);
}
